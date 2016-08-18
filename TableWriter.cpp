

#include "TableWriter.h"
#include "private.h"


namespace EThing {
	
	
	bool TableWriter::open(const Id& id){
		index_ = 0;
		return connect(id.data());
	}
	
	
	/*
		{
			"field":<value>,
			...
		}
	
	*/
	bool TableWriter::write_(const char * field, const char * data, size_t size, bool quoted){
		return (
			print(index_++ ? ",\"" : "{\"") &&
			print(field) &&
			print("\":") &&
			(!quoted || print("\"")) &&
			write((const uint8_t*)data,size) &&
			(!quoted || print("\""))
		);
	}
	
	bool TableWriter::store(const char * field, long data){
		char buf[12]; //  32 bits (4 bytes), from -2,147,483,648 to 2,147,483,647
		ltoa(data,buf,10);
		return write_(field,buf,strlen(buf),false);
	}
	
	bool TableWriter::store(const char * field, unsigned long data){
		char buf[11]; //  32 bits (4 bytes), from 0 to 4,294,967,295 (2^32 - 1)
		ultoa(data,buf,10);
		return write_(field,buf,strlen(buf),false);
	}
	
	bool TableWriter::store(const char * field, double data, unsigned char precision){
		char buf[16];
		dtostrf(data,0,precision,buf);
		return write_(field,buf,strlen(buf),false);
	}
	
	
	bool TableWriter::close(){
		if(index_)
			print("}");
		
		if(send()){
			Request::close();
			return getStatusCode() == 200;
		}
		return false;
	}
	
}
