#define F_CPU 16000000UL 
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void USART_Init(uint16_t baud) {
	UBRR0 = F_CPU / 16 / 9600 - 1;  
	UCSR0B = (1 << TXEN0) | (1 << RXEN0); // Включаем передатчик и приёмник
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8 бит данных, 1 стоп-бит
}

void USART_SendChar(char data) {
	while (!(UCSR0A & (1 << UDRE0))); // Ждём готовности
	UDR0 = data;                       // Отправляем символ
}

void USART_SendString(const char *str) {
	while (*str) {
		USART_SendChar(*str++);
	}
}

int main(void) {
	DDRB |= (1 << PINB5); 
	USART_Init(103);    
	
	while (1) {
		PORTB ^= (1 << PINB5); // Переключаем светодиод
		USART_SendString("LED toggled!\r\n");
		_delay_ms(1000);
	}
}