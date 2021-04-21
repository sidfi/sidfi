#include "config.h"

#define _DISABLE_OPENADC10_CONFIGPORT_WARNING
#define _SUPPRESS_PLIB_WARNING
#include <plib.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"
#include "diskio.h"
#include "ff.h"
#include "st7735s.h"

#include "fonts.h"
#include "gfx.h"

#define CLOCKFREQ (50000000ul)  // This is set in in Configuration Bits.h
#define BUSFREQ (CLOCKFREQ / 1) // This is set in in Configuration Bits.h

#define nsleep(ns)                                                             \
  {                                                                            \
    uint32_t c =                                                               \
        ReadCoreTimer() + (((uint32_t)(ns)) / (2000000000 / CLOCKFREQ));       \
    while (ReadCoreTimer() < c)                                                \
      ;                                                                        \
  }
#define halt()                                                                 \
  {                                                                            \
    while (1)                                                                  \
      ;                                                                        \
  }

#define MEMORY_VIRTUAL_PAGE_NUMBER 64
#define MEMORY_PHYSICAL_PAGE_NUMBER 48
#define MEMORY_PAGE_SIZE 1024
#define MEMORY_PAGE_BITS 10
#define MEMORY_PAGE_MASK 0x3ff
#define MEMORY_UNALLOCATED_PAGE_NUMBER UINT16_MAX

uint16_t memory_page_table[MEMORY_VIRTUAL_PAGE_NUMBER];
uint8_t memory[MEMORY_PHYSICAL_PAGE_NUMBER * MEMORY_PAGE_SIZE];
uint8_t memory_free_page = 0;

void memory_init() {
  for (size_t i = 0; i < MEMORY_VIRTUAL_PAGE_NUMBER; ++i)
    memory_page_table[i] = MEMORY_UNALLOCATED_PAGE_NUMBER;
}

static bool memory_virtual_to_physical(uint16_t v_address,
                                       uint16_t *p_address) {
  size_t i = v_address >> MEMORY_PAGE_BITS;
  size_t p = memory_page_table[i];

  if (p == MEMORY_UNALLOCATED_PAGE_NUMBER) {
    if (memory_free_page == MEMORY_PHYSICAL_PAGE_NUMBER)
      return false;

    memory_page_table[i] = p = memory_free_page;
    memory_free_page++;
  }

  *p_address = (p << MEMORY_PAGE_BITS) | (v_address & MEMORY_PAGE_MASK);
  return true;
}

static bool memory_write(uint16_t address, uint8_t data) {
  bool res = memory_virtual_to_physical(address, &address);
  if (res)
    memory[address] = data;
  return res;
}

static uint8_t memory_read(uint16_t address) {
  bool res = memory_virtual_to_physical(address, &address);
  return res ? memory[address] : 0;
}

static bool is_mos6581_address(uint16_t address) {
  return address >= 0xD400 && address < 0xD800;
}

static void mos6581_write(uint8_t address, uint8_t data) {
  SPI2BUF = address;
  while (SPI2STATbits.SPIBUSY)
    ;

  SPI2BUF = data;
  while (SPI2STATbits.SPIBUSY)
    ;

  _LATB2 = 0;
  nsleep(5000);
  _LATB2 = 1;
}

void cpu_memory_write(uint16_t address, uint8_t data) {
  if (is_mos6581_address(address)) {
    mos6581_write(address & 0x1f, data);
  } else {
    memory_write(address, data);
  }
}

uint8_t cpu_memory_read(uint16_t address) {
  if (is_mos6581_address(address)) {
    return 0;
  }

  return memory_read(address);
}

#define PSID_MIN_HEADER_LENGTH 118 // Version 1
#define PSID_MAX_HEADER_LENGTH 124 // Version 2

// Offsets of fields in header (all fields big-endian)
enum {
  PSID_MAGIC_ID = 0,   // 'PSID'
  PSID_VERSION = 4,    // 1 or 2
  PSID_LENGTH = 6,     // Header length
  PSID_LOAD = 8,       // C64 load address
  PSID_INIT = 10,      // C64 init routine address
  PSID_PLAY = 12,      // C64 replay routine address
  PSID_NUMBER = 14,    // Number of subsongs
  PSID_DEFSONG = 16,   // Main subsong number
  PSID_SPEED = 18,     // Speed flags (1 bit/song)
  PSID_NAME = 22,      // Module name (ISO Latin1 character set)
  PSID_AUTHOR = 54,    // Author name (dto.)
  PSID_COPYRIGHT = 86, // Copyright info (dto.)
  PSID_FLAGS = 118,    // Flags (only in version 2 header)
  PSID_RESERVED = 120
};

