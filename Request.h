#ifndef _ETHING_REQUEST_H_
#define _ETHING_REQUEST_H_


#define ETHING_HTTP_TIMEOUT 5000

 

#include <Client.h>

class EThing;

class Request : public Stream
{

public:
	
	enum Method {GET,POST,PUT,PATCH,DELETE};
	
	Request(EThing* ething);
	
	
	bool connect(Method method, const char * url, const char * contentType = NULL);
	
	
	inline size_t write(uint8_t character){
		return write(&character,1);
	}
	
	
	size_t write(const uint8_t *buf, size_t size);
	
	
	inline int available(){
		return client_->available();
	}
	
	inline int read(){
		return client_->read();
	}
	
	inline int read(uint8_t *buf, size_t size){
		return client_->read(buf, size);
	}
	
	inline int peek(){
		return client_->peek();
	}
	
	inline void flush(){
		return client_->flush();
	}
	
	inline operator bool(){
		return (bool)client_->connected();
	}
	
	inline void stop(){
		return client_->stop();
	}
	
	inline int getStatusCode() const {
		return statusCode_;
	};
	
	bool isSuccessful(){
		return statusCode_ >=200 && statusCode_ <300;
	}

	bool waitResponse();
	
	size_t readBytesUntilOneMatch(const char * terminators, char *buffer, size_t length);
	
private:
	
	EThing* ething_;
	Client* client_;
	bool write_;
	int statusCode_;
	
};




#endif /*_ETHING_REQUEST_H_*/
