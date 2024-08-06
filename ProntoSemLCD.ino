#include <ArduinoJson.h>
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include <Adafruit_SPIDevice.h>

#include <Adafruit_MCP23X08.h>
#include <Adafruit_MCP23X17.h>

#include <Ezo_i2c.h>
#include <Ezo_i2c_util.h>
#include <iot_cmd.h>
#include <sequencer1.h>
#include <sequencer2.h>
#include <sequencer3.h>
#include <sequencer4.h>

#include <iot_cmd.h>
#include <Ezo_i2c.h>                                             //include the EZO I2C library from https://github.com/Atlas-Scientific/Ezo_I2c_lib
#include <Wire.h>                                                //include Arduinos i2c library
#include <ESP8266WiFi.h>                                         //include esp8266 wifi library 
#include "ThingSpeak.h"                                          //include thingspeak library  
#include <sequencer3.h>                                          //imports a 3 function sequencer 
#include <sequencer1.h>                                          //imports a 1 function sequencer 
#include <Ezo_i2c_util.h>                                        //brings in common print statements
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_MCP23X17.h>

#include <LiquidCrystal_I2C.h>

// LiquidCrystal_I2C Serial(0x3F, 16, 2); // set the Serial address to 0x3F for a 20 chars and 4 line display


#define saida1 0     // MCP23XXX pin LED is attached to
#define saida2 1     // MCP23XXX pin LED is attached to
#define saida3 2     // MCP23XXX pin LED is attached to
#define saida4 3     // MCP23XXX pin LED is attached to
#define saida5 4     // MCP23XXX pin LED is attached to
#define saida6 5     // MCP23XXX pin LED is attached to
#define saida7 6     // MCP23XXX pin LED is attached to
#define saida8 7     // MCP23XXX pin LED is attached to
#define saida9 8     // MCP23XXX pin LED is attached to
#define saida10 9     // MCP23XXX pin LED is attached to
#define saida11 10     // MCP23XXX pin LED is attached to
#define saida12 11     // MCP23XXX pin LED is attached to

#define botaosair 12     // MCP23XXX pin LED is attached to
#define botaoentrar 13     // MCP23XXX pin LED is attached to
#define botaomudar 14     // MCP23XXX pin LED is attached to


Adafruit_MCP23X17 mcp;

WiFiClient  client;                                              //declare that this device connects to a Wi-Fi network,create a connection to a specified internet IP address

// #define BLYNK_PRINT Serial

//----------------Fill in your Wi-Fi / ThingSpeak Credentials-------
const String ssid = "CLARO_2G6469F3"; //"CLARO_2G6469F3"; // "Redmi Note 9"; // "Saturno"; // "Redmi Note 9"; //                                //The name of the Wi-Fi network you are connecting to

const String pass = "C96469F3"; //"C96469F3";  //"Pedro0123"; // "familia1";  // "Pedro0123"; //                           //Your WiFi network password

const long myChannelNumber = 1466041;                            //Your Thingspeak channel number
const char * myWriteAPIKey = "45RRXIFNQCRJK1B3";                 //Your ThingSpeak Write API Key

int tentativa = 0;
int modolocal = 0;

int modomanual = 0;


//------------------------------------------------------------------

char auth[] = "HGlB7-MGyuTw8n271lxceq3CyyMcj-Kj";

float blynk_temp_min = 15;
float blynk_temp_max = 30;
float blynk_tempo_temp = 5;

float blynk_ph_min = 5;
float blynk_ph_max = 8;
float blynk_tempo_ph = 5;

float blynk_orp_min = 200;
float blynk_orp_max = 600;
float blynk_tempo_orp = 5;

float blynk_do_min = 5;
float blynk_do_max = 20;
float blynk_tempo_do = 5;

float blynk_sal_min = 0;
float blynk_sal_max = 42;
float blynk_tempo_ec = 5;

float blynk_cond_min = 15000;
float blynk_cond_max = 25000;


Ezo_board DO = Ezo_board(97, "DO");      //create an EC circuit object who's address is 100 and name is "EC"
Ezo_board ORP = Ezo_board(98, "ORP");    //create an RTD circuit object who's address is 102 and name is "RTD"
Ezo_board PH = Ezo_board(99, "PH");    //create an RTD circuit object who's address is 102 and name is "RTD"
Ezo_board RTD = Ezo_board(102, "RTD");    //create an RTD circuit object who's address is 102 and name is "RTD"
Ezo_board EC = Ezo_board(100, "EC");

#define addressDO 97               //default I2C ID number for EZO DO Circuit.
#define addressORP 98               //default I2C ID number for EZO ORP Circuit.
#define addressPH 99              //default I2C ID number for EZO RTD Circuit.
#define addressEC 100
#define addressRTD 102              //default I2C ID number for EZO RTD Circuit.

char computerdata[20];           //we make a 20 byte character array to hold incoming data from a pc/mac/other.
byte received_from_computer = 0; //we need to know how many characters have been received.
byte serial_event = 0;           //a flag to signal when data has been received from the pc/mac/other.
byte code = 0;                   //used to hold the I2C response code.
char ec_data[32];                //we make a 32 byte character array to hold incoming data from the EC circuit.
char ph_data[20];
byte in_char = 0;                //used as a 1 byte buffer to store inbound bytes from the EC Circuit.
byte i = 0;                      //counter used for ec_data array.
int time_ = 570;                 //used to change the delay needed depending on the command sent to the EZO Class EC Circuit.
int tempodeespera = 1000;




char *ec;                        //char pointer used in string parsing.
char *tds;                       //char pointer used in string parsing.
char *sal;                       //char pointer used in string parsing.
char *sg;                        //char pointer used in string parsing.

float ec_float;                  //float var used to hold the float value of the conductivity.
float tds_float;                 //float var used to hold the float value of the TDS.
float sal_float;                 //float var used to hold the float value of the salinity.
float sg_float;                  //float var used to hold the float value of the specific gravity.
float sal1;
float sal2;

float ph_float;                  //float var used to hold the float value of the pH.
float RTD_float;                 //float var used to hold the float value of the RTD.
float ORP_float;                 //float var used to hold the float value of the ORP.
float DO_float;                  //float var used to hold the float value of the D.O.
float EC_float;

WidgetLED led1(V18);
WidgetLED led2(V19);
WidgetLED led3(V20);
WidgetLED led4(V21);
WidgetLED led5(V22);
WidgetLED led6(V23);
WidgetLED led7(V24);
WidgetLED led8(V25);
WidgetLED led9(V26);
WidgetLED led10(V27);
WidgetLED led11(V28);
WidgetLED led12(V29);

bool rele1 = 0;
bool rele2 = 0;
bool rele3 = 0;
bool rele4 = 0;
bool rele5 = 0;
bool rele6 = 0;
bool rele7 = 0;
bool rele8 = 0;
bool rele9 = 0;
bool rele10 = 0;
bool rele11 = 0;
bool rele12 = 0;

int blynk_botao1 = 0;
int blynk_botao2 = 0;
int blynk_botao3 = 0;
int blynk_botao4 = 0;
int blynk_botao5 = 0;
int blynk_botao6 = 0;
int blynk_botao7 = 0;
int blynk_botao8 = 0;
int blynk_botao9 = 0;
int blynk_botao10 = 0;
int blynk_botao11 = 0;
int blynk_botao12 = 0;
BlynkTimer timer;

#include <ESP8266WiFi.h>
const int myled = 0;
WiFiServer server(80);

bool wifi_isconnected() {                       //function to check if wifi is connected
  return (WiFi.status() == WL_CONNECTED);
  modolocal = 0;
  tentativa = 0;
}

