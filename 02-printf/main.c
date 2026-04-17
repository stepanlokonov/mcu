#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

#define DEVICE_NAME "my-pico-device"
#define DEVICE_VRSN "v0.0.1"

uint32_t global_variable = 0;
const uint32_t constant_variable = 42;

int main() {
    stdio_init_all();
    
    while(1) {
        printf("Hello World!\n");
        printf("device name: '%s', firmware version: %s\n", DEVICE_NAME, DEVICE_VRSN);
        
        uint64_t timestamp = time_us_64();
        printf("system timestamp: %llu us\n", timestamp);
        
        uint32_t stack_variable = 8888;
        printf("stack variable | addr = 0x%p | value = %u\n", &stack_variable, stack_variable);
        printf("stack variable | addr = 0x%p | value = %X\n", &stack_variable, stack_variable);
        printf("stack variable | addr = 0x%p | value = 0x%X\n", &stack_variable, stack_variable);
        
        global_variable++;
        printf("global variable | addr = 0x%p | value = %u\n", &global_variable, global_variable);
        
        uint32_t* heap_variable = (uint32_t*)malloc(sizeof(uint32_t));
        if (heap_variable != NULL) {
            *heap_variable = 5555;
            printf("heap variable | addr = 0x%p | value = %u\n", heap_variable, *heap_variable);
            free(heap_variable);
        }
        
        printf("constant variable | addr = 0x%p | value = %u\n", &constant_variable, constant_variable);
        
        // Безопасный вывод строковой константы
        printf("constant string | addr = 0x%p | first 4 bytes: 0x%02X%02X%02X%02X, [%s]\n", 
               DEVICE_NAME, 
               DEVICE_NAME[0], DEVICE_NAME[1], DEVICE_NAME[2], DEVICE_NAME[3],
               DEVICE_NAME);
        
        // Чтение регистра чипа (только для разрешённых адресов)
        // На RP2040 адрес 0x40000000 - это начало адресного пространства IO
        volatile uint32_t* chip_id_reg = (volatile uint32_t*)0x40000000;
        printf("reg chip id | addr = 0x%p | value = 0x%08X\n", chip_id_reg, *chip_id_reg);
        
        // ВАЖНО: Чтение по произвольному адресу 0x20002278 может вызвать Hard Fault
        // Этот адрес может быть недоступен для чтения
        printf("var by addr | addr = 0x%p | value = %u (MAY CAUSE FAULT)\n", 
               (void*)0x20002278, "UNSAFE");
        
        // Безопасный вывод адреса функции
        printf("main function | addr = 0x%p\n", main);
        
        sleep_ms(1000);
    }
    
    return 0;
}
