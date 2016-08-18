

/*
* Necessary include
*/

#include <SPI.h>
#include <Ethernet.h>
#include <EThing.h>


// eThing server hostname or IP :
const char api_host[] = "";
// set your API key here : 
const char api_key[] = "";
// table name where to store the data
const char tablename[] = "data.db";


/*
*  ETHERNET CONFIGURATION
*/


// set the mac address of your ethernet board here :
byte mac[]        = {0x90, 0xA2, 0xDA, 0x00, 0x34, 0x58 };

EthernetClient ethernetClient;




EThing::Id tableId; // holds id of the table

void setup(void)
{
	
	/* Serial initialization */ 
	
	Serial.begin(9600);
	Serial.println();
	
	
	/* Ethernet initialization */ 
	
	if (Ethernet.begin(mac) == 0) {
		Serial.println("Err: unable to connect to the network using DHCP");
		while(1);
	}
	
	
	/* EThing initialization */ 
	
	Serial.print("EThing server : ");
	Serial.println(api_host);
	Serial.print("API key : ");
	Serial.println(api_key);
	
	EThing::init(api_host,api_key,&ethernetClient);
	
	
	/* find if a the table already exist ? */
	String query = "name == '$' and createdBy == me";
	query.replace("$",tablename);
	tableId = EThing::findOne(query.c_str());
	if(!tableId){
		
		/* the table do not exist, so create it ! */
		int httpCode; // hold the response http code
		tableId = EThing::Table::create(tablename,0,0,&httpCode);
		
		if(!tableId){
			/* unable to create the table */
			if(httpCode==0){
				Serial.print("Err: server unreachable ");
				Serial.println(api_host);
			} else {
				Serial.print("Err: server returns code ");
				Serial.println(httpCode);
			}
			while(1);
		}
		
	}
	
}

void loop(void)
{
	
	/* send data at regular intervals */
	
	int sensorValue = analogRead(A0);
	
	EThing::TableWriter tableWriter;
	if(tableWriter.open(tableId)){
		
		tableWriter.store("value",sensorValue);
		
		if(tableWriter.close()){
			Serial.println("data sent");
		} else {
			Serial.print("Err: server returns code ");
			Serial.println(tableWriter.getStatusCode());
		}
			
	}
	else {
		Serial.print("Err: unreachable ");
		Serial.println(api_host);
	}
	
	
	delay(300); // sleep for 5 minutes
	
}

