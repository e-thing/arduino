#ifndef _ETHING_TABLE_H_
#define _ETHING_TABLE_H_



#include "Request.h"
#include "Object.h"
#include "Id.h"

class EThing;

class Table
{
	
public:
	
	struct PointF {
		unsigned long timestamp;
		float value;
	};

	struct PointI {
		unsigned long timestamp;
		long value;
	};
	
	Table(EThing* ething, const char * tablename);
	Table(EThing* ething, const Id& id);
	
	bool push(const Object& data);
	bool push(const char * key, const char * buffer, uint8_t length);
	bool push(const char * key, const char * value);
	bool push(const char * key, float value);
	bool push(const char * key, bool value);
	bool push(const char * key, long value);
	inline bool push(const char * key, int value){
		return push(key, (long)value);
	}
	
	typedef void (*KeyValueHandler)(int, char*, char*, Object::type_t, void*); // key, value, isValueString, context
	int fetch(KeyValueHandler handler, int length, const char * keys = NULL, int start = 0, const char* sort = NULL, const char* query = NULL, void * context = NULL);
	int fetch(Object * data, int length, const char * keys = NULL, int start = 0, const char* sort = NULL, const char* query = NULL);
	int fetch(bool * data, int length, const char * key, int start = 0, const char* sort = NULL, const char* query = NULL);
	int fetch(long * data, int length, const char * key, int start = 0, const char* sort = NULL, const char* query = NULL);
	int fetch(float * data, int length, const char * key, int start = 0, const char* sort = NULL, const char* query = NULL);
	int fetch(char ** data, int length, const char * key, int start = 0, const char* sort = NULL, const char* query = NULL);
	int fetch(PointI * data, int length, const char * key, int start = 0, const char* sort = NULL, const char* query = NULL);
	int fetch(PointF * data, int length, const char * key, int start = 0, const char* sort = NULL, const char* query = NULL);
	
	
private:
	
	const char * tablename_;
	EThing* ething_;
	bool id_;

};



#endif /*_ETHING_TABLE_H_*/
