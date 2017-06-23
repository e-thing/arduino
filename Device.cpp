
#include "Device.h"
#include <EThing.h>


#define INTERNAL_BUFFER_SIZE 128


Device::Device(EThing* ething, const char * name) : 
	ething_(ething),
	name_(name),
	id_(false)
{}

Device::Device(EThing* ething, const Id& id) : 
	ething_(ething),
	name_(id.data()),
	id_(true)
{}



bool Device::exec(const char * operation, const Object& data){
	char url[INTERNAL_BUFFER_SIZE];
	int i;
	Request req(ething_);
	
	strcpy(url,"/devices/");
	if(!id_) strcat(url,"~/");
	strcat(url,name_);
	strcat(url,"/");
	strcat(url,operation);
	
	if(req.connect(Request::POST, url, "application/json")){
		
		data.serialize(req);
		
		req.waitResponse();
		req.stop();
	}
	
	return req.isSuccessful();
}

