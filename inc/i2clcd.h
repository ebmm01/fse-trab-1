#ifndef I2CLCD_H_
#define I2CLCD_H_

void lcd_init(void);
void lcd_byte(int bits, int mode);
void lcd_toggle_enable(int bits);

// added by Lewis
void typeInt(int i);
void typeFloat(float myFloat);
void lcdLoc(int line); //move cursor
void ClrLcd(void); // clr LCD return home
void typeln(const char *s);
void typeChar(char val);

int init_lcd();

void write_on_lcd(char* line_1, char* line_2);

#endif /* I2CLCD_H_ */


