#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <driver/gpio.h>
#define LED GPIO_NUM_2

void app_main(void)
{
	gpio_set_direction(LED,GPIO_MODE_OUTPUT);

	while (1)
	{
        printf("LED ligado!\n");
        gpio_set_level(LED,1);
        usleep(1000000);
        printf("LED Desligado\n");
        gpio_set_level(LED,0);
        usleep(1000000);
    }
}
