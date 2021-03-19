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

void handle_temperature_by_signal_control(float signal_control);
void handle_init();
void handle_close();

int fan_value, resistor_value;

int main(void) {
    // Configurações de inicialização
    handle_init();

    int i = 1;
    float internal_temp, ref_temp, external_temp, signal_control;

    signal(SIGINT, handle_close);
    signal(SIGKILL, handle_close);

    while(1) {
        char line_1[20], line_2[20];

        // Obtenho as temperaturas
        internal_temp = get_temp(INTERNAL_TEMP);
        ref_temp = get_temp(POTEN_TEMP);
        external_temp = get_external_temperature();

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

            printf("\nSinal de controle:: %.2f", signal_control);
            printf("\nTI:: %.2f, TR:: %.2f, TE:: %.2f", internal_temp, ref_temp, external_temp);

            sprintf(line_1, "TI:%.2f TR:%.2f", internal_temp, ref_temp);
            sprintf(line_2, "TE:%.2f ::", external_temp);
            
            // Escrevo no lcd
            write_on_lcd(line_1, line_2);

            // Controlo a temperatura
            handle_temperature_by_signal_control(signal_control);

            //Escrevo o log no csv
            write_csv_on_file(internal_temp, external_temp, ref_temp, resistor_value, fan_value);

            usleep(800000);
            i++;
        }
        else {
            printf("\n Dados inválidos recebidos \n");
        }
    }

    return 0;
}

void handle_init() {
    init_bme();
    init_lcd();
    pid_configura_constantes(5, 1, 5);
    handle_file_creation();
}

void handle_close() {
    printf("\nEncerrando execução...\n");
    write_on_lcd("", "");
    set_resistor_intensity(0);
    set_fan_intensity(0);
    close_bme();
    exit(0);
}

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