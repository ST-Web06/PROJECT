#include "LCD.h"

static void lcd_pulse_enable() {
	PORTD |= (1 << LCD_EN);
	_delay_us(1);
	PORTD &= ~(1 << LCD_EN);
	_delay_us(100);
}

static void lcd_send(uint8_t value, uint8_t mode) {
	if (mode) PORTD |= (1 << LCD_RS);
	else PORTD &= ~(1 << LCD_RS);
	
	// Старшие 4 бита
	PORTC = (PORTC & 0x03) | ((value & 0xF0) >> 2);
	lcd_pulse_enable();
	
	// Младшие 4 бита
	PORTC = (PORTC & 0x03) | ((value & 0x0F) << 2);
	lcd_pulse_enable();
	
	_delay_us(100);
}

void lcd_init() {
	// Настройка портов
	DDRD |= (1 << LCD_RS) | (1 << LCD_EN);
	DDRC |= (1 << LCD_D4) | (1 << LCD_D5) | (1 << LCD_D6) | (1 << LCD_D7);
	
	_delay_ms(50);
	
	// Инициализация 4-битного режима
	lcd_send(0x33, 0);
	lcd_send(0x32, 0);
	lcd_send(0x28, 0); // 4-bit, 2-line, 5x8 font
	lcd_send(0x0C, 0); // Display on, cursor off
	lcd_send(0x06, 0); // Auto-increment
	lcd_clear();
}

void lcd_cmd(uint8_t cmd) {
	lcd_send(cmd, 0);
}

void lcd_data(uint8_t data) {
	lcd_send(data, 1);
}

void lcd_print(const char *str) {
	while (*str) lcd_data(*str++);
}

void lcd_clear() {
	lcd_cmd(0x01);
	_delay_ms(2);
}

void lcd_set_cursor(uint8_t row, uint8_t col) {
	uint8_t address = (row == 0) ? 0x80 : 0xC0;
	lcd_cmd(address + col);
	_delay_us(50);
}