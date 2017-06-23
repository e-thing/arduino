#ifndef _ETHING_DEVICE_H_
#define _ETHING_DEVICE_H_



#include "Request.h"
#include "Object.h"
#include "Id.h"

class EThing;

class Device
{
	
public:

	Device(EThing* ething, const char * name);
	Device(EThing* ething, const Id& id);
	
	
	bool exec(const char * operation, const Object& data = Object());
	
private:
	
	const char * name_;
	EThing* ething_;
	bool id_;
	
		
};


#endif /*_ETHING_DEVICE_H_*/

