

/*
* Necessary include
*/

#include <SPI.h>
#include <Ethernet.h>
#include <EThing.h>



/*
*  ETHERNET CONFIGURATION
*/


// set the mac address of your ethernet board here :
byte mac[]        = {0x90, 0xA2, 0xDA, 0x00, 0x34, 0x58 };

EthernetClient ethernetClient;

EThing ething("<API-KEY>","<HOSTNAME>",&ethernetClient);


void setup(void)
{
	
	/* Serial initialization */ 
	
	Serial.begin(115200);
	Serial.println();
	
	
	/* Ethernet initialization */ 
	
	if (Ethernet.begin(mac) == 0) {
		Serial.println("Err: unable to connect to the network using DHCP");
		while(1);
	}
	
	
	
	Table table(&ething, Id("VmX09uC"));
	
	//long data[5];
	//int size = table.fetch(data, 5, "value");
	
	float data[5];
	int size = table.fetch(data, 5, "value");
	
	int i;
	for(i=0; i<size; i++){
		Serial.print("[");
		Serial.print(i);
		Serial.print("]=");
		Serial.println(data[i]);
	}
	
	
}

void loop(void)
{
	
	
}

