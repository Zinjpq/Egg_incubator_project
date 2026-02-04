/*
 * AVRGCC1.c
 *
 * Created: 8/7/2013 5:36:28 PM
 *  Author: ismail
 */
#include "DHT.h"
#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#define _lcd_data PORTD
#define _ctrl PORTC
#define en PC5
#define rw PC4
#define rs PC3

//Function for sending commands to LCD
void lcd_cmd(unsigned char command){
  //Put command on the Data Bus

  unsigned char temp;
  temp =_lcd_data;

  temp &=(0x0F);
  temp |= (command & 0xF0);

_lcd_data=temp;

  //Enable LCD for command writing
 // PORTD = 0b10000000;

//rs=0;
//rw=0;
//en=1;
_ctrl=(0<<rs)|(0<<rw)|(1<<en);

  //Allow delay for LCD to read the databus
  _delay_ms(2);

  //Disable LCD again
 // PORTD = 0b00000000;
//rs=0;
//rw=0;
//en=0;

_ctrl=(0<<rs)|(0<<rw)|(0<<en);

  //Allow some more delay
  _delay_ms(2);
}

//Function for sending Data to LCD
void lcd_data(unsigned char data){
  //Put data on Data Bus



   unsigned char temp;
  temp =_lcd_data;

  temp &=(0x0F);
  temp |= (data & 0xF0);

_lcd_data=temp;




  //Set R/S (Regiter Select) to High, and Enable to High
 // PORTD = 0b10100000;

//rs=1;
//rw=0;
//en=1;

_ctrl=(1<<rs)|(0<<rw)|(1<<en);

  //Allow for delay
  _delay_ms(2);

  //Disable LCD again
 // PORTD = 0b00100000;

 //rs=1;
//rw=0;
//en=0;

_ctrl=(1<<rs)|(0<<rw)|(0<<en);

  //Allow for some more delay
  _delay_ms(2);
}



void lcd_cmd_4bit(unsigned char command){



	lcd_cmd(command);


	lcd_cmd(command<<4);			//shift left 4-bit sothat lower nibble(1 nibble=4 bit) goes to upper nibble





}


void lcd_data_4bit(unsigned char data){

	lcd_data(data);


	lcd_data(data<<4);			//shift left 4-bit sothat lower nibble(1 nibble=4 bit) goes to upper nibble


}






//Function to send String to LCD
void lcd_send_string(char* string){
  while(*string){
  //Send value of pointer as data to LCD
  lcd_data_4bit(*string);
  //Increment string pointer
  string++;
  }
}

//Function to Initilise LCD by sending appropriate Commands
void init_lcd(){

	lcd_cmd_4bit(0x02);		// to initialize LCD in 4-bit mode.
	lcd_cmd_4bit(0x28);		//to initialize LCD in 2 lines, 5X7 dots and 4bit mode.



  //Set Cursor off - Enable LCD
   lcd_cmd_4bit(0x0E);
  //Clear Screen
  lcd_cmd_4bit(0x01);
  //Goto first position
  lcd_cmd_4bit(0x80);


lcd_cmd_4bit(0x06);

}

/* this function is written to convert interger value to their corresponding ASCII value*/
void lcd_adc_2_ascii(uint16_t adc_out)
{
	unsigned int adc_out1;
	int i=0;
	char position=0xC3;    //4 character er last posision (1024)

	for(i=0;i<=3;i++)
	{
		adc_out1=adc_out%10;
		adc_out=adc_out/10;
		lcd_cmd_4bit(position);
		lcd_data_4bit(48+adc_out1);
		position--;
	}
}

void initDHT(void)
{
    /* Set LED as output */
    SET_BIT(DDRB,LED);
    
    /* According to the DHT11's datasheet, the sensor needs about
       1-2 seconds to get ready when first getting power, so we
       wait
     */
    _delay_ms(2000);
}



