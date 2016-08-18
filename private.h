#ifndef _ETHING_PRIVATE_H_
#define _ETHING_PRIVATE_H_


#include <Client.h>
#include <Request.h>


namespace EThing {
	

	extern const char * apiHost;
	extern const char * apiKey;
	extern Client* client;
	
	
	Client* connect(Method method,const char * url0, const char * url1, const char * contentType);
	
	// return the response status code
	int waitResponseAndReadHeader(Client* client);
	
	
	// for chunked print
	int chunkedPrint(Client* client, const char * data, size_t size);
	inline int chunkedPrint(Client* client, const char * data){
		return chunkedPrint(client,data,strlen(data));
	}
	
}



#endif /*_ETHING_PRIVATE_H_*/
