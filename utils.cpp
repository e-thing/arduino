

#include "utils.h"
#include <avr/eeprom.h>


namespace EThing {
	
	
	
	char * urlencode(const char * string, char * buf, size_t bufSize){
		char c, *hex = "0123456789abcdef", *pt = buf;

		while( (c = *string++) != '\0' ){
			if( ('a' <= c && c <= 'z')
			|| ('A' <= c && c <= 'Z')
			|| ('0' <= c && c <= '9') ){
				if(bufSize>1){
					*buf++ = c;
					bufSize--;
				}
			} else {
				if(bufSize>3){
					*buf++ = '%';
					*buf++ = hex[c >> 4];
					*buf++ = hex[c & 15];
					bufSize-=3;
				}
			}
		}
		*buf++ = '\0';
		
		return pt;
	}
	
	
	//CRC-8 - based on the CRC8 formulas by Dallas/Maxim
	//code released under the therms of the GNU GPL 3.0 license
	uint8_t CRC8(const uint8_t *data, size_t len) {
	  uint8_t crc = 0x00;
	  while (len--) {
		uint8_t extract = *data++;
		for (uint8_t tempI = 8; tempI; tempI--) {
		  uint8_t sum = (crc ^ extract) & 0x01;
		  crc >>= 1;
		  if (sum) {
			crc ^= 0x8C;
		  }
		  extract >>= 1;
		}
	  }
	  return crc;
	}
	
	Id getIdFromEeprom(int address) {
		EThing::Id id;
		uint8_t i;
		unsigned char * pt = (unsigned char *)address;
		char buf[ETHING_ID_LENGTH];
		
		for(i=0;i<ETHING_ID_LENGTH;i++)
			buf[i] = eeprom_read_byte(pt++);
		
		// crc check
		if(eeprom_read_byte(pt++) == CRC8((const uint8_t*)buf,ETHING_ID_LENGTH))
			id = Id(buf);
		
		return id;
	}
	
	void storeIdInEeprom(const Id& id, int address) {
		uint8_t i;
		unsigned char * pt = (unsigned char *)address;
		
		// write the id
		for(i=0;i<ETHING_ID_LENGTH;i++)
			eeprom_write_byte(pt++,id.data()[i]);
		
		eeprom_write_byte(pt++,CRC8((const uint8_t*)id.data(),ETHING_ID_LENGTH));
	}
	
}
