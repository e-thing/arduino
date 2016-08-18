
#include "FileWriter.h"
#include "config.h"


namespace EThing {
	
	
	bool FileWriter::open(const Id& id){
		setMethod(PUT);
		setContentType(NULL);
		return connect(id.data());
	}
	
	bool FileWriter::close(){
		if(send()){
			int statuscode = getStatusCode();
			Request::close();
			return (statuscode >= 200 && statuscode < 300);
		}
		return false;
	}
	
	bool FileWriter::createAndOpen(const char * name, unsigned long expireAfter){
		
		/*
			
			{"name":"..."[,"expireAfter":"this"]}
			
		*/
		
		setMethod(POST);
		setContentType("multipart/related; boundary="ETHING_MULTIPART_BOUNDARY);
		
		if(connect()){
			
			char buf[11]; // used to store a unsigned long as string
			
			print("\r\n--"ETHING_MULTIPART_BOUNDARY"\r\n");
			
			print("Content-Type: application/json\r\n\r\n{\"name\":\"");
			print(name);
			print("\",\"expireAfter\":");
			print(ultoa(expireAfter,buf,10));
			print("}");
			
			print("\r\n--"ETHING_MULTIPART_BOUNDARY"\r\n");
			print("\r\n");
			
			return true;
		}
		
		return false;
	}
	
	
	
	
	
}