enum sid_result {
  SIDFILE_OK,
  SIDFILE_ERROR_IO,
  SIDFILE_ERROR_MALFORMED,
  SIDFILE_ERROR_TOOBIG
};

struct sid {
  const char *module_name;
  const char *author_name;
  const char *copyright_info;
  size_t num_of_songs;
  size_t first_song;
  uint16_t init_address;
  uint16_t play_address;
  uint16_t load_address;
  uint32_t speed_flags;
};

static uint16_t read_uint16(const uint8_t *data, uint16_t offset) {
  return (data[offset] << 8) | data[offset + 1];
}

static uint32_t read_uint32(const uint8_t *data, uint16_t offset) {
  return (data[offset] << 24) | (data[offset + 1] << 16) |
         (data[offset + 2] << 8) | data[offset + 3];
}

static bool is_header(const uint8_t *data) {
  uint32_t magic_id = read_uint32(data, PSID_MAGIC_ID);
  uint16_t version = read_uint16(data, PSID_VERSION);
  return magic_id == 0x50534944 && (version == 1 || version == 2);
}

#define SID_READ_BUFFER_SIZE 512
uint8_t sid_read_buffer[SID_READ_BUFFER_SIZE];

enum sid_result sid_read_file(struct sid *sid, const TCHAR *file_path) {
  FIL f;
  FRESULT fr = f_open(&f, file_path, FA_READ);
  if (fr != FR_OK) {
    return SIDFILE_ERROR_IO;
  }

  UINT read;
  static uint8_t sid_header[PSID_MAX_HEADER_LENGTH];
  fr = f_read(&f, sid_header, PSID_MAX_HEADER_LENGTH, &read);

  if (fr != FR_OK) {
    f_close(&f);
    return SIDFILE_ERROR_IO;
  }

  if (read < PSID_MIN_HEADER_LENGTH || !is_header(sid_header)) {
    f_close(&f);
    return SIDFILE_ERROR_MALFORMED;
  }

  sid->num_of_songs = read_uint16(sid_header, PSID_NUMBER);

  if (sid->num_of_songs == 0) {
    sid->num_of_songs = 1;
  }

  sid->first_song = read_uint16(sid_header, PSID_DEFSONG);
  if (sid->first_song) {
    sid->first_song--;
  }
  if (sid->first_song >= sid->num_of_songs) {
    sid->first_song = 0;
  }

  sid->init_address = read_uint16(sid_header, PSID_INIT);
  sid->play_address = read_uint16(sid_header, PSID_PLAY);
  sid->speed_flags = read_uint32(sid_header, PSID_SPEED);

  sid->module_name = (const char *)(sid_header + PSID_NAME);
  sid->author_name = (const char *)(sid_header + PSID_AUTHOR);
  sid->copyright_info = (const char *)(sid_header + PSID_COPYRIGHT);

  FSIZE_t dataOffset = read_uint16(sid_header, PSID_LENGTH);
  fr = f_lseek(&f, dataOffset);

  if (fr != FR_OK) {
    f_close(&f);
    return SIDFILE_ERROR_IO;
  }

  // Find load address

  sid->load_address = read_uint16(sid_header, PSID_LOAD);
  if (sid->load_address == 0) {
    uint8_t lo, hi;
    fr = f_read(&f, &lo, 1, &read);
    if (fr != FR_OK) {
      f_close(&f);
      return SIDFILE_ERROR_IO;
    }

    fr = f_read(&f, &hi, 1, &read);
    if (fr != FR_OK) {
      f_close(&f);
      return SIDFILE_ERROR_IO;
    }

    sid->load_address = (hi << 8) | lo;
  }

  if (sid->init_address == 0) {
    sid->init_address = sid->load_address;
  }

  size_t address = sid->load_address;

  do {
    fr = f_read(&f, sid_read_buffer, SID_READ_BUFFER_SIZE, &read);

    for (size_t i = 0; i < read; ++i) {
      if (!memory_write(address + i, sid_read_buffer[i]))
        return SIDFILE_ERROR_TOOBIG;
    }

    address += read;

    if (fr != FR_OK) {
      f_close(&f);
      return SIDFILE_ERROR_IO;
    }
  } while (read != 0);

