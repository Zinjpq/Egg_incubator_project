#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>

/*
- Dòng đầu tiên định nghĩa hằng số F_CPU với giá trị là tần số hoạt động của vi điều khiển (16MHz).
- Dòng thứ hai và thứ ba là các chỉ thị tiền xử lý để sử dụng hàm _delay_ms() từ thư viện <util/delay.h>.
- Dòng thứ tư và thứ năm là các chỉ thị tiền xử lý để sử dụng các định nghĩa và hàm liên quan đến vi điều khiển AVR từ thư viện <avr/io.h>.
- Dòng thứ sáu là chỉ thị tiền xử lý để sử dụng hàm snprintf() từ thư viện <stdio.h>.

*/
void adc_init() {
    ADMUX |= (1 << REFS0);
    ADCSRA |= (1 << ADEN) | (1 << ADPS1) | (1 << ADPS0);
}
/*
- Hàm adc_init() được sử dụng để khởi tạo module Analog-to-Digital Converter (ADC) trên vi điều khiển AVR.
- Dòng thứ hai đặt bit REFS0 trong thanh ghi ADMUX để chọn điện áp tham chiếu AVCC làm nguồn tham chiếu cho ADC.
- Dòng thứ ba đặt các bit ADEN, ADPS1 và ADPS0 trong thanh ghi ADCSRA để bật ADC và thiết lập chia tỷ lệ mẫu (prescaler) cho ADC.
*/

uint16_t adc_read(uint8_t adc_port) {
    ADMUX = (1 << REFS0) | adc_port;
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC)) {
        _delay_ms(1);
    }
    return ADCW;
}

/*
- Hàm adc_read() được sử dụng để đọc giá trị từ kênh ADC đã chọn.
- Tham số adc_port được truyền vào để chỉ định kênh ADC cần đọc.
- Dòng thứ hai cài đặt bit REFS0 và các bit kênh ADC trong thanh ghi ADMUX để chọn nguồn tham chiếu và kênh ADC.
- Dòng thứ ba đặt bit ADSC trong thanh ghi ADCSRA để bắt đầu quá trình chuyển đổi ADC.
- Vòng lặp while kiểm tra bit ADSC trong thanh ghi ADCSRA để đợi quá trình chuyển đổi hoàn thành.
- Hàm _delay_ms(1) được gọi để đảm bảo thời gian chờ giữa các lần kiểm tra bit ADSC.
- Cuối cùng, hàm trả về giá trị đã chuyển đổi được lưu trong thanh ghi ADCW.
*/
void lcd_command(uint8_t cmd) {
    PORTA &= ~(1 << 0);
    PORTD = cmd;
    PORTA |= (1 << 2);
    _delay_ms(1);
    PORTA &= ~(1 << 2);
}

void lcd_data(uint8_t data) {
    PORTA |= (1 << 0);
    PORTD = data;
    PORTA |= (1 << 2);
    _delay_ms(1);
    PORTA &= ~(1 << 2);
}

void lcd_string(const char* str) {
    int i = 0;
    while (str[i] != '\0') {
        lcd_data(str[i]);
        i++;
    }
}

void lcd_float(float number) {
    char str[6];
    snprintf(str, sizeof(str), "%.2f", number);
    lcd_string(str);
}
/*
- Các hàm lcd_command(), lcd_data(), lcd_string() và lcd_float() được sử dụng để tương tác với màn hình LCD.
- Hàm lcd_command() được sử dụng để gửi lệnh tới màn hình LCD.
- Hàm lcd_data() được sử dụng để gửi dữ liệu (ký tự) tới màn hình LCD.
- Hàm lcd_string() được sử dụng để gửi một chuỗi ký tự tới màn hình LCD.
- Hàm lcd_float() được sử dụng để gửi một số thực (float) tới màn hình LCD. Hàm này sử dụng snprintf() để chuyển đổi số thành chuỗi và sau đó gọi hàm lcd_string() để gửi chuỗi đó tới màn hình LCD.

*/

int main(void) {
    DDRA |= (1 << DDA0) | (1 << DDA1) | (1 << DDA2);
    DDRD = 0xFF;
    DDRB = 0xFF;
    DDRC = 0x00;

    int num = 0;
    int y = 35;
    adc_init();

    while (1) {
        lcd_command(0x38);
        lcd_command(0x0C);
        lcd_command(0x80);

        num = adc_read(1) / 3.41;
        lcd_string("Temp: ");
        lcd_float(num);

        lcd_command(0xC0);
        lcd_string("Temp want: ");
        lcd_float(y);

        if (PINC & (1 << 0)) {
            _delay_ms(20);
            if (PINC & (1 << 0)) {
                y++;
            }
        }
        if (PINC & (1 << 1)) {
            _delay_ms(20);
            if (PINC & (1 << 1)) {
                y--;
            }
        }

        if (num >= (y - 0.5) && num <= (y + 0.5)) {
            PORTB &= ~(1 << 0);
            PORTB &= ~(1 << 1);
        }
        else if (num > (y + 0.5)) {
            PORTB &= ~(1 << 0);
            PORTB |= (1 << 1);
        }
        else {
            PORTB |= (1 << 0);
            PORTB &= ~(1 << 1);
        }
    }
}
/*
- Hàm main() là hàm chính thực hiện chương trình chính trên vi điều khiển.
- Các dòng đầu tiên (DDRA, DDRD, DDRB, DDRC) là các cài đặt Direction Register để thiết lập các chân I/O tương ứng là đầu ra hoặc đầu vào.
- Biến num và y được khởi tạo với giá trị ban đầu là 0 và 35.
- Hàm adc_init() được gọi để khởi tạo module ADC.
- Trong vòng lặp while(1), các lệnh lcd_command() được gọi để cài đặt màn hình LCD.
- Hàm adc_read(1) được gọi để đọc giá trị từ kênh ADC thứ 1 và chia cho 3.41 để chuyển đổi thành giá trị nhiệt độ.
- Hàm lcd_string() và lcd_float() được gọi để hiển thị giá trị đọc được trên màn hình LCD.
- Các điều kiện if kiểm tra các nút nhấn trên chân PC0 và PC1. Nếu nút được nhấn, biến y sẽ được tăng hoặc giảm một đơn vị.
- Cuối cùng, các điều kiện if cuối cùng kiểm tra giá trị nhiệt độ và điều khiển các chân PB0 và PB1 để bật hoặc tắt các thiết bị ngoại vi tương ứng.
*/
