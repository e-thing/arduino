

#include "Request.h"
#include "Arduino.h"
#include <EThing.h>


Request::Request(EThing* ething) :
	ething_(ething),
	client_(ething->client_)
{}


bool Request::connect(Method method, const char * url, const char * contentType){
	
	// reset client
	if(client_->connected())
		client_->stop();
	
	// reset status code
	statusCode_ = 0;
	write_ = false;
	
	
	if(client_->connect(ething_->hostname_,ething_->port_)){
		
		// method
		switch(method){
			case GET:
				client_->print("GET");
				break;
			case POST:
				client_->print("POST");
				break;
			case PUT:
				client_->print("PUT");
				break;
			case PATCH:
				client_->print("PATCH");
				break;
			case DELETE:
				client_->print("DELETE");
				break;
		}
		
		// url
		client_->print(" ");
		client_->print(ething_->path_);
		client_->print(url);
		
		// version + host + api key
		client_->print(" HTTP/1.0\r\nHost: ");
		client_->print(ething_->hostname_);
		client_->print("\r\nX-API-KEY: ");
		client_->print(ething_->apikey_);
		
		// battery information
		client_->print("\r\nX-DEV-BAT: ");
		ething_->batteryLevel_==ETHING_NO_BAT ? client_->print("null") : client_->print(ething_->batteryLevel_, DEC);
		
		// content Type
		if(contentType!=NULL){
			client_->print("\r\nContent-Type: ");
			client_->print(contentType);
		}
		
		// transfert Encoding + end of the header
		if(method==GET || method==DELETE){
			// no body
			client_->print("\r\nContent-Length: 0\r\n\r\n");
		} else {
			// a body will be sent in this request, make the transfert be chuncked
			client_->print("\r\nTransfer-Encoding: chunked\r\n\r\n");
			write_ = true;
		}
		
		return true;
	}
	
	return false;
}


bool Request::waitResponse(){
	unsigned long t0 = millis();
	statusCode_ = 0;
	
	if(write_){
		// stop writing the request
		write(NULL,0);
		write_ = false;
	}
	
	// wait for the response
	while(client_->available() == 0 && millis()-t0 < ETHING_HTTP_TIMEOUT) delay(5);
	
	// HTTP/1.0 200 OK\r\n
	if(client_->available() && client_->find(" ")){
		statusCode_ = (int)client_->parseInt();
		// read all the header
		if(!client_->find("\r\n\r\n"))
			statusCode_ = 0; // an error occurs while reading the header : reset the status code
	}
	
	return isSuccessful();
}


/* write access */

size_t Request::write(const uint8_t *buf, size_t size){
	// chunked write
	return (
		write_ &&
		client_->print((int)size,HEX) && 
		client_->print("\r\n")==2 && 
		client_->write(buf,size)==size && 
		client_->print("\r\n")==2
	) ? size : 0;
}


size_t Request::readBytesUntilOneMatch(const char * terminators, char *buffer, size_t length)
{
  if (length < 1) return 0;
  size_t index = 0;
  int nTerminators = strlen(terminators), n;
  while (index < length) {
    int c = timedRead();
    if (c < 0) break;
	for(n=0; n<nTerminators; n++){
		if(terminators[n] == c) return index;
	}
    *buffer++ = (char)c;
    index++;
  }
  return index; // return number of characters, not including null terminator
}
