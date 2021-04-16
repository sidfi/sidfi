#include "cpu.h"
#include "mos6502.h"

#include <new>

static uint8_t mos6502_buffer[sizeof(mos6502)];

void cpu_init(cpu_read read, cpu_write write) {
  new (mos6502_buffer)
    mos6502((mos6502::BusRead)read, (mos6502::BusWrite)write);
}

void cpu_reset() { ((mos6502 *)mos6502_buffer)->Reset(); }

void cpu_irq() { ((mos6502 *)mos6502_buffer)->IRQ(); }

void cpu_run(cpu_break brk) {
  ((mos6502 *)mos6502_buffer)->Run((mos6502::Break)brk);
}
