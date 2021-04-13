#define _DISABLE_OPENADC10_CONFIGPORT_WARNING
#define _SUPPRESS_PLIB_WARNING
#include <plib.h>

#include "st7735s_compat.h"

void SPI_Init(void) {
	// Pin_Low(CS);
}

void Pin_CS_Low(void) {
  _LATB0 = 0;
}

void Pin_CS_High(void) {
  _LATB0 = 1;
}

void Pin_RES_High(void) {
  _LATB3 = 1;
}

void Pin_RES_Low(void) {
  _LATB3 = 0;
}

void Pin_DC_High(void) {
  _LATB1 = 1;
}

void Pin_DC_Low(void) {
  _LATB1 = 0;
}

void Pin_BLK_Pct(uint8_t pct) {
}

void SPI_send(uint16_t len, uint8_t *data) {
	Pin_CS_Low();
	while (len--) {
        SPI2BUF = *(data++);
        while (SPI2STATbits.SPIBUSY)
          ;
    }

	Pin_CS_High();
}

void SPI_TransmitCmd(uint16_t len, uint8_t *data) {
    Pin_DC_Low();
    SPI_send(len, data);
}

void SPI_TransmitData(uint16_t len, uint8_t *data) {
    Pin_DC_High();
    SPI_send(len, data);
}

void SPI_Transmit(uint16_t len, uint8_t *data) {
    SPI_TransmitCmd(1, data++);
    if (--len)
       SPI_TransmitData(len, data);
}

#define CLOCKFREQ (50000000ul)  // This is set in in Configuration Bits.h

void _Delay(uint32_t d) {
    uint32_t c =
        ReadCoreTimer() + (((uint32_t)(d * 1000000)) / (2000000000 / CLOCKFREQ));
    while (ReadCoreTimer() < c)
      ;
}

