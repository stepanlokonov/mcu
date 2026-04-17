#include <stdio.h>
#include "pico/stdlib.h"

#define LED_PIN 25
#define DEVICE_NAME "Echo Device"
#define VERSION "1.0.0"

int main()
{
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, false);
    
    while (1)
    {
        char symbol = getchar();
        printf("received char: %c [ ASCII code: %d ]\n", symbol, symbol);
    
        switch(symbol)
        {
            case 'e':
                gpio_put(LED_PIN, true);
                printf("led enable done\n");
                break;
            case 'd':
                gpio_put(LED_PIN, false);
                printf("led disable done\n");
                break;
            case 'v':
            {
                printf("Device name: %s\n", DEVICE_NAME);
                printf("Version: %s\n", VERSION);
                break;
            }
            default:
                break;
        }
    }
    return 0;
}