#define F_CPU 16000000UL  // 16 MHz for Arduino Nano

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Define baud rate
#define USART_BAUDRATE 9600   
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

volatile unsigned char value;  
volatile bool new_data_received = false;

ISR(USART_RX_vect){
  value = UDR0;             // read UART register into value
  UDR0 = value;             // echo the received character
  new_data_received = true; // set flag to indicate new data received
}

void USART_Init(void){
  // Set baud rate
  UBRR0H = (BAUD_PRESCALE >> 8); 
  UBRR0L = BAUD_PRESCALE; 

  // Enable receiver and transmitter and receive complete interrupt 
  UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
}

void USART_SendByte(uint8_t u8Data){
  // Wait until last byte has been transmitted
  while((UCSR0A & (1 << UDRE0)) == 0);

  // Transmit data
  UDR0 = u8Data;
}

uint8_t USART_ReceiveByte(){
  while((UCSR0A & (1 << RXC0)) == 0);
  return UDR0;
}

void Led_init(void){
  // Set PORTB as output, all off
  DDRB = 0xFF;       
  PORTB = 0xFF;        
}

int main(void){
  USART_Init();  // Initialise USART
  sei();         // enable all interrupts
  Led_init();    // init LEDs for testing
  value = 'A';   // initial value
  PORTB = ~value; // 0 = LED on
  
  for(;;){    // Repeat indefinitely
    if (new_data_received) {
      USART_SendByte(value);  // send value 
      new_data_received = false; // reset flag
    }
    _delay_ms(250);         // delay to stop screen cluttering up    
  }
}
