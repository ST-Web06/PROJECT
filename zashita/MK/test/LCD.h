#ifndef LCD_H
#define LCD_H

#include <avr/io.h>
#include <util/delay.h>

#define LCD_RS PD7
#define LCD_EN PD5
#define LCD_D4 PC2
#define LCD_D5 PC3
#define LCD_D6 PC4
#define LCD_D7 PC5

void lcd_init();
void lcd_cmd(uint8_t cmd);
void lcd_data(uint8_t data);
void lcd_print(const char *str);
void lcd_clear();
void lcd_set_cursor(uint8_t row, uint8_t col);

#endif