  f_close(&f);
  return SIDFILE_OK;
}

#define SONG_LENGTHS_MAX 16

struct song_lengths {
  size_t num;
  uint32_t ms[SONG_LENGTHS_MAX];
};

#define SONG_LENGTHS_LINE_BUFFER_SIZE 2048

enum song_lengths_res {
  SONG_LENGTHS_FOUND,
  SONG_LENGTHS_NOT_FOUND,
  SONG_LENGTHS_ERROR_IO
};

enum song_lengths_parse_state {
  SONG_LENGTHS_PARSE_STATE_NONE,
  SONG_LENGTHS_PARSE_STATE_MINUTES,
  SONG_LENGTHS_PARSE_STATE_SECONDS,
  SONG_LENGTHS_PARSE_STATE_MS
};

void song_lengths_read_value(struct song_lengths *song_lengths, size_t i,
                             enum song_lengths_parse_state parse_state,
                             uint32_t parse_value) {
  switch (parse_state) {
  case SONG_LENGTHS_PARSE_STATE_MINUTES:
    song_lengths->ms[i] += parse_value * 60000;
    break;
  case SONG_LENGTHS_PARSE_STATE_SECONDS:
    song_lengths->ms[i] += parse_value * 1000;
    break;
  case SONG_LENGTHS_PARSE_STATE_MS:
    song_lengths->ms[i] += parse_value;
    break;
  }
}

#define SONG_LENGTHS_PARSE_VALUE_MAX_DIGITS 3

uint32_t song_lengths_parse_value(
    size_t k, uint32_t parse_digits[SONG_LENGTHS_PARSE_VALUE_MAX_DIGITS]) {
  uint16_t parse_value = 0;
  const uint32_t multiple[SONG_LENGTHS_PARSE_VALUE_MAX_DIGITS] = {1, 10, 100};
  for (size_t i = 0; i < k; i++) {
    parse_value += parse_digits[i] * multiple[k - i - 1];
  }
  return parse_value;
}

enum song_lengths_res
song_lengths_read_line(uint8_t *line, const TCHAR *sid_file_path,
                       struct song_lengths *song_lengths) {
  static int last_strcmp_res = -1;
  if (line[0] == ';') {
    last_strcmp_res = strcmp(&line[2], sid_file_path);
  } else if (last_strcmp_res == 0) {
    last_strcmp_res = -1;

    size_t l = strlen(line);
    size_t k = 0;

    enum song_lengths_parse_state parse_state = SONG_LENGTHS_PARSE_STATE_NONE;
    uint32_t parse_digits[SONG_LENGTHS_PARSE_VALUE_MAX_DIGITS];

    song_lengths->num = 0;
    song_lengths->ms[0] = 0;

    for (size_t i = 0; i < l; i++) {
      if (line[i] == '=') {
        parse_state = SONG_LENGTHS_PARSE_STATE_MINUTES;
        k = 0;
      } else if (parse_state == SONG_LENGTHS_PARSE_STATE_MINUTES &&
                 line[i] == ':') {
        song_lengths_read_value(song_lengths, song_lengths->num, parse_state,
                                song_lengths_parse_value(k, parse_digits));

        parse_state = SONG_LENGTHS_PARSE_STATE_SECONDS;
        k = 0;
      } else if (parse_state == SONG_LENGTHS_PARSE_STATE_SECONDS &&
                 line[i] == '.') {
        song_lengths_read_value(song_lengths, song_lengths->num, parse_state,
                                song_lengths_parse_value(k, parse_digits));

        parse_state = SONG_LENGTHS_PARSE_STATE_MS;
        k = 0;
      } else if (line[i] == ' ') {
        if (song_lengths->num == SONG_LENGTHS_MAX - 1)
          break;

        song_lengths_read_value(song_lengths, song_lengths->num, parse_state,
                                song_lengths_parse_value(k, parse_digits));
        song_lengths->num++;

        song_lengths->ms[song_lengths->num] = 0;

        parse_state = SONG_LENGTHS_PARSE_STATE_MINUTES;
        k = 0;
      } else if (line[i] >= '0' && line[i] <= '9' &&
                 k < SONG_LENGTHS_PARSE_VALUE_MAX_DIGITS) {
        parse_digits[k] = (line[i] - 0x30);
        k++;
      }
    }
    song_lengths_read_value(song_lengths, song_lengths->num, parse_state,
                            song_lengths_parse_value(k, parse_digits));
    song_lengths->num++;

    return SONG_LENGTHS_FOUND;
  }
  return SONG_LENGTHS_NOT_FOUND;
}

