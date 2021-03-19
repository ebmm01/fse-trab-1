#include <stdio.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h>        //Used for UART
#include <string.h>
#include "crc16.h"
#include "uart.h"
#include "gpio.h"
#include "i2cbme.h"
#include "pid.h"
#include "i2clcd.c"

void handle_temperature_by_signal_control(float signal_control);
void handle_init();
void handle_close();

int main(void) {
    // Configurações de inicialização
    handle_init();

    int i = 1;
    float internal_temp, ref_temp, external_temp, signal_control;

    while(i<20) {
        char line_1[20], line_2[20];

        // Obtenho as temperaturas
        internal_temp = get_temp(INTERNAL_TEMP);
        ref_temp = get_temp(POTEN_TEMP);
        external_temp = get_external_temperature();

        // Atualizo a temperatura de referência
        pid_atualiza_referencia(ref_temp);

        // Atualizo o sinal de controle
        signal_control = pid_controle(internal_temp);

        printf("\nSinal de controle:: %.2f\n", signal_control);
        printf("\nTI:: %.2f, TR:: %.2f, TE:: %.2f", internal_temp, ref_temp, external_temp);

        sprintf(line_1, "TI:%.2f TR:%.2f", internal_temp, ref_temp);
        sprintf(line_2, "TE:%.2f ::", external_temp);
        
        // Escrevo no lcd
        write_on_lcd(line_1, line_2);

        // Controlo a temperatura
        handle_temperature_by_signal_control(signal_control);

        sleep(1);
        i++;
    }
    

    handle_close();
    
    return 0;
}

void handle_init() {
    init_bme();
    init_lcd();
    pid_configura_constantes(5, 1, 5);
}

void handle_close() {
    close_bme();
    set_fan_intensity(0);
    set_resistor_intensity(0);
}

void handle_temperature_by_signal_control(float signal_control) {
	if (signal_control > 0 ) {
		set_resistor_intensity((int) signal_control);
		set_fan_intensity(0);
	}
	else {
		if (signal_control < FAN_THRESHOLD) {
			set_fan_intensity((int) (-1*signal_control));
		}
		else {
			set_fan_intensity(0);
		}
		set_resistor_intensity(0);
	}
}