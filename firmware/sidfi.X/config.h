#ifndef CONFIGURATION_BITS_H
#define CONFIGURATION_BITS_H

#pragma config FNOSC = PRIPLL                                       // Oscillator Selection
                                                                    // PRI	is Primary oscillator (XT, HS, EC)
                                                                    // PRIPLL	is Primary oscillator (XT, HS, EC) w/ PLL
                                                                    // SOSC	is Secondary oscillator
                                                                    // LPRC	is Low power RC oscillator
                                                                    // FRC	is Fast RC oscillator
                                                                    // FRCPLL	is Fast RC oscillator w/ PLL
                                                                    // FRCDIV16	is Fast RC oscillator with divide by 16
                                                                    // FRCDIV	is Fast RC oscillator with divide

#pragma config POSCMOD = HS                                         // Primary Oscillator Selection
                                                                    // HS	is HS oscillator
                                                                    // EC	is EC oscillator
                                                                    // XT	is XT oscillator
                                                                    // OFF	is Disabled

#pragma config FPLLIDIV = DIV_4                                     // PLL Input Divide by 1, 2, 3, 4, 5, 6, 10 or 12
#pragma config FPLLMUL = MUL_20                                     // PLL Multiply by 15, 16, 17, 18, 19, 20, 21 or 24
#pragma config FPLLODIV = DIV_2                                     // PLL Output Divide by 1, 2, 4, 8, 16, 32, 64, or 256

#pragma config FPBDIV = DIV_1                                       // Peripheral Bus Clock Divide by 1, 2, 4 or 8

#pragma config FSOSCEN = OFF                                        // Secondary oscillator OFF or ON
#pragma config IESO = OFF                                           // Internal External Switchover (Two-Speed Start-up) OFF or ON
#pragma config OSCIOFNC = OFF                                       // CLKO output signal active on the OSCO pin. Select ON or OFF

#pragma config FCKSM = CSDCMD                                       // Clock Switching and Monitor Selection
                                                                    // CSECME	is Clock Switching Enabled, Clock Monitoring Enabled
                                                                    // CSECMD	is Clock Switching Enabled, Clock Monitoring Disabled
                                                                    // CSDCMD	is Clock Switching Disabled, Clock Monitoring Disabled


#pragma config PMDL1WAY = OFF                                       // Peripheral Module Disable Configuration
#pragma config IOL1WAY = OFF                                        // Peripheral Pin Select Configuration
                                                                    // ON        Allow only one reconfiguration (default)
                                                                    // OFF       Allow multiple reconfigurations

#pragma config JTAGEN = OFF                                         // JTAG Enable

#ifdef __DEBUG
#pragma config DEBUG = ON                                           // Background Debugger ON or OFF
#else
#pragma config DEBUG = OFF                                          // Background Debugger ON or OFF
#endif

#pragma config UPLLEN = ON                                          // USB PLL ON or OFF
#pragma config UPLLIDIV = DIV_5                                     // USB PLL Input Divide by 1, 2, 3, 4, 5, 6, 10 or 12

#pragma config FVBUSONIO = OFF                                      // USB VBUS_ON pin control
#pragma config FUSBIDIO = OFF                                       // USB USBID pin control

#pragma config FWDTEN = OFF                                         // Watchdog Timer ON or OFF
#pragma config WDTPS = PS128                                        // Watchdog Timer Postscale from 1:1 to 1:1,048,576
#pragma config WINDIS = OFF                                         // Watchdog Timer Window Enable (ON = Window Mode, OFF = Non Window Mode)
#pragma config FWDTWINSZ = WINSZ_75                                 // Watchdog Timer Window Size (75%, 50%, 37%, 25%)

#pragma config CP = OFF                                             // Code Protect Enable ON or OFF (prevents ANY read/write)
#pragma config BWP = OFF                                            // Boot Flash Write Protect OFF

#pragma config PWP = OFF                                            // Program Flash Write Protect ON, OFF or PWP1K to PWP32K in steps of 1K

#pragma config ICESEL = ICS_PGx3                                    // ICE/ICD Communications Channel Select

#endif  // CONFIGURATION_BITS_H