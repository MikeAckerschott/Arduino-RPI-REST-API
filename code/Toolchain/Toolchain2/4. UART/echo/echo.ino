#define F_CPU 16000000UL  // 16 MHz for Arduino Nano

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

// Define baud rate
#define USART_BAUDRATE 9600   
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

volatile unsigned char value;  
volatile bool new_data_received = false;

ISR(USART_RX_vect){
  value = UDR0;             // read UART register into value. 19.7.1
  USART_SendByte(value);  // send value back to terminal
}

// Datasheet 19.5
void USART_Init(void){
  // Set baud rate
  UBRR0H = (BAUD_PRESCALE >> 8); 
  UBRR0L = BAUD_PRESCALE; 

  // Enable receiver and transmitter 
  UCSR0B = (1 << RXEN0) | (1 << TXEN0)

  // Enable receive complete interrupt  19.7.3 
  UCSR0B |= (1 << RXCIE0);
}

void USART_SendByte(uint8_t u8Data){
  // Wait until last byte has been transmitted. 19.6.1
  while((UCSR0A & (1 << UDRE0)) == 0);

  // Transmit data. 19.6.1
  UDR0 = u8Data;
}

int main(void){
  USART_Init();  // Initialise USART
  sei();         // enable all interrupts
  value = 'A';   // initial value
  
  while (1) {
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_mode();
  }
  
}