void reconnect_wifi()
{ //function to reconnect wifi if its not connected

  digitalWrite(myled, HIGH);
  Serial.println("VOID");
  Serial.println("CONECTANDO");
  delay(500);

  //WiFi.begin("malab", "bartira02");
  WiFi.begin(ssid, pass);
  Serial.println("connecting to wifi");
  delay(500);

  while ((WiFi.status() != WL_CONNECTED) && (tentativa < 15))
  {
    tentativa++;
    delay(500);
    Serial.println(".");

    
    Serial.println("Wifi Conectando:");
    
    Serial.println(tentativa);
    delay(1000);
  }
  if (tentativa > 14)
  {
    
    Serial.println("Wifi:");
    
    Serial.println("Falhou");
    delay(4000);

    modolocal = 1;
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Wifi:");
    
    Serial.println("Conectado");
    Serial.print("ESP8266 IPv4: ");
    Serial.println(WiFi.localIP());
    server.begin();

    delay(2000);

    modolocal = 0;
  }

}

void thingspeak_send() {                                          //if we're datalogging
  if (wifi_isconnected()) {
    int return_code = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if (return_code == 200) {                                                          //code for successful transmission
      Serial.println("sent to thingspeak");
      modolocal = 0;
    }
    else {
      Serial.println("couldnt send to thingspeak, ERROR: ");
      Serial.println(return_code);
      Serial.println("Ou modo local");
    }
  }
}

void step1();    //forward declarations of functions to use them in the sequencer before defining them
void step2();
void step3();

const unsigned long thingspeak_delay = 1000;            //how long we wait to send values to thingspeak, in milliseconds
const unsigned long reading_delay = 1000;                 //how long we wait to receive a response, in milliseconds

Sequencer3 Seq(&step1, reading_delay,  //calls the steps in sequence with time in between them
               &step2, reading_delay,
               &step3, 2000);


Sequencer1 Wifi_Seq( &reconnect_wifi, 10000);   //calls the wifi reconnect function every 10 seconds

Sequencer1 Thingspeak_seq( &thingspeak_send, thingspeak_delay);    //calls the function to send data to thingspeak every [thingspeak_delay] milliseconds



// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.

void setup() {                                                                    //set up the hardware

  pinMode(myled, OUTPUT);
  digitalWrite(myled, LOW);

  Wire.begin();                                                                   //enable I2C port
  Serial.begin(9600);                                                             //enable serial port, set baud rate to 9600
  WiFi.mode(WIFI_STA);                                                            //set ESP8266 mode as a station to be connected to wifi network
  ThingSpeak.begin(client);                                                       //enable ThingSpeak connection
  
  Serial.print("Welcome");
  
  Serial.println(" - ALTAMAR ");
  delay(5000);
  

  reconnect_wifi();

  Seq.reset();
  Thingspeak_seq.reset();
  
  
  if (modolocal == 0)
  {
    Serial.println("Modo local == 0");
  }


//  if (!mcp.begin_I2C()) {
//    //if (!mcp.begin_SPI(CS_PIN)) {
//    Serial.println("Error.");
//    // ISSO GERA O LOOP INFINITO DEPOIS DO ERROR. -> while (1);
//  }
  mcp.pinMode(saida12, OUTPUT);
  mcp.pinMode(saida1, OUTPUT);
  mcp.pinMode(saida2, OUTPUT);
  mcp.pinMode(saida3, OUTPUT);
  mcp.pinMode(saida4, OUTPUT);
  mcp.pinMode(saida5, OUTPUT);
  mcp.pinMode(saida6, OUTPUT);
  mcp.pinMode(saida7, OUTPUT);
  mcp.pinMode(saida8, OUTPUT);
  mcp.pinMode(saida9, OUTPUT);
  mcp.pinMode(saida10, OUTPUT);
  mcp.pinMode(saida11, OUTPUT);


  mcp.digitalWrite(saida12, HIGH);
  mcp.digitalWrite(saida1, HIGH);
  mcp.digitalWrite(saida2, HIGH);
  mcp.digitalWrite(saida3, HIGH);
  mcp.digitalWrite(saida4, HIGH);
  mcp.digitalWrite(saida5, HIGH);
  mcp.digitalWrite(saida6, HIGH);
  mcp.digitalWrite(saida7, HIGH);
  mcp.digitalWrite(saida8, HIGH);
  mcp.digitalWrite(saida9, HIGH);
  mcp.digitalWrite(saida10, HIGH);
  mcp.digitalWrite(saida11, HIGH);

  mcp.pinMode(botaosair, INPUT_PULLUP);
  mcp.pinMode(botaoentrar, INPUT_PULLUP);
  mcp.pinMode(botaomudar, INPUT_PULLUP);





}

void serialEvent() {                                                              //this interrupt will trigger when the data coming from the serial monitor(pc/mac/other) is received.
  received_from_computer = Serial.readBytesUntil(13, computerdata, 20);           //we read the data sent from the serial monitor(pc/mac/other) until we see a <CR>. We also count how many characters have been received.
  computerdata[received_from_computer] = 0;                                       //stop the buffer from transmitting leftovers or garbage.
  serial_event = true;                                                            //set the serial event flag.
}


