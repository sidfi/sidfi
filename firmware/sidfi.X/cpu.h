#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

typedef void (*cpu_write)(uint16_t, uint8_t);
typedef uint8_t (*cpu_read)(uint16_t);
typedef bool (*cpu_break)();

void cpu_init(cpu_read read, cpu_write write);
void cpu_reset();
void cpu_irq();
void cpu_run(cpu_break brk);

#ifdef __cplusplus
}
#endif