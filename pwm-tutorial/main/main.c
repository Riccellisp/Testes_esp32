
// - Libraries
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "driver/gpio.h"
#include "esp_system.h"
#include "driver/ledc.h"

// HW mapping
#define LED GPIO_NUM_2

// --- Macros
#define set_duty(duty)    ledc_set_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,duty)

#define upt_duty          ledc_update_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0)


// function prototypes
void pwm_init(void);

// --- main
void app_main(void)
{
	pwm_init();

	set_duty(4095);

	upt_duty;

	gpio_set_direction(LED, GPIO_MODE_OUTPUT);

	while(1)
	{
		printf("LED LIGADO \n");
		gpio_set_level(LED,1);

		set_duty(1000);

		upt_duty;

		usleep(800000);

		printf("LED DESLIGADO \n");
		gpio_set_level(LED,0);

		set_duty(7000);

		upt_duty;

		usleep(800000);
	}// END WHILE

}// END MAIN

void pwm_init(void)
{
	ledc_timer_config_t pwm_timer = {
			.speed_mode = LEDC_LOW_SPEED_MODE,
			.timer_num = LEDC_TIMER_0,
			.duty_resolution = LEDC_TIMER_13_BIT,
			.freq_hz = 2000,
			.clk_cfg = LEDC_AUTO_CLK
	};

	ledc_timer_config(&pwm_timer);

	ledc_channel_config_t pwm_ch ={
			.speed_mode = LEDC_LOW_SPEED_MODE,
			.channel = LEDC_CHANNEL_0,
			.timer_sel = LEDC_TIMER_0,
			.intr_type = LEDC_INTR_DISABLE,
			.gpio_num = 13,
			.duty = 0,
			.hpoint = 0
	};

	ledc_channel_config(&pwm_ch);
}