void leituradecomando()
{

  
  
  Serial.println("VOID LEITURA");

  while (mcp.digitalRead(botaosair) == HIGH)
  {

    time_ = 1000;


    delay(2000);
    Serial.println("Entrou no while do leitura de comandos");
    
    

    Serial.println("Cal Sensor RTD ?");
    
    Serial.println("PRESSIONE BOTAO ");
    delay(4000);

    if ( mcp.digitalRead(botaoentrar) == LOW)
    {
      while ( mcp.digitalRead(botaosair) == HIGH)
      {
        
        
        Serial.println("Selecionado RTD");
        
        Serial.println("PRESS P CAL");
        delay(2000);
        if ( mcp.digitalRead(botaoentrar) == LOW)
        {

          serialEvent();
          Wire.beginTransmission(addressRTD);
          delay(1000);


          char calrtd[8] = "cal,20" ;
          Wire.write(calrtd);

          delay(1000);
          Wire.endTransmission();
          
          
          Serial.println(calrtd);
          delay(2000);
        }
      }
    }

    //PARA SEGUNDO SENSOR

    delay(2000);
    
    
    Serial.println("Cal Sensor PH ?");
    
    Serial.println("PRESSIONE BOTAO ");
    delay(2000);
    if ( mcp.digitalRead(botaoentrar) == LOW)
    {
      while ( mcp.digitalRead(botaosair) == HIGH)
      {

        
        
        Serial.println("1 - ph 7 ?");
        
        Serial.println("PRESS P CAL");
        delay(2000);

        if ( mcp.digitalRead(botaoentrar) == LOW)
        {

          serialEvent();
          Wire.beginTransmission(addressPH);
          delay(1000);


          char calph[] = "cal,mid,7" ; //colocar mid,high,low
          Wire.write(calph);

          delay(1000);
          Wire.endTransmission();
          
          
          Serial.println(calph);
          delay (2000);
        }


        delay(2000);
        
        
        Serial.println("2 - ph 4 ?");
        
        Serial.println("PRESSIONE BOTAO ");
        delay(2000);


        if ( mcp.digitalRead(botaoentrar) == LOW)
        {

          serialEvent();
          Wire.beginTransmission(addressPH);
          delay(1000);


          char calph[10] = "cal,low,4" ; //colocar mid,high,low
          Wire.write(calph);

          delay(1000);
          Wire.endTransmission();
          
          
          Serial.println(calph);
          delay (2000);
        }

        delay(2000);
        
        
        Serial.println("1 - ph 10 ?");
        
        Serial.println("PRESSIONE BOTAO ");
        delay(2000);

        if ( mcp.digitalRead(botaoentrar) == LOW)
        {

          serialEvent();
          Wire.beginTransmission(addressPH);
          delay(1000);


          char calph[] = "cal,high,10" ; //colocar mid,high,low
          Wire.write(calph);

          delay(1000);
          Wire.endTransmission();
          
          
          Serial.println(calph);
          delay (2000);
        }
      }
    }
    //PARA terceiro SENSOR

    delay(2000);
    
    
    Serial.println("Cal Sensor DO ?");
    
    Serial.println("PRESSIONE BOTAO ");
    delay(4000);

    if ( mcp.digitalRead(botaoentrar) == LOW)
    {
      while ( mcp.digitalRead(botaosair) == HIGH)
      {
        
        
        Serial.println("O2 ambiente ?");
        
        Serial.println("PRESS P CAL");
        delay(2000);

        if ( mcp.digitalRead(botaoentrar) == LOW)
        {

          serialEvent();
          Wire.beginTransmission(addressDO);
          delay(1000);


          char caldo[] = "cal" ;
          Wire.write(caldo);

          delay(1000);
          Wire.endTransmission();
          
          
          Serial.println(caldo);
          delay (2000);
        }

        
        
        Serial.println("O2 zero ?");
        
        Serial.println("PRESS P CAL");
        delay(2000);

        if ( mcp.digitalRead(botaoentrar) == LOW)
        {

          serialEvent();
          Wire.beginTransmission(addressDO);
          delay(1000);


          char caldo[] = "cal,0" ;
          Wire.write(caldo);

          delay(1000);
          Wire.endTransmission();
          
          
          Serial.println(caldo);
          delay(2000);
        }

      }
    }

    //PARA quarto SENSOR


    
    
    Serial.println("Cal Sensor ORP ?");
    
    Serial.println("PRESSIONE BOTAO ");
    delay(4000);

    if ( mcp.digitalRead(botaoentrar) == LOW)
    {
      while ( mcp.digitalRead(botaosair) == HIGH)
      {
        
        
        Serial.println("ORP 225 ?");
        
        Serial.println("PRESS P CAL");
        delay(2000);

        if ( mcp.digitalRead(botaoentrar) == LOW)
        {

          serialEvent();
          Wire.beginTransmission(addressORP);
          delay(1000);


          char calorp[] = "cal,225" ;
          Wire.write(calorp);

          delay(1000);
          Wire.endTransmission();
          
          
          Serial.println(calorp);
          delay (2000);
        }
      }
    }

    //PARA quinto SENSOR

    delay(2000);
    
    
    Serial.println("Cal Sensor EC ?");
    
    Serial.println("PRESSIONE BOTAO ");
    delay(4000);

    if ( mcp.digitalRead(botaoentrar) == LOW)
    {
      while ( mcp.digitalRead(botaosair) == HIGH)
      {
        
        
        Serial.println("Cal dry ?");
        
        Serial.println("PRESS P CAL");
        delay(2000);

        if ( mcp.digitalRead(botaoentrar) == LOW)
        {

          serialEvent();
          Wire.beginTransmission(addressEC);
          delay(1000);


          char calec[] = "cal,dry" ;
          Wire.write(calec);

          delay(1000);
          Wire.endTransmission();
          
          
          Serial.println(calec);
          delay (2000);
        }

        
        
        Serial.println("Cal,low,12880 ?");
        
        Serial.println("PRESS P CAL");
        delay(2000);

        if ( mcp.digitalRead(botaoentrar) == LOW)
        {

          serialEvent();
          Wire.beginTransmission(addressEC);
          delay(1000);


          char calec[] = "cal,low,12880" ;
          Wire.write(calec);

          delay(1000);
          Wire.endTransmission();
          
          
          Serial.println(calec);
          delay(2000);
        }

        
        
        Serial.println("Cal,high,150000?");
        
        Serial.println("PRESS P CAL");
        delay(2000);

        if ( mcp.digitalRead(botaoentrar) == LOW)
        {

          serialEvent();
          Wire.beginTransmission(addressEC);
          delay(1000);


          char calec[] = "cal,high,150000" ;
          Wire.write(calec);

          delay(1000);
          Wire.endTransmission();
          
          
          Serial.println(calec);
          delay(2000);
        }
      }
    }

    if (serial_event == true) {                                                     //if a command was sent to the EZO device.
      for (i = 0; i <= received_from_computer; i++) {                               //set all char to lower case, this is just so this exact sample code can recognize the "sleep" command.
        computerdata[i] = tolower(computerdata[i]);                                 //"Sleep" ≠ "sleep"
      }
      i = 0;                                                                        //reset i, we will need it later
      if (computerdata[0] == 'c' || computerdata[0] == 'r')time_ = 1000;             //if a command has been sent to calibrate or take a reading we wait 815ms so that the circuit has time to take the reading.
      else time_ = 500;                                                             //if any other command has been sent we wait only 250ms.
    }





    if (strcmp(computerdata, "sleep") != 0)
    { //if the command that has been sent is NOT the sleep command, wait the correct amount of time and request data.
      delay(time_);                                                               //wait the correct amount of time for the circuit to complete its instruction.
      Wire.requestFrom(addressRTD, 20, 1);                                           //call the circuit and request 20 bytes (this may be more than we need)
      code = Wire.read();                                                         //the first byte is the response code, we read this separately.
    }



    
    
    Serial.println("saiu do if");
    delay(1000);


    if (computerdata[0] == '2')
    {
      Serial.println("Digite o comando PARA MODULO PH");
      delay(5000);
      serialEvent();
      Serial.println(received_from_computer);
      Wire.beginTransmission(addressPH);                                              //call the circuit by its ID number.
      Wire.write(computerdata);                                                     //transmit the command that was sent through the serial port.
      Wire.endTransmission();

      if (strcmp(computerdata, "sleep") != 0) {                                      //if the command that has been sent is NOT the sleep command, wait the correct amount of time and request data.


        delay(time_);                                                               //wait the correct amount of time for the circuit to complete its instruction.

        Wire.requestFrom(addressPH, 20, 1);                                           //call the circuit and request 20 bytes (this may be more than we need)
        code = Wire.read();                                                         //the first byte is the response code, we read this separately.


      }
    }

    if (computerdata[0] == '3')
    {

      Serial.println("Digite o comando PARA MODULO DO");
      delay(5000);
      serialEvent();
      Serial.println(received_from_computer);
      Wire.beginTransmission(addressDO);                                              //call the circuit by its ID number.
      Wire.write(computerdata);                                                     //transmit the command that was sent through the serial port.
      Wire.endTransmission();

      if (strcmp(computerdata, "sleep") != 0) {                                      //if the command that has been sent is NOT the sleep command, wait the correct amount of time and request data.


        delay(time_);                                                               //wait the correct amount of time for the circuit to complete its instruction.

        Wire.requestFrom(addressDO, 20, 1);                                           //call the circuit and request 20 bytes (this may be more than we need)
        code = Wire.read();                                                         //the first byte is the response code, we read this separately.


      }
    }

    if (computerdata[0] == '4')
    {
      Serial.println("Digite o comando PARA MODULO ORP");
      delay(5000);
      serialEvent();
      Serial.println(received_from_computer);
      Wire.beginTransmission(addressORP);                                              //call the circuit by its ID number.
      Wire.write(computerdata);                                                     //transmit the command that was sent through the serial port.
      Wire.endTransmission();
      if (strcmp(computerdata, "sleep") != 0) {                                      //if the command that has been sent is NOT the sleep command, wait the correct amount of time and request data.


        delay(time_);                                                               //wait the correct amount of time for the circuit to complete its instruction.

        Wire.requestFrom(addressORP, 20, 1);                                           //call the circuit and request 20 bytes (this may be more than we need)
        code = Wire.read();                                                         //the first byte is the response code, we read this separately.

      }

    }

    if (computerdata[0] == '5')
    {
      Serial.println("Digite o comando PARA MODULO EC");
      delay(5000);
      serialEvent();
      Serial.println(received_from_computer);
      Wire.beginTransmission(addressEC);                                              //call the circuit by its ID number.
      Wire.write(computerdata);                                                     //transmit the command that was sent through the serial port.
      Wire.endTransmission();

      if (strcmp(computerdata, "sleep") != 0) {                                      //if the command that has been sent is NOT the sleep command, wait the correct amount of time and request data.


        delay(2000);                                                               //wait the correct amount of time for the circuit to complete its instruction.

        Wire.requestFrom(addressEC, 20, 1);                                           //call the circuit and request 20 bytes (this may be more than we need)
        code = Wire.read();                                                         //the first byte is the response code, we read this separately.

      }
    }







    switch (code) {                       //switch case based on what the response code is.
      case 1:                             //decimal 1.
        Serial.println("Success");        //means the command was successful.
        break;                            //exits the switch case.

      case 2:                             //decimal 2.
        Serial.println("Failed");         //means the command has failed.
        break;                            //exits the switch case.

      case 254:                           //decimal 254.
        Serial.println("Pending");        //exits the switch case.

      case 255:                           //decimal 255.
        Serial.println("No Data");          //means the command has not yet been finished calculating.
        break;                          //means there is no further data to send.
        break;                            //exits the switch case.
    }                                                   //end the I2C data transmission.








    while (Wire.available()) {            //are there bytes to receive.
      in_char = Wire.read();              //receive a byte.
      ph_data[i] = in_char;               //load this byte into our array.
      i += 1;                             //incur the counter for the array element.
      if (in_char == 0) {                 //if we see that we have been sent a null command.
        i = 0;                            //reset the counter i to 0.
        break;                            //exit the while loop.

      }

      Serial.println(ph_data);              //print the data.

      serial_event = false;                   //reset the serial event flag.

      delay(5000);


    }
  }
}

