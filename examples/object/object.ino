

/*
* Necessary include
*/

#include <EThing.h>




Object data;

void setup(void)
{
	
	/* Serial initialization */ 
	
	Serial.begin(115200);
	Serial.println();


	Serial.println("1");
	
  
	data.add("int",-1565);
	data.add("string","toto");
	data.add("bool",false);
	
	data.serialize(Serial);


	data.clear();
	Serial.println();
	
	
}

void loop(void)
{
	if(Serial.available()){
  
		Serial.println("read");
		Serial.println(Serial.available());
		if(Object::parse(Serial,&data))
		  data.serialize(Serial);
		data.clear();
		Serial.flush();
	}
}