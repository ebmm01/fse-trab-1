#include <wiringPi.h> /* include wiringPi library */
#include <stdio.h>    
#include <softPwm.h>  /* include header file for software PWM */
#include <unistd.h>

int main(void){
	int PWM_pin = 4;		/* GPIO1 as per WiringPi,GPIO18 as per BCM */
	int intensity = 100;
	wiringPiSetup();		/* initialize wiringPi setup */
	pinMode(PWM_pin,OUTPUT);	/* set GPIO as output */
	softPwmCreate(PWM_pin,1,100);	/* set PWM channel along with range*/
	// while (1)
	//   {
	// 	for (intensity = 0; intensity < 101; intensity++)
	// 	{
	// 	  softPwmWrite (PWM_pin, intensity); /* change the value of PWM */
	// 	  delay (10) ;
	// 	}
	// 	delay(1);

	// 	for (intensity = 100; intensity >= 0; intensity--)
	// 	{
	// 	  softPwmWrite (PWM_pin, intensity);
	// 	  delay (10);
	// 	}
	// 	delay(1);
		
	// }

    softPwmWrite (PWM_pin, intensity);

    sleep(3);

    softPwmWrite (PWM_pin, 0);

    sleep(1);

    return 0;
}