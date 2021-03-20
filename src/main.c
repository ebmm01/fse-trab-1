#include <stdio.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h>        //Used for UART
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include "crc16.h"
#include "uart.h"
#include "gpio.h"
#include "i2cbme.h"
#include "pid.h"
#include "i2clcd.h"
#include "csv.h"
#include "menu.h"
#include "update_temp.h"

void handle_temperature_by_signal_control(float signal_control);
void handle_init();
void handle_close();

pthread_t thread;

int main(void) {
    // Configurações de inicialização
    handle_init();

    
    signal(SIGINT, handle_close);

    pthread_create(&thread, NULL, &execute_menu, NULL); 
    pthread_detach(thread); 

    while(1) {
        update_temp(highlight, manual_temp);
        usleep(800000);
    }

    handle_close();

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
    pthread_cancel(thread);
    write_on_lcd("", "");
    set_resistor_intensity(0);
    set_fan_intensity(0);
    close_bme();
    exit(0);
}
