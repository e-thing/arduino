#ifndef _ETHING_FILE_H_
#define _ETHING_FILE_H_



#include "Request.h"
#include "Stream.h"
#include "Id.h"

class EThing;

class File: public Stream
{
	
public:
	
	enum access_t { NONE=0, READ = 0x001, WRITE = 0x010, APPEND = 0x110 };
	
	
	File(EThing* ething, const char * filename);
	File(EThing* ething, const Id& id);
	
	bool open(access_t access);
	
	
	
	size_t write(uint8_t character);
	size_t write(const uint8_t *buf, size_t size);
	
	
	int available();
	int read();
	int read(uint8_t *buf, size_t size);
	int peek();
	
	
	void flush();
	
	inline operator bool(){
		return access_!=NONE && req_;
	}
	
	
	bool close();
	
	
private:
	
	const char * filename_;
	access_t access_;
	Request req_;
	bool id_;

};



#endif /*_ETHING_FILE_H_*/
