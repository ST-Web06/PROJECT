
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "LCD.h"

#define BAUD 9600
#define LED_PIN PB5

// Инициализация UART
void uart_init() {
	UBRR0H = (F_CPU/16/BAUD-1) >> 8;
	UBRR0L = (F_CPU/16/BAUD-1);
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_send(char c) {
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}

void uart_print(const char* str) {
	while (*str) uart_send(*str++);
}

int main() {
	// Инициализация периферии
	DDRB |= (1 << LED_PIN);  // Настройка PB5 как выход
	PORTB &= ~(1 << LED_PIN); // Изначально выключен
	
	uart_init();    // Инициализация UART
	lcd_init();     // Инициализация LCD
	lcd_clear();    // Очистка дисплея
	
	// Стартовое сообщение
	lcd_print("System Ready");
	uart_print("System Ready\r\n");
	
	char buffer[32];
	uint8_t idx = 0;
	
	while (1) {
		// Обработка входящих команд
		if (UCSR0A & (1 << RXC0)) {
			char c = UDR0;
			
			if (c == '\r' || c == '\n') {
				buffer[idx] = '\0';
				
				// Команда для светодиода
				if (strcmp(buffer, "LED_ON") == 0) {
					PORTB |= (1 << LED_PIN);
					uart_print("LED:ON\r\n");
				}
				else if (strcmp(buffer, "LED_OFF") == 0) {
					PORTB &= ~(1 << LED_PIN);
					uart_print("LED:OFF\r\n");
				}
				// Команда для LCD
				else if (strncmp(buffer, "LCD:", 4) == 0) {
					lcd_clear();
					lcd_print(buffer + 4); // Вывод текста после "LCD:"
					uart_print("LCD_OK:");
					uart_print(buffer + 4);
					uart_print("\r\n");
				}
				
				idx = 0;
			}
			else if (idx < sizeof(buffer)-1) {
				buffer[idx++] = c;
			}
		}
	}
}