#include "Table.h"
#include <EThing.h>
#include "JSON.h"

#define INTERNAL_BUFFER_SIZE 128


Table::Table(EThing* ething, const char * tablename) : 
	ething_(ething),
	tablename_(tablename),
	id_(false)
{}

Table::Table(EThing* ething, const Id& id) : 
	ething_(ething),
	tablename_(id.data()),
	id_(true)
{}




bool Table::push(const Object& data){
	char url[INTERNAL_BUFFER_SIZE];
	int i;
	Request req(ething_);
	
	strcpy(url,"/tables/");
	if(!id_) strcat(url,"~/");
	strcat(url,tablename_);
	
	if(req.connect(Request::POST, url, "application/json")){
		
		data.serialize(req);
		
		req.waitResponse();
		req.stop();
	}
	
	return req.isSuccessful();
}

bool Table::push(const char * key, const char * buffer, uint8_t length){
	Object data;
	data.add(key,buffer,length);
	return push(data);
}

bool Table::push(const char * key, const char * value){
	Object data;
	data.add(key,value);
	return push(data);
}

bool Table::push(const char * key, float value){
	Object data;
	data.add(key,value);
	return push(data);
}

bool Table::push(const char * key, bool value){
	Object data;
	data.add(key,value);
	return push(data);
}

bool Table::push(const char * key, long value){
	Object data;
	data.add(key,value);
	return push(data);
}



struct fetchHandlerContext_t {
	int cnt;
	Table::KeyValueHandler handler;
	void* context;
};

void fetchHandler(char* key, char* value, Object::type_t type, void* context){
	fetchHandlerContext_t* ct = (fetchHandlerContext_t*)context;
	if(ct->handler != NULL){
		(*ct->handler)(ct->cnt, key, value, type, ct->context);
	}
}


int Table::fetch(KeyValueHandler handler, int length, const char * keys, int start, const char* sort, const char* query, void * context){
	char buf[INTERNAL_BUFFER_SIZE];
	char * bufPtr, * key, * value;
	char c;
	bool string;
	Request req(ething_);
	fetchHandlerContext_t ct = {0,handler,context};
	
	// construct the url !
	strcpy(buf,"/tables/");
	if(!id_) strcat(buf,"~/");
	strcat(buf,tablename_);
	strcat(buf,"?datefmt=TIMESTAMP&length=");
	bufPtr = buf + strlen(buf);
	itoa(length,bufPtr,10);
	strcat(buf,"&start=");
	bufPtr = buf + strlen(buf);
	itoa(start,bufPtr,10);
	if(keys!=NULL){
		strcat(buf,"&fields=");
		strcat(buf,keys);
	}
	if(sort!=NULL){
		strcat(buf,"&sort=");
		strcat(buf,sort);
	}
	if(query!=NULL){
		strcat(buf,"&query=");
		strcat(buf,query);
	}
	
	if(req.connect(Request::GET, buf)){
		
		if(req.waitResponse() && req.read()=='['){
			
			do {
				if(!JSON::parseObject(req, fetchHandler, &ct)){
					return 0;
				}
				
				ct.cnt++;
			} while(req.read()==',');
			
		}
		
		req.stop();
	}
	
	return ct.cnt;
}




void populateDataSet(int index, char * key, char * value, Object::type_t type, void * context){
	Object* obj = ((Object*)context) + index;
	switch(type){
		case Object::STRING:
			obj->add(key, value);
			break;
		case Object::INT:
			obj->add(key,atol(value));
			break;
		case Object::FLOAT:
			obj->add(key,(float)atof(value));
			break;
		case Object::BOOL:
			obj->add(key,value[0]=='t');
			break;
	}
}

void populateBoolArray(int index, char * key, char * value, Object::type_t type, void * context){
	((bool*)context)[index] = strlen(value)>0 && strcmp(value,"false")!=0 && strcmp(value,"0")!=0;
}

void populateIntArray(int index, char * key, char * value, Object::type_t type, void * context){
	((long*)context)[index] = atol(value);
}

void populateFloatArray(int index, char * key, char * value, Object::type_t type, void * context){
	((float*)context)[index] = atof(value);
}

void populateStringArray(int index, char * key, char * value, Object::type_t type, void * context){
	strcpy(((char**)context)[index], value);
}


void populateTimeSerieF(int index, char * key, char * value, Object::type_t type, void * context){
	if(strcmp(key,"date")==0)
		((Table::PointF*)context)[index].timestamp = atol(value);
	else
		((Table::PointF*)context)[index].value = atof(value);
}

void populateTimeSerieI(int index, char * key, char * value, Object::type_t type, void * context){
	if(strcmp(key,"date")==0)
		((Table::PointI*)context)[index].timestamp = atol(value);
	else
		((Table::PointI*)context)[index].value = atol(value);
}




int Table::fetch(Object * data, int length, const char * keys, int start, const char* sort, const char* query){
	return fetch(populateDataSet, length, keys, start, sort, query, data);
}

int Table::fetch(bool * data, int length, const char * key, int start, const char* sort, const char* query){
	return fetch(populateBoolArray, length, key, start, sort, query, data);
}

int Table::fetch(long * data, int length, const char * key, int start, const char* sort, const char* query){
	return fetch(populateIntArray, length, key, start, sort, query, data);
}

int Table::fetch(float * data, int length, const char * key, int start, const char* sort, const char* query){
	return fetch(populateFloatArray, length, key, start, sort, query, data);
}

int Table::fetch(char ** data, int length, const char * key, int start, const char* sort, const char* query){
	return fetch(populateStringArray, length, key, start, sort, query, data);
}

int Table::fetch(PointI * data, int length, const char * key, int start, const char* sort, const char* query){
	char keys[32] = "date,";
	strcat(keys, key);
	return fetch(populateTimeSerieI, length, keys, start, sort, query, data);
}

int Table::fetch(PointF * data, int length, const char * key, int start, const char* sort, const char* query){
	char keys[32] = "date,";
	strcat(keys, key);
	return fetch(populateTimeSerieF, length, keys, start, sort, query, data);
}