void condicoesmanuais()
{

  if (rele1 == 1)
  {
    led1.on();
    rele1 = 1;
    mcp.digitalWrite(saida1, LOW);
  }
  else
  {
    led1.off();
    rele1 = 0;
    mcp.digitalWrite(saida1, HIGH);
  }

  if (rele2 == 1)
  {
    led2.on();
    rele2 = 1;
    mcp.digitalWrite(saida2, LOW);
  }
  else
  {
    led2.off();
    rele2 = 0;
    mcp.digitalWrite(saida2, HIGH);
  }

  if (rele3 == 1)
  {
    led3.on();
    rele3 = 1;
    mcp.digitalWrite(saida3, LOW);
  }
  else
  {
    led3.off();
    rele3 = 0;
    mcp.digitalWrite(saida3, HIGH);
  }

  if (rele4 == 1)
  {
    led4.on();
    rele4 = 1;
    mcp.digitalWrite(saida4, LOW);
  }
  else
  {
    led4.off();
    rele4 = 0;
    mcp.digitalWrite(saida4, HIGH);
  }
  if (rele5 == 1)
  {
    led5.on();
    rele5 = 1;
    mcp.digitalWrite(saida5, LOW);
  }
  else
  {
    led5.off();
    rele5 = 0;
    mcp.digitalWrite(saida5, HIGH);
  }
  if (rele6 == 1)
  {
    led6.on();
    rele6 = 1;
    mcp.digitalWrite(saida6, LOW);
  }
  else
  {
    led6.off();
    rele6 = 0;
    mcp.digitalWrite(saida6, HIGH);
  }
  if (rele7 == 1)
  {
    led7.on();
    rele7 = 1;
    mcp.digitalWrite(saida7, LOW);
  }
  else
  {
    led7.off();
    rele7 = 0;
    mcp.digitalWrite(saida7, HIGH);
  }
  if (rele8 == 1)
  {
    led8.on();
    rele8 = 1;
    mcp.digitalWrite(saida8, LOW);
  }
  else
  {
    led8.off();
    rele8 = 0;
    mcp.digitalWrite(saida8, HIGH);
  }
  if (rele9 == 1)
  {
    led9.on();
    rele9 = 1;
    mcp.digitalWrite(saida9, LOW);
  }
  else
  {
    led9.off();
    rele9 = 0;
    mcp.digitalWrite(saida9, HIGH);
  }
  if (rele10 == 1)
  {
    led10.on();
    rele10 = 1;
    mcp.digitalWrite(saida10, LOW);
  }
  else
  {
    led10.off();
    rele10 = 0;
    mcp.digitalWrite(saida10, HIGH);
  }
  if (rele11 == 1)
  {
    led11.on();
    rele11 = 1;
    mcp.digitalWrite(saida11, LOW);
  }
  else
  {
    led11.off();
    rele11 = 0;
    mcp.digitalWrite(saida11, HIGH);
  }
  if (rele12 == 1)
  {
    led12.on();
    rele12 = 1;
    mcp.digitalWrite(saida12, LOW);
  }
  else
  {
    led12.off();
    rele12 = 0;
    mcp.digitalWrite(saida12, HIGH);
  }
}

std::vector<String> splitString(const String& input, char delimiter) {
  std::vector<String> result;
  String substring;
  int lastIndex = 0;
  int delimiterIndex;

  while ((delimiterIndex = input.indexOf(delimiter, lastIndex)) != -1) {
    substring = input.substring(lastIndex, delimiterIndex);
    result.push_back(substring);
    lastIndex = delimiterIndex + 1;
  }

  substring = input.substring(lastIndex);
  result.push_back(substring);

  return result;
}

String urlGetThingspeak(String channelId, String field, String apiKey) {
  return "https://api.thingspeak.com/channels/" + channelId + "/fields/" + field + "/last.json?api_key=" + apiKey;
}

void loop() {

  /*
   * 2422964
    1 - TempMax
    2 - TempMin
    3 - phMax
    4 - phMin
    5 - OxiMax
    6 - OxiMin
    7 - OrpMax
    8 - OrpMin

    2422965
    1 - CondMax
    2 - CondMin
    3 - SalMax
    4 - SalMin
  */

  /*Limites Sensores*/

  // Vai passar 8
  for (int i = 1; i <= 8; i++) {
    delay(5000);
    String url = urlGetThingspeak("2422964", (String)i, "40EXWJPSN80PFOFU");
    String field = "";

    WiFiClient client;
    if (client.connect("api.thingspeak.com", 80)) {
      client.print("GET " + url + " HTTP/1.1\r\n" +
                 "Host: api.thingspeak.com\r\n" +
                 "Connection: close\r\n\r\n");

      while (!client.available()) {
        delay(100);
      }

      String response = client.readString();

      std::vector<String> jsonResponse = splitString(response, '\"');
      String f = jsonResponse[11];
      field = f;
    }
    
    switch(i) {
      case 1:
        blynk_temp_max = atof(field.c_str());
        Serial.println(blynk_temp_max);
        break;
      case 2:
        blynk_temp_min = atof(field.c_str());
        Serial.println(blynk_temp_min);
        break;
      case 3:
        blynk_ph_max = atof(field.c_str());
        Serial.println(blynk_ph_max);
        break;
      case 4:
        blynk_ph_min = atof(field.c_str());
        Serial.println(blynk_ph_min);
        break;
      case 5:
        blynk_do_max = atof(field.c_str());
        Serial.println(blynk_do_max);
        break;
      case 6:
        blynk_do_min = atof(field.c_str());
        Serial.println(blynk_do_min);
        break;
      case 7:
        blynk_orp_max = atof(field.c_str());
        Serial.println(blynk_orp_max);
        break;
      case 8:
        blynk_orp_min = atof(field.c_str());
        Serial.println(blynk_orp_min);
        break;
      default:
        break; 
    }
  }

  // Vai passar 4
  for (int i = 1; i <= 4; i++) {
    delay(5000);
    String url = urlGetThingspeak("2422965", (String)i, "OKPJYMR8BY4DS76U");
    String field = "";

    WiFiClient client;
    if (client.connect("api.thingspeak.com", 80)) {
      client.print("GET " + url + " HTTP/1.1\r\n" +
                 "Host: api.thingspeak.com\r\n" +
                 "Connection: close\r\n\r\n");

      while (!client.available()) {
        delay(100);
      }

      String response = client.readString();

      std::vector<String> jsonResponse = splitString(response, '\"');
      String f = jsonResponse[11];
      field = f;
    }
    
    switch(i) {
      case 1:
        blynk_cond_max = atof(field.c_str());
        Serial.println(blynk_temp_max);
        break;
      case 2:
        blynk_cond_min = atof(field.c_str());
        Serial.println(blynk_temp_min);
        break;
      case 3:
        blynk_sal_max = atof(field.c_str());
        Serial.println(blynk_ph_max);
        break;
      case 4:
        blynk_sal_min = atof(field.c_str());
        Serial.println(blynk_ph_min);
        break;
      default:
        break; 
    }
  }
 
  Serial.println("Modo Local: ");
  Serial.println(modolocal);

  if (modolocal == 0)
  {
    
    
    Serial.println("0 - Modo Local: ");
    
    Serial.println("Desativado");
    delay(5000);
  }
  if (modolocal == 1)
  {
    
    
    Serial.println("0 - Modo Local: ");
    
    Serial.println("Ativado");
    delay(5000);
  }

  if (modomanual == 0)
  {
    
    
    Serial.println("0 - Modo Manual: ");
    
    Serial.println("Desativado");
    delay(5000);
  }
  if (modomanual == 1)
  {
    
    
    Serial.println("0 - Modo Manual: ");
    
    Serial.println("Ativado");
    delay(5000);
  }

  
  
  Serial.println("1 - PRESSIONE P");
  
  Serial.println("CALIBRAR:");
  delay(5000);

  if (mcp.digitalRead(botaoentrar) == LOW)      leituradecomando();



  delay(1000);

  
  
  Serial.println("2 - Rotina: ");
  
  Serial.println("WiFi");
  delay(5000);


  Wifi_Seq.run(); // conecta no wifi e espera 10s
  Seq.run();      //realiza as leituras, espera 1s, manda pro thingspeak, espera 1s, step 2 (nada por enquanto), espera 2s



  
  
  Serial.println("3 - Rotina: ");
  
  Serial.println("Envia para Blynk");
  delay(5000);


  Blynk.run();

  
  
  Serial.println("4 - Rotina: ");
  
  Serial.println("Recebe do Blynk");
  delay(5000);

  timer.run();

  step3() ;

  
  
  Serial.println("5 - Rotina: ");
  
  Serial.println("Atuadores");
  delay(5000);


  if ( modomanual == 0 )   condicoes();



  Serial.println("Fim do Loop");
  Serial.println("Aguardar 1s");
  delay(1000);

  
  
  Serial.println("6 - Rotina: ");
  
  Serial.println("Saidas");
  delay(5000);

  if ( modomanual == 1) condicoesmanuais();

  part2();

  Blynk.notify("Ciclo Finalizado, Atualizado!");

  tentativa = 0;

}

