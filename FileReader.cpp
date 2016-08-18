#include "FileReader.h"


namespace EThing {
	
	
	bool FileReader::open(const Id& id){
		if(send(id.data()) && getStatusCode() == 200)
			return true;
		close();
		return false;
	}
	
	
	
	
}