uint8_t fetchData(uint8_t* arr)
{
    uint8_t data [5];
    uint8_t cnt, check;
    int8_t i,j;
    
    /******************* Sensor communication start *******************/
    
    /* Set data pin as output first */
    SET_BIT(DDRB,DHT_PIN);
    
    /* First we need milliseconds delay, so set clk/1024 prescaler */
    TCCR0B = 0x05;
    
    TCNT0 = 0;
    
    /* Clear bit for 20 ms */
    CLEAR_BIT(DHT_PORT_OUT,DHT_PIN);
    
    /* Wait about 20 ms */
    while(TCNT0 < 160);
    
    /* Now set Timer0 with clk/8 prescaling.
     Gives 1탎 per cycle @8Mhz */
    TCCR0B = 0x02;
    
    TCNT0 = 0;
    
    /* Pull high again */
    SET_BIT(DHT_PORT_OUT,DHT_PIN);
    
    /* And set data pin as input */
    CLEAR_BIT(DDRB,DHT_PIN);
    
    /* Wait for response from sensor, 20-40탎 according to datasheet */
    while(IS_SET(DHT_PORT_IN,DHT_PIN))
    { if (TCNT0 >= 60) return 0; }
    
    /************************* Sensor preamble *************************/
    
    TCNT0 = 0;
    
    /* Now wait for the first response to finish, low ~80탎 */
    while(!IS_SET(DHT_PORT_IN,DHT_PIN))
    { if (TCNT0 >= 100) return 0; }
    
    TCNT0 = 0;
    
    /* Then wait for the second response to finish, high ~80탎 */
    while(IS_SET(DHT_PORT_IN,DHT_PIN))
    { if (TCNT0 >= 100) return 0; }
    
    /********************* Data transmission start **********************/
    
    for (i = 0; i < 5; ++i)
    {
        for(j = 7; j >= 0; --j)
        {
            TCNT0 = 0;
            
            /* First there is always a 50탎 low period */
            while(!IS_SET(DHT_PORT_IN,DHT_PIN))
            { if (TCNT0 >= 70) return 0; }
            
            TCNT0 = 0;
            
            /* Then the data signal is sent. 26 to 28탎 (ideally)
             indicate a low bit, and around 70탎 a high bit */
            while(IS_SET(DHT_PORT_IN,DHT_PIN))
            { if (TCNT0 >= 90) return 0; }
            
            /* Store the value now so that the whole checking doesn't
             move the TCNT0 forward by too much to make the data look
             bad */
            cnt = TCNT0;
            
            if (cnt >= 20 && cnt <= 35)
            { CLEAR_BIT(data[i],j); }
            
            else if (cnt >= 60 && cnt <= 80)
            { SET_BIT(data[i],j); }
            
            else return 0;
        }
    }
    
    /********************* Sensor communication end *********************/
    
    check = (data[0] + data[1] + data[2] + data[3]) & 0xFF;
    
    if (check != data[4]) return 0;
    
    for(i = 0; i < 4; ++i)
    { arr[i] = data[i]; }
    
    return 1;
}

int main(){
  //Set _lcd_data as Output
  DDRB = 0xff;
  //Set PD.5,6 and 7 as Output
  DDRC= 0xFF;

  //Give Inital Delay for LCD to startup as LCD is a slower Device
  _delay_ms(16);

  init_lcd();
 uint16_t data11=2012;
 //////////////////////////
     uint8_t data [4];
    
    int8_t i;
    
    initDHT();

  while(1){
	  
	if(fetchData(data))
    {
        for(i = data[2]; i >= 0; --i)
        {
            SET_BIT(PORTB,LED);
            _delay_ms(100);
            
            CLEAR_BIT(PORTB,LED);
            _delay_ms(500);
        }
		
	//Goto Line-1,first position
   lcd_cmd_4bit(0x80);
   lcd_adc_2_ascii(data[1]);
   //Goto Line-2, first position
   lcd_cmd_4bit(0xC0);
    lcd_adc_2_ascii(data[3]);
		
    }
	/////////////////  
   //Goto Line-1,first position
  // lcd_cmd_4bit(0x80);
  // lcd_send_string("ISMAIL HOSSAIN");
   //Goto Line-2, first position
  // lcd_cmd_4bit(0xC0);
  //  lcd_adc_2_ascii(data11);
  //lcd_send_string("ROLL:201214019");
  }
  return 0;
}
