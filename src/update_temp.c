#include <stdio.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h>        //Used for UART
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include "crc16.h"
#include "uart.h"
#include "gpio.h"
#include "i2cbme.h"
#include "pid.h"
#include "i2clcd.h"
#include "csv.h"

volatile int fan_value = 0, resistor_value = 0;

volatile float internal_temp = 0, ref_temp = 0, external_temp = 0, signal_control = 0, pot_temp = 0;

void handle_temperature_by_signal_control(float signal_control) {
    if (signal_control > 0 ) {
		resistor_value = (int) signal_control;
		fan_value = 0;
	}
	else {
		if (signal_control < FAN_THRESHOLD) {
			fan_value = (int) signal_control;
		}
		else {
			fan_value = 0;
		}
		resistor_value = 0;
	}
    set_fan_intensity((-1*fan_value));
    set_resistor_intensity(resistor_value);
}

void update_temp(int type, int manual_ref_value) {
    char line_1[20], line_2[20];

    // Obtenho as temperaturas
    internal_temp = get_temp(INTERNAL_TEMP);
    external_temp = get_external_temperature();
    pot_temp = get_temp(POTEN_TEMP);

    if (type == 0)
        ref_temp = pot_temp;
    else 
        ref_temp = (float) manual_ref_value;

    /** Validação dos valores recebidos. As funções chamadas garantem que
     * se qualquer temperatura < 0, então o valor recebido é inválido.
     */
    if (internal_temp >= 0 &&
        ref_temp >= 0 && 
        external_temp >= 0) {

        // Atualizo a temperatura de referência
        pid_atualiza_referencia(ref_temp);

        // Atualizo o sinal de controle
        signal_control = pid_controle(internal_temp);

        sprintf(line_1, "TI:%.2f TR:%.2f", internal_temp, ref_temp);
        sprintf(line_2, "TE:%.2f ::", external_temp);
        
        // Escrevo no lcd
        write_on_lcd(line_1, line_2);

        // Controlo a temperatura
        handle_temperature_by_signal_control(signal_control);

        //Escrevo o log no csv
        write_csv_on_file(internal_temp, external_temp, ref_temp, resistor_value, fan_value);
    }
}