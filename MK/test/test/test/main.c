#define F_CPU 16000000UL // Частота МК (16 МГц)
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void USART_Init(uint16_t baud) {
	UBRR0 = F_CPU / 16 / 9600 - 1;  // 9600 бод при 8 МГц: (8000000/(16*9600))-1 = 51
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
	DDRB |= (1 << PINB5); // PB5 (D13) как выход
	USART_Init(103);    // 9600 бод при 16 МГц (UBRR = [F_CPU/(16*baud)]-1)
	
	while (1) {
		PORTB ^= (1 << PINB5); // Переключаем светодиод
		USART_SendString("LED toggled!\r\n");
		_delay_ms(1000);
	}
}