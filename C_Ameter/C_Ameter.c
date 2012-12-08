// SensoryMotor
// Sends sensory inputs to, and receives motor outputs from a serial port connection
// (works with the python app InputsVehicleBrain.app and circuit Gizmo_new.cad)
// Emily Mackevicius Nov 27, 2012
//
// Based on several of Neil Gershenfeld's HTMAA example files //


#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#define output(directions,pin) (directions |= pin) // set port direction for output
#define input(directions,pin) (directions &= (~pin)) // set port direction for input
#define set(port,pin) (port |= pin) // set port pin
#define clear(port,pin) (port &= (~pin)) // clear port pin
#define pin_test(pins,pin) (pins & pin) // test for port pin
#define bit_test(byte,bit) (byte & (1 << bit)) // test for bit set
#define bit_delay_time 8.5 // bit delay for 115200 with overhead
#define bit_delay() _delay_us(bit_delay_time) // RS232 bit delay
#define half_bit_delay() _delay_us(bit_delay_time/2) // RS232 half bit delay
#define char_delay() _delay_ms(10) // char delay

#define serial_port PORTA
#define serial_direction DDRA
#define serial_pins PINA
#define serial_pin_in (1 << PA0)
#define serial_pin_out (1 << PA1)

#define max_buffer 25

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
   //
   // char delay
   //
   bit_delay();
   }

void put_flash_string(volatile unsigned char *port, unsigned char pin, PGM_P str) {
   //
   // print a null-terminated string from flash
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

void put_ram_string(volatile unsigned char *port, unsigned char pin, char *str) {
   //
   // print a null-terminated string from SRAM
   //
   static int index;
   index = 0;
   do {
      put_char(port, pin, str[index]);
      ++index;
      } while (str[index] != 0);
   }

int main(void) {
   //
   // main
   //
   static char chr;
   static char buffer[max_buffer] = {0};
   static int index;
   static uint16_t count;
   static uint8_t cycle;
   static unsigned char sense0_lo, sense0_hi, sense1_lo, sense1_hi;
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
   // main loop
   //
   index = 0;
   while (1) {
      get_char(&serial_pins, serial_pin_in, &chr);
      int temp = chr-'0';
      if (temp == 7) {
	//read sensors
	 //
	 // init A/D
	 //
	 ADCSRB =  (0 << ADLAR); // right adjust
	 ADMUX = (0 << MUX3) | (0 << MUX2) | (1 << MUX1) | (0 << MUX0); // ADC2 = PA2
	 ADCSRA = (1 << ADEN) // enable
	   | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // prescaler /128
	 //
	 // initiate conversion
	 //
	 ADCSRA |= (1 << ADSC);
	 //
	 // wait for completion
	 //
	 while (ADCSRA & (1 << ADSC))
	   ;
	 //
	 // record result
	 //
	 sense0_lo = ADCL;
	 sense0_hi = ADCH;
      	 //
      // send framing
      //
	 output(serial_direction, serial_pin_out);
      	 put_char(&serial_port, serial_pin_out, '1');
      	 char_delay();
      	 put_char(&serial_port, serial_pin_out, '2');
      	 char_delay();
      	 put_char(&serial_port, serial_pin_out, '3');
      	 char_delay();
      	 put_char(&serial_port, serial_pin_out, '4');
      	 char_delay();
	 // send data
	 put_char(&serial_port, serial_pin_out, sense0_lo);
	 char_delay();
	 put_char(&serial_port, serial_pin_out, sense0_hi);
	 char_delay();
	 put_char(&serial_port, serial_pin_out, sense0_lo);
	 char_delay();
	 put_char(&serial_port, serial_pin_out, sense0_hi);
	 char_delay();
	 input(serial_direction, serial_pin_out);
      }
      }
   }
