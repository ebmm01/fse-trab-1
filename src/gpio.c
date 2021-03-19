#include <wiringPi.h> /* include wiringPi library */
#include <stdio.h>    
#include <softPwm.h>  /* include header file for software PWM */
#include <unistd.h>

#define DEVICE_RESISTOR 4
#define DEVICE_FAN 5

void __handle_gpio_device(int device_type, int intensity) {
	wiringPiSetup();		/* initialize wiringPi setup */
	pinMode(device_type, OUTPUT);	/* set GPIO as output */
	softPwmCreate(device_type, 1, 100);	/* set PWM channel along with range*/

    softPwmWrite (device_type, intensity);
	usleep(10000);
}

/* Utilitary function. */
void set_resistor_intensity(int intensity) {
	__handle_gpio_device(DEVICE_RESISTOR, intensity);
}

/* Utilitary function. */
void set_fan_intensity(int intensity) {
	__handle_gpio_device(DEVICE_FAN, intensity);
}