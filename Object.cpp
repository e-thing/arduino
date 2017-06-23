#include "Object.h"
#include <string.h>
#include <stdlib.h>
#include "JSON.h"


Object::Object() : buf_(NULL) {
	clear();
}

Object::~Object()
{
	if(buf_!=NULL) free(buf_);
}

Object::Object(const Object &obj)
{
	*this = obj;
}

Object & Object::operator = (const Object &obj)
{
	if (this == &obj) return *this;
	
	clear();
	if(obj.length_){
		context_ = obj.context_;
		length_ = obj.length_;
		bufPtr_ = obj.bufPtr_;
		reserve(obj.capacity_);
	}
	
	return *this;
}

void Object::clear(){
	if(buf_!=NULL) free(buf_);
	capacity_ = 0;
	buf_ = NULL;
	bufPtr_ = NULL;
	context_ = NULL;
	length_ = 0;
}


bool Object::add(const char * key, const char * value, uint8_t length){
	return add(key, strlen(key), STRING, (void*)value, length);
}

bool Object::add(const char * key, const char * value){
	return add(key, strlen(key), STRING, (void*)value, strlen(value));
}

bool Object::add(const char * key, float value){
	return add(key, strlen(key), FLOAT, (void*)&value);
}

bool Object::add(const char * key, bool value){
	return add(key, strlen(key), BOOL, (void*)&value);
}

bool Object::add(const char * key, long value){
	return add(key, strlen(key), INT, (void*)&value);
}


const char* Object::getString(const char * key, char * buf, size_t buflen) const{
	uint8_t * pt = findKey(key);
	return pt!=NULL ? getDataAsString(pt, buf, buflen) : NULL;
}

bool Object::getBool(const char * key) const{
	uint8_t * pt = findKey(key);
	return pt!=NULL ? getDataAsBool(pt) : false;
}

long Object::getInt(const char * key) const{
	uint8_t * pt = findKey(key);
	return pt!=NULL ? getDataAsInt(pt) : 0;
}

float Object::getFloat(const char * key) const{
	uint8_t * pt = findKey(key);
	return pt!=NULL ? getDataAsFloat(pt) : 0.;
}


void Object::serialize(Print& printer) const{
	uint8_t * pt = buf_;
	int i;
	printer.print('{');
	for(i=0; i<length_; i++){
		if(i>0) printer.print(',');
		info_t info = get(pt);
		printer.print('"');
		printer.print(info.key);
		printer.print("\":");
		switch(info.type){
			case STRING:
				printer.print('"');
				printer.print((char*)info.data);
				printer.print('"');
				break;
			case INT:
				printer.print(*((long*)info.data));
				break;
			case FLOAT:
				printer.print(*((float*)info.data));
				break;
			case BOOL:
				printer.print(*((bool*)info.data) ? "true" : "false");
				break;
		}
		pt = next(pt);
	}
	printer.print('}');
}


bool Object::parse(Stream& stream, Object * object){
	return JSON::parseObject(stream, object);
}


uint8_t * Object::next(uint8_t * pt) const{
	info_t info = get(pt);
	switch(info.type){
		case STRING:
			info.data += strlen((char*)info.data) + 1;
			break;
		case INT:
			info.data += sizeof(long);
			break;
		case FLOAT:
			info.data += sizeof(float);
			break;
		case BOOL:
			info.data += sizeof(bool);
			break;
	}
	
	return (uint8_t *)info.data;
}

Object::info_t Object::get(uint8_t * pt) const {
	info_t info;
	info.key = (char *)pt;
	pt += strlen((char *)pt);
	pt++;
	info.type = (type_t)(*pt);
	pt++;
	info.data = (void*)pt;
	return info;
}


Object::type_t Object::getType() const {
	info_t info = get(context_);
	return info.type;
}

const void* Object::getRawData() const {
	info_t info = get(context_);
	return info.data;
}

bool Object::getDataAsBool(uint8_t * pt) const {
	info_t info = get(pt);
	bool value = false;
	switch(info.type){
		case STRING:
			break;
		case INT:
			value = *((long*)info.data) != 0;
			break;
		case FLOAT:
			value = *((float*)info.data) != 0.;
			break;
		case BOOL:
			value = *((bool*)info.data);
			break;
	}
	return value;
}

long Object::getDataAsInt(uint8_t * pt) const {
	info_t info = get(pt);
	long value = 0;
	switch(info.type){
		case STRING:
			value = atol( (char*) info.data);
			break;
		case INT:
			value = *((long*)info.data);
			break;
		case FLOAT:
			value = *((float*)info.data);
			break;
		case BOOL:
			value = *((bool*)info.data);
			break;
	}
	return value;
}

float Object::getDataAsFloat(uint8_t * pt) const {
	info_t info = get(pt);
	long value = 0;
	switch(info.type){
		case STRING:
			value = atof( (char*) info.data);
			break;
		case INT:
			value = *((long*)info.data);
			break;
		case FLOAT:
			value = *((float*)info.data);
			break;
		case BOOL:
			value = *((bool*)info.data);
			break;
	}
	return value;
}

char * Object::getDataAsString(uint8_t * pt, char * buf, size_t buflen) const {
	info_t info = get(pt);
	switch(info.type){
		case STRING:
			strncpy(buf, (char*) info.data, buflen);
			break;
		case INT:
			ltoa(*((long*)info.data),buf,10);
			break;
		case FLOAT:
			dtostrf(*((float*)info.data),2,6,buf);
			break;
		case BOOL:
			strncpy(buf, *((bool*)info.data) ? "true" : "false", buflen);
			break;
	}
	return buf;
}

uint8_t * Object::findKey(const char * key) const{
	int i;
	uint8_t * pt = buf_;
	for(i=0; i<length_; i++){
		if(strcmp(key, (char *)pt)==0)
			return pt;
		pt = next(pt);
	}
	return NULL;
}

bool Object::add(const char * key, uint8_t keyLength, type_t type, const void * value, uint8_t valueLength){
	
	int size = bufPtr_ - buf_ + keyLength + 2;
	
	switch(type){
		case STRING:
			size++;// including the \0
			break;
		case INT:
			valueLength = sizeof(long);
			break;
		case FLOAT:
			valueLength = sizeof(float);
			break;
		case BOOL:
			valueLength = sizeof(bool);
			break;
	}
	
	size += valueLength;
	
	
	// length check
	if(reserve(size)){
		
		// key
		memcpy(bufPtr_, key, keyLength);
		bufPtr_ += keyLength;
		*bufPtr_ = '\0';
		bufPtr_++;
		
		// type
		*bufPtr_ = (uint8_t)type;
		bufPtr_++;
		
		// value
		memcpy(bufPtr_, value, valueLength);
		bufPtr_ += valueLength;
		if(type==STRING){
			*bufPtr_ = '\0';
			bufPtr_++;
		}
		
		length_++;
		
		return true;
	}
	
	return false;
}



bool Object::reserve(unsigned int size)
{
	if (buf_ && capacity_ >= size) return true;
	
	int bufPtrIndex = bufPtr_ - buf_;
	int contextIndex = context_ - buf_;
	
	uint8_t *newbuffer = (uint8_t *)realloc(buf_, size);
	if (newbuffer) {
		buf_ = newbuffer;
		bufPtr_ = buf_ + bufPtrIndex;
		context_ = buf_ + contextIndex;
		capacity_ = size;
		return true;
	}
	return false;
}

