#ifndef _ETHING_H_
#define _ETHING_H_



#define ETHING_NO_BAT -1
 
#include <Client.h>


class EThing
{
	
public:
	EThing(const char * apikey, const char * hostname, Client* client, uint16_t port = 80, const char * path = "/ething/api");
	
private:
	const char * apikey_;
	const char * hostname_;
	const char * path_;
	Client* client_;
	uint16_t port_;
	int batteryLevel_;
	
	friend class Request;
	
public:
	
	inline void setBatteryLevel(int batteryLevel){
		batteryLevel_ = batteryLevel;
	}
	
	bool notify(const char * subject, const char * content);
	
	bool trigger(const char * eventName);
	
	/* internal data */
	
public:

	
	
	bool setData(const char * key, const char * buffer, uint8_t length);
	bool setData(const char * key, const char * value);
	bool setData(const char * key, float value, uint8_t decimals = 2);
	bool setData(const char * key, bool value);
	bool setData(const char * key, long value);
	
	
	char* getDataString(const char * key, char *buffer, uint8_t bufferLength);
	bool getDataBool(const char * key);
	long getDataInt(const char * key);
	float getDataFloat(const char * key);
	
private:
	
	bool setData(const char * key, const char* data, uint8_t length, bool quoted);
	bool getData(const char * key, char* buff, uint8_t buffLength);
		
		
		
};


#include "Id.h"
#include "Device.h"
#include "File.h"
#include "Table.h"


#endif /*_ETHING_H_*/