BLYNK_CONNECTED() {
  Blynk.syncAll();
}

void part2()
{

  Serial.println("========");
  // Serial.println(digitalRead(0));



  
  
  Serial.println("Variaveis e");
  
  Serial.println("Saidas: "); // You can make spaces using well... spaces
  delay(1000);

  

   // Or setting the cursor in the desired position.
  Serial.println("TEMP: ");
  Serial.println(RTD_float);
  
  Serial.println("S1: ");
  Serial.println(rele1);
  Serial.println("   S2: ");
  Serial.println(rele2);

  delay(1000);
  

   // Or setting the cursor in the desired position.
  Serial.println("PH: ");
  Serial.println(ph_float);
  
  Serial.println("S3: ");
  Serial.println(rele3);
  Serial.println("   S4: ");
  Serial.println(rele4);

  delay(1000);
  

   // Or setting the cursor in the desired position.
  Serial.println("Oxigenio: ");
  Serial.println(DO_float);
  
  Serial.println("S5: ");
  Serial.println(rele5);
  Serial.println("   S6: ");
  Serial.println(rele6);

  delay(1000);
  

   // Or setting the cursor in the desired position.
  Serial.println("ORP: ");
  Serial.println(ORP_float);
  
  Serial.println("S7: ");
  Serial.println(rele7);
  Serial.println("   S8: ");
  Serial.println(rele8);

  delay(1000);
  

   // Or setting the cursor in the desired position.
  Serial.println("Cond: ");
  Serial.println(ec_float);
  
  Serial.println("S9: ");
  Serial.println(rele9);
  Serial.println("   S10: ");
  Serial.println(rele10);

  delay(1000);
  

   // Or setting the cursor in the desired position.
  Serial.println("Sal: ");
  Serial.println(sal_float );
  
  Serial.println("S11: ");
  Serial.println(rele11);
  Serial.println("   S12: ");
  Serial.println(rele12);

  delay(1000);
  

  
  Serial.println("Press Para Modo");
  
  Serial.println("Manual");
  delay(3000);

  if (mcp.digitalRead(botaoentrar) == LOW) menu();

}

