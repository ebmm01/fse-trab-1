#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h>        //Used for UART
#include <string.h>
#include "crc16.h"

#define UART_ADRESS "/dev/serial0"

#define ARDUINO_ADRESS 0x01
#define ARDUINO_CODE 0x23

#define WAIT_TIME_MICROSECONDS 100000 // 100ms ou 0.1s


int uart0_filestream = -1;

int init_uart_connection() {

    uart0_filestream = open(UART_ADRESS, O_RDWR | O_NOCTTY | O_NDELAY);      //Open in non blocking read/write mode
    
    if (uart0_filestream == -1)
        return 0;

    struct termios options;

    tcgetattr(uart0_filestream, &options);
    
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;     //<Set baud rate B115200
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);

    return 1;
}

void close_uart() {
    close(uart0_filestream);
}

int send_message_to_uart(int solicitation_command) {
    unsigned char tx_buffer[15];
    unsigned char *p_tx_buffer;
    
    p_tx_buffer = &tx_buffer[0];
    
    *p_tx_buffer++ = ARDUINO_ADRESS;

    // Código da função
    *p_tx_buffer++ = ARDUINO_CODE;

    // Dados a serem enviados
    *p_tx_buffer++ = solicitation_command;
    
    *p_tx_buffer++ = 9;
    *p_tx_buffer++ = 0;
    *p_tx_buffer++ = 1;
    *p_tx_buffer++ = 1;

    short crc = calcula_CRC(tx_buffer,  (p_tx_buffer - &tx_buffer[0]));

    *p_tx_buffer++ = crc & 0xff;

    *p_tx_buffer++ = (crc >> 8) & 0xff;

    if (uart0_filestream != -1)
    {
        int count = write(uart0_filestream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));
        if (count < 0)
            return -1; // ERROR
        else
            return 1;
    }
    else
        return -1;
}

float receive_temperature() {
    if (uart0_filestream != -1)
    {
        unsigned char rx_buffer[256];
        int rx_length = read(uart0_filestream, (void*)rx_buffer, 255);      //Filestream, buffer to store in, number of bytes to read (max)
        if (rx_length < 0)
        {
            return -1;
        }
        else if (rx_length == 0)
        {
            return -1;
        }
        else
        {
            rx_buffer[rx_length] = '\0';

            if (validate_CRC(rx_buffer, rx_length)) {
                float currentInteger;
                memcpy(&currentInteger, rx_buffer + 3, 4);

                return currentInteger;
            }
            else {
                return -1;
            }
        }
    }
    return -1;
}

float get_temp(int temp_type) {
    init_uart_connection();
    send_message_to_uart(temp_type);
    usleep(WAIT_TIME_MICROSECONDS);

    float received_value = receive_temperature();

    close_uart();

    return received_value;
}