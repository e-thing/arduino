#ifndef _ETHING_JSON_H_
#define _ETHING_JSON_H_

#include "Object.h"
#include <Stream.h>

class JSON
{
	
public:
	
	typedef void (*KeyValueHandler)(char*, char*, Object::type_t, void*); // key, value, type, context
	static bool parseObject(Stream& stream, KeyValueHandler handler = NULL, void* context = NULL);
	
	static bool parseObject(Stream& stream, Object * obj);
	
private:
	enum state_t { ERROR=0, INIT, KEY, VALUE, END };

};



#endif /*_ETHING_JSON_H_*/
