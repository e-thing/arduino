



/*
* Necessary include
*/

#include <SPI.h>
#include <Ethernet.h>
#include <EThing.h>



char api_host[64];
char api_key[64];


/*
*  ETHERNET CONFIGURATION
*/


// set the mac address of your ethernet board here :
byte mac[]        = {0x90, 0xA2, 0xDA, 0x00, 0x34, 0x58 };

EthernetClient ethernetClient;




void help(){
	Serial.println();
	Serial.println("EThing demo");
	Serial.println(" c : configure");
	Serial.println(" l : list the resources");
	Serial.println(" F : create a new file");
	Serial.println(" f : read a file");
	Serial.println(" T : create a new table");
	Serial.println(" t : read a table");
	Serial.println(" r : remove a resource");
	Serial.println();
}


void configure(){
	char buf[64];
	int buflen;
	
	Serial.println("URL/IP of your EThing server:");
	
	while(!Serial.available());
	buflen = Serial.readBytes(buf,64);
	buf[buflen] = '\0';
	
	strncpy(api_host,buf,buflen); 
	
	Serial.println("API KEY of your device:");
	
	while(!Serial.available());
	buflen = Serial.readBytes(buf,64);
	buf[buflen] = '\0';
	
	strncpy(api_key,buf,buflen);
	
	// save into the eeprom
	eeprom_write_byte((uint8_t *)0,0x5E); // this value tells that the memory contains a configuration set
	eeprom_write_block((const void *)api_host,(void *)1,64);
	eeprom_write_block((const void *)api_key,(void *)66,64);
	
	EThing::init(api_host,api_key,&ethernetClient);
}


void loadConfiguration(){
	// load from EEPROM
	if(eeprom_read_byte((uint8_t *)0) == 0x5E){
		eeprom_read_block((void *)api_host,(const void *)1,64);
		eeprom_read_block((void *)api_key,(const void *)66,64);
		
		EThing::init(api_host,api_key,&ethernetClient);
	}
	else {
		// no configuration set !
		// create a new one
		return configure();
	}
}


void list(){
	
	char query[64];
	int querylen;
	
	Serial.println("query or leave empty:");
	Serial.println();
	
	EThing::Request req(EThing::GET,"resources?fields=id,name,type&q=");
	if(req.send(query)){
	  // print the status code
	  int httpCode = req.getStatusCode();
	  if(httpCode==200){
		  // read the response
		  
		  // this request returns JSON
		  // parse the response
		  // for each resource object
		  while(req.find("{")){
		  	// parse a resource object
		  	while(req.find(":")){
		  		// read and print the value until ',' or '}'
		  		char c;
		  		while(req.readBytes(&c,1) && c!=',' && c!='}'){ // timed read
		  			Serial.print(c);
		  		}
		  		if(c=='}'){
		  			// end of the resource object
		  			Serial.println();
		  			break;
		  		}
		  		else
		  			Serial.print(' ');
		  	}
		  }
		  
		  Serial.println("OK");
		  
	  } else {
			Serial.print("Err: server returns code ");
			Serial.println(httpCode);
	  }
	  req.close();
	} else {
		Serial.print("Err: unreachable ");
		Serial.println(api_host);
	}
}

void createFile(){
	char name[64], data[64];
	int namelen, datalen;
	
	Serial.println("Name:");
	
	while(!Serial.available());
	namelen = Serial.readBytes(name,64);
	name[namelen] = '\0';
	
	Serial.println("Content of the file (64b max):");
	
	while(!Serial.available());
	datalen = Serial.readBytes(data,64);
	
	EThing::FileWriter fileWriter;
	if(fileWriter.createAndOpen(name)){
		
		fileWriter.write((const uint8_t*)data,datalen);
		
		if(!fileWriter.close()){
			Serial.print("Err: server returns code ");
			Serial.println(fileWriter.getStatusCode());
		}
		else
			Serial.println("OK");
	}
	else {
		Serial.print("Err: unreachable ");
		Serial.println(api_host);
	}
	
}

