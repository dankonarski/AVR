#include <avr/io.h>
#include <util/delay.h>

int main(void) {
  DDRC |= (1<<PC0); //Set pin as Output LED 7
  DDRC |= (1<<PC1); //Set pin as Output LED 6
  DDRC |= (1<<PC2); //Set pin as Output LED 5
  DDRC |= (1<<PC3); //Set pin as Output LED 3
  DDRC |= (1<<PC4); //Set pin as Output LED 2
  DDRC |= (1<<PC5); //Set pin as Output LED 1
  DDRD |= (1<<PD4); //Set pin as Output LED 4
  DDRB |= (1<<PB0); //Set pin as Output BUZZER

  initialize();

  while (1) {
    brick(genNumber());
    _delay_ms(2000);
    offbrick();
    _delay_ms(2000);
  }
}

void brick (int number) {
  _delay_ms(100);
  beep(20);
  _delay_ms(100);
  switch(number) {
    case 1 :
      PORTD |= (1<<PD4); //Turns ON PD
      break;
    case 2 :
      PORTC |= (1<<PC0); //Turns ON PC
      _delay_ms(30);
      PORTC |= (1<<PC5); //Turns ON PC
      _delay_ms(30);
      break;
    case 3 :
      PORTC |= (1<<PC2); //Turns ON PC
      _delay_ms(30);
      PORTC |= (1<<PC3); //Turns ON PC
      _delay_ms(30);
      PORTD |= (1<<PD4); //Turns ON PD
      _delay_ms(30);
      PORTC |= (1<<PC5); //Turns ON PC test
      break;
    case 4 :
      PORTC |= (1<<PC5); //Turns ON PC
      _delay_ms(30);
      PORTC |= (1<<PC3); //Turns ON PC
      _delay_ms(30);
      PORTC |= (1<<PC2); //Turns ON PC
      _delay_ms(30);
      PORTC |= (1<<PC0); //Turns ON PC
      break;
    case 5 :
      PORTC |= (1<<PC5); //Turns ON PC
      PORTC |= (1<<PC3); //Turns ON PC
      PORTC |= (1<<PC2); //Turns ON PC
      PORTC |= (1<<PC0); //Turns ON PC
      PORTD |= (1<<PD4); //Turns ON PD
      break;
    case 6 :
      PORTC |= (1<<PC5); //Turns ON PC
      PORTC |= (1<<PC4); //Turns ON PC
      PORTC |= (1<<PC3); //Turns ON PC
      PORTC |= (1<<PC2); //Turns ON PC
      PORTC |= (1<<PC1); //Turns ON PC
      PORTC |= (1<<PC0); //Turns ON PC
      break;
    }
  }

void offbrick () {
  PORTC &= ~(1<<PC0); //Turns OFF PC
  PORTC &= ~(1<<PC1); //Turns OFF PC
  PORTC &= ~(1<<PC2); //Turns OFF PC
  PORTC &= ~(1<<PC3); //Turns OFF PC
  PORTC &= ~(1<<PC4); //Turns OFF PC
  PORTC &= ~(1<<PC5); //Turns OFF PC
  PORTD &= ~(1<<PD4); //Turns OFF PC
}

void beep (int times) {
  int i = 0;
  for(i=0;i<times;i++){
    PORTB |= (1<<PB0); //Turns ON BUZZER
    _delay_ms(1);
    PORTB &= ~(1<<PB0); //Turns OFF BUZZER
    _delay_ms(1);
  }
}

void initialize () {
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
}

int genNumber() {
        int num = (rand() % 
           (3 - 1 + 1)) + 1; 
        return num; 
}
