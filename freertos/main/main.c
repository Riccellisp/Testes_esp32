
// - Libraries
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "driver/gpio.h"
#include "esp_system.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "driver/gpio.h"
#include "esp_system.h"
#include "driver/ledc.h"

// HW mapping
#define LED GPIO_NUM_2
#define BT1 GPIO_NUM_23
#define BT2 GPIO_NUM_22
// --- Macros
#define set_duty(duty)    ledc_set_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,duty)

#define upt_duty          ledc_update_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0)


// function prototypes
uint16_t pwm_control(void);
void pwm_init(void);

// Tasks

void Task1()
{
	while(1)
	{
		set_duty(pwm_control());
		upt_duty;
		vTaskDelay(10/portTICK_PERIOD_MS);
	}// end while

}// end task1

void Task2()
{
	static bool state = 0;

	while(1)
	{
		printf("duty percentual: %.2f\n",pwm_control()/8192.0);
		gpio_set_level(LED,state);
		state = !state;
		vTaskDelay(1000/portTICK_PERIOD_MS);
	}// end while

}// end task2


// --- main
void app_main(void)
{
	pwm_init();

	set_duty(4095);

	upt_duty;

	esp_rom_gpio_pad_select_gpio(LED);
	gpio_set_direction(LED, GPIO_MODE_OUTPUT);

	esp_rom_gpio_pad_select_gpio(BT1);
	gpio_set_direction(BT1, GPIO_MODE_INPUT);

	esp_rom_gpio_pad_select_gpio(BT2);
	gpio_set_direction(BT2, GPIO_MODE_INPUT);

	xTaskCreate(&Task1, // task addr
				"pwm",  // identification
				4096,   // bytes
				NULL,   //send a parameter to the task
				5,      // priority (1 to 5)
				NULL);  // handler

	xTaskCreate(&Task2, // task addr
					"percent",  // identification
					2048,   // bytes
					NULL,   //send a parameter to the task
					1,      // priority (1 to 5)
					NULL);  // handler

}// END MAIN

uint16_t pwm_control(void)
{
	static uint16_t dutycyc = 4095;

	if(!gpio_get_level(BT1))
		dutycyc+=10;
	if(!gpio_get_level(BT2))
		dutycyc-=10;
	return dutycyc&0x1FFF;
}// end pwm_control


void pwm_init(void)
{
	ledc_timer_config_t pwm_timer = {
			.speed_mode = LEDC_LOW_SPEED_MODE,
			.timer_num = LEDC_TIMER_0,
			.duty_resolution = LEDC_TIMER_13_BIT,
			.freq_hz = 1000,
			.clk_cfg = LEDC_AUTO_CLK
	};

	ledc_timer_config(&pwm_timer);

	ledc_channel_config_t pwm_ch ={
			.speed_mode = LEDC_LOW_SPEED_MODE,
			.channel = LEDC_CHANNEL_0,
			.timer_sel = LEDC_TIMER_0,
			.intr_type = LEDC_INTR_DISABLE,
			.gpio_num = 15,
			.duty = 0,
			.hpoint = 0
	};

	ledc_channel_config(&pwm_ch);
}




