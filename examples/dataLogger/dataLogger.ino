

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
	
	
	
	
}

void loop(void)
{
	
	/* store data at regular intervals in a table */
	
	int sensorValue = analogRead(A0);
	
	//Table table(&ething, "sensor");
	Table table(&ething, Id("VmX09uC"));
	
	
	if(table.push("value",sensorValue)){
		Serial.println("Success");
	} else {
		Serial.println("Error");
	}
	
	delay(300000); // sleep for 5 minutes
	
}

