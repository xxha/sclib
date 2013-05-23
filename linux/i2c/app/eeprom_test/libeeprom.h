
#ifndef __LIBEEPROM__
#define	__LIBEEPROM__

extern int eeprom_open();
extern int eeprom_close();

extern unsigned char eeprom_read_byte(unsigned int addr);
extern int eeprom_write_byte(unsigned int addr, unsigned char data);
extern int eeprom_read_buf(unsigned int start, unsigned int len, unsigned char * buf);
extern int eeprom_write_buf(unsigned int start, unsigned int len, unsigned char * buf);

#define	MAXSIZE					256

#endif
