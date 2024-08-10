#include <mega16.h>

// Alphanumeric LCD functions
#include <alcd.h>
#include <delay.h>
// Declare your global variables here

#define ADC_VREF_TYPE ((1<<REFS1) | (1<<REFS0) | (1<<ADLAR))

#define up PINB.0 // nut nhan Up o pin B0
#define dw PINB.1
#define fan PORTB.2 
#define light PORTB.3
unsigned char temp, m;

//Luu du lieu vao EEPROM
void EEPROM_write(unsigned int uiAddress, unsigned char ucData){
    while(EECR & (1<<EEWE));
    EEAR = uiAddress;
    EEDR = ucData;
    EECR |= (1<<EEMWE);
    EECR |= (1<<EEWE);
}

//Doc du lieu tu EEPROM
unsigned char EEPROM_read(unsigned int uiAddress){
    while(EECR & (1<<EEWE));
    EEAR = uiAddress;      
    EECR |= (1<<EERE);
    return EEDR;
}

//Doc tin hieu ADC
unsigned char read_adc(unsigned char adc_input)                         {
    ADMUX = adc_input | ADC_VREF_TYPE;
    // Delay needed for the stabilization of the ADC input voltage
    delay_us(10);
    // Start the AD conversion
    ADCSRA|=(1<<ADSC);
    // Wait for the AD conversion to complete
    while ((ADCSRA & (1<<ADIF))==0);
    ADCSRA|=(1<<ADIF);
    return ADCH;
}

//Dieu chinh nhiet do bang nut nhan
void button(){
    delay_ms(20); 
    if(up == 0)
    {
        m++;
        if(m > 99) m = 0;
        while(up == 0);
    }  
    if(dw == 0)
    {
        m--;
        if(m == 255) m = 0;
        while(dw == 0);
    }        
    EEPROM_write(0,m);              
}

//Dieu khien quat va den
void out_relay()
{
    if(temp > m)
    {
        fan=1;
        light = 0;
    }     
    else
    {
        fan = 0;
        light = 1;
    }
}

//Khoi tao ADC
void ADC_init(){
    ADMUX=ADC_VREF_TYPE;
    ADCSRA=(1<<ADEN) | (0<<ADSC) | (1<<ADATE) | (0<<ADIF) | (0<<ADIE) | (0<<ADPS2) | (1<<ADPS1) | (0<<ADPS0);
}

void main(void){
    lcd_init(16);//Khai bao su dung lcd1602
    /*
    Doi voi LCD2004: lcd_init(20);
    */

    ADC_init();          
    DDRA=0x00;
    DDRB=0xfc;// chan b0-b1 la input, b2-b7 la output  
    DDRD=0xff; 

    m = EEPROM_read(0);

    while (1)
    {
        // Place your code here
        lcd_gotoxy(0,0);
        lcd_puts("NHIET DO:"); 
        lcd_gotoxy(10,0);  
        lcd_putchar(0x30+temp/10);             
        lcd_putchar(0x30+temp%10);
        lcd_gotoxy(13,0);     
        lcd_puts("*C");
        temp=read_adc(0); 
        lcd_gotoxy(0,1);
        lcd_puts("CAI DAT:");
        lcd_gotoxy(10,1);
        lcd_putchar(0x30+m/10); 
        lcd_putchar(0x30+m%10);
        lcd_gotoxy(13,1);     
        lcd_puts("*C"); 
        delay_ms(100);
        out_relay();  
        button(); 
    }   
}
