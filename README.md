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


The eThing librairy need an internet connection to work.

You can use either the [Ethernet library](https://www.arduino.cc/en/Reference/Ethernet) or the [WiFi library](https://www.arduino.cc/en/Reference/WiFi)
or any other librairy that are compatible with the [`Client`](https://www.arduino.cc/en/Reference/ClientConstructor) absctract class.

You must provide an ething server hostname/ip, an API key and a client to the `EThing::init` funtion.

To get an API key, first create a new device under the Device page. Then go to the developer page to get his dedicated API key.

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
    EThing::init("<ETHING_SERVER_HOSTNAME_OR_IP>","5a8ce7ef-8ed6-4b5e-a0cb-737506678e1b",&client);
    
    // initialize the Ethernet or WiFi library
}
```


### Code examples

Once the library is correctly initialized, use the different EThing classes to create resource (File, Table ...) and to store data in it !

Note: since it is possible for two or more resources to share the same name, the unique way to identify a resource is by its id.

Use the `EThing::Id` class to store the id.

```cpp
EThing::Id id("55c0eda");
// to print it
id.printTo(Serial);
// or
Serial.print(id.data());
// to test if an EThing::Id object store a valid it, just cast it into a boolean or use the .isValid() method
if(id)
    /* valid */
else
    /* invalid */
```

> **Note :** Most of the EThing functions deals with id rather than resource's name.


#### Create a file and write in it

```cpp
EThing::FileWriter fileWriter;

if(fileWriter.createAndOpen("myfile.txt")){
	
    /* send your content */
    fileWriter.print("hello world !");
    
    if(fileWriter.close()){
        /* success */
    }
    else {
        /* error : the HTTP response code is given by fileWriter.getStatusCode() */
    }
}
else {
    /* error : unable to reach the server */
}
```


#### read a file

```cpp
EThing::FileReader fileReader;

if(fileReader.open(EThing::Id("55c0eda"))){
    // make a buffered read
    char buff[24], n;
    while((n=fileReader.readBytes(buff,24))==24)
        Serial.write(buff,n);
    
    fileReader.close();
}
else {
    /* error : the HTTP response code is given by fileReader.getStatusCode() */
}
```


#### create an empty file

```cpp
EThing::Id fileId = EThing::File::create("myfile.txt");

if(fileId){
    /* the file was created */
    /* the fileId store the unique id of the just created file necessary to identify it */
    /* you can now write into that file using the EThing::FileWriter class */
}
else {
    /* error */
}
```


#### create a table

```cpp
EThing::Id tableId = EThing::Table::create("mytable.db");

if(tableId){
    /* the table was created */
    /* the fileId store the unique id of the just created table necessary to identify it */
    /* you can now add data to that table using the EThing::TableWriter class */
}
else {
    /* error */
}
```

#### append data to a table

```cpp
EThing::TableWriter tableWriter;
/* replace by your own table id */
EThing::Id tableId("56bdf0f");

if(tableWriter.open(tableId)){
	
    /* send your data */
    tableWriter.store("int",-785);
    tableWriter.store("float",4.123456789);
    tableWriter.store("string","foobar");
	
    if(tableWriter.close()){
        /* success */
    }
    else {
        /* error : the HTTP response code is given by tableWriter.getStatusCode() */
		
        if(tableWriter.getStatusCode()==401){
            /* not authorized ! */
        }
        else if(tableWriter.getStatusCode()==404){
            /* the table does not exist, check the table's id */
        }
        else {
            /* other error */
        }
    }
}
else {
    /* error : unable to reach the server */
}
```

#### send a notification

```cpp
// send an email to me !
if(EThing::notify("subject", "my message here")){
    /* success */
}
else {
    /* error */
}
```