void menu()
{
  
  
  Serial.println("MENU");


  while (mcp.digitalRead(botaosair) == HIGH)
  {
    delay(2000);
    Serial.println("Entrou no menu");
    
    
    Serial.println("MODO MANUAL:");
    
    Serial.println("MODO: ");
    Serial.println(modomanual);
    delay(4000);

    if (mcp.digitalRead(botaoentrar) == LOW)
    {
      Serial.println("MUDAR MODO");
      modomanual = !modomanual;
      delay(100);
      
      
      Serial.println("MODO MANUAL:");
      
      Serial.println("MODO: ");
      Serial.println(modomanual);
      delay(1000);
    }


    Serial.println("Entrou no menu");
    
    
    Serial.println("Mudar Saidas:");
    
    Serial.println("S1: ");
    Serial.println(rele1);
    delay(1000);

    if (mcp.digitalRead(botaoentrar) == LOW)
    {
      Serial.println("chavear rele 1");
      rele1 = !rele1;
      delay(100);
      
      
      Serial.println("Mudar Saidas:");
      
      Serial.println("S1: ");
      Serial.println(rele1);
    }

    delay(2000);
    Serial.println("Entrou no menu");
    
    
    Serial.println("Mudar Saidas:");

    
    Serial.println("S2: ");
    Serial.println(rele2);
    delay(1000);

    if (mcp.digitalRead(botaoentrar) == LOW)
    {
      Serial.println("chavear rele 2");
      rele2 = !rele2;
      delay(100);
      
      
      Serial.println("Mudar Saidas:");

      
      Serial.println("S2: ");
      Serial.println(rele2);
    }

    delay(2000);
    Serial.println("Entrou no menu");
    
    
    Serial.println("Mudar Saidas:");
    
    Serial.println("S3: ");
    Serial.println(rele3);
    delay(1000);

    if (mcp.digitalRead(botaoentrar) == LOW)
    {
      Serial.println("chavear rele 3");
      rele3 = !rele3;
      delay(100);
      
      
      Serial.println("Mudar Saidas:");
      
      Serial.println("S3: ");
      Serial.println(rele3);
    }

    delay(2000);
    Serial.println("Entrou no menu");
    
    
    Serial.println("Mudar Saidas:");
    
    Serial.println("S4: ");
    Serial.println(rele4);
    delay(1000);

    if (mcp.digitalRead(botaoentrar) == LOW)
    {
      Serial.println("chavear rele 4");
      rele4 = !rele4;
      delay(100);
      
      
      Serial.println("Mudar Saidas:");
      
      Serial.println("S4: ");
      Serial.println(rele4);
    }

    delay(2000);
    Serial.println("Entrou no menu");
    
    
    Serial.println("Mudar Saidas:");
    
    Serial.println("S5: ");
    Serial.println(rele5);
    delay(1000);

    if (mcp.digitalRead(botaoentrar) == LOW)
    {
      Serial.println("chavear rele 5");
      rele5 = !rele5;
      delay(100);
      
      
      Serial.println("Mudar Saidas:");
      
      Serial.println("S5: ");
      Serial.println(rele5);
    }

    delay(2000);
    Serial.println("Entrou no menu");
    
    
    Serial.println("Mudar Saidas:");
    
    Serial.println("S6: ");
    Serial.println(rele6);
    delay(1000);

    if (mcp.digitalRead(botaoentrar) == LOW)
    {
      Serial.println("chavear rele 6");
      rele6 = !rele6;
      delay(100);
      
      
      Serial.println("Mudar Saidas:");
      
      Serial.println("S6: ");
      Serial.println(rele6);
    }

    delay(2000);
    Serial.println("Entrou no menu");
    
    
    Serial.println("Mudar Saidas:");
    
    Serial.println("S7: ");
    Serial.println(rele7);
    delay(1000);

    if (mcp.digitalRead(botaoentrar) == LOW)
    {
      Serial.println("chavear rele 7");
      rele7 = !rele7;
      delay(100);
      
      
      Serial.println("Mudar Saidas:");
      
      Serial.println("S7: ");
      Serial.println(rele7);
    }

    delay(2000);
    Serial.println("Entrou no menu");
    
    
    Serial.println("Mudar Saidas:");
    
    Serial.println("S8: ");
    Serial.println(rele8);
    delay(1000);

    if (mcp.digitalRead(botaoentrar) == LOW)
    {
      Serial.println("chavear rele 8");
      rele8 = !rele8;
      delay(100);
      
      
      Serial.println("Mudar Saidas:");
      
      Serial.println("S8: ");
      Serial.println(rele8);
    }


    delay(2000);
    Serial.println("Entrou no menu");
    
    
    Serial.println("Mudar Saidas:");
    
    Serial.println("S9: ");
    Serial.println(rele9);
    delay(1000);

    if (mcp.digitalRead(botaoentrar) == LOW)
    {
      Serial.println("chavear rele 9");
      rele9 = !rele9;
      delay(100);
      
      
      Serial.println("Mudar Saidas:");
      
      Serial.println("S9: ");
      Serial.println(rele9);
    }


    delay(2000);
    Serial.println("Entrou no menu");
    
    
    Serial.println("Mudar Saidas:");
    
    Serial.println("S10: ");
    Serial.println(rele10);
    delay(1000);

    if (mcp.digitalRead(botaoentrar) == LOW)
    {
      Serial.println("chavear rele 10");
      rele10 = !rele10;
      delay(100);
      
      
      Serial.println("Mudar Saidas:");
      
      Serial.println("S10: ");
      Serial.println(rele10);
    }


    delay(2000);
    Serial.println("Entrou no menu");
    
    
    Serial.println("Mudar Saidas:");
    
    Serial.println("S11: ");
    Serial.println(rele11);
    delay(1000);

    if (mcp.digitalRead(botaoentrar) == LOW)
    {
      Serial.println("chavear rele 11");
      rele11 = !rele11;
      delay(100);
      
      
      Serial.println("Mudar Saidas:");
      
      Serial.println("S11: ");
      Serial.println(rele11);
    }

    delay(2000);
    Serial.println("Entrou no menu");
    
    
    Serial.println("Mudar Saidas:");
    
    Serial.println("S12: ");
    Serial.println(rele12);
    delay(1000);

    if (mcp.digitalRead(botaoentrar) == LOW)
    {
      Serial.println("chavear rele 12");
      rele12 = !rele12;
      delay(100);
      
      
      Serial.println("Mudar Saidas:");
      
      Serial.println("S12: ");
      Serial.println(rele12);
    }
  }
}


void condicoes()
{
  Serial.println("-------------------");
  Serial.println("Condiçoes e Atuadores");


  if ( RTD_float < blynk_temp_min ) //  if ( RTD_float < blynk_temp_min )
  {
    Serial.println(" Temperatura Medida abaixo da faixa");
    led1.on();
    led2.off();
    rele1 = 1;
    rele2 = 0;
    mcp.digitalWrite(saida1, LOW);
    mcp.digitalWrite(saida2, HIGH);
    //delay( blynk_tempo_temp * 1000 );
    delay(500);
  }
  if ( RTD_float > blynk_temp_max )
  {
    Serial.println(" Temperatura Medida acima da faixa");
    led1.off();
    led2.on();
    rele1 = 0;
    rele2 = 1;
    mcp.digitalWrite(saida1, HIGH);
    mcp.digitalWrite(saida2, LOW);
    //delay(blynk_tempo_temp * 1000 );
    delay(500);
  }
  if ( RTD_float >= blynk_temp_min && RTD_float <= blynk_temp_max )
  {
    Serial.println(" Temperatura Medida dentro da faixa");
    led1.off();
    led2.off();
    rele1 = 0;
    rele2 = 0;
    mcp.digitalWrite(saida1, HIGH);
    mcp.digitalWrite(saida2, HIGH);
    delay(500);
  }

  if (ph_float < blynk_ph_min) //2
  {
    Serial.println(" PH Medido abaixo da faixa");
    led3.on();
    led4.off();
    rele3 = 1;
    rele4 = 0;
    mcp.digitalWrite(saida3, LOW);
    mcp.digitalWrite(saida4, HIGH);
    //delay( blynk_tempo_ph * 1000 );
    delay(500);
  }
  if (ph_float > blynk_ph_max)
  {
    Serial.println(" PH Medida acima da faixa");
    led3.off();
    led4.on();
    rele3 = 0;
    rele4 = 1;
    mcp.digitalWrite(saida3, HIGH);
    mcp.digitalWrite(saida4, LOW);
    //delay( blynk_tempo_ph * 1000 );
    delay(500);

  }
  if ( ph_float >= blynk_ph_min && ph_float <= blynk_ph_max ) // ( RTD_float > blynk_temp_min && RTD_float < blynk_temp_max )
  {
    Serial.println(" PH Medida dentro da faixa");
    led3.off();
    led4.off();
    rele3 = 0;
    rele4 = 0;
    mcp.digitalWrite(saida3, HIGH);
    mcp.digitalWrite(saida4, HIGH);
    delay(500);

  }

  if (DO_float < blynk_do_min) //3
  {
    Serial.println(" DO Medido abaixo da faixa");
    led5.on();
    led6.off();
    rele5 = 1;
    rele6 = 0;
    mcp.digitalWrite(saida5, LOW);
    mcp.digitalWrite(saida6, HIGH);
    //delay(blynk_tempo_do * 1000 );
    delay(500);
  }
  if (DO_float > blynk_do_max)
  {
    Serial.println(" DO Medida acima da faixa");
    led5.off();
    led6.on();
    rele5 = 0;
    rele6 = 1;
    mcp.digitalWrite(saida5, HIGH);
    mcp.digitalWrite(saida6, LOW);
    //delay(blynk_tempo_do * 1000 );
    delay(500);

  }
  if ( DO_float >= blynk_do_min && DO_float <= blynk_do_max ) // ( RTD_float > blynk_temp_min && RTD_float < blynk_temp_max )
  {
    Serial.println(" DO Medida dentro da faixa");
    led5.off();
    led6.off();
    rele5 = 0;
    rele6 = 0;
    mcp.digitalWrite(saida5, HIGH);
    mcp.digitalWrite(saida6, HIGH);
    delay(500);

  }
  if ( ORP_float < blynk_orp_min ) //4
  {
    Serial.println(" ORP Medido abaixo da faixa");
    led7.on();
    led8.off();
    rele7 = 1;
    rele8 = 0;
    mcp.digitalWrite(saida7, LOW);
    mcp.digitalWrite(saida8, HIGH);
    //delay(blynk_tempo_orp * 1000 );
    delay(500);
  }
  if ( ORP_float > blynk_orp_max )
  {
    Serial.println(" ORP Medida acima da faixa");
    led7.off();
    led8.on();
    rele7 = 0;
    rele8 = 1;
    mcp.digitalWrite(saida7, HIGH);
    mcp.digitalWrite(saida8, LOW);
    //delay(blynk_tempo_orp * 1000);
    delay(500);

  }
  if ( ORP_float >= blynk_orp_min && ORP_float <= blynk_orp_max )
  {
    Serial.println(" DO Medida dentro da faixa");
    led7.off();
    led8.off();
    rele7 = 0;
    rele8 = 0;
    mcp.digitalWrite(saida7, HIGH);
    mcp.digitalWrite(saida8, HIGH);
    delay(500);
  }

  if (ec_float < blynk_cond_min) //5
  {
    Serial.println(" Condutividade Medido baixo da faixa");
    led9.on();
    led10.off();
    rele9 = 1;
    rele10 = 0;
    mcp.digitalWrite(saida9, LOW);
    mcp.digitalWrite(saida10, HIGH);
    // delay(blynk_tempo_ec * 1000 );
    delay(500);
  }
  if (ec_float > blynk_cond_max)
  {
    Serial.println(" Condutividade Medida acima da faixa");
    led9.off();
    led10.on();
    rele9 = 0;
    rele10 = 1;
    mcp.digitalWrite(saida9, HIGH);
    mcp.digitalWrite(saida10, LOW);
    //delay(blynk_tempo_ec * 1000 );
    delay(500);
  }
  if ( ec_float >= blynk_cond_min && ec_float <= blynk_cond_max ) // ORP_float > blynk_orp_min && ORP_float < blynk_orp_max
  {
    Serial.println(" Condutividade Medida dentro da faixa");
    led9.off();
    led10.off();
    rele9 = 0;
    rele10 = 0;
    mcp.digitalWrite(saida9, HIGH);
    mcp.digitalWrite(saida10, HIGH);
    delay(500);
  }

  if (sal_float < blynk_sal_min) //6
  {
    Serial.println(" Salinidade Medido abaixo da faixa");
    led11.on();
    led12.off();
    rele11 = 1;
    rele12 = 0;
    mcp.digitalWrite(saida11, LOW);
    mcp.digitalWrite(saida12, HIGH);
    //  delay(blynk_tempo_ec * 1000 );
    delay(500);
  }
  if (sal_float > blynk_sal_max)
  {
    Serial.println(" Salinidade Medida acima da faixa");
    led11.off();
    led12.on();
    rele11 = 0;
    rele12 = 1;
    mcp.digitalWrite(saida11, HIGH);
    mcp.digitalWrite(saida12, LOW);
    //delay(blynk_tempo_ec * 1000 );
  }
  if ( sal_float >= blynk_sal_min && sal_float <= blynk_sal_max )
  {
    Serial.println(" Salinidade Medida dentro da faixa");
    led11.off();
    led12.off();
    rele11 = 0;
    rele12 = 0;
    mcp.digitalWrite(saida11, HIGH);
    mcp.digitalWrite(saida12, HIGH);
    delay(500);
  }

  Serial.println("Rele 01: ");
  Serial.println(rele1);
  Serial.println("Rele 02: ");
  Serial.println(rele2);
  Serial.println("Rele 03: ");
  Serial.println(rele3);
  Serial.println("Rele 04: ");
  Serial.println(rele4);
  Serial.println("Rele 05: ");
  Serial.println(rele5);
  Serial.println("Rele 06: ");
  Serial.println(rele6);
  Serial.println("Rele 07: ");
  Serial.println(rele7);
  Serial.println("Rele 08: ");
  Serial.println(rele8);
  Serial.println("Rele 09: ");
  Serial.println(rele9);
  Serial.println("Rele 10: ");
  Serial.println(rele10);
  Serial.println("Rele 11: ");
  Serial.println(rele11);
  Serial.println("Rele 12: ");
  Serial.println(rele12);
  Serial.println();

  Serial.println("-------------------");
}