void readFile(){
	char buf[16];
	int buflen, n;
	
	Serial.println("Id:");
	
	while(!Serial.available());
	buflen = Serial.readBytes(buf,16);
	buf[buflen] = '\0';
	
	if(buflen != ETHING_ID_LENGTH){
		Serial.print("Err: invalid ID: ");
		Serial.println(buf);
		return;
	}
	
	EThing::FileReader fileReader;
	if(fileReader.open(EThing::Id(buf))){
		// make a buffered read
		while(n=fileReader.readBytes(buf,16)){
			Serial.write(buf,n);
			if(n<16) break;
		}
		Serial.println();
		
		fileReader.close();
		
		Serial.println("OK");
	}
	else {
		int httpCode = fileReader.getStatusCode();
		if(httpCode==0){
			Serial.print("Err: unreachable ");
			Serial.println(api_host);
		}
		else {
			Serial.print("Err: server returns code ");
			Serial.println(httpCode);
		}
	}
	
}

void createTable(){
	char name[64], data[64];
	int namelen, datalen, httpCode;
	
	Serial.println("Name:");
	
	while(!Serial.available());
	namelen = Serial.readBytes(name,64);
	name[namelen] = '\0';
	
	EThing::Id id = EThing::Table::create(name,0,0,&httpCode);
	
	if(!id){
		if(httpCode==0){
			Serial.print("Err: unreachable ");
			Serial.println(api_host);
		} else {
			Serial.print("Err: server returns code ");
			Serial.println(httpCode);
		}
		return;
	}
	
	Serial.println("Column:");
	
	while(!Serial.available());
	namelen = Serial.readBytes(name,64);
	name[namelen] = '\0';
	
	Serial.println("Data(64b max):");
	
	while(!Serial.available());
	datalen = Serial.readBytes(data,64);
	
	EThing::TableWriter tableWriter;
	if(tableWriter.open(id)){
		
		tableWriter.store(name,data,datalen);
		
		if(tableWriter.close()){
			Serial.println("OK");
		} else {
			Serial.print("Err: server returns code ");
			Serial.println(tableWriter.getStatusCode());
		}
			
	}
	else {
		Serial.print("Err: unreachable ");
		Serial.println(api_host);
	}
	
}

void readTable(){
	char url[22] = "table/";
	char * id = url+6;
	int idlen;
	
	Serial.println("Id:");
	
	while(!Serial.available());
	idlen = Serial.readBytes(id,16);
	id[idlen] = '\0';
	
	if(idlen != ETHING_ID_LENGTH){
		Serial.print("Err: invalid ID: ");
		Serial.println(id);
		return;
	}
	
	EThing::Request req(EThing::GET,url);
	if(req.send("?limit=20")){
		int httpCode = req.getStatusCode();
		
		if(httpCode==200){
			
			// read the response
			while(req){
				if(req.available())
					Serial.write(req.read());
				else
					delay(5); // temporise
			}
			
			Serial.println();
			Serial.println("OK");
		}
		else {
			Serial.print("Err: server returns code ");
			Serial.println(httpCode);
		}
		
		req.close();
	}
	else {
		Serial.print("Err: unreachable ");
		Serial.println(api_host);
	}
}

void remove(){
	char id[16];
	int idlen;
	
	Serial.println("Id:");
	
	while(!Serial.available());
	idlen = Serial.readBytes(id,16);
	id[idlen] = '\0';
	
	if(idlen != ETHING_ID_LENGTH){
		Serial.print("Err: invalid ID: ");
		Serial.println(id);
		return;
	}
	
	EThing::Request req(EThing::DELETE,"resources/");
	if(req.send(id)){
		int httpCode = req.getStatusCode();
		req.close();
		
		if(httpCode==200){
			Serial.println("OK");
		}
		else {
			Serial.print("Err: server returns code ");
			Serial.println(httpCode);
		}
	}
	else {
		Serial.print("Err: unreachable ");
		Serial.println(api_host);
	}
}







void setup(void)
{
	
	Serial.begin(115200);
	Serial.println();
	
	loadConfiguration();
	
	if (Ethernet.begin(mac) == 0) {
		Serial.println("Err: unable to connect to the network using DHCP");
		while(1);
	}
	
	help();
	
}

void loop(void)
{

	if(Serial.available()){
		
		switch(Serial.read()){
			case 'c':
				configure();
				break;
			case 'l':
				list();
				break;
			case 'F':
				createFile();
				break;
			case 'f':
				readFile();
				break;
			case 'T':
				createTable();
				break;
			case 't':
				readTable();
				break;
			case 'r':
				remove();
				break;
			default:
				help();
				break;
		}
		
	}
	
}

