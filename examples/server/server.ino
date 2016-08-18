
/*

e-Thing

This example shows how to control an Arduino through the e-Thing web client interface.

First you need to create a device with the ip address of the Arduino and the following Swagger specification.

The associated Swagger specification :

{
  "swagger": "2.0",
  "info": {
    "title": "eThing server device example.",
    "description": "Switched on and off a led connected to an Arduino.",
    "version": "1"
  },
  "paths": {
    "/on": {
      "get": {
        "summary": "Turn on the led.",
        "responses": {
          "200": {
            "description": "The led has turned on."
          }
        }
      }
    },
    "/off": {
      "get": {
        "summary": "Turn off the led.",
        "responses": {
          "200": {
            "description": "The led has turned off."
          }
        }
      }
    },
    "/state": {
      "get": {
        "summary": "Return the state of the led.",
        "produces": [
          "text/plain"
        ],
        "responses": {
          "200": {
            "description": "The state of the led. The response is either ON or OFF.",
            "schema": {
              "type": "string",
              "enum": ["ON","OFF"]
            }
          }
        }
      }
    }
  }
}

*/


/*
* Necessary include
*/

#include <SPI.h>
#include <Ethernet.h>


const int led_pin = 8;
int led_state = LOW;


/*
*  ETHERNET CONFIGURATION
*/


// set the mac address of your ethernet board here :
byte mac[] = {0x90, 0xA2, 0xDA, 0x00, 0x34, 0x58 };
// the IP address of your arduino :
IPAddress ip(192, 168, 1, 177);

// Initialize the Ethernet server library
// (port 80 is default for HTTP):
EthernetServer server(80);





void setup(void)
{
	
	pinMode(led_pin, OUTPUT);
	digitalWrite(led_pin, led_state);
	
	/* Serial initialization */ 
	
	Serial.begin(9600);
	Serial.println();
	
	
	/* Ethernet initialization */ 
	
	Ethernet.begin(mac,ip);
	server.begin();
	Serial.print("server is at ");
	Serial.println(Ethernet.localIP());
	
}

void loop(void)
{
	/* listen to incoming client */
	
	EthernetClient client = server.available();
	if (client) {
		
		/* parse the incoming HTTP request */
		
		char buf[64];
		bool show404 = true;
		
		// read the method :
		if(client.readBytesUntil(' ',buf,64) && strncmp(buf,"GET",3)==0){
			
			// read the url :
			if(client.readBytesUntil(' ',buf,64)){
				
				if(strncmp(buf,"/on",3)==0){
					// turn on the led
					led_state = HIGH;
					digitalWrite(led_pin, led_state);
					client.println("HTTP/1.1 200 OK");
					client.println();
					show404 = false;
				}
				else if(strncmp(buf,"/off",4)==0){
					// turn off the led
					led_state = LOW;
					digitalWrite(led_pin, led_state);
					client.println("HTTP/1.1 200 OK");
					client.println();
					show404 = false;
				}
				else if(strncmp(buf,"/state",6)==0){
					// return the state of the led
					client.println("HTTP/1.1 200 OK");
					client.println("Content-Type: text/plain");
					client.println();
					client.print(led_state ? "ON" : "OFF");
					show404 = false;
				}
			}
			
		}
		
		if(show404){
			// error 404 on any other requests !
			client.println("HTTP/1.1 404 Not Found");
			client.println();
		}
		
		// give the web browser time to receive the data
		delay(1);
		// close the connection:
		client.stop();
	}
	
}

