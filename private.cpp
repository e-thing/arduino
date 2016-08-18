


#include "config.h"
#include "private.h"
#include <Arduino.h>


namespace EThing {
	
	
	const char * apiHost = NULL;
	const char * apiKey = NULL;
	Client* client = NULL;
	
	
	
	
	

	
	Client* connect(Method method,const char * url0, const char * url1, const char * contentType){
		
		/*
		
		GET /ething/api/??? HTTP/1.1
		Host: example.com
		X-API-KEY: c59234c6-d13d-4025-a9bd-09704e3684d5
		Content-Type: application/json
		Transfer-Encoding: chunked
		or
		Content-Length: 0
		
		*/
		
		if(client->connect(apiHost,ETHING_PORT)){
			
			// method
			switch(method){
				case GET:
					client->print("GET");
					break;
				case POST:
					client->print("POST");
					break;
				case PUT:
					client->print("PUT");
					break;
				case PATCH:
					client->print("PATCH");
					break;
			}
			
			// url
			client->print(" "ETHING_PATH);
			client->print(url0);
			if(url1!=NULL)
				client->print(url1);
			
			// version + host + api key
			client->print(" HTTP/1.0\r\nHost: ");
			client->print(apiHost);
			client->print("\r\nX-API-KEY: ");
			client->print(apiKey);
			
			
			// content Type
			if(contentType!=NULL){
				client->print("\r\nContent-Type: ");
				client->print(contentType);
			}
			
			// transfert Encoding + end of the header
			if(method==POST || method==PUT)
				// a body will be sent in this request, make the transfert be chuncked
				client->print("\r\nTransfer-Encoding: chunked\r\n\r\n");
			else
				client->print("\r\nContent-Length: 0\r\n\r\n");
			
			
			
			return client;
		}
		
		return NULL;
	}
	
	
	
	int waitResponseAndReadHeader(Client* client){
		unsigned long t0 = millis();
		int statusCode = 0;
		
		// wait for the response
		while(client->available() == 0 && millis()-t0 < ETHING_HTTP_TIMEOUT) delay(5);
		
		// HTTP/1.0 200 OK\r\n
		if(client->available() && client->find(" ")){
			statusCode = (int)client->parseInt();
			// read all the header
			if(!client->find("\r\n\r\n"))
				statusCode = 0; // an error occurs while reading the header : reset the status code
		}
		
		return statusCode;
	}
	
	
	int chunkedPrint(Client* client, const char * data, size_t size){
		return (
			client->print((int)size,HEX) && 
			client->print("\r\n")==2 && 
			client->write((const uint8_t *)data,size)==size && 
			client->print("\r\n")==2
		) ? size : 0;
	}
	
}
