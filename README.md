e-Thing [Arduino]
=========


## Installation


1. Make sure you are using [Arduino IDE v1.6.0+](http://www.arduino.cc/en/Main/Software).
2. Import eThing Library. See [Arduino Guide](https://www.arduino.cc/en/Guide/Libraries).
3. Open one of the example sketch (File > Sketchbook > libraries > eThing).
4. Set up the sketch with your own API key and other configuration.
5. Compile and upload the sketch to the Arduino.
6. Open the Serial Monitor (the button on top right of your sketch) and check the sketch is running well.


## How to use the library


### Initialization


The eThing librairy needs an internet connection to work.

You can use either the [Ethernet library](https://www.arduino.cc/en/Reference/Ethernet) or the [WiFi library](https://www.arduino.cc/en/Reference/WiFi)
or any other librairy that are compatible with the [`Client`](https://www.arduino.cc/en/Reference/ClientConstructor) absctract class.

You must provide an ething server hostname/ip, an API key and a client to the `EThing::init` funtion.

To get an API key, first create a HTTP device under the Device page. Then go to the developer page to get his dedicated API key.

```cpp
// choose between
EthernetClient client; // Ethernet library
// or
WiFiClient client; // WiFi library
// or a custom class herited from the Client abstract class
CustomClient client;


void setup(void)
{
    // First argument : set the hostname or IP address where the eThing server is hosted
    // Second argument : set your device key here
    EThing::init("5a8ce7ef-8ed6-4b5e-a0cb-737506678e1b","<ETHING_SERVER_HOSTNAME_OR_IP>",&client);
    
    // initialize the Ethernet or WiFi library
}
```


### Code examples

Many examples can be found in the 'examples' directory.

