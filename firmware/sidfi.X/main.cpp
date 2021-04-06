#include "config.h"

#define _DISABLE_OPENADC10_CONFIGPORT_WARNING
#define _SUPPRESS_PLIB_WARNING
#include <plib.h>

#include <stdint.h>
#include <stdlib.h>

#include "diskio.h"
#include "ff.h"
#include "mos6502.h"

#define CLOCKFREQ (50000000ul)  // This is set in in Configuration Bits.h
#define BUSFREQ (CLOCKFREQ / 1) // This is set in in Configuration Bits.h

#define nsleep(ns)                                                             \
  {                                                                            \
    uint32_t c =                                                               \
        ReadCoreTimer() + (((uint32_t)(ns)-500) / (2000000000 / CLOCKFREQ));   \
    while (ReadCoreTimer() < c)                                                \
      ;                                                                        \
  }
#define halt()                                                                 \
  {                                                                            \
    while (1)                                                                  \
      ;                                                                        \
  }

#include <stdbool.h>
#include <stdint.h>

#define MEMORY_VIRTUAL_PAGE_NUMBER 64
#define MEMORY_PHYSICAL_PAGE_NUMBER 56
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

static uint16_t memory_virtual_to_physical(uint16_t address) {
  size_t i = address >> MEMORY_PAGE_BITS;
  size_t p = memory_page_table[i];

  if (p == MEMORY_UNALLOCATED_PAGE_NUMBER) {
    if (memory_free_page == MEMORY_PHYSICAL_PAGE_NUMBER)
      halt();

    memory_page_table[i] = p = memory_free_page;
    memory_free_page++;
  }

  return (p << MEMORY_PAGE_BITS) | (address & MEMORY_PAGE_MASK);
}

void mos6581_write(uint8_t address, uint8_t data) {
  SPI2BUF = address;
  while (SPI2STATbits.SPITBF)
    ;

  SPI2BUF = data;
  while (SPI2STATbits.SPIBUSY)
    ;

  _LATB2 = 1;
  nsleep(5000);
  _LATB2 = 0;
}

void memory_write(uint16_t address, uint8_t data) {
  if (address >= 0xD400 && address < 0xD800) {
    mos6581_write(address & 0x1f, data);
  } else {
    memory[memory_virtual_to_physical(address)] = data;
  }
}

