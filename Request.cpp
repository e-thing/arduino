

#include "Request.h"
#include "private.h"


namespace EThing {
	
	bool Request::connect(const char * query){
		// reset client
		if(client_->connected())
			client_->stop();
		
		// reset status code
		statusCode_ = 0;
		
		return client_ = EThing::connect(method_,url_,query,contentType_);
	}
	
	bool Request::send(const char * data, const char * contentType){
		
		bool hasBody = method_!=EThing::GET && method_!=EThing::DELETE;
		if(contentType)
			setContentType(contentType);
		
		if(client_->connected() || connect( hasBody ? NULL : data)){
			
			// chunked print for non GET requests
			if(hasBody){ // POST, PUT, PATCH
				if(data)
					print(data);
				chunkedPrint(client_,NULL,0);
			}
			
			// wait for a response
			statusCode_ = waitResponseAndReadHeader(client_);
			
			return statusCode_ != 0;
		}
		
		return false;
	}
	
	size_t Request::write(uint8_t character){
		return chunkedPrint(client_,(const char *)&character,1);
	}
	
	size_t Request::write(const uint8_t *buf, size_t size){
		return chunkedPrint(client_,(const char *)buf,size);
	}
	
}