#define SONG_LENGTHS_READ_BUFFER_SIZE 512
uint8_t song_lengths_read_buffer[SONG_LENGTHS_READ_BUFFER_SIZE];

FIL song_lengths_file;

void song_lengths_open_file(const TCHAR *song_lengths_file_path) {
  f_open(&song_lengths_file, song_lengths_file_path, FA_READ);
}

enum song_lengths_res
song_lengths_find_in_file(const TCHAR *sid_file_path,
                       struct song_lengths *song_lengths) {
  static uint8_t line_buffer[SONG_LENGTHS_LINE_BUFFER_SIZE];
  static UINT read = 0;

  size_t i = 0;
  FRESULT fr;

  do {
    static size_t k = 0;
    while (k != read) {
      uint8_t c = song_lengths_read_buffer[k++];
      if (c == '\n' || c == '\r' || i == SONG_LENGTHS_LINE_BUFFER_SIZE - 1) {
        if (i == 0)
          continue;

        line_buffer[i] = 0;
        i = 0;

        if (song_lengths_read_line(line_buffer, sid_file_path, song_lengths) ==
            SONG_LENGTHS_FOUND)
          return SONG_LENGTHS_FOUND;
      } else
        line_buffer[i++] = c;
    }

    k = 0;
    fr = f_read(&song_lengths_file, song_lengths_read_buffer, SONG_LENGTHS_READ_BUFFER_SIZE, &read);
    if (fr != FR_OK)
      return SONG_LENGTHS_ERROR_IO;

  } while (read != 0);

  fr = f_lseek(&song_lengths_file, 0);

  if (fr != FR_OK)
    return SONG_LENGTHS_ERROR_IO;

  return SONG_LENGTHS_NOT_FOUND;
}

void sid_init_player(struct sid *sid, uint8_t song_number) {
  // install reset vector for microplayer (0x0000)
  memory_write(0xFFFD, 0x00);
  memory_write(0xFFFC, 0x00);

  // install IRQ vector for play routine launcher (0x0013)
  memory_write(0xFFFF, 0x00);
  memory_write(0xFFFE, 0x13);

  // install the micro player, 6502 assembly code
  memory_write(0x0000, 0xA9); // A = 0, load A with the song number
  memory_write(0x0001, song_number);

  memory_write(0x0002, 0x20);                     // jump sub to INIT routine
  memory_write(0x0003, sid->init_address & 0xFF); // lo addr
  memory_write(0x0004, (sid->init_address >> 8) & 0xFF); // hi addr

  memory_write(0x0005, 0x58); // enable interrupt
  memory_write(0x0006, 0xEA); // nop
  memory_write(0x0007, 0x4C); // jump to 0x0006
  memory_write(0x0008, 0x06);
  memory_write(0x0009, 0x00);

  memory_write(0x0013, 0xEA); // nop  //0xA9; // A = 1
  memory_write(0x0014, 0xEA); // nop //0x01;
  memory_write(0x0015, 0xEA); // 0x78 CLI
  memory_write(0x0016, 0x20); // jump sub to play routine
  memory_write(0x0017, sid->play_address & 0xFF);
  memory_write(0x0018, (sid->play_address >> 8) & 0xFF);
  memory_write(0x0019, 0xEA); // 0x58 SEI
  memory_write(0x001A, 0x40); // RTI: return from interrupt
}

uint32_t sid_song_speed_ns(struct sid *sid, uint8_t song_number) {
  return ((sid->speed_flags >> (song_number % 32)) & 1) ? 16666666 /* 60Hz */
                                                        : 20000000 /* 50Hz */;
}

void __ISR(_TIMER_4_VECTOR, ipl1auto) T4Interrupt(void) {
  disk_timerproc();

  mT4ClearIntFlag();
}

uint32_t next_random() {
  static uint32_t random = 0;
  random += ReadCoreTimer();
  return random;
}

static char traverse_files_buffer[1024];
static size_t traverse_files_dir_index = -1;
static DIR traverse_files_dirs[6];

static FRESULT traverse_files_open_dir() {
  FRESULT res;
  res = f_opendir(&traverse_files_dirs[traverse_files_dir_index + 1],
                  traverse_files_buffer); /* Open the directory */
  if (res != FR_OK)
    return res;

  traverse_files_dir_index++;

  return res;
}

