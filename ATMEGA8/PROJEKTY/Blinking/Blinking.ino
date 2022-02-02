#include <avr/io.h>
#include <util/delay.h>

int main(void) {
  DDRC |= (1<<PC5); //Set pin as Output LED 1
  DDRC |= (1<<PC4); //Set pin as Output LED 2
  DDRC |= (1<<PC3); //Set pin as Output LED 3
  DDRD |= (1<<PD4); //Set pin as Output LED 4
  DDRC |= (1<<PC2); //Set pin as Output LED 5
  DDRC |= (1<<PC1); //Set pin as Output LED 6
  DDRC |= (1<<PC0); //Set pin as Output LED 7
  DDRB |= (1<<PB0); //Set pin as Output BUZZER
  int i=0;

    for(i=0;i<100;i++){
      PORTB |= (1<<PB0); //Turns ON BUZZER
      _delay_ms(1);
      PORTB &= ~(1<<PB0); //Turns OFF BUZZER
      _delay_ms(1);
    }
    _delay_ms(100);
    for(i=0;i<100;i++){
      PORTB |= (1<<PB0); //Turns ON BUZZER
      _delay_ms(1);
      PORTB &= ~(1<<PB0); //Turns OFF BUZZER
      _delay_ms(1);
    }
    _delay_ms(100);
    for(i=0;i<300;i++){
      PORTB |= (1<<PB0); //Turns ON BUZZER
      _delay_ms(1);
      PORTB &= ~(1<<PB0); //Turns OFF BUZZER
      _delay_ms(1);
    }

  
  while (1) {
    PORTC |= (1<<PC0); //Turns ON PC0
    _delay_ms(200);
    PORTC &= ~(1<<PC0); //Turns OFF PC0
    _delay_ms(200);

    PORTC |= (1<<PC1); //Turns ON PC0
    _delay_ms(200);
    PORTC &= ~(1<<PC1); //Turns OFF PC0
    _delay_ms(200);

    PORTC |= (1<<PC2); //Turns ON PC0
    _delay_ms(200);
    PORTC &= ~(1<<PC2); //Turns OFF PC0
    _delay_ms(200);

    PORTD |= (1<<PD4); //Turns ON PC0
    _delay_ms(200);
    PORTD &= ~(1<<PD4); //Turns OFF PC0
    _delay_ms(200);

    PORTC |= (1<<PC3); //Turns ON PC0
    _delay_ms(200);
    PORTC &= ~(1<<PC3); //Turns OFF PC0
    _delay_ms(200);

    PORTC |= (1<<PC4); //Turns ON PC0
    _delay_ms(200);
    PORTC &= ~(1<<PC4); //Turns OFF PC0
    _delay_ms(200);

    PORTC |= (1<<PC5); //Turns ON PC0
    _delay_ms(200);
    PORTC &= ~(1<<PC5); //Turns OFF PC0
    _delay_ms(200);

    
  }
}
