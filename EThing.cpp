
#include "EThing.h"
#include "private.h"


namespace EThing {
	
	
	
	void init(const char * apiHost, const char * apiKey, Client * client){
		
		EThing::apiHost = apiHost;
		EThing::apiKey = apiKey;
		EThing::client = client;
		
	}
	
	Id findOne(const char * query, int * statusCode){
		
		Id id;
		Request req(GET,"resources?limit=1&fields=id&q=");
		
		// encode an URI component
		char encodedQuery[ETHING_QUERY_ENCODED_MAX_SIZE];
		if(req.send(urlencode(query,encodedQuery,ETHING_QUERY_ENCODED_MAX_SIZE)) && req.getStatusCode() == 200){
			// parse the response
			if(req.find("\"id\":\"")){
				// read the first id
				char sid[ETHING_ID_LENGTH];
				if(req.read((uint8_t*)sid,ETHING_ID_LENGTH)==ETHING_ID_LENGTH){
					id = Id(sid);
				}
			}
		}
		req.close();
		
		if(statusCode!=NULL)
			*statusCode = req.getStatusCode();
		
		return id;
	}
	
	
}