static void traverse_files_close_dir() {
  f_closedir(&traverse_files_dirs[traverse_files_dir_index]);

  traverse_files_dir_index--;
}

static FRESULT traverse_files_next(char **path) {
  FRESULT res;
  FILINFO fno;
  static size_t slash_pos[6];

  if (traverse_files_dir_index == -1) {
    res = traverse_files_open_dir();

    if (res != FR_OK)
      return res;

    slash_pos[traverse_files_dir_index] = -1;

    return traverse_files_next(path);
  }

  if (slash_pos[traverse_files_dir_index] != -1) {
    traverse_files_buffer[slash_pos[traverse_files_dir_index]] = 0;
    slash_pos[traverse_files_dir_index] = -1;
  }

  res = f_readdir(&traverse_files_dirs[traverse_files_dir_index], &fno);
  if (res != FR_OK)
    return res;

  if (!fno.fname[0]) {
    traverse_files_close_dir();

    return traverse_files_next(path);
  }

  size_t l = strlen(traverse_files_buffer);
  slash_pos[traverse_files_dir_index] = l;

  traverse_files_buffer[l] = '/';
  strcpy(&traverse_files_buffer[l + 1], fno.fname);

  if (fno.fattrib & AM_DIR) {
    res = traverse_files_open_dir();

    if (res != FR_OK)
      return res;

    slash_pos[traverse_files_dir_index] = -1;

    return traverse_files_next(path);
  }

  *path = traverse_files_buffer;
  return res;
}

static void traverse_files_init(const char *base_path) {
  while (traverse_files_dir_index != -1) {
    f_closedir(&traverse_files_dirs[traverse_files_dir_index]);
    traverse_files_dir_index--;
  }

  strcpy(traverse_files_buffer, base_path);
}

static uint32_t break_timer0 = 0;
static uint32_t break_timer1 = 0;

static bool set_break_ns(uint32_t ns0, uint32_t ns1) {
  break_timer0 = ns0 / (2000000000 / CLOCKFREQ);
  break_timer1 = ns1 / (2000000000 / CLOCKFREQ);
  WriteCoreTimer(0);
}

static bool should_break0() { return ReadCoreTimer() >= break_timer0; }
static bool should_break1() { return ReadCoreTimer() >= break_timer1; }

char *itoa(int value) {
  static char buffer[12]; // 12 bytes is big enough for an INT32
  int original = value;   // save original value

  int c = sizeof(buffer) - 1;

  buffer[c] = 0; // write trailing null in last byte of buffer

  if (value < 0) // if it's negative, note that and take the absolute value
    value = -value;

  do // write least significant digit of value that's left
  {
    buffer[--c] = (value % 10) + '0';
    value /= 10;
  } while (value);

  if (original < 0)
    buffer[--c] = '-';

  return &buffer[c];
}

static FATFS fs;

#define SCANCODE_NONE 7
#define SCANCODE_RIGHT 8
#define SCANCODE_LEFT 9
#define SCANCODE_CTR 10
#define SCANCODE_KEY2 11
#define SCANCODE_DOWN 12
#define SCANCODE_KEY1 13
#define SCANCODE_UP 14
#define SCANCODE_KEY3 15

