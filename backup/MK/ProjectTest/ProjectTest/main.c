#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define UBRR_VALUE ((F_CPU/16/BAUD_RATE)-1)

#define LED_DDR DDRB
#define LED_PORT PORTB
#define LED_PIN PB5

#define CMD_BUFFER_SIZE 32
char cmdBuffer[CMD_BUFFER_SIZE];
uint8_t cmdIndex = 0;

void USART_Init() {
	UBRR0H = (uint8_t)(UBRR_VALUE>>8);
	UBRR0L = (uint8_t)UBRR_VALUE;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
}

void USART_SendChar(char c) {
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = c;
}

void USART_SendString(const char* str) {
	while(*str) USART_SendChar(*str++);
}

void ProcessCommand(const char* cmd) {
	
	if(strcmp(cmd, "LED_ON") == 0) {
		LED_PORT |= (1<<LED_PIN);
		USART_SendString("LED: ON\r\n");
	}
	else if(strcmp(cmd, "LED_OFF") == 0) {
		LED_PORT &= ~(1<<LED_PIN);
		USART_SendString("LED: OFF\r\n");
	}
	
	
	else if(strncmp(cmd, "TEXT:", 5) == 0) {
		USART_SendString("DISPLAY: ");
		USART_SendString(cmd+5);
		USART_SendString("\r\n");
	}
	
	
	else {
		USART_SendString("ERROR: Unknown command '");
		USART_SendString(cmd);
		USART_SendString("'\r\n");
	}
}

int main(void) {
	
	LED_DDR |= (1<<LED_PIN);
	USART_Init();
	
	
	USART_SendString("\r\n=== SYSTEM READY ===\r\n");
	USART_SendString("Commands:\r\n");
	USART_SendString("LED_ON, LED_OFF, TEXT:your_message\r\n");

	while(1) {
		
		if(UCSR0A & (1<<RXC0)) {
			char c = UDR0;
			
			if(c == '\r' || c == '\n') {
				if(cmdIndex > 0) {
					cmdBuffer[cmdIndex] = '\0';
					ProcessCommand(cmdBuffer);
					cmdIndex = 0;
				}
			}
			else if(cmdIndex < CMD_BUFFER_SIZE-1) {
				cmdBuffer[cmdIndex++] = c;
			}
		}
		
		
		_delay_ms(10);
	}
}