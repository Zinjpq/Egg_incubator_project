//Lưu dữ liệu vào EEPROM
void EEPROM_write(unsigned int uiAddress, unsigned char ucData){
    while(EECR & (1<<EEWE));
    EEAR = uiAddress;
    EEDR = ucData;
    EECR |= (1<<EEMWE);
    EECR |= (1<<EEWE);
}

//Đọc dữ liệu từ EEPROM
unsigned char EEPROM_read(unsigned int uiAddress){
    while(EECR & (1<<EEWE));
    EEAR = uiAddress;      
    EECR |= (1<<EERE);
    return EEDR;
}

///////////////////////////////////
// Cách dùng:
EEPROM_write(0x00, 0x0A); //Lưu giá trị 0x0A vào địa chỉ 0x00
EEPROM_read(0x00); //Đọc giá trị từ địa chỉ 0x00