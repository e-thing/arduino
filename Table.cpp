
#include "Table.h"
#include "Request.h"


namespace EThing {
	
	namespace Table {
	
		
		Id create(const char * name, unsigned long expireAfter, unsigned long maxLength, int * statusCode){
			
			/*
				
				{"name":"..."[,"expireAfter":0][,"maxLength":0]}
				
			*/
			
			Id id;
			Request req(POST,"table?fields=id","application/json");
			if(req.connect()){
				
				char buf[11]; // used to store a unsigned long as string
				
				req.print("{\"name\":\"");
				req.print(name);
				req.print("\",\"expireAfter\":");
				req.print(ultoa(expireAfter,buf,10));
				req.print(",\"maxLength\":");
				req.print(ultoa(maxLength,buf,10));
				req.print("}");
				
				if(req.send() && req.getStatusCode() == 201){
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
			}
			
			if(statusCode!=NULL)
				*statusCode = req.getStatusCode();
			
			return id;
		}
		
		
	}
	
}