void step1()
{
  Serial.println("-------------------");
  Serial.println("Leituras Sensores:");

  RTD.send_read_cmd();
  delay(800);
  receive_and_print_reading(RTD);
  RTD_float = RTD.get_last_received_reading();

  //Serial.println("RTD: ");
  //Serial.println(RTD_float);
  //Serial.println(" - ");
  Serial.println();

  PH.send_read_cmd();
  delay(800);
  receive_and_print_reading(PH);
  ph_float = PH.get_last_received_reading();

  //Serial.println("PH: ");
  //Serial.println(ph_float);
  //Serial.println(" - ");
  Serial.println();

  DO.send_read_cmd();
  delay(800);
  receive_and_print_reading(DO);
  DO_float = DO.get_last_received_reading();
  //Serial.println("DO2: ");
  //Serial.println(DO_float);
  //Serial.println(" - ");
  Serial.println();

  ORP.send_read_cmd();
  delay(800);
  receive_and_print_reading(ORP);
  ORP_float = ORP.get_last_received_reading();
  Serial.println();

  serial_event = true;
  computerdata[0] = 'r';

  if (serial_event == true) {                                                     //if a command was sent to the EZO device.
    for (i = 0; i <= received_from_computer; i++) {                               //set all char to lower case, this is just so this exact sample code can recognize the "sleep" command.
      computerdata[i] = tolower(computerdata[i]);                                 //"Sleep" ≠ "sleep"
    }
    i = 0;                                                                        //reset i, we will need it later
    if (computerdata[0] == 'c' || computerdata[0] == 'r')time_ = 570;             //if a command has been sent to calibrate or take a reading we wait 570ms so that the circuit has time to take the reading.
    else time_ = 250;                                                             //if any other command has been sent we wait only 250ms.


    Wire.beginTransmission(addressEC);                                            //call the circuit by its ID number.
    Wire.write(computerdata);                                                   //transmit the command that was sent through the serial port.
    Wire.endTransmission();                                                     //end the I2C data transmission.

    Serial.println(ec_float);
    if (strcmp(computerdata, "sleep") != 0) {                                   //if the command that has been sent is NOT the sleep command, wait the correct amount of time and request data.
      //if it is the sleep command, we do nothing. Issuing a sleep command and then requesting data will wake the EC circuit.

      delay(time_);                                                             //wait the correct amount of time for the circuit to complete its instruction.

      Wire.requestFrom(addressEC, 32, 1);                                         //call the circuit and request 32 bytes (this could be too small, but it is the max i2c buffer size for an Arduino)
      code = Wire.read();                                                       //the first byte is the response code, we read this separately.

      switch (code) {                           //switch case based on what the response code is.
        case 1:                                 //decimal 1.
          Serial.println("Success");            //means the command was successful.
          break;                                //exits the switch case.

        case 2:                                 //decimal 2.
          Serial.println("Failed");             //means the command has failed.
          break;                                //exits the switch case.

        case 254:                               //decimal 254.
          Serial.println("Pending");            //means the command has not yet been finished calculating.
          break;                                //exits the switch case.

        case 255:                               //decimal 255.
          Serial.println("No Data");            //means there is no further data to send.
          break;                                //exits the switch case.
      }

      while (Wire.available()) {                 //are there bytes to receive.
        in_char = Wire.read();                   //receive a byte.
        ec_data[i] = in_char;                    //load this byte into our array.
        i += 1;                                  //incur the counter for the array element.
        if (in_char == 0) {                      //if we see that we have been sent a null command.
          i = 0;                                 //reset the counter i to 0.
          break;                                 //exit the while loop.
        }
      }

      Serial.println(ec_data);                  //print the data.
      Serial.println();                         //this just makes the output easier to read by adding an extra blank line
    }
    serial_event = false;                      //reset the serial event flag.


    if (computerdata[0] == 'r') string_pars(); //uncomment this function if you would like to break up the comma separated string into its individual parts.
  }

  EC.send_read_cmd();
  delay(1000);
  receive_and_print_reading(EC);               //get the reading from the EC circuit and print it
  delay(1000);
  serial_event = true ;
  computerdata[0] = 'r';

  Serial.println();

  Serial.println("-------------------");
}

