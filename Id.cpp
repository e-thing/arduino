
#include "Id.h"



namespace EThing {
	
	
	Id::Id(){
		data_[0] = '\0';
	}
	
	Id::Id(const char * id){
		memcpy(data_,id,ETHING_ID_LENGTH);
		data_[ETHING_ID_LENGTH] = '\0';
	}
	
	
}
