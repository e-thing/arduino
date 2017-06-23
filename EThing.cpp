
#include "EThing.h"
#include "Request.h"


EThing::EThing(const char * apikey, const char * hostname, Client* client, uint16_t port, const char * path) :
	apikey_(apikey),
	hostname_(hostname),
	path_(path),
	client_(client),
	port_(port),
	batteryLevel_(ETHING_NO_BAT)
{}
	

bool EThing::notify(const char * subject, const char * content) {
	
	Request req(this);
	
	if(req.connect(Request::POST, "/notification", "application/json")){
		
		req.print("{\"subject\":\"");
		req.print(subject);
		req.print("\",\"body\":\"");
		req.print(content);
		req.print("\"}");
		
		req.waitResponse();
		req.stop();
	}
	
	return req.isSuccessful();
	
}

bool EThing::trigger(const char * eventName) {
	
	Request req(this);
	
	char url[64];
	
	strcpy(url,"/rules/trigger/");
	strcat(url,eventName);
	
	if(req.connect(Request::GET, url)){
		req.waitResponse();
		req.stop();
	}
	
	return req.isSuccessful();
	
}


bool EThing::setData(const char * key, const char * buffer, uint8_t length){
	return setData(key, buffer, length, true);
}

bool EThing::setData(const char * key, const char * value){
	return setData(key, value, strlen(value));
}

bool EThing::setData(const char * key, float value, uint8_t decimals){
	char buffer[16];
	return setData(key, dtostrf(value,2,decimals,buffer), strlen(buffer), false);
}

bool EThing::setData(const char * key, bool value){
	return setData(key, value ? "true" : "false", value ? 4 : 5, false);
}

bool EThing::setData(const char * key, long value){
	char buffer[12];
	return setData(key, ltoa(value,buffer,10), strlen(buffer), false);
}


char* EThing::getDataString(const char * key, char *buffer, uint8_t bufferLength){
	return getData(key, buffer, bufferLength) ? buffer : NULL;
}

bool EThing::getDataBool(const char * key){
	char buffer[6];
	getData(key, buffer, 6);
	return strcmp(buffer,"true") == 0;
}

long EThing::getDataInt(const char * key){
	char buffer[12];
	getData(key, buffer, 12);
	return atol(buffer);
}

float EThing::getDataFloat(const char * key){
	char buffer[16];
	getData(key, buffer, 16);
	return atof(buffer);
}


bool EThing::setData(const char * key, const char* data, uint8_t length, bool quoted){
	Request req(this);
	
	if(req.connect(Request::PATCH, "/resources/me", "application/json")){
		
		req.print("{\"data\":{\"");
		req.print(key);
		req.print("\":");
		if(quoted) req.write('"');
		req.write((const uint8_t *)data,length);
		if(quoted) req.write('"');
		req.print("}}");
		
		req.waitResponse();
		req.stop();
	}
	
	return req.isSuccessful();
}

bool EThing::getData(const char * key, char* buff, uint8_t buffLength){
	Request req(this);
	
	if(req.connect(Request::GET, "/resources/me?fields=data")){
		if(req.waitResponse()){
			while(req.find((char*)key)){
				req.read();// "
				req.read();// :
				if(req.peek()=='"') req.read();
				req.readBytesUntilOneMatch("\",}", buff, buffLength);
			}
		}
		
		req.stop();
	}
	
	return req.isSuccessful();
}