int main() {
  ANSELA = 0;
  ANSELB = 0;
  mJTAGPortEnable(0);

  SYSTEMConfigPerformance(CLOCKFREQ);
  mOSCSetPBDIV(OSC_PB_DIV_1);

  LATA = LATB = 0;
  TRISA = TRISB = 0xffffffff;
  CNENA = CNENB = CNCONA = CNCONB = 0;
  CNPUA = CNPUB = CNPDA = CNPDB = 0;

  PR4 = 1000 * (BUSFREQ / 2 / 1000000) - 1;
  T4CON = 0x8010;
  mT4SetIntPriority(1);
  mT4ClearIntFlag();
  mT4IntEnable(1);

  _TRISB2 = 0; _LATB2 = 1;  // MOS 6581 CS        (6)
  PPSOutput(3, RPA4, SDO2); // MOS 6581 / LCD SDO (12)
                            // MOS 6581 / LCD SCK (26)

  _TRISB13 = 0; _LATB13 = 1; // SD CARD CS  (24)
  _TRISB11 = 1;              // SD CARD CD  (22)
  PPSOutput(2, RPB5, SDO1);  // SD CARD SDO (14)
  PPSInput(2, SDI1, RPB8);   // SD CARD SDI (17)
                             // SD CARD SCK (25)

  _TRISB0 = 0; _LATB0 = 1; // LCD CS    (4)
  _TRISB1 = 0; _LATB1 = 1; // LCD DC    (5)
  _TRISB3 = 0; _LATB3 = 0; // LCD RESET (7)

  _TRISB4 = 1;  // KEY CTR   (11)
  _TRISB7 = 1;  // KEY UP    (16)
  _TRISB9 = 1;  // KEY DOWN  (18)
  _TRISB10 = 1; // KEY RIGHT (21)

  INTEnableSystemMultiVectoredInt();
  INTEnableInterrupts();

  nsleep(1000000);

  SpiChnOpen(SPI_CHANNEL2,
             (SpiOpenFlags)(SPI_OPEN_MODE8 | SPI_OPEN_MSTEN | SPI_OPEN_DISSDI |
                            SPI_OPEN_CKE_REV),
             2);

  ST7735S_Init();
  setOrientation(R0);

  f_mount(&fs, "", 0);

  setbgColor(31, 63, 31);
  setFont(ter_u16n);

  memory_init();
  cpu_init(cpu_memory_read, cpu_memory_write);

  traverse_files_init("");
  song_lengths_open_file("/DOCUMENTS/Songlengths.md5");

  next_random();

  struct sid sid;
  struct song_lengths song_lengths;
  
  memset(&sid, 0, sizeof(struct sid));
  memset(&song_lengths, 0, sizeof(struct song_lengths));
  
  uint32_t timer = 0;
  uint8_t song_number = 0;
  uint32_t song_speed_ns = 0;
  uint32_t song_length_ms = 0;
  uint8_t debounce_counter = 0;
  uint8_t prev_scancode = 1;
  enum song_lengths_res slr = 0;

  while (1) {
    if (!timer) {
      if (!sid.num_of_songs || ++song_number == sid.num_of_songs) {
        setColor(31, 63, 31);
        fillScreen();
        setColor(0, 63, 0);
        drawText(0, 0, "Loading...");
        flushBuffer();

        while (1) {
          char *sid_file_path;
          FRESULT fr;
          uint32_t i = next_random() % 10000;          

          while (i--)
            fr = traverse_files_next(&sid_file_path);
            
          if (fr != FR_OK)
            continue;

          enum sid_result sidr = sid_read_file(&sid, sid_file_path);
          if (sidr == SIDFILE_OK) {
            slr = song_lengths_find_in_file(sid_file_path, &song_lengths);
            song_number = 0;
            break;
          }
        }
      }
      song_length_ms =
          ((slr == SONG_LENGTHS_FOUND && song_number < song_lengths.num)
               ? song_lengths.ms[song_number]
               : 60000);
      song_speed_ns = sid_song_speed_ns(&sid, song_number);
      timer = (song_length_ms * 1000) / (song_speed_ns / 1000);

      sid_init_player(&sid, song_number);

      set_break_ns(1000000, 0);
      cpu_reset();
      cpu_run(should_break0);

      setColor(31, 63, 31);
      fillScreen();
      setColor(0, 0, 31);
      uint16_t line = 0;
      /*size_t len = strlen(sid_file_path);
      for (size_t i = 0; i <= len / 16; i++)
        drawText(0, (line++) * 16, sid_file_path + (i * 16));*/
      drawText(0, (line++) * 16, sid.module_name);
      drawText(0, (line++) * 16, itoa(song_number));
      drawText(0, (line++) * 16, sid.author_name);
      drawText(0, (line++) * 16, sid.copyright_info);
      flushBuffer();
    } else
      timer--;

    set_break_ns(song_speed_ns - 4000000, song_speed_ns);
    cpu_irq();
    cpu_run(should_break0);

    uint8_t scancode = (_RB4 << 3) | (_RB7 << 2) | (_RB9 << 1) | _RB10;

    if (scancode != prev_scancode) {
      debounce_counter = 0;
      prev_scancode = scancode;
    } else
      debounce_counter++;

    if (debounce_counter == 3) {
      debounce_counter = 0;

      if (scancode == SCANCODE_KEY1) {
        timer = 0;
        continue;
      }
    }

    uint32_t seconds = (timer * (song_speed_ns / 1000)) / 1000000;

    drawText(0, 112, itoa(seconds / 60));
    drawText(32, 112, itoa(seconds % 60));
    drawText(96, 112, itoa(scancode));
    flushBuffer();

    while (!should_break1())
      ;
  }

  return 0;
}
