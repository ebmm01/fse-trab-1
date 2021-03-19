#define INTERNAL_TEMP 0xC1
#define POTEN_TEMP 0xC2

int init_uart_connection();
void close_uart();
int send_message_to_uart(int solicitation_command);
float receive_temperature();
float get_temp(int temp_type);