void string_pars() {                  //this function will break up the CSV string into its 4 individual parts. EC|TDS|SAL|SG.
  //this is done using the C command “strtok”.

  ec = strtok(ec_data, ",");          //let's pars the string at each comma.
  tds = strtok(NULL, ",");            //let's pars the string at each comma.
  sal = strtok(NULL, ",");            //let's pars the string at each comma.
  sg = strtok(NULL, ",");             //let's pars the string at each comma.

  Serial.println(ec_float);

  //uncomment this section if you want to take the values and convert them into floating point number.

  ec_float = atof(ec);
  tds_float = atof(tds);
  sal_float = atof(sal);
  sg_float = atof(sg);


  Serial.println("EC:");                //we now print each value we parsed separately.
  Serial.println(ec_float);                 //this is the EC value.

  Serial.println("TDS:");               //we now print each value we parsed separately.
  Serial.println(tds_float);                //this is the TDS value.

  Serial.println("SAL:");               //we now print each value we parsed separately.
  Serial.println(sal_float);                //this is the salinity value.

  Serial.println("SG:");               //we now print each value we parsed separately.
  Serial.println(sg_float);                //this is the specific gravity.
  Serial.println();                  //this just makes the output easier to read by adding an extra blank line

}
void step2()
{
  Serial.println("-------------------");
  Serial.println("Enviar para ThingSpeak:");

  if (RTD_float < -1)
  {
    Serial.println("Field 1 Status: Não enviado ( valor negativo )");
  }
  else
  {
    ThingSpeak.setField(1, RTD_float);                 //assign temperature readings to the third column of thingspeak channel
    delay(15000);
    Serial.println("Field 1 Status: ");
    Thingspeak_seq.run();
    //
  }

  if (ph_float < -1)
  {
    Serial.println("Field 2 Status: Não enviado ( valor negativo )");
  }
  else
  {
    ThingSpeak.setField(2, ph_float);                 //assign temperature readings to the third column of thingspeak channel
    delay(15000);
    Serial.println("Field 2 Status: ");
    Thingspeak_seq.run();
  }

  if (DO_float < -1)
  {
    Serial.println("Field 3 Status: Não enviado ( valor negativo )");
  }
  else
  {
    ThingSpeak.setField(3, DO_float);                 //assign temperature readings to the third column of thingspeak channel
    delay(15000);
    Serial.println("Field 3 Status: ");
    Thingspeak_seq.run();
  }

  if (ORP_float < -1)
  {
    Serial.println("Field 4 Status: Não enviado ( valor negativo )");
  }
  else
  {
    ThingSpeak.setField(4, ORP_float);                 //assign temperature readings to the third column of thingspeak channel
    delay(15000);
    Serial.println("Field 4 Status: ");
    Thingspeak_seq.run();
  }

  if (ec_float < -1)
  {
    Serial.println("Field 5 Status: Não enviado ( valor negativo )");
  }
  else
  {
    ThingSpeak.setField(5, ec_float);                 //assign temperature readings to the third column of thingspeak channel
    delay(15000);
    Serial.println("Field 5 Status: ");
    Thingspeak_seq.run();
  }

  if (sal_float < -1)
  {
    Serial.println("Field 6 Status: Não enviado ( valor negativo )");
  }
  else
  {
    ThingSpeak.setField(6, sal_float);                 //assign temperature readings to the third column of thingspeak channel
    delay(15000);
    Serial.println("Field 6 Status: ");
    Thingspeak_seq.run();
    Serial.println("-------------------");
  }

}

//RECEBER DADO DO APP
BLYNK_WRITE(V0)
{
  blynk_temp_max = param.asFloat();
}
//RECEBER DADO DO APP

BLYNK_WRITE(V1)
{
  blynk_temp_min = param.asFloat();
}

BLYNK_WRITE(V2)
{
  blynk_ph_max = param.asFloat();
}

BLYNK_WRITE(V3)
{
  blynk_ph_min = param.asFloat();
}

BLYNK_WRITE(V4)
{
  blynk_do_max = param.asFloat();
}
BLYNK_WRITE(V5)
{
  blynk_do_min = param.asFloat();
}

BLYNK_WRITE(V6)
{
  blynk_orp_max = param.asFloat();
}
BLYNK_WRITE(V7)
{
  blynk_orp_min = param.asFloat();
}
BLYNK_WRITE(V8)
{
  blynk_cond_max = param.asFloat();
}
BLYNK_WRITE(V9)
{
  blynk_cond_min = param.asFloat();
}
BLYNK_WRITE(V10)
{
  blynk_sal_max = param.asFloat();
}
BLYNK_WRITE(V11)
{
  blynk_sal_min = param.asFloat();
}

BLYNK_WRITE(V30)
{
  blynk_tempo_temp = param.asFloat();
}

BLYNK_WRITE(V31)
{
  blynk_tempo_ph = param.asFloat();
}

BLYNK_WRITE(V32)
{
  blynk_tempo_do = param.asFloat();
}

BLYNK_WRITE(V33)
{
  blynk_tempo_orp = param.asFloat();
}

BLYNK_WRITE(V34)
{
  blynk_tempo_ec = param.asFloat();
}

BLYNK_WRITE(V40)
{
  blynk_botao1 = param.asFloat();
}

BLYNK_WRITE(V41)
{
  blynk_botao2 = param.asFloat();
}
BLYNK_WRITE(V42)
{
  blynk_botao3 = param.asFloat();
}
BLYNK_WRITE(V43)
{
  blynk_botao4 = param.asFloat();
}
BLYNK_WRITE(V44)
{
  blynk_botao5 = param.asFloat();
}
BLYNK_WRITE(V45)
{
  blynk_botao6 = param.asFloat();
}
BLYNK_WRITE(V46)
{
  blynk_botao7 = param.asFloat();
}
BLYNK_WRITE(V47)
{
  blynk_botao8 = param.asFloat();
}
BLYNK_WRITE(V48)
{
  blynk_botao9 = param.asFloat();
}
BLYNK_WRITE(V49)
{
  blynk_botao10 = param.asFloat();
}
BLYNK_WRITE(V50)
{
  blynk_botao11 = param.asFloat();
}
BLYNK_WRITE(V51)
{
  blynk_botao12 = param.asFloat();
}

void step3()

{
  //Escreve no Blynk
  Serial.println("-------------------");
  Serial.println("Blynk:");

  Serial.println("Tempo de Ajuste Temperatura: ");
  Serial.println(blynk_tempo_temp);
  Serial.println(" em minutos ");

  Serial.println("Tempo de Ajuste PH: ");
  Serial.println(blynk_tempo_ph);
  Serial.println(" em minutos ");

  Serial.println("Tempo de Ajuste DO: ");
  Serial.println(blynk_tempo_do);
  Serial.println(" em minutos ");

  Serial.println("Tempo de Ajuste ORP: ");
  Serial.println(blynk_tempo_orp);
  Serial.println(" em minutos ");

  Serial.println("Tempo de Ajuste EC: ");
  Serial.println(blynk_tempo_ec);
  Serial.println(" em minutos ");

  Serial.println("Range Temperatura Alvo: ");
  Serial.println(blynk_temp_min);
  Serial.println(" - ");
  Serial.println(blynk_temp_max);

  Serial.println("Range pH Alvo: ");
  Serial.println(blynk_ph_min);
  Serial.println(" - ");
  Serial.println(blynk_ph_max);

  Serial.println("Range DO Alvo: ");
  Serial.println(blynk_do_min);
  Serial.println(" - ");
  Serial.println(blynk_do_max);

  Serial.println("Range ORP Alvo: ");
  Serial.println(blynk_orp_min);
  Serial.println(" - ");
  Serial.println(blynk_orp_max);

  Serial.println("Range Condutividade Alvo: ");
  Serial.println(blynk_cond_min);
  Serial.println(" - ");
  Serial.println(blynk_cond_max);

  Serial.println("Range Salinidade Alvo: ");
  Serial.println(blynk_sal_min);
  Serial.println(" - ");
  Serial.println(blynk_sal_max);

  delay(2000);
  Serial.println("-------------------");

}