uint8_t memory_read(uint16_t address) {
  if (address >= 0xD400 && address < 0xD800) {
    return 0;
  }

  return memory[memory_virtual_to_physical(address)];
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

#define SIDFILE_READ_BUFFER_SIZE 1024

enum { SIDFILE_OK, SIDFILE_ERROR_FILENOTFOUND, SIDFILE_ERROR_MALFORMED };

struct sid {
  const char *module_name;
  const char *author_name;
  const char *copyright_info;
  int num_of_songs;
  int first_song;
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

int sid_read_file(struct sid *sid, const TCHAR *path) {
  FIL f;
  FRESULT fr = f_open(&f, path, FA_READ);
  if (fr != FR_OK) {
    return SIDFILE_ERROR_FILENOTFOUND;
  }

  uint8_t header[PSID_MAX_HEADER_LENGTH];
  UINT read;
  fr = f_read(&f, header, PSID_MAX_HEADER_LENGTH, &read);

  if (fr != FR_OK || read < PSID_MIN_HEADER_LENGTH || !is_header(header)) {
    f_close(&f);
    return SIDFILE_ERROR_MALFORMED;
  }

  sid->num_of_songs = read_uint16(header, PSID_NUMBER);

  if (sid->num_of_songs == 0) {
    sid->num_of_songs = 1;
  }

  sid->first_song = read_uint16(header, PSID_DEFSONG);
  if (sid->first_song) {
    sid->first_song--;
  }
  if (sid->first_song >= sid->num_of_songs) {
    sid->first_song = 0;
  }

  sid->init_address = read_uint16(header, PSID_INIT);
  sid->play_address = read_uint16(header, PSID_PLAY);
  sid->speed_flags = read_uint32(header, PSID_SPEED);

  sid->module_name = (const char *)(header + PSID_NAME);
  sid->author_name = (const char *)(header + PSID_AUTHOR);
  sid->copyright_info = (const char *)(header + PSID_COPYRIGHT);

  FSIZE_t dataOffset = read_uint16(header, PSID_LENGTH);
  fr = f_lseek(&f, dataOffset);

  if (fr != FR_OK) {
    f_close(&f);
    return SIDFILE_ERROR_MALFORMED;
  }

  // Find load address

  sid->load_address = read_uint16(header, PSID_LOAD);
  if (sid->load_address == 0) {
    uint8_t lo, hi;
    fr = f_read(&f, &lo, 1, &read);
    if (fr != FR_OK) {
      f_close(&f);
      return SIDFILE_ERROR_MALFORMED;
    }

    fr = f_read(&f, &hi, 1, &read);
    if (fr != FR_OK) {
      f_close(&f);
      return SIDFILE_ERROR_MALFORMED;
    }

    sid->load_address = (hi << 8) | lo;
  }

  if (sid->init_address == 0) {
    sid->init_address = sid->load_address;
  }

  size_t address = sid->load_address;
  uint8_t buffer[SIDFILE_READ_BUFFER_SIZE];

  do {
    fr = f_read(&f, buffer, SIDFILE_READ_BUFFER_SIZE, &read);

    for (size_t i = 0; i < read; ++i) {
      memory_write(address + i, buffer[i]);
    }

    address += read;

    if (fr != FR_OK) {
      f_close(&f);
      return SIDFILE_ERROR_MALFORMED;
    }
  } while (read != 0);

  f_close(&f);
  return SIDFILE_OK;
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

extern "C" void __ISR(_TIMER_4_VECTOR, ipl1auto) T4Interrupt(void) {
  disk_timerproc();

  mT4ClearIntFlag();
}

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

  _TRISB2 = 0;              // MOS 6581 CS  (6)
  PPSOutput(3, RPA4, SDO2); // MOS 6581 SDO (12)
                            // MOS 6581 SCK (26)

  SpiChnOpen(SPI_CHANNEL2,
             (SpiOpenFlags)(SPI_OPEN_MODE8 | SPI_OPEN_MSTEN | SPI_OPEN_DISSDI),
             50);

  _TRISB13 = 0;
  _LATB13 = 1;              // SD CARD CS  (24)
  _TRISB11 = 1;             // SD CARD CD  (22)
  PPSOutput(2, RPB5, SDO1); // SD CARD SDO (14)
  PPSInput(2, SDI1, RPB8);  // SD CARD SDI (17)
                            // SD CARD SCK (25)

  SpiChnOpen(SPI_CHANNEL1,
             (SpiOpenFlags)(SPI_OPEN_MODE8 | SPI_OPEN_MSTEN | SPI_OPEN_CKE_REV),
             125);

  INTEnableSystemMultiVectoredInt();
  INTEnableInterrupts();

  nsleep(1000000);

  memory_init();

  FATFS fs;
  FRESULT fr = f_mount(&fs, "", 0);
  if (fr != FR_OK)
    halt();

  struct sid sid;
  int sidr = sid_read_file(&sid, "R-Type.sid");
  if (sidr != SIDFILE_OK)
    halt();

  uint8_t song_number = 0;
  sid_init_player(&sid, song_number);

  mos6502 cpu(memory_read, memory_write);

  cpu.Reset();
  cpu.Run(100000);

  while (1) {
    uint32_t c = ReadCoreTimer() + (sid_song_speed_ns(&sid, song_number) /
                                    (2000000000 / CLOCKFREQ));

    // trigger IRQ interrupt
    cpu.IRQ();

    // execute the player routine
    cpu.Run(0);

    while (ReadCoreTimer() < c)
      ;
  }

  return 0;
}