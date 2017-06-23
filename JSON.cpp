#include "JSON.h"



bool JSON::parseObject(Stream& stream, KeyValueHandler handler, void* context){
	int skip = 0;
	state_t state = INIT;
	char * value;
	char * bufw;
	Object::type_t type;
	char buf[128], c;
	bool string = false, escaped = false;
	
	while(state!=ERROR && state!=END && stream.readBytes(&c,1)){
		
		
		if(c=='"' && !escaped){
			string=!string;
		}
		
		if(skip>0){
			if(!string){
				if(c=='}' || c==']')
					skip--;
				else if(c=='{' || c=='[')
					skip++;
			}
			continue;
		}
		
		if(!string && (c==' '||c=='\n'||c=='\r'||c=='\t')){
			continue;
		}
		
		switch(state){
			case INIT:
				state = c=='{' ? KEY : ERROR;
				break;
			case KEY:
			case VALUE:
				if(c=='"' && !escaped){
					if(string){
						type = Object::STRING;
						if(state == KEY) {
							bufw = buf;
						}
					}
				} else if(c==':' && !string){
					*bufw='\0';
					bufw++;
					value=bufw;
					state = VALUE;
				}  else if((c==','||c=='}')&&!string){
					*bufw='\0';
					bufw++;
					
					(*handler)(buf, value, type, context);
					
					state = c==',' ? KEY : END;
				} else {
					if(bufw==value){
						// first char
						if(!string){
							if(c=='t' || c=='f'){ // true or false
								type = Object::BOOL;
							} else if(c=='n'){
								type = Object::INVALID;
							} else if(c=='{' || c=='[') {
								skip++;
								type = Object::INVALID;
								break;
							} else {
								type = Object::INT;
							}
						}
					}
					if(c=='.' && type == Object::INT) type=Object::FLOAT;
					*bufw=(char)c;
					bufw++;
				}
				break;
				
		}
		
		if(string){
			if(escaped)
				escaped=false;
			else if(c=='\\')
				escaped=true;
		}
	}
	
	return state==END;
}

void fillObject(char* key, char* value, Object::type_t type, void* context){
	
	switch(type){
		case Object::STRING:
			((Object*)context)->add(key,value);
			break;
		case Object::INT:
			((Object*)context)->add(key,atol(value));
			break;
		case Object::FLOAT:
			((Object*)context)->add(key,(float)atof(value));
			break;
		case Object::BOOL:
			((Object*)context)->add(key,value[0]=='t');
			break;
	}
	
}

bool JSON::parseObject(Stream& stream, Object * obj){
	
	parseObject(stream, fillObject, obj);
	
}

