#include <avr/io.h>
#include <util/delay.h>

int main(void) {
  DDRB |= (1<<0); //Set pin as Output
  DDRB |= (1<<1); //Set pin as Output
  DDRD &= ~(1<<1);//Set pin as Input
  PORTD |= (1<<1);
  int mode;
  mode = 0;
  _delay_ms(10);
  while (1) {
    if (bit_is_clear(PIND , 1))
    {
      mode = mode + 1;
      PORTB |= (1<<1);
      _delay_ms(300);
      PORTB &= ~(1<<1);      
      _delay_ms(300);
    }
    
    if (mode == 0)
    {
      PORTB |= (1<<0);
      _delay_ms(500);
      PORTB &= ~(1<<0);
      _delay_ms(500);      
    }
    
    if (mode == 1)
    {
      PORTB |= (1<<0);
      _delay_ms(1000);
      PORTB &= ~(1<<0);
      _delay_ms(1000);
    }

    if (mode == 2)
    {
      PORTB |= (1<<0);
      _delay_ms(3000);
      PORTB &= ~(1<<0);
      _delay_ms(3000);      
    }

    if (mode > 2)
    {
      mode = 0;
    }
  }
}
