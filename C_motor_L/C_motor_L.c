//
//
// hello.ftdi.44.echo.interrupt.c
//
// 115200 baud FTDI character echo, interrupt version
//
// set lfuse to 0x7E for 20 MHz xtal
//
// Neil Gershenfeld
// 12/8/10
//
// (c) Massachusetts Institute of Technology 2010
// Permission granted for experimental and personal use;
// license for commercial sale available from MIT.
//

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#define output(directions,pin) (directions |= pin) // set port direction for output
#define input(directions,pin) (directions &= (~pin)) // set port direction for input
#define set(port,pin) (port |= pin) // set port pin
#define clear(port,pin) (port &= (~pin)) // clear port pin
#define pin_test(pins,pin) (pins & pin) // test for port pin
#define bit_test(byte,bit) (byte & (1 << bit)) // test for bit set
#define bit_delay_time 100// with 9600 baud rate //8.5 // bit delay for 115200 with overhead
#define bit_delay() _delay_us(bit_delay_time) // RS232 bit delay
#define half_bit_delay() _delay_us(bit_delay_time/2) // RS232 half bit delay
#define char_delay() _delay_ms(10) // char delay
#define position_delay() _delay_ms(1000)

#define serial_port PORTA
#define serial_direction DDRA
#define serial_pins PINA
#define serial_pin_in (1 << PA0)
#define serial_pin_out (1 << PA1)
#define serial_interrupt (1 << PCIE0)
#define serial_interrupt_pin (1 << PCINT0)

#define bridge_port PORTA // H-bridge port
#define bridge_direction DDRA // H-bridge direction
#define IN1 (1 << PA2) // IN1
#define IN2 (1 << PA3) // IN2

#define PWM_direction DDRA
#define PWM_port PORTA
#define PWM1 (1 << PA5) 
#define PWM2 (1 << PA6)

volatile unsigned char chr = (char)175;
volatile int speedfor = 0;  // 136 (w 2 pretty full 9V batteries) is lowest speed that will overcome static friction.
volatile int speedback = 0;  // 136 (w 2 pretty full 9V batteries) is lowest speed that will overcome static friction.



void get_char(volatile unsigned char *pins, unsigned char pin, char *rxbyte) {
   //
   // read character into rxbyte on pins pin
   //    assumes line driver (inverts bits)
   //
   *rxbyte = 0;
   while (pin_test(*pins,pin))
      //
      // wait for start bit
      //
      ;
   //
   // delay to middle of first data bit
   //
   half_bit_delay();
   bit_delay();
   //
   // unrolled loop to read data bits
   //
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 0);
   else
      *rxbyte |= (0 << 0);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 1);
   else
      *rxbyte |= (0 << 1);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 2);
   else
      *rxbyte |= (0 << 2);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 3);
   else
      *rxbyte |= (0 << 3);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 4);
   else
      *rxbyte |= (0 << 4);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 5);
   else
      *rxbyte |= (0 << 5);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 6);
   else
      *rxbyte |= (0 << 6);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 7);
   else
      *rxbyte |= (0 << 7);
   //
   // wait for stop bit
   //
   bit_delay();
   half_bit_delay();
   }

void get_char_quick(volatile unsigned char *pins, unsigned char pin, char *rxbyte) {
   //
   // read character into rxbyte on pins pin
   //    assumes line driver (inverts bits)
   //
   *rxbyte = 0;
   /* int n = 1;  */
   /* int p = 0; */
   /* while (n>0) { */
   /*   p =  (!(pin_test(*pins,pin))); */
   /*   if (p) { */
   /*     n = 0;} */
   /*   n = n-1;} */
   /* if (p) { */
      //
      // wait for start bit
      //
   //
   // delay to middle of first data bit
   //
       half_bit_delay();
       bit_delay();
   //
   // unrolled loop to read data bits
   //
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 0);
   else
      *rxbyte |= (0 << 0);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 1);
   else
      *rxbyte |= (0 << 1);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 2);
   else
      *rxbyte |= (0 << 2);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 3);
   else
      *rxbyte |= (0 << 3);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 4);
   else
      *rxbyte |= (0 << 4);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 5);
   else
      *rxbyte |= (0 << 5);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 6);
   else
      *rxbyte |= (0 << 6);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 7);
   else
      *rxbyte |= (0 << 7);
   //
   // wait for stop bit
   //
   bit_delay();
   half_bit_delay();
}//}

