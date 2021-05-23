# SIDfi

![Board](/doc/board.jpg)

SIDfi is a single-board self-contained SID file player. SIDfi requires the original MOS 6581 chip or a pin-compatible replacement such as [ARMSID](https://www.nobomi.cz/8bit/armsid/index_en.php). SIDfi plays tune files such as [High Voltage SID Collection](https://hvsc.c64.org/) from a FAT-formatted memory card and displays current tune information on a small LCD screen. Buttons on the LCD screen can be used to control the playback.

## Design Notes

- Emulates MOS 6502 using PIC32MX270F256 microcontroller using [MOS6502 library](https://github.com/gianlucag/mos6502);
- [Waveshare 1.44inch LCD HAT](https://www.waveshare.com/1.44inch-lcd-hat.htm) and [ST7735S library](https://github.com/bersch/ST7735S) for display and control;
- [Waveshare Micro SD Storage Board](https://www.waveshare.com/micro-sd-storage-board.htm) and [FatFs library](http://www.elm-chan.org/fsw/ff/00index_e.html) for memory card support;
- 2N3904 transistor for audio amplifier;
- Standard 3mm headset jack for audio output;
- USB 2.0 Type-B connector for 5V power supply (consumes 300mA).

## PCB rendering

![PCB](/hardware/sidfi/sidfi.png)