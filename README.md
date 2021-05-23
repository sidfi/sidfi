# SIDfi

SIDfi is single-board self-contained SID-file player. SIDfi requires the original MOS 6581 chip or pin-compatible replacement such as [ARMSID](https://www.nobomi.cz/8bit/armsid/index_en.php). SIDfi plays tune files (such as [High Voltage SID Collection](https://hvsc.c64.org/)) from a FAT-formatted memory card and displays current tune information on a small LCD screen. Buttons on LCD screen can be used to control the playback.

## Design

- Emulates MOS 6502 using PIC32MX270F256 microcontroller
- Uses [Waveshare 1.44inch LCD HAT](https://www.waveshare.com/1.44inch-lcd-hat.htm) for display and control
- Uses [Waveshare Micro SD Storage Board](https://www.waveshare.com/micro-sd-storage-board.htm)
- Uses 2N3904 transistor for audio amplifier
- Standard 3mm headset jack for audio output
- USB 2.0 Type-B connector for 5V power supply (consumes 300mA)

## PCB rendering

![PCB](/hardware/sidfi/sidfi.png)