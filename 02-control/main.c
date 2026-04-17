#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "stdio-task/stdio-task.h"
#include "protocol-task/protocol-task.h"
#include "led-task/led-task.h"

#define DEVICE_NAME "My RP2040 Device"
#define DEVICE_VRSN "1.0.0"

// Прототипы функций
void version_callback(const char* args);
void led_on_callback(const char* args);
void led_off_callback(const char* args);
void led_blink_callback(const char* args);
void led_blink_set_period_ms_callback(const char* args);
void help_callback(const char* args);
void mem_callback(const char* args);
void wmem_callback(const char* args);

// Внешние переменные из led-task.c
extern uint LED_BLINK_PERIOD_US;
extern led_state_t led_state;

// Массив команд
api_t device_api[] =
{
    {"version", version_callback, "get device name and firmware version"},
    {"on", led_on_callback, "turn LED on"},
    {"off", led_off_callback, "turn LED off"},
    {"blink", led_blink_callback, "make LED blink"},
    {"set_period", led_blink_set_period_ms_callback, "set blink period in milliseconds"},
    {"mem", mem_callback, "read memory at address (hex)"},
    {"wmem", wmem_callback, "write memory at address (hex) with value (hex)"},
    {"help", help_callback, "print all available commands"},
    {NULL, NULL, NULL},
};

void version_callback(const char* args)
{
    printf("device name: '%s', firmware version: %s\n", DEVICE_NAME, DEVICE_VRSN);
}

void led_on_callback(const char* args)
{
    printf("LED ON\n");
    led_task_state_set(LED_STATE_ON);
}

void led_off_callback(const char* args)
{
    printf("LED OFF\n");
    led_task_state_set(LED_STATE_OFF);
}

void led_blink_callback(const char* args)
{
    printf("LED BLINK\n");
    led_task_state_set(LED_STATE_BLINK);
}

void led_blink_set_period_ms_callback(const char* args)
{
    uint32_t period_ms = 0;
    sscanf(args, "%u", &period_ms);
    
    if (period_ms == 0)
    {
        printf("error: invalid period value. Use: set_period <milliseconds>\n");
        return;
    }
    
    led_task_set_blink_period_ms(period_ms);
    printf("blink period set to %u ms\n", period_ms);
}

void mem_callback(const char* args)
{
    uint32_t address = 0;
    if (sscanf(args, "%x", &address) != 1)
    {
        printf("error: invalid address. Use: mem <hex_address>\n");
        return;
    }
    
    uint32_t value = *(volatile uint32_t*)address;
    printf("mem 0x%08X = 0x%08X (%u)\n", address, value, value);
}

void wmem_callback(const char* args)
{
    uint32_t address = 0;
    uint32_t value = 0;
    
    if (sscanf(args, "%x %x", &address, &value) != 2)
    {
        printf("error: invalid arguments. Use: wmem <hex_address> <hex_value>\n");
        return;
    }
    
    *(volatile uint32_t*)address = value;
    printf("wmem 0x%08X = 0x%08X\n", address, value);
}

void help_callback(const char* args)
{
    printf("Available commands:\n");
    for (int i = 0; device_api[i].command_name != NULL; i++)
    {
        printf("  %s - %s\n", device_api[i].command_name, device_api[i].command_help);
    }
}

int main() {
    stdio_init_all();
    stdio_task_init();
    protocol_task_init(device_api);
    led_task_init();

    while (1) {
        char* command = stdio_task_handle();
        if (command != NULL) {
            protocol_task_handle(command);
        }
        led_task_handler();
    }

    return 0;
}