void put_char(volatile unsigned char *port, unsigned char pin, char txchar) {
   //
   // send character in txchar on port pin
   //    assumes line driver (inverts bits)
   //
   // start bit
   //
   clear(*port,pin);
   bit_delay();
   //
   // unrolled loop to write data bits
   //
   if bit_test(txchar,0)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   if bit_test(txchar,1)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   if bit_test(txchar,2)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   if bit_test(txchar,3)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   if bit_test(txchar,4)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   if bit_test(txchar,5)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   if bit_test(txchar,6)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   if bit_test(txchar,7)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   //
   // stop bit
   //
   set(*port,pin);
   bit_delay();
   }

void put_string(volatile unsigned char *port, unsigned char pin, PGM_P str) {
   //
   // send character in txchar on port pin
   //    assumes line driver (inverts bits)
   //
   static char chr;
   static int index;
   index = 0;
   do {
      chr = pgm_read_byte(&(str[index]));
      put_char(port, pin, chr);
      ++index;
      } while (chr != 0);
   }

void delay_us(int n) {
  while(n--) {
    _delay_us(1);
  }
}

void delay_ms(int n) {
  while(n--) {
    _delay_ms(1);
  }
}

// This rotates the wheel either 
void rotate_wheel(int *to_move, int *to_not_move) {
  	int n_tries = 10;
	int is_start_bit = 0;
	while (n_tries>0) {
	  is_start_bit =  (!(pin_test(serial_pins, serial_pin_in)));
	  if (is_start_bit) {
	    n_tries = 0;}
	  n_tries = n_tries-1;}
	if (is_start_bit) {
	  get_char_quick(&serial_pins, serial_pin_in, &chr);
      	*to_move = (int)chr;
      	*to_not_move = 0;
}


      	   /* clear(bridge_port,IN2); */
      	   /* clear(bridge_port, IN1); */
      	   /* delay_ms(1000); */
}
   

int main(void) { 
  
   //
   // set clock divider to /1
   //
   CLKPR = (1 << CLKPCE);
   CLKPR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);
   //
   // initialize output pins
   //
   set(serial_port, serial_pin_out);
   input(serial_direction, serial_pin_out);
   //
   // initialize H-bridge pins
   //
   clear(bridge_port, IN1);
   output(bridge_direction, IN1);
   clear(bridge_port, IN2);
   output(bridge_direction, IN2);
   //
   // set up timer 1
   //
   TCCR1A = (1 << COM1A1) | (0 << COM1A0); // clear OC1A on compare match
   TCCR1A |= (1 << COM1B1) | (0 << COM1B0); // clear OC1B on compare match
   TCCR1B = (0 << CS12) | (1 << CS11) | (0 << CS10) | (1 << WGM13); // prescaler /8, phase and frequency correct PWM, ICR1 TOP
   ICR1 = 255; // ~20 ms frequency
   //
   // set PWM pin to output
   //
   clear(PWM_port, PWM1);
   output(PWM_direction, PWM1);
   clear(PWM_port, PWM2);
   output(PWM_direction, PWM2);
   //
   // main loop
   //
   OCR1A = 0; 
   while (1) {
      get_char(&serial_pins, serial_pin_in, &chr);
      if ((chr == 'p')) { // p for left, n for right
      	/* get_char(&serial_pins, serial_pin_in, &chr); */
      	/* get_char(&serial_pins, serial_pin_in, &chr); */
      	get_char(&serial_pins, serial_pin_in, &chr);
	OCR1A = 1*(int)chr;
	OCR1B = 0; 
	/* get_char(&serial_pins, serial_pin_in, &chr); */
	/* OCR1B = 100*(int)chr;  */
      	/* get_char(&serial_pins, serial_pin_in, &chr); */
      	/* get_char(&serial_pins, serial_pin_in, &chr); */
      }
      if ((chr == 'o')) { // o for left, m for right
      	get_char(&serial_pins, serial_pin_in, &chr);
      	OCR1B = 1*(int)chr;
      	OCR1A = 0;
      }
   }
   }

