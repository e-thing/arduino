
#include <EThing.h>
#include <SPI.h>
#include <Ethernet.h>




/*
*  ETHERNET CONFIGURATION
*/


// set the mac address of your ethernet board here :
byte mac[]        = {0x90, 0xA2, 0xDA, 0x00, 0x34, 0x58 };

EthernetClient client;



EThing ething("<API-KEY>","<HOSTNAME>",&client);






void setup(void)
{
	
	/* Serial initialization */ 
	
	Serial.begin(115200);
	Serial.println();
	
	
	/* initialization */ 
	if (Ethernet.begin(mac) == 0) {
		Serial.println("Err: unable to connect to the network using DHCP");
		while(1);
	}
	
	
	File file(&ething, "file.txt");
	
	// save data in file
	
	if(file.open(File::WRITE)){ // if you want append data, use instead File::APPEND
		
		file.print("hello world !");
		
		if(file.close()){
			Serial.println("Success");
		} else {
			Serial.println("Error");
		}
		
	} else {
		Serial.println("Error");
	}
	
	
	// read data 
	
	if(file.open(File::READ)){
		
		while(file.available()){
			Serial.print( (char) file.read() );
		}
		
		file.close();
		
	} else {
		Serial.println("Error");
	}
	
	
	while(1);
	
}

void loop(void)
{
	
}
