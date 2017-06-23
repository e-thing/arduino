#include "File.h"
#include <EThing.h>

File::File(EThing* ething, const char * filename) : 
	filename_(filename),
	access_(NONE),
	req_(ething),
	id_(false)
{}

File::File(EThing* ething, const Id& id) : 
	filename_(id.data()),
	access_(NONE),
	req_(ething),
	id_(true)
{}

bool File::open(access_t access){
	char url[64];
	
	strcpy(url,"/files/");
	if(!id_) strcat(url,"~/");
	strcat(url,filename_);
	
	access_ = access;
	
	if(access_==APPEND){
		strcat(url,"?append=1");
	}
	
	if(access_&WRITE){
		if(req_.connect(Request::PUT, url, "application/json")){
			return true;
		}
		
	} else if(access_&READ){
		if(req_.connect(Request::GET, url) && req_.waitResponse()){
			return true;
		}
			
	}
	
	// an error occurs
	access_ = NONE;
	close();
	
	return false;
}


/* write access */

size_t File::write(uint8_t character){
	return access_&WRITE ? req_.write(character) : 0;
}

size_t File::write(const uint8_t *buf, size_t size){
	return access_&WRITE ? req_.write(buf, size) : 0;
}


/* read access */

int File::available(){
	return access_&READ ? req_.available() : 0;
}
int File::read(){
	return access_&READ ? req_.read() : 0;
}
int File::read(uint8_t *buf, size_t size){
	return access_&READ ? req_.read(buf, size) : 0;
}
int File::peek(){
	return access_&READ ? req_.peek() : 0;
}
void File::flush(){
	if(access_&READ)
		req_.flush();
}


/* read & write access */

bool File::close(){
		
	if(access_&WRITE){
		req_.waitResponse();
	}
		
	req_.stop();
	
	access_=NONE;
	
	return req_.isSuccessful();
}

