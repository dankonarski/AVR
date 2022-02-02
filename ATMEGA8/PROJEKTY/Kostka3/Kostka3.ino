#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void) {
  DDRC |= (1<<PC0); //Set pin as Output LED 1
  DDRC |= (1<<PC1); //Set pin as Output LED 2
  DDRC |= (1<<PC2); //Set pin as Output LED 3
  DDRC |= (1<<PC3); //Set pin as Output LED 4
  DDRC |= (1<<PC4); //Set pin as Output LED 5
  DDRC |= (1<<PC5); //Set pin as Output LED 6
  DDRD |= (1<<PD4); //Set pin as Output LED 7
  DDRD &= ~(1 << PD2); //Set pin as Input BUTTON1
  DDRD &= ~(1 << PD3); //Set pin as Input BUTTON1  
  DDRB |= (1<<PB0);    //Set pin as Output BUZZER

  PORTD |= (1 << PD2); //Pull-up BUTTON1
  PORTD |= (1 << PD3); //Pull-up BUTTON2
  MCUCR |= (1 << ISC00);  // Set INT0 to trigger
  MCUCR |= (1 << ISC01);  // set INT1 to trigger
  GICR |= (1 << INT0);    // Turns on INT0
  GICR |= (1 << INT1);    // Turns on INT1
  
  PORTC = 0b00000111;
  _delay_ms(500);
  PORTC = 0b00111000;
  _delay_ms(500);
  PORTC = 0b00111111;
  _delay_ms(2000);
  PORTC = 0b00000000;
  sei();
  while (1) {
    _delay_ms(10000);
    PORTD |= (1<<PD4); //Turns ON PD
    beepLow(30);
    PORTD &= ~(1<<PD4); //Turns OFF PC
  }
}

ISR (INT0_vect)
{
  cli();
  _delay_ms(20);
  PORTD |= (1<<PD4); //Turns ON PD
  beepHigh(20);
  PORTD &= ~(1<<PD4); //Turns OFF PC
  _delay_ms(20);
  PORTD |= (1<<PD4); //Turns ON PD
  beepHigh(20);
  PORTD &= ~(1<<PD4); //Turns OFF PC
  _delay_ms(20);
  PORTD |= (1<<PD4); //Turns ON PD
  beepLow(20);
  PORTD &= ~(1<<PD4); //Turns OFF PC
  _delay_ms(20);
  PORTD |= (1<<PD4); //Turns ON PD
  beepLow(20);
  PORTD &= ~(1<<PD4); //Turns OFF PC
  _delay_ms(20);
  sei();
}

ISR (INT1_vect)
{
  cli();
  _delay_ms(20);
  brick();
  _delay_ms(20);
  sei();
}

void brick () {
  int num = (rand() % (6 - 1 + 1)) + 1;
  //int num = 1;
  beepLow(30);
  beepHigh(50);
  if (num == 1) {
    PORTD |= (1<<PD4); //Turns ON PD
    _delay_ms(2000);
    PORTD &= ~(1<<PD4); //Turns OFF PD    
  }
  if (num == 2) {
    PORTC |= (1<<PC0); //Turns ON PC
    PORTC |= (1<<PC5); //Turns ON PC
    _delay_ms(2000);
    PORTC &= ~(1<<PC0); //Turns OFF PC
    PORTC &= ~(1<<PC5); //Turns OFF PC
  }
  if (num == 3) {
    PORTC |= (1<<PC2); //Turns ON PC
    PORTC |= (1<<PC3); //Turns ON PC
    PORTD |= (1<<PD4); //Turns ON PD
    _delay_ms(2000);
    PORTC &= ~(1<<PC2); //Turns OFF PC
    PORTC &= ~(1<<PC3); //Turns OFF PC
    PORTD &= ~(1<<PD4); //Turns OFF PD
  }
  if (num == 4) {
    PORTC |= (1<<PC0); //Turns ON PC
    PORTC |= (1<<PC2); //Turns ON PC
    PORTC |= (1<<PC3); //Turns ON PC            
    PORTC |= (1<<PC5); //Turns ON PC
    _delay_ms(2000);
    PORTC &= ~(1<<PC0); //Turns OFF PC
    PORTC &= ~(1<<PC2); //Turns OFF PC
    PORTC &= ~(1<<PC3); //Turns OFF PC            
    PORTC &= ~(1<<PC5); //Turns OFF PC
  }
  if (num == 5) {
    PORTC |= (1<<PC5); //Turns ON PC
    PORTC |= (1<<PC3); //Turns ON PC
    PORTC |= (1<<PC2); //Turns ON PC
    PORTC |= (1<<PC0); //Turns ON PC
    PORTD |= (1<<PD4); //Turns ON PD
    _delay_ms(2000);
    PORTC &= ~(1<<PC5); //Turns OFF PC
    PORTC &= ~(1<<PC3); //Turns OFF PC
    PORTC &= ~(1<<PC2); //Turns OFF PC
    PORTC &= ~(1<<PC0); //Turns OFF PC
    PORTD &= ~(1<<PD4); //Turns OFF PC
  }
  if (num == 6) {
    PORTC = 0b00111111;
    _delay_ms(2000);
    PORTC = 0b00000000;
  }
}

void beepHigh (int times) {
  int i = 0;
  for(i=0;i<times;i++){
    PORTB |= (1<<PB0); //Turns ON BUZZER
    _delay_ms(1);
    PORTB &= ~(1<<PB0); //Turns OFF BUZZER
    _delay_ms(1);
  }
}

void beepLow (int times) {
  int i = 0;
  for(i=0;i<times;i++){
    PORTB |= (1<<PB0); //Turns ON BUZZER
    _delay_ms(2);
    PORTB &= ~(1<<PB0); //Turns OFF BUZZER
    _delay_ms(2);
  }
}
