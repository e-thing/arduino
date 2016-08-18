

#include "Notification.h"
#include "Request.h"


namespace EThing {
	
	
	bool notify(const char * subject, const char * message, int * statusCode){
		/*
				
			{"subject":"...","body":"..."}
			
		*/
		
		Request req(POST,"notification","application/json");
		if(req.connect()){
			req.print("{\"subject\":\"");
			req.print(subject);
			req.print("\",\"body\":\"");
			req.print(message);
			req.print("\"}");
			
			req.send();
			req.close();
		}
		
		if(statusCode!=NULL)
			*statusCode = req.getStatusCode();
		
		return req.getStatusCode()==200;
	}
	
	
}
