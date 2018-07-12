//Code von Lennart O.

//Bibliotheken einbinden

#include <mcp_can.h> 
#include <SPI.h>
#include <SimpleTimer.h>

//Konstanten und Variablen deklarieren

#define SPI_CS_PIN 10 //CS Pin
#define POTI 0 //A0 für Poti

word outputvoltage = 1162; //setze max Spannung auf 116,2V (offset = 0,1)
word outputcurrent = 320; //setze max Strom auf 32A (offset = 0,1)
float poti = 0;

unsigned long int sendId = 0x1806E5F4;
unsigned char voltamp[8] = {highByte(outputvoltage), lowByte(outputvoltage), highByte(outputcurrent), lowByte(outputcurrent), 0x00,0x00,0x00,0x00};

unsigned char len = 0; //Länge emfpangener CAN Nachricht
unsigned char buf[8]; //Buffer für Daten aus CAN Nachricht
unsigned long int receiveId; //ID des Senders

//Objekte deklarieren

MCP_CAN CAN(SPI_CS_PIN); //CS Pin für SPI setzen

SimpleTimer timer1; //timer Objekt erzeugen

//Funktionen

/************************************************
** Function name:           canRead
** Descriptions:            read CAN message
*************************************************/

void canRead(){

  if(CAN_MSGAVAIL == CAN.checkReceive()){ //auf Nachrichten prüfen

    CAN.readMsgBuf(&len, buf); // read data, len: data length, buf: data buffer

    receiveId = CAN.getCanId(); //CAN-ID lesen

    if(receiveId == 0x18FF50E5){ //CAN Bus ID vom Ladegerät

      Serial.println("CAN Daten vom Ladegerät empfangen!");

      Serial.print("CAN ID: ");
      Serial.print(receiveId, HEX); //ID ausgeben

      Serial.print(" / CAN Daten: ");
      for(int i = 0; i<len; i++){ //Daten ausgeben

        if( buf[i] < 0x10){ // führende Null wenn nur eine Ziffer
          Serial.print("0");
        }

        Serial.print(buf[i],HEX);
        Serial.print(" ");          // Leerzeichen

      }

      Serial.println(); //Absatz

      Serial.print("Ladespannung: ");
      float pv_voltage = (((float)buf[0]*256.0) + ((float)buf[1]))/10.0; //highByte/lowByte + offset
      Serial.print(pv_voltage);
      Serial.print(" V / Ladestrom: ");
      float pv_current = (((float)buf[2]*256.0) + ((float)buf[3]))/10.0; //highByte/lowByte + offset
      Serial.print(pv_current);
      Serial.println(" A"); //Absatz

      switch (buf[4]) { //Fehlerbyte auslesen

        case B00000001: Serial.println("Fehler: Hardwarefehler");break;
        case B00000010: Serial.println("Fehler: Übertemperatur");break;
        case B00000100: Serial.println("Fehler: Eingangsspannung unzulässig");break;
        case B00001000: Serial.println("Fehler: Batterie nicht verbunden");break;
        case B00010000: Serial.println("Fehler: CAN-Bus Fehler");break;
        case B00001100: Serial.println("Fehler: Keine Eingangsspannung");break;

      }

    }

  }

}

/************************************************
** Function name:           canWrite
** Descriptions:            write CAN message
*************************************************/

String canWrite(unsigned char data[8], unsigned long int id){

  byte sndStat = CAN.sendMsgBuf(id, 1, 8, data); //Nachricht senden (ID, extended Frame, Datenlänge, Daten)

  if(sndStat == CAN_OK) //Statusbyte für Übertragung
    return "CAN Nachricht erfolgreich gesendet";
  else
    return "Fehler bei Nachrichtenübertragung";

}

/************************************************
** Function name:           setVoltage
** Descriptions:            set target voltage
*************************************************/

void setVoltage(int t_voltage) { //can be used to set desired voltage to i.e. 80% SOC

  if(t_voltage >= 980 && t_voltage <= 1164){
    
    outputvoltage = t_voltage;
    
  }

 }

/************************************************
** Function name:           setCurrent
** Descriptions:            set target current
*************************************************/

void setCurrent(int t_current) { //can be used to reduce or adjust charging speed

  if(t_current >= 0 && t_current <= 320){
    
    outputcurrent = t_current;
    
  }

 }

/************************************************
** Function name:           readPoti
** Descriptions:            read poti to adjust current
*************************************************/

void readPoti() {

  poti = analogRead(POTI); //Wert vom Poti einlesen
  int current = (int) (320.0*(poti/850.0)) //keine 1024 erreicht, daher 850
    
  if(current > 320){

   current = 320; //begrenze auf 320 falls Ergebnis größer ist

  }  
 
  setCurrent(current); 

 }

/************************************************
** Function name:           myTimer1
** Descriptions:            function of timer1
*************************************************/

void myTimer1() { //zyklisch vom Timer aufgerufene Funktion


  Serial.print("Eingestellter Ladestrom: ");
  Serial.print((float)outputcurrent/10.0); //Current setpoint ausgeben
  Serial.println(" A");
  unsigned char voltamp[8] = {highByte(outputvoltage), lowByte(outputvoltage), highByte(outputcurrent), lowByte(outputcurrent), 0x00,0x00,0x00,0x00}; //Nachricht neu generieren

  Serial.println(canWrite(voltamp, sendId)); //Nachricht senden und Ergebnis ausgeben
  canRead(); //Lesefunktion aufrufen
  Serial.println(); //Absatz

}

/************************************************
** Function name:           setup
** Descriptions:            Arduino setup
*************************************************/

void setup() {

  Serial.begin(115200); //Serielle Schnittstelle starten

  while(CAN_OK != CAN.begin(CAN_250KBPS, MCP_8MHz)){ //CAN Bus initialisieren

    Serial.println("CAN Initialisierung fehlgeschlagen, Neustart");
    delay(200);

  }

  Serial.println("CAN Initialisierung erfolgreich");

  timer1.setInterval(950, myTimer1); //Zeit und Funktion des Timers definieren

}

/************************************************
** Function name:           loop
** Descriptions:            Arduino loop
*************************************************/

void loop() {

  timer1.run(); //Timer starten

}
