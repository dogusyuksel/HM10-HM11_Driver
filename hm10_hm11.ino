#include <assert.h>


#define WAKEUP_MESSAGE_LEN 80

//total lne is 78
typedef struct __attribute__((__packed__))
{
  char StartingCharacters[7]; //"OK+DISC", len is 7
  char delim1; //':'
  char FactoryID[8]; //"4C000215" is IBeacon for example, len is 8
  char delim2; //':'
  char UUID[32]; // you can look at the datasheet with DISI? command
  char delim3; //':'
  char Major[4];
  char Minor[4];
  char MeasuredPower[2];
  char delim4;
  char MACAddress[12];
  char delim5;
  char RSSI[4];
}DISI_Answer;

/*
* Name: HMSoft; Baud: 9600, N, 8, 1; Pin code: 000000; Peripheral Role; transmit mode (MODE 0 actually)
*/


bool switchStudyFunction(uint8_t prm); //send "AT+COMP[P1]" and get "OK+Set:[P1]"

bool isDeviceAwake(); //send AT and wait just "OK"
bool wakeUpDevice(); //send >80 character and wait just "OK+WAKE", NOTE THAT: no answer coming

//bool setTransmissionMode(); //send AT+MODE0 and wait "OK+Set:0"

bool openCloseNotify(bool open); //send "AT+NOTI0" to close and send "AT+NOTI1" to open it. If notif is open, then module
//will inform us with "OK+LOST" if it is connected to a device of out "AT" command. It first disconnected then send the response
//if module is not connected to device, normally it returns "OK" to "AT" command.,
/*If this value is set to 1, when link ESTABLISHED or LOSTED module will send
OK+CONN or OK+LOST string through UART*/

void qetModuleMACAddress(char *address); //it returns mac address (12 characters) in address array. Send "AT+ADDR?" and returns "OK+ADDR:341513D9A655" for example

uint8_t qetAdvertisingInterval(); // send "AT+ADVI?" and get "OK+Get:0"
bool setAdvertisingInterval(uint8_t prm); //prm will fit to info bottom. send "AT+ADVI0" and get "OK+Set:0" for example
/*
P1: 0 ~ F
0: 100ms
1: 152.5 ms
2: 211.25 ms
3: 318.75 ms
4: 417.5 ms
5: 546.25 ms
6: 760 ms
7: 852.5 ms
8: 1022.5 ms
9: 1285 ms
A: 2000ms
B: 3000ms
C: 4000ms
D: 5000ms
E: 6000ms
F: 7000ms
Default: 9
HMSoft Default: 0
HMSensor Default: 9
*/

//APPLE_ANCS_SWITCH: not used this project. For more info, please look at the datasheet

bool openCloseWhiteListProperty(bool open); //send "AT+ALLO0" to close and "AT+ALLO1" to open. "OK+Set:0" or "OK+Set:1" received
bool isWhiteListPropertyOpen(); //send "AT+ALLO?" and get "OK+ Get:0" 0 means off, 1 means on
void getWhiteListSelectedDeviceMACAddress(uint8_t listIdx, char *readMAcAddress); //send "AT+AD1??" to get 1. mac address, we can send 1, 2 or 3 as index
//and we receive "OK+AD1?:000000000000" for example
bool setWhiteListIndexWithSpecifiedMACAddress(uint8_t idx, char *macAddress); //send "AT+AD1001122334455" and receive "OK+AD1001122334455"
//as you can see send commands first character is index, it could be 1, 2 or 3 and mac address will be 12 characters len

uint8_t getBatteryPercentage(); //send "AT+BATT?" and get "OK+Get:100"
bool setBaudRate(uint8_t selection); // send "AT+BAUD0" and get "OK+Set:0", NOTE THAT: it requires power on/off
/*
0---------9600
1---------19200
2---------38400
3---------57600
4---------115200
5---------4800
6---------2400
7---------1200
8---------230400
Default: 0(9600)
*/

//QUERY and SET LINK LAYER CONNECTION TIME, we will not add this commands, look to datasheet for more info

bool setBLECharacteristic(uint16_t type); //default 0xFFE1, we can set it between 0x0001-0xFFFE, send "AT+CHAR0xAAA0", get "OK+Set:0xAAA0"
uint16_t getBLECharacteristic(); //send "AT+CHAR?" and get "OK+Get:0xFFE1"

bool clearLastConnectedDeviceAddress(); //send "AT+CLEAR" and get "OK+CLEAR"
bool tryToConnectLastSucceedDevice(); //send "AT+CONNL" and receive "OK+CONN[P1]" where
/*
P1: L, E, F, N
L: Connecting
E: Connect error
F: Connect Fail
N: No Address

Notice: Only Central role is used.
Required: AT+IMME1 and AT+ROLE1 and AT+SAVE1.
*/

bool isDeviceConnected(); //if it is connected, return true

bool tryToConnectDeviceUseMAC(char *mac); //send "AT+CON0017EA090909" to connect mac-> 00:17:EA:09:09:09
/*
OK+CONNA ========= Accept request, connecting
OK+CONNE ========= Connect error
OK+CONN ========= Connected, if AT+NOTI1 is setup
OK+CONNF ========= Connect Failed, After 10 seconds

Required: AT+IMME1 and AT+ROLE1.
*/
/*
* scenario 1 : for example send a dummy mac addres then we get A, then after 10 seconds we get F
* scenario 2 : for example send a normal mac addres then we get A, then we get nothing and connection was made SUCCESSFULLY.
*/

//NOTE THAT: to connect a device by using UUID, look to datasheet for more info

bool tryToConnectDiscoveredDeviceArrayIndex(uint8_t idx); //send "AT+CONN[P1]" and get "OK+CONN[P2]"
/*
P1: 0~5
P2: E, F, 0~5
E: Link error
F: Link failed
0~5: Array index.

This command is use after execute AT+DISC?
AT+DISC? command will return discovered devices
The first discovered device array index is 0, second device array index is 1,
AT+DISC? Command could return more than 6 devices, buy only first 6 device could
use array index, other devices must use AT+CO or AT+LN command.
*/

bool startDeviceDiscoveryScan(); // send "AT+DISC?", get "OK+DISCS and OK+DIS[P0][P1][P2] and OK+DISCE"
/*
P0: C, 0, 1, 2; 1 Byte Length
C: Common string
0~2: device Address type
P1: device MAC string
6 Bytes length
DISCS: Start discovery
DISCE: End discovery

Required: AT+ROLE1, AT+IMME1
AT+SHOW command module help you to get more device information.
OK+DISCS is mean: module start discovery now.
OK+DISCE is mean: module already stopped discovery.

After AT+DISC? Command, you will need to choose a device to connect, You
can use AT+CONN command or AT+CO command or AT+LN command. Or you
can use AT+DISI? Command or AT+DISA? Command to get more different
information.
*/

bool scanIBeaconDiscoveryScan(char MacList[][13], uint8_t MacAddressCount, int *RSSList); //send "AT+DISI?", get "OK+DISC[P0:P1:P2:P3:P4]"
/*
P0: Factory ID
P1: iBeacon UUID
P2: Major Value
Minor Value
Measured Power
P3: MAC
P4: RSSI

Please set AT+ROLE1 and AT+IMME1 first.
e.g.
Send: AT+DISI?
Recv: OK+DISCS (Scan start)
Recv: OK+DIS[P0:P1:P2:P3:P4] (if have one device)
Recv: OK+DIS[P0:P1:P2:P3:P4] (if have two devices)
………..
Recv: OK+DISCE (Scan end)
P0 length is 8 Bytes; P1 length is 32 Bytes; P2 length is 10 Bytes; P3 length is 12
Bytes, P4 length is 4 Bytes, Those values all is ASCII mode.
P2 include Major Value (length 4 Bytes);
Minor Value (length 4 Bytes);
Measured Power (length 2 Bytes)
If the device not enable iBeacon function, P0, P1, P2 will use ‘0’ to fill in.
*/

bool isUARTRxGainOpen(); //send "AT+GAIN?" and get "OK+ Get:[P1]" where P1:0 is no RX gain, P1:1 Opened means
bool setUARTRxGain(uint8_t prm); //send "AT+GAIN[P1]" and get "OK+ Set:[P1]"
/*
P1: 0, 1
0: No RX gain
1: Open RX gain
Default: 0
*/

bool getModuleWorkType(); //send "AT+IMME?" and get "OK+ Get:[P1]"
bool setModuleWorkType(uint8_t prm); //send "AT+IMME[P1]" and get "OK+ Set:[P1]"
/*
P1: 0, 1
1: When module is powered on,
only respond the AT Command,
don’t do anything. Until AT +
START, AT+CON, AT+CONNL
commands is received.
0: When power on, module will
start work immediately
Default: 0
*/

//NOTE THAT: SET AND GET IBEACON UUID AND MOJOR MINOR DATA AND MEASURED POWER, for more info, please look to datasheet

uint8_t getModuleWorkMode(); //send "AT+MODE?" and get "OK+Get:[P1]"
bool setModuleWorkMode(uint8_t prm); //send "AT+MODE[P1]" and get "OK+Set:[P1]"
/*
P1: 0, 1, 2
0: Transmission Mode
1: PIO collection Mode +
Mode 0
2: Remote Control Mode +
Mode 0
Default: 0
*/

void getModuleName(char *name); // send "AT+NAME?" and get "OK+NAME[P1]"
bool setModuleName(char *name); //send "AT+NAME[P1]" and get "OK+Set[P1]"
/*
P1: module name, Max length
is 12.
Default: HMSoft
*/

uint8_t getOutputDriverPower(); //send "AT+PCTL?" and get "OK+Get:[P1]"
bool setOutputDriverPower(uint8_t prm); //send "AT+PCTL[P1]" and "OK+Set:[P1]"
/*
P1: 0,1
0:Normal power output
1:Max power output
Default: 1
*/

void getPinCode(uint32_t *pin); //send "AT+PASS?" and get "OK+Get:[P1]"
bool setPinCode(uint32_t pin); //send "AT+PASS[P1]" and get "OK+Set:[P1]"

uint8_t getModulePower(); //send "AT+POWE?" and get "OK+Get:[P1]"
bool setModulePower(uint8_t prm); //send "AT+ POWE [P1]" and get "OK+Set:[P1]"
/*
P1: 0 ~ 3
0: -23dbm
1: -6dbm
2: 0dbm
3: 6dbm
Default: 2
*/

uint8_t getModuleSleepType(); //send "AT+PWRM?" and get "OK+Get:[P1]"
bool setPowerModuleSleepType(uint8_t prm); //send "AT+PWRM[P1]" and get "OK+Set:[P1]"
/*
P1: 0~1
0:Auto sleep
1:Don’t auto sleep
Default: 1
*/

uint8_t getDeviceRole(); //send "AT+ROLE?" and get "OK+Get:[P1]"
bool setDeviceRole(uint8_t prm); // send "AT+ROLE[P1]" and get "OK+Set:[P1]"
/*
P1: 0, 1
0: Peripheral
1: Central
Default: 0
*/

void getRSSIValue(); //send "AT+RSSI?" and get "OK+RSSI:[P1]"
/*Require: AT+MODE value > 0    and This command only used by Remote device query after connected.*/

void getLastConnectedDeviceMACAddress(char *address); //send "AT+RADD?" and get "OK+RADD:MAC Address"

uint8_t getDiscoveryTimeWhenModuleIsMaster(); //send "AT+SCAN?" and get "OK+Get:[P1]"
bool setDiscoveryTimeWhenModuleIsMaster(uint8_t prm); //send "AT+SCAN[P1]" and get "OK+Set:[P1]"
/*
P1: 1 ~ 9
Unit: Second
Default: 3 Seconds
*/

void getModuleVersion(char *version); //send "AT+VERR?" and get "version"

uint16_t getServiceUUID(); // send "AT+UUID?" and get "OK+Get:[P1]"
bool setServiceUUID(uint16_t prm); //send "AT+UUID[P1]" and get "OK+Set:[P1]"
/*
P1: 0x0001~0xFFFE
Default: 0xFFE0
*/

uint8_t getBondMode(); //send "AT+TYPE?" and get "OK+Get:[P1]"
bool setBondMode(uint8_t prm); // send "AT+TYPE[P1]" and get "OK+Set:[P1]"
/*
P1: 0~2
0:Not need PIN Code
1:Auth not need PIN
2:Auth with PIN
3:Auth and bonded

Default: 0
*/

uint32_t getModuleConnectRemoteDeviceTimeout(); //send "AT+TCON?" and get "OK+Get:[P1]"
bool setModuleConnectRemoteDeviceTimeout(uint32_t prm); //send "AT+TCON[P1]" and get "OK+Set:[P1]"
/*
P1 is timeout value. When
time is up, module will not
connect this address
anymore, then enter search
mode.
P1 allowed value:
000000~999999
Unit is ms.
Default:
000000 Connect forever
*/

uint8_t getStatusOfShowDeviceInfoWhenDiscovery(); //send "AT+SHOW?" and get "OK+Get:[P1]"
bool setStatusOfShowDeviceInfoWhenDiscovery(uint8_t prm); //send "AT+SHOW[P1]" and get "OK+Set:[P1]"
/*
P1: 0, 1, 2
0:Don’t show information
1:Show name
2: Show RSSI
3: Show RSSI and Name
Default: 0
*/

bool iBeaconDeployMode(uint8_t prm); //send "AT+DELO[P1]" and get "OK+DELO[P1]"
/*
P1: 1, 2
1: Allowed to broadcast and
scanning
2: Only allow broadcast

After receive OK+DELO[P1], module will reset after 500ms.
This command will let module into non-connectable status until next power on.
*/

void restoreToFactoryDefault(); //send "AT+RENEW" and get "OK+RENEW"

void restartModule(); //send "AT+RESET" and get "OK+RESET"


typedef enum {UART_ERR_OK, UART_ERR_TIMEOUT} Uart_Errors;
void SendData(char *bufferToSend, int len); //send buffer via serial port
Uart_Errors GetData(char *receivedBuff, int expectedLen, unsigned int timeOut); //expect some data in specified time


void setup() 
{
  // initialize both serial ports of HM interface:
  Serial.begin(9600);

  restoreToFactoryDefault();
  
  assert(isDeviceAwake() == true); //device must be awake
  
  if(getModuleWorkType() == false)
    setModuleWorkType(1);
  
  
  assert(openCloseNotify(false) == true); //AT+NOTI1
  
  if(getDeviceRole() != 1)
    assert(setDeviceRole(1) == true); //AT+ROLE1
  
  assert(setDiscoveryTimeWhenModuleIsMaster(3) == true);
  
  restartModule();
}

void loop() 
{
  /*
      Send: AT+DISI?
      Recv: OK+DISCS (Scan start)
      Recv: OK+DIS[P0:P1:P2:P3:P4] (if have one device)
      Recv: OK+DIS[P0:P1:P2:P3:P4] (if have two devices)
      ………..
      Recv: OK+DISCE (Scan end)
  */
  uint8_t discoTime = getDiscoveryTimeWhenModuleIsMaster();
  if(discoTime==0)discoTime=3;
  
  uint8_t receivedBuff[90];
  memset(receivedBuff, 0, 90);
  
  Serial.flush();
  sprintf((char *)receivedBuff, (char *)"AT+DISI?");
  SendData(receivedBuff, (int)strlen((char *)receivedBuff));
  
  //then seek very long answer
  //wait discoTime
  unsigned long currentMillis = millis();
  unsigned long elapsedTime = ((uint32_t)((discoTime+1) * 1000));
  while((millis() - currentMillis) < elapsedTime);



  
  //Now we can get a list of iBeacon devices via serial port


  
}

void SendData(char *bufferToSend, int len)
{
  Serial.write(bufferToSend, len);
}

Uart_Errors GetData(char *receivedBuff, int expectedLen, unsigned int timeOut)
{
  int bufferCounter = 0;
  unsigned long startTime = millis();

  while((millis() - startTime) < timeOut)
  {
    if (Serial.available() > 0) 
    {
        // read the incoming byte:
        byte incomingByte = Serial.read();
        receivedBuff[bufferCounter++] = (char)incomingByte;

        if(bufferCounter >= expectedLen)
          return UART_ERR_OK;
     }
  }
  return UART_ERR_TIMEOUT;
}


bool switchStudyFunction(uint8_t prm)
{
  //write the code for STD libraries
  //control prm first
  assert(prm <= 1);
  
  uint8_t receivedBuff[9] = {0};
  memset(receivedBuff, 0, 9);
  
   //send "AT+COMP[P1]" and get "OK+Set:[P1]"
  sprintf((char *)receivedBuff, (char *)"AT+COMP%d", prm);
  SendData(receivedBuff, strlen((char *)receivedBuff));

  memset(receivedBuff, 0, 9);
  //"OK+Set:[P1]" eg: len is 8, we will seek 0 or 1
  Uart_Errors errCondition = GetData(receivedBuff, 8, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, (char *)"OK+Set:", 7) == 0))
  {
    //no need to control the return val
    return true;
  }
  return false;
}

bool isDeviceAwake()
{
  //write the code for STD libraries
  uint8_t receivedBuff[3] = {0};
  memset(receivedBuff, 0, 3);
  
  sprintf((char *)receivedBuff, (char *)"AT");
  SendData(receivedBuff, strlen((char *)receivedBuff));

  memset(receivedBuff, 0, 3);
  Uart_Errors errCondition = GetData(receivedBuff, 2, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, "OK", 2) == 0)) 
  {
    return true;
  }
  return false;
}

bool wakeUpDevice()
{
  //write the code for STD libraries
  //to wake up, send >80 character
  uint8_t dummyMessage[WAKEUP_MESSAGE_LEN] = {0};
  memset(dummyMessage, (uint8_t)'A', WAKEUP_MESSAGE_LEN);
  SendData(dummyMessage, WAKEUP_MESSAGE_LEN);
  
  //wait "OK+WAKE" or nothing, len is 7
  uint8_t receivedBuff[8] = {0};
  memset(receivedBuff, 0, 8);
  Uart_Errors errCondition = GetData(receivedBuff, 7, 2000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, "OK+WAKE", 7) == 0)) 
  {
    return true;
  }
  else if(errCondition == UART_ERR_TIMEOUT)
  {
    if((int)(strlen((char *)receivedBuff)) == 0) //it means device is already waked up
      return true;
  }
  return false;
}
      
bool openCloseNotify(bool open)
{
  //write the code for STD libraries
  
  uint8_t receivedBuff[9] = {0};
  memset(receivedBuff, 0, 9);
  
  if(open) //send 1
    sprintf((char *)receivedBuff, (char *)"AT+NOTI1");
  else //send 0
    sprintf((char *)receivedBuff, (char *)"AT+NOTI0");
  
  SendData(receivedBuff, strlen((char *)receivedBuff));

  //"OK+Set:[P1]" received //len 8
  memset(receivedBuff, 0, 9);
  Uart_Errors errCondition = GetData(receivedBuff, 8, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, "OK+Set:", 7) == 0)) 
  {
    return true;
  }
  return false;
}

void qetModuleMACAddress(char *address)
{
  //write the code for STD libraries
  bool weGetMac = false;
  
  uint8_t receivedBuff[21] = {0};
  memset(receivedBuff, 0, 21);
  
  sprintf((char *)receivedBuff, (char *)"AT+ADDR?");
  
  SendData(receivedBuff, strlen((char *)receivedBuff));

  //"OK+ADDR:341513D9A655" received, if leading characters are 0 maybe total mac adress len is less than 12, len is 20
  memset(receivedBuff, 0, 21);
  Uart_Errors errCondition = GetData(receivedBuff, 20, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, "OK+ADDR:", 8) == 0)) 
  {
    weGetMac = true;
  }
  else if(errCondition == UART_ERR_TIMEOUT)
  {
    if((int)(strlen((char *)receivedBuff)) > 8) //it means we get smaller mac
      weGetMac = true;
  }
  else
    weGetMac = false;
  
  if(weGetMac)
  {
    //split data with ':' delim and return mac address
    //we know total received message len and messages first part
    memcpy(address, &receivedBuff[8], (strlen((char *)receivedBuff) - 8));
  }
}
      
uint8_t qetAdvertisingInterval()
{
  //write the code for STD libraries
  bool weGetIt = false;
  
  uint8_t receivedBuff[9] = {0};
  memset(receivedBuff, 0, 9);
  
  sprintf((char *)receivedBuff, "AT+ADVI");
  SendData(receivedBuff, strlen((char *)receivedBuff));

  //"OK+Get:0" received, len is 8, note that max data is F, it means 1 byte
  memset(receivedBuff, 0, 9);
  Uart_Errors errCondition = GetData(receivedBuff, 8, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, "OK+Get:", 7) == 0)) 
  {
    weGetIt = true;
  }
  else
    weGetIt = false;
  
  if(weGetIt)
  {
    //split data with ':' delim and return mac address
    //we know total received message len and messages first part
    char c = receivedBuff[7];
    if((c >= 0x41) && (c <= 0x46)) //it means between 'A' or 'F'
      return (uint8_t)(c - 0x41 + 10);
    else if((c >= 0x61) && (c <= 0x66)) //it means between 'a' or 'f'
      return (uint8_t)(c - 0x61 + 10);
    else  if((c >= 0x30) && (c <= 0x39))//it means between '0' or '9'
      return (uint8_t)(c - 0x30);
  }
  return 255;
}


bool setAdvertisingInterval(uint8_t prm)
{
  //write the code for STD libraries
  //control prm
  assert(prm <= 0x0F);
  
  uint8_t receivedBuff[9] = {0};
  memset(receivedBuff, 0, 9);
  
  sprintf((char *)receivedBuff, (char *)"AT+ADVI%X", prm);
  SendData(receivedBuff, strlen((char *)receivedBuff));

  //wait "OK+Set:0"
  memset(receivedBuff, 0, 9);
  Uart_Errors errCondition = GetData(receivedBuff, 8, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, "OK+Set:", 7) == 0)) 
  {
    return true;
  }
  return false;
}
  
bool openCloseWhiteListProperty(bool open)
{
  //write the code for STD libraries
  
  uint8_t receivedBuff[9] = {0};
  memset(receivedBuff, 0, 9);
  
  if(open) //send 1
    sprintf((char *)receivedBuff, (char *)"AT+ALLO1");
  else //send 0
    sprintf((char *)receivedBuff, (char *)"AT+ALLO0");
  
  SendData(receivedBuff, strlen((char *)receivedBuff));

  //"OK+Set:[P1]" received //len 8
  memset(receivedBuff, 0, 9);
  Uart_Errors errCondition = GetData(receivedBuff, 8, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, "OK+Set:", 7) == 0)) 
  {
    return true;
  }
  return false;
}

bool isWhiteListPropertyOpen()
{
  //write the code for STD libraries
  bool weGetIt = false;
  
  uint8_t receivedBuff[9] = {0};
  memset(receivedBuff, 0, 9);
  
  sprintf((char *)receivedBuff, (char *)"AT+ALLO?");
  SendData(receivedBuff, strlen((char *)receivedBuff));

  //"OK+Get:[P1]" received, len is 8
  memset(receivedBuff, 0, 9);
  Uart_Errors errCondition = GetData(receivedBuff, 8, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, "OK+Get:", 7) == 0)) 
  {
    weGetIt = true;
  }
  else
    weGetIt = false;
  
  if(weGetIt)
  {
    //split data with ':' delim and return mac address
    //we know total received message len and messages first part
    char c = receivedBuff[7];
    if(c == '1')
      return true;
  }
  return false;
}


void getWhiteListSelectedDeviceMACAddress(uint8_t listIdx, char *readMAcAddress)
{
  //write the code for STD libraries
  
  //control prm first
  assert((listIdx >= 1) && (listIdx <= 9));
  
  char expectedCommand[9] = {0};
  memset(expectedCommand, 0, 9);
  sprintf(expectedCommand, "OK+AD%d?:", listIdx);
  
  bool weGetMac = false;
  
  uint8_t receivedBuff[21] = {0};
  memset(receivedBuff, 0, 21);
  
  sprintf((char *)receivedBuff, (char *)"AT+AD%d??", listIdx);
  SendData(receivedBuff, strlen((char *)receivedBuff));

  //"OK+AD1?:000000000000" received, if leading characters are 0 maybe total mac adress len is less than 12, len is 20
  memset(receivedBuff, 0, 21);
  Uart_Errors errCondition = GetData(receivedBuff, 20, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, expectedCommand, 8) == 0)) 
  {
    weGetMac = true;
  }
  else if(errCondition == UART_ERR_TIMEOUT)
  {
    if((int)(strlen((char *)receivedBuff)) > 8) //it means we get smaller mac
      weGetMac = true;
  }
  else
    weGetMac = false;
  
  if(weGetMac)
  {
    //split data with ':' delim and return mac address
    //we know total received message len and messages first part
    memcpy(readMAcAddress, &receivedBuff[8], (strlen((char *)receivedBuff) - 8));
  }
  
  return;
}

bool setWhiteListIndexWithSpecifiedMACAddress(uint8_t idx, char *macAddress)
{
  //write the code for STD libraries
  
  //control prm first
  assert((idx >= 1) && (idx <= 3));
  
  //send "AT+AD1001122334455" and receive "OK+Set:001122334455"
  char expectedCommand[20] = {0};
  memset(expectedCommand, 0, 20);

  int padCount = 12 - strlen(macAddress); //mac addres should be 12 character len
  char paddedMacAddress[13] = {0};
  memset(paddedMacAddress, '0', padCount);
  int dataLen = strlen(macAddress);
  for(int i = 0; i < dataLen; i++)
    paddedMacAddress[i + padCount] = macAddress[i];
  
  sprintf(expectedCommand, "OK+AD%d%s", idx, paddedMacAddress);
  
  
  uint8_t receivedBuff[21] = {0};
  memset(receivedBuff, 0, 21);
  
  sprintf((char *)receivedBuff, (char *)"AT+AD%d%s", idx, paddedMacAddress);
  SendData(receivedBuff, strlen((char *)receivedBuff));

  memset(receivedBuff, 0, 21);
  Uart_Errors errCondition = GetData(receivedBuff, 18, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, expectedCommand, 18) == 0)) 
  {
    return true;
  }
  else
    return false;
}

uint8_t getBatteryPercentage()
{
  //write the code for STD libraries
  //send "AT+BATT?" and get "OK+Get:100"
  bool weGetMac = false;
  
  uint8_t receivedBuff[11] = {0};
  memset(receivedBuff, 0, 11);
  
  sprintf((char *)receivedBuff, (char *)"AT+BATT?");
  SendData(receivedBuff, strlen((char *)receivedBuff));

  //"OK+Get:100" received, if battery is not 100 then it may be one character len, max len is 10
  memset(receivedBuff, 0, 11);
  Uart_Errors errCondition = GetData(receivedBuff, 10, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, (char *)"OK+Get:", 7) == 0)) //if this condition satisfies then it is 100 no change
  {
    return 100;;
  }
  else if(errCondition == UART_ERR_TIMEOUT)
  {
    if((int)(strlen((char *)receivedBuff)) > 7) //it means we get smaller battery percentage
      weGetMac = true;
  }
  else
    weGetMac = false;
  
  if(weGetMac)
  {
    //split data with ':' delim and return mac address
    //we know total received message len and messages first part
    //create a buffer to get data part
    int dataPartLen = ((strlen((char *)receivedBuff) - 7));
    char *batt = new char[(dataPartLen + 1)]; //+1 means null character
    if(!batt)
      return 0;
    
    memset(batt, 0, (dataPartLen + 1));
    memcpy(batt, &receivedBuff[7], dataPartLen);
    //now try to convert the buffer to in
    uint8_t batVal = (uint8_t)(atoi(batt));
    delete[] batt;
    return batVal;
  }
  
  return 0;
}

bool setBaudRate(uint8_t selection)
{
  //write the code for STD libraries
  //first control inputs
  assert(selection <= 8);
  
  //send "AT+BAUD0" and get "OK+Set:0"
  bool weGetMac = false;
  
  uint8_t receivedBuff[9] = {0};
  memset(receivedBuff, 0, 9);
  
  sprintf((char *)receivedBuff , (char *)"AT+BAUD%d", selection);
  SendData(receivedBuff, strlen((char *)receivedBuff));

  //"OK+Set:0" where '0' has to be 'selection' itself
  memset(receivedBuff, 0, 9);
  Uart_Errors errCondition = GetData(receivedBuff, 8, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, (char *)"OK+Set:", 7) == 0)) //if this condition satisfies then it is 100 no change
  {
    weGetMac = true;
  }
  else
    weGetMac = false;
  
  if(weGetMac)
  {
    //split data with ':' delim and return mac address
    //we know data part is the last char
    if((receivedBuff[7] - 0x30) == selection)
      return true;
  }
  
  return false;
}

bool setBLECharacteristic(uint16_t type)
{
  //write the code for STD libraries
  //send "AT+CHAR0xAAA0", get "OK+Set:0xAAA0"
  bool weGetMac = false;
  
  uint8_t receivedBuff[14] = {0};
  memset(receivedBuff, 0, 14);
  
  sprintf((char *)receivedBuff, (char *)"AT+CHAR0x%.04X", type);
  SendData(receivedBuff, strlen((char *)receivedBuff));

  //"OK+Set:0" where '0' has to be 'selection' itself
  memset(receivedBuff, 0, 14);
  Uart_Errors errCondition = GetData(receivedBuff, 13, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, (char *)"OK+Set:0x", 9) == 0)) //if this condition satisfies then it is 100 no change
  {
    weGetMac = true;
  }
  else if(errCondition == UART_ERR_TIMEOUT) //maybe it pads zero to start of the type eg: 0xFE means 0x00FE
  {
    if((int)(strlen((char *)receivedBuff)) > 9) //it means we get smaller type
      weGetMac = true;
  }
  else
    weGetMac = false;
  
  if(weGetMac)
  {
    //split data with ':' delim and return mac address
    //we know total received message len and messages first part
    char readType[5] = {0};
    memset(readType, 0, 5);
    memcpy(readType, &receivedBuff[9], (strlen((char *)receivedBuff) - 9));
    
    //convert hex string to in and compare with input
    int number = (int)strtol(readType, NULL, 16);
    if(number == type)
      return true;
  }
  return false;
}

uint16_t getBLECharacteristic()
{
  //write the code for STD libraries
  //send "AT+CHAR?" and get "OK+Get:0xFFE1"
  bool weGetMac = false;
  
  uint8_t receivedBuff[14] = {0};
  memset(receivedBuff, 0, 14);
  
  sprintf((char *)receivedBuff, (char *)"AT+CHAR?");
  SendData(receivedBuff, strlen((char *)receivedBuff));

  //"OK+Set:0" where '0' has to be 'selection' itself
  memset(receivedBuff, 0, 14);
  Uart_Errors errCondition = GetData(receivedBuff, 13, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, (char *)"OK+Get:0x", 9) == 0)) //if this condition satisfies then it is 100 no change
  {
    weGetMac = true;
  }
  else if(errCondition == UART_ERR_TIMEOUT) //maybe it pads zero to start of the type eg: 0xFE means 0x00FE
  {
    if((int)(strlen((char *)receivedBuff)) > 9) //it means we get smaller type
      weGetMac = true;
  }
  else
    weGetMac = false;
  
  if(weGetMac)
  {
    //split data with ':' delim and return mac address
    //we know total received message len and messages first part
    char readType[5] = {0};
    memset(readType, 0, 5);
    memcpy(readType, &receivedBuff[9], (strlen((char *)receivedBuff) - 9));
    
    //convert hex string to in and compare with input
    int number = (int)strtol(readType, NULL, 16);
    return number;
  }
  return 0;
}
      

bool clearLastConnectedDeviceAddress()
{
  //write the code for STD libraries
  //send "AT+CLEAR" and get "OK+CLEAR"
  uint8_t receivedBuff[9] = {0};
  memset(receivedBuff, 0, 9);
  
  sprintf((char *)receivedBuff, (char *)"AT+CLEAR");
  SendData(receivedBuff, strlen((char *)receivedBuff));

  //"OK+CLEAR" len is 8

  memset(receivedBuff, 0, 9);
  Uart_Errors errCondition = GetData(receivedBuff, 8, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, (char *)"OK+CLEAR", 8) == 0))
  {
    return true;
  }
  else
    return false;
}

bool tryToConnectLastSucceedDevice()
{
  //write the code for STD libraries
  return true;
}

bool tryToConnectDeviceUseMAC(char *mac)
{
  //write the code for STD libraries  
  //send "AT+CON0017EA090909" to connect mac-> 00:17:EA:09:09:09
  //first control the input
  assert(strlen(mac) <= 12);
  
  //create padding zero mac address
  char macAddress[13] = {0};
  memset(macAddress, 0, 13);
  
  memcpy(macAddress, mac, strlen(mac));
  if(strlen(mac) < 12)
  {
    //make zero padding
    memset(macAddress, 0, 13);
    int paddingCounter = 12 - strlen(mac);
    int inputLen = strlen(mac);
    //padding part
    memset(macAddress, '0', paddingCounter);
    //fill other parts
    for(int i = 0; i < inputLen; i++)
    {
      macAddress[i + paddingCounter] = mac[i];
    }
  }
  
  uint8_t receivedBuff[30] = {0};
  memset(receivedBuff, 0, 30);
  
  //now we can use 'macAddress' buffer
  sprintf((char *)receivedBuff, (char *)"AT+CON%s", macAddress);
  SendData((uint8_t *)receivedBuff, strlen((char *)receivedBuff));
  
  //wait for a while
  memset(receivedBuff, 0, 30);
  Uart_Errors errCondition = GetData(receivedBuff, 30, 5000); //wait 5 seconds here

  return isDeviceConnected();
}

bool isDeviceConnected()
{
  if(isDeviceAwake())
    return false;
  return true;
}

bool tryToConnectDiscoveredDeviceArrayIndex(uint8_t idx)
{
  //write the code for STD libraries
  //TODO, fill here later
  return true;
}

bool startDeviceDiscoveryScan()
{
  //write the code for STD libraries
  //TODO, fill here later
  return true;
}

bool scanIBeaconDiscoveryScan(char MacList[][13], uint8_t MacAddressCount, int *RSSList)
{
  //write the code for STD libraries
  return true;
}


bool isUARTRxGainOpen()
{
  //write the code for STD libraries
  uint8_t receivedBuff[9] = {0};
  memset(receivedBuff, 0, 9);
  
  sprintf((char *)receivedBuff, (char *)"AT+GAIN?");
  //send "AT+GAIN?" and get "OK+Get:[P1]" where P1:0 is no RX gain, P1:1 Opened means
  SendData(receivedBuff, (int)strlen((char *)receivedBuff));

  //"OK+Get:0" eg: len is 8
  memset(receivedBuff, 0, 9);
  Uart_Errors errCondition = GetData(receivedBuff, 8, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, (char *)"OK+Get:", 7) == 0))
  {
    if(strlen((char *)receivedBuff) == 8)
    {
      if(receivedBuff[7] == '1') //it means opened
        return true;
    }
  }
  return false;
}

bool setUARTRxGain(uint8_t prm)
{
  //write the code for STD libraries
  //control prm first
  assert(prm <= 1);
  
  uint8_t receivedBuff[9] = {0};
  memset(receivedBuff, 0, 9);
  
  sprintf((char *)receivedBuff, (char *)"AT+GAIN%d", prm);
  //send "AT+GAIN[P1]" and get "OK+Set:[P1]"
  SendData(receivedBuff, (int)(strlen((char *)receivedBuff)));

  //"OK+Set:0" eg: len is 8
  memset(receivedBuff, 0, 9);
  Uart_Errors errCondition = GetData(receivedBuff, 8, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, (char *)"OK+Set:", 7) == 0))
  {
    //no need to control last parameter
    return true;
  }
  
  return false;
}

bool getModuleWorkType()
{
  //write the code for STD libraries
  uint8_t receivedBuff[9] = {0};
  memset(receivedBuff, 0, 9);
  
  sprintf((char *)receivedBuff, (char *)"AT+IMME?");
  //send "AT+IMME?" and get "OK+Get:[P1]"
  SendData(receivedBuff, (int)strlen((char *)receivedBuff));

  //"OK+Get:0" eg: len is 8
  memset(receivedBuff, 0, 9);
  Uart_Errors errCondition = GetData(receivedBuff, 8, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, (char *)"OK+Get:", 7) == 0))
  {
    if(strlen((char *)receivedBuff) == 8)
    {
      if(receivedBuff[7] == '1') //it means work type is "waiting AT command status"
        return true;
    }
  }
  return false;
}

bool setModuleWorkType(uint8_t prm)
{
  //write the code for STD libraries
  //control prm first
  assert(prm <= 1);
  
  uint8_t receivedBuff[9] = {0};
  memset(receivedBuff, 0, 9);
  
  sprintf((char *)receivedBuff, (char *)"AT+IMME%d", prm);
  //send "AT+IMME[P1]" and get "OK+ Set:[P1]"
  SendData(receivedBuff, (int)strlen((char *)receivedBuff));

  //"OK+Set:0" eg: len is 8
  memset(receivedBuff, 0, 9);
  Uart_Errors errCondition = GetData(receivedBuff, 8, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, (char *)"OK+Set:", 7) == 0))
  {
    //no need to control last parameter
    return true;
  }
  return false;
}

uint8_t getModuleWorkMode()
{
  //write the code for STD libraries
  uint8_t receivedBuff[9] = {0};
  memset(receivedBuff, 0, 9);
  
  sprintf((char *)receivedBuff, (char *)"AT+MODE?");
  //send "AT+MODE?" and get "OK+Get:[P1]"
  SendData(receivedBuff, (int)strlen((char *)receivedBuff));

  //"OK+Get:0" eg: len is 8, we will seek 0, 1 or 2
  memset(receivedBuff, 0, 9);
  Uart_Errors errCondition = GetData(receivedBuff, 8, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, (char *)"OK+Get:", 7) == 0))
  {
    if(strlen((char *)receivedBuff) == 8)
    {
      if(receivedBuff[7] == '1') //it means collection
        return 1;
      else if(receivedBuff[7] == '2') //it means remote
        return 2;
    }
  }
  return 0; //it means transmission mode
}

bool setModuleWorkMode(uint8_t prm)
{
  //write the code for STD libraries
  //control prm first
  assert(prm <= 2);
  
  uint8_t receivedBuff[9] = {0};
  memset(receivedBuff, 0, 9);
  
  sprintf((char *)receivedBuff, (char *)"AT+MODE%d", prm);
  //send "AT+MODE[P1]" and get "OK+Set:[P1]"
  SendData(receivedBuff, (int)strlen((char *)receivedBuff));

  //"OK+Set:0" eg: len is 8
  memset(receivedBuff, 0, 9);
  Uart_Errors errCondition = GetData(receivedBuff, 8, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, (char *)"OK+Set:", 7) == 0))
  {
    //no need to control last parameter
    return true;
  }
  return false;
}

void getModuleName(char *name)
{
  //write the code for STD libraries
  // send "AT+NAME?" and get "OK+NAME[P1]"
  bool readNameValid = false;
  
  uint8_t receivedBuff[20] = {0};
  memset(receivedBuff, 0, 20);
  
  sprintf((char *)receivedBuff, (char *)"AT+NAME?");
  SendData(receivedBuff, (int)strlen((char *)receivedBuff));

  //"OK+NAME[P1]" note that max name len is 12 so max buffer is 19
  memset(receivedBuff, 0, 20);
  Uart_Errors errCondition = GetData(receivedBuff, 20, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, (char *)"OK+NAME", 7) == 0))
  {
    readNameValid = true;
  }
  else if(errCondition == UART_ERR_TIMEOUT)
  {
    if(strlen((char *)receivedBuff) > 7) //it means there  is also a valid name
    {
      readNameValid = true;
    }
  }
  
  if(readNameValid)
  {
    int nameLen = strlen((char *)receivedBuff) - 7;
    memcpy((uint8_t *)name, (uint8_t *)(&receivedBuff[7]), nameLen);
  }
  return;
}

bool setModuleName(char *name)
{
  //write the code for STD libraries
  //send "AT+NAME[P1]" and get "OK+Set[P1]"
  
  //control prm first
  assert(strlen((char *)name) <= 12);
  
  uint8_t receivedBuff[20] = {0};
  memset(receivedBuff, 0, 20);
  
  sprintf((char *)receivedBuff, (char *)"AT+NAME%s", name);
  SendData(receivedBuff, (int)strlen((char *)receivedBuff));

  int expectedLen = strlen((char *)name) + 6;
  //"OK+Set[P1]" note that max name len is 12 so max buffer is 19
  memset(receivedBuff, 0, 20);
  Uart_Errors errCondition = GetData(receivedBuff, expectedLen, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, (char *)"OK+Set", 6) == 0))
  {
    return true;
  }
  return false;
}

uint8_t getOutputDriverPower()
{
  //write the code for STD libraries
  uint8_t receivedBuff[9] = {0};
  memset(receivedBuff, 0, 9);
  
  sprintf((char *)receivedBuff, (char *)"AT+PCTL?");
  //send "AT+PCTL?" and get "OK+Get:[P1]"
  SendData(receivedBuff, (int)strlen((char *)receivedBuff));

  //"OK+Get:0" eg: len is 8, we will seek 0, 1
  memset(receivedBuff, 0, 9);
  Uart_Errors errCondition = GetData(receivedBuff, 8, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, (char *)"OK+Get:", 7) == 0))
  {
    if(strlen((char *)receivedBuff) == 8)
    {
      if(receivedBuff[7] == '1') //it means collection
        return 1;
      else if(receivedBuff[7] == '0') //it means remote
        return 0;
    }
  }
  return 0;
}

bool setOutputDriverPower(uint8_t prm)
{
  //write the code for STD libraries
  //control prm first
  assert(prm <= 1);
  
  uint8_t receivedBuff[9] = {0};
  memset(receivedBuff, 0, 9);
  
  sprintf((char *)receivedBuff, (char *)"AT+PCTL%d", prm);
  //send "AT+PCTL[P1]" and "OK+Set:[P1]"
  SendData(receivedBuff, (int)strlen((char *)receivedBuff));

  //"OK+Set:0" eg: len is 8
  memset(receivedBuff, 0, 9);
  Uart_Errors errCondition = GetData(receivedBuff, 8, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, (char *)"OK+Set:", 7) == 0))
  {
    //no need to control last parameter
    return true;
  }
  return false;
}

void getPinCode(uint32_t *pin)
{
  //write the code for STD libraries
  //send "AT+PASS?" and get "OK+Get:[P1]"
  
  uint8_t receivedBuff[14] = {0};
  memset(receivedBuff, 0, 14);
  
  sprintf((char *)receivedBuff, (char *)"AT+PASS?");
  SendData(receivedBuff, (int)strlen((char *)receivedBuff));
  
  bool validData = false;
  //we seek "OK+Get:[P1]" and P1 is max "999999" which means 6 char len so max is 7+6 = 13
  memset(receivedBuff, 0, 14);
  Uart_Errors errCondition = GetData(receivedBuff, 14, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, (char *)"OK+Get:", 7) == 0))
  {
    validData = true;
  }
  else if(errCondition == UART_ERR_TIMEOUT)
  {
    if(strlen((char *)receivedBuff) > 7) //it means there  is also a valid name
    {
      validData = true;
    }
  }
  
  if(validData)
  {
    int nameLen = strlen((char *)receivedBuff) - 7;
    
    //convert the char array to int
    int i;
    sscanf((char *)(&receivedBuff[7]), "%d", &i);
    
    *pin = i;
  }
}

bool setPinCode(uint32_t pin)
{
  //write the code for STD libraries
  //send "AT+PASS[P1]" and get "OK+Set:[P1]"
  
  //control prm first
  assert(pin <= 999999);
  
  
  uint8_t receivedBuff[14] = {0};
  memset(receivedBuff, 0, 14);
  
  sprintf((char *)receivedBuff, (char *)"AT+PASS%.06d", pin);
  SendData(receivedBuff, (int)strlen((char *)receivedBuff));

  //"OK+Set:P1" eg: len is 7 + strlen, max is 13
  memset(receivedBuff, 0, 14);
  Uart_Errors errCondition = GetData(receivedBuff, 13, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, (char *)"OK+Set:", 7) == 0))
  {
    //no need to control last parameter
    return true;
  }
  return false;
}
      

uint8_t getModulePower()
{
  //write the code for STD libraries
  //send "AT+POWE?" and get "OK+Get:[P1]"
  uint8_t receivedBuff[9] = {0};
  memset(receivedBuff, 0, 9);
  
  sprintf((char *)receivedBuff, (char *)"AT+POWE?");
  SendData(receivedBuff, (int)strlen((char *)receivedBuff));

  //"OK+Get:[P1]" eg: len is 8, we will seek 0 to 3
  memset(receivedBuff, 0, 9);
  Uart_Errors errCondition = GetData(receivedBuff, 8, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, (char *)"OK+Get:", 7) == 0))
  {
    if(strlen((char *)receivedBuff) == 8)
    {
      if(receivedBuff[7] == '0') //it means collection
        return 0;
      else if(receivedBuff[7] == '1') //it means remote
        return 1;
      else if(receivedBuff[7] == '2') //it means remote
        return 2;
      else if(receivedBuff[7] == '3') //it means remote
        return 3;
    }
  }
  return 0;
}

bool setModulePower(uint8_t prm)
{
  //write the code for STD libraries
  //control the prm
  assert(prm <= 3);
  
  uint8_t receivedBuff[9] = {0};
  memset(receivedBuff, 0, 9);
  
  sprintf((char *)receivedBuff, (char *)"AT+POWE%d", prm);
  //send "AT+POWE[P1]" and get "OK+Set:[P1]"
  SendData(receivedBuff, (int)strlen((char *)receivedBuff));

  //"OK+Set:[P1]" eg: len is 8, we will seek 0 to 3
  memset(receivedBuff, 0, 9);
  Uart_Errors errCondition = GetData(receivedBuff, 8, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, (char *)"OK+Set:", 7) == 0))
  {
    //no need to control the return val
    return true;
  }
  return false;
}

uint8_t getModuleSleepType()
{
  //write the code for STD libraries
  //send "AT+PWRM?" and get "OK+Get:[P1]"
  uint8_t receivedBuff[9] = {0};
  memset(receivedBuff, 0, 9);
  
  sprintf((char *)receivedBuff, (char *)"AT+PWRM?");
  SendData(receivedBuff, (int)strlen((char *)receivedBuff));

  //"OK+Get:[P1]" eg: len is 8, we will seek 0 or 1
  memset(receivedBuff, 0, 9);
  Uart_Errors errCondition = GetData(receivedBuff, 8, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, (char *)"OK+Get:", 7) == 0))
  {
    if(strlen((char *)receivedBuff) == 8)
    {
      if(receivedBuff[7] == '0') //it means auto sleep
        return 0;
      else if(receivedBuff[7] == '1') //it means dont auto sleep
        return 1;
    }
  }
  return 0;
}

bool setPowerModuleSleepType(uint8_t prm)
{
  //write the code for STD libraries
  //control prm
  assert(prm <= 1);
  
  uint8_t receivedBuff[9] = {0};
  memset(receivedBuff, 0, 9);
  
  sprintf((char *)receivedBuff, (char *)"AT+PWRM%d", prm);
  //send "AT+PWRM[P1]" and get "OK+Set:[P1]"
  SendData(receivedBuff, (int)strlen((char *)receivedBuff));

  //"OK+Set:[P1]" eg: len is 8, we will seek 0 or 1
  memset(receivedBuff, 0, 9);
  Uart_Errors errCondition = GetData(receivedBuff, 8, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, (char *)"OK+Set:", 7) == 0))
  {
    //no need to control the return val
    return true;
  }
  return false;
}

uint8_t getDeviceRole()
{
  //write the code for STD libraries
  //send "AT+ROLE?" and get "OK+Get:[P1]"
  uint8_t receivedBuff[9] = {0};
  memset(receivedBuff, 0, 9);
  
  sprintf((char *)receivedBuff, (char *)"AT+ROLE?");
  SendData(receivedBuff, (int)strlen((char *)receivedBuff));

  //"OK+Get:[P1]" eg: len is 8, we will seek 0 or 1
  memset(receivedBuff, 0, 9);
  Uart_Errors errCondition = GetData(receivedBuff, 8, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, (char *)"OK+Get:", 7) == 0))
  {
    if(strlen((char *)receivedBuff) == 8)
    {
      if(receivedBuff[7] == '0')
        return 0;
      else if(receivedBuff[7] == '1')
        return 1;
    }
  }
  return 0;
}

bool setDeviceRole(uint8_t prm)
{
  //write the code for STD libraries
  //control prm first
  assert(prm <= 1);
  
  uint8_t receivedBuff[9] = {0};
  memset(receivedBuff, 0, 9);
  
  sprintf((char *)receivedBuff, (char *)"AT+ROLE%d", prm);
  // send "AT+ROLE[P1]" and get "OK+Set:[P1]"
  SendData(receivedBuff, (int)strlen((char *)receivedBuff));

  //"OK+Set:[P1]" eg: len is 8, we will seek 0 or 1
  memset(receivedBuff, 0, 9);
  Uart_Errors errCondition = GetData(receivedBuff, 8, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, (char *)"OK+Set:", 7) == 0))
  {
    //no need to control the return val
    return true;
  }
  return false;
}

void getRSSIValue()
{
  //write the code for STD libraries
  //TODO, fill here later
  return;
}

void getLastConnectedDeviceMACAddress(char *address)
{
  //write the code for STD libraries
  //TODO, fill here later
  return;
}
      

uint8_t getDiscoveryTimeWhenModuleIsMaster()
{
  //write the code for STD libraries
  //send "AT+SCAN?" and get "OK+Get:[P1]"
  uint8_t receivedBuff[9] = {0};
  memset(receivedBuff, 0, 9);
  
  sprintf((char *)receivedBuff, (char *)"AT+SCAN?");
  SendData(receivedBuff, (int)strlen((char *)receivedBuff));

  //"OK+Get:[P1]" eg: len is 8, we will seek 1 to 9
  memset(receivedBuff, 0, 9);
  Uart_Errors errCondition = GetData(receivedBuff, 8, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, (char *)"OK+Get:", 7) == 0))
  {
    if(strlen((char *)receivedBuff) == 8)
    {
      if(receivedBuff[7] == '1') 
        return 1;
      else if(receivedBuff[7] == '2') 
        return 2;
      else if(receivedBuff[7] == '3') 
        return 3;
      else if(receivedBuff[7] == '4') 
        return 4;
      else if(receivedBuff[7] == '5') 
        return 5;
      else if(receivedBuff[7] == '6') 
        return 6;
      else if(receivedBuff[7] == '7') 
        return 7;
      else if(receivedBuff[7] == '8') 
        return 8;
      else if(receivedBuff[7] == '9') 
        return 9;
    }
  }
  return 0;
}

bool setDiscoveryTimeWhenModuleIsMaster(uint8_t prm)
{
  //write the code for STD libraries
  //control prm first
  assert((prm >= 1) && (prm <= 9));
  
  uint8_t receivedBuff[9] = {0};
  memset(receivedBuff, 0, 9);
  
  sprintf((char *)receivedBuff, (char *)"AT+SCAN%d", prm);
  //send "AT+SCAN[P1]" and get "OK+Set:[P1]"
  SendData(receivedBuff, (int)strlen((char *)receivedBuff));

  //"OK+Set:[P1]" eg: len is 8, we will seek 0 or 1
  memset(receivedBuff, 0, 9);
  Uart_Errors errCondition = GetData(receivedBuff, 8, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, (char *)"OK+Set:", 7) == 0))
  {
    //no need to control the return val
    return true;
  }
  return false;
}

void getModuleVersion(char *version)
{
  //write the code for STD libraries
  uint8_t receivedBuff[40] = {0};
  memset(receivedBuff, 0, 40);
  
  sprintf((char *)receivedBuff, (char *)"AT+VERR?");
  //send "AT+VERR?" and get "HMSoft V604", so split answer with ' ' delim and discard 'V' sign
  SendData(receivedBuff, (int)strlen((char *)receivedBuff));

  //lets say we expect 40 characters (of course not) and split it in timeout error condition
  memset(receivedBuff, 0, 40);
  Uart_Errors errCondition = GetData(receivedBuff, 40, 1000);
  if (errCondition == UART_ERR_TIMEOUT)
  {
    int len = strlen((char *)receivedBuff);
    int lastSpaceIdx = 0;
    //split here, get last space character
    for(int i = 0; i < len; i++)
    {
      if(receivedBuff[i] == ' ')
        lastSpaceIdx = i;
    }
    if(lastSpaceIdx != 0)
    {
      memcpy((uint8_t *)version, &receivedBuff[lastSpaceIdx + 2], (len - lastSpaceIdx - 2)); //+2 means discard space and 'V' characters
    }
  }
  return;
}
      

uint16_t getServiceUUID()
{
  //write the code for STD libraries
  //TODO, fill here later
  return 0;
}

bool setServiceUUID(uint16_t prm)
{
  //write the code for STD libraries
  //TODO, fill here later
  return true;
}

uint8_t getBondMode()
{
  //write the code for STD libraries
  //send "AT+TYPE?" and get "OK+Get:[P1]"
  uint8_t receivedBuff[9] = {0};
  memset(receivedBuff, 0, 9);
  
  sprintf((char *)receivedBuff,(char *)"AT+TYPE?");
  SendData(receivedBuff, (int)strlen((char *)receivedBuff));

  //"OK+Get:[P1]" eg: len is 8, we will seek 0 to 3
  memset(receivedBuff, 0, 9);
  Uart_Errors errCondition = GetData(receivedBuff, 8, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, (char *)"OK+Get:", 7) == 0))
  {
    if(strlen((char *)receivedBuff) == 8)
    {
      if(receivedBuff[7] == '0') 
        return 0;
      else if(receivedBuff[7] == '1') 
        return 1;
      else if(receivedBuff[7] == '2') 
        return 2;
      else if(receivedBuff[7] == '3') 
        return 3;
    }
  }
  return 0;
}

bool setBondMode(uint8_t prm)
{
  //write the code for STD libraries
  //control prm first
  assert(prm <= 3);
  
  uint8_t receivedBuff[9] = {0};
  memset(receivedBuff, 0, 9);
  
  sprintf((char *)receivedBuff, (char *)"AT+TYPE%d", prm);
  // send "AT+TYPE[P1]" and get "OK+Set:[P1]"
  SendData(receivedBuff, (int)strlen((char *)receivedBuff));

  //"OK+Set:[P1]" eg: len is 8, we will seek 0 or 1
  memset(receivedBuff, 0, 9);
  Uart_Errors errCondition = GetData(receivedBuff, 8, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, (char *)"OK+Set:", 7) == 0))
  {
    //no need to control the return val
    return true;
  }
  return false;
}

uint32_t getModuleConnectRemoteDeviceTimeout()
{
  //write the code for STD libraries
  //send "AT+TCON?" and get "OK+Get:[P1]" where P1 is between 000000~999999
  uint8_t receivedBuff[40] = {0};
  memset(receivedBuff, 0, 40);
  
  sprintf((char *)receivedBuff, (char *)"AT+TCON?");
  SendData(receivedBuff, (int)strlen((char *)receivedBuff));

  //lets say we expect 40 characters (of course not) and split it in timeout error condition
  memset(receivedBuff, 0, 40);
  Uart_Errors errCondition = GetData(receivedBuff, 40, 1000);
  if (errCondition == UART_ERR_TIMEOUT)
  {
    int len = strlen((char *)receivedBuff);
    char timeStr[7] = {0};
    memset(timeStr, 0, 7);
    
    memcpy((uint8_t *)timeStr, &receivedBuff[7], (len - 7));
    int i = 0;
    sscanf(timeStr, "%d", &i);
    return (uint32_t)i;
  }
  return 0;
}

bool setModuleConnectRemoteDeviceTimeout(uint32_t prm)
{
  //write the code for STD libraries
  //control prm
  assert(prm <= 999999);
  
  uint8_t receivedBuff[14] = {0};
  memset(receivedBuff, 0, 14);
  
  sprintf((char *)receivedBuff, (char *)"AT+TCON%.06d", prm);
  //send "AT+TCON[P1]" and get "OK+Set:[P1]"
  SendData(receivedBuff, (int)strlen((char *)receivedBuff));

  //"OK+Set:P1" eg: len is 7 + strlen, max is 13
  memset(receivedBuff, 0, 14);
  Uart_Errors errCondition = GetData(receivedBuff, 13, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, (char *)"OK+Set:", 7) == 0))
  {
    //no need to control last parameter
    return true;
  }
  return false;
}

uint8_t getStatusOfShowDeviceInfoWhenDiscovery()
{
  //write the code for STD libraries
  //send "AT+SHOW?" and get "OK+Get:[P1]"
  uint8_t receivedBuff[9] = {0};
  memset(receivedBuff, 0, 9);
  
  sprintf((char *)receivedBuff, (char *)"AT+SHOW?");
  SendData(receivedBuff, (int)strlen((char *)receivedBuff));

  //"OK+Get:[P1]" eg: len is 8, we will seek 0 to 3
  memset(receivedBuff, 0, 9);
  Uart_Errors errCondition = GetData(receivedBuff, 8, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, (char *)"OK+Get:", 7) == 0))
  {
    if(strlen((char *)receivedBuff) == 8)
    {
      if(receivedBuff[7] == '0') 
        return 0;
      else if(receivedBuff[7] == '1') 
        return 1;
      else if(receivedBuff[7] == '2') 
        return 2;
      else if(receivedBuff[7] == '3') 
        return 3;
    }
  }
  return 0;
}

bool setStatusOfShowDeviceInfoWhenDiscovery(uint8_t prm)
{
  //write the code for STD libraries
  //control prm first
  assert(prm <= 3);
  
  uint8_t receivedBuff[9] = {0};
  memset(receivedBuff, 0, 9);
  
  sprintf((char *)receivedBuff, (char *)"AT+SHOW%d", prm);
  //send "AT+SHOW[P1]" and get "OK+Set:[P1]"
  SendData(receivedBuff, (int)strlen((char *)receivedBuff));

  //"OK+Set:[P1]" eg: len is 8, we will seek 0 to 3
  memset(receivedBuff, 0, 9);
  Uart_Errors errCondition = GetData(receivedBuff, 8, 1000);
  if ((errCondition == UART_ERR_OK) && (strncmp((char *)receivedBuff, (char *)"OK+Set:", 7) == 0))
  {
    //no need to control the return val
    return true;
  }
  return false;
}

bool iBeaconDeployMode(uint8_t prm)
{
  //write the code for STD libraries
  //TODO, fill here later
  return true;
}

void restoreToFactoryDefault()
{
  //write the code for STD libraries
  //send "AT+RENEW" and get "OK+RENEW"
  uint8_t receivedBuff[20] = {0};
  memset(receivedBuff, 0, 20);
  
  sprintf((char *)receivedBuff, (char *)"AT+RENEW");
  SendData(receivedBuff, (int)strlen((char *)receivedBuff));
  
  //wait for a while and seek "OK+RESET", len is 
  memset(receivedBuff, 0, 20);
  Uart_Errors errCondition = GetData(receivedBuff, 8, 5000); //wait 5 seconds here or the answer
}
    

void restartModule()
{
  //write the code for STD libraries
    //now we can use 'macAddress' buffer
  uint8_t receivedBuff[20] = {0};
  memset(receivedBuff, 0, 20);
  
  sprintf((char *)receivedBuff, (char *)"AT+RESET");
  SendData(receivedBuff, (int)strlen((char *)receivedBuff));
  
  //wait for a while and seek "OK+RESET", len is 
  memset(receivedBuff, 0, 20);
  Uart_Errors errCondition = GetData(receivedBuff, 8, 5000); //wait 5 seconds here or the answer
}

/////////////////////////////////////////////////////////////////////
//USAGE EXAMPLES OF THESE FUNCTIONS//////////////////////////////////
/*uint8_t readDataFromHM10[64] = {0};
  memset(readDataFromHM10, 0, 64);
  uint8_t readUBFromHM10 = 0;
  uint16_t readUSFromHM10 = 0;
  uint32_t readUIFromHM10 = 0;
  
  if(hm10drv.isDeviceAwake())
    uartdrv.CreateAndSendData(USART3, (char *)"OK LINE: %d\r\n", __LINE__);
  
  hm10drv.restoreToFactoryDefault();
  hm10drv.restartModule();
  
  if(hm10drv.wakeUpDevice())
    uartdrv.CreateAndSendData(USART3, (char *)"OK LINE: %d\r\n", __LINE__);
  
  if(hm10drv.openCloseNotify(false))
    uartdrv.CreateAndSendData(USART3, (char *)"OK LINE: %d\r\n", __LINE__);
  
  memset(readDataFromHM10, 0, 64);
  hm10drv.qetModuleMACAddress((char *)readDataFromHM10);
  uartdrv.CreateAndSendData(USART3, (char *)"OK LINE: %d and mac: %s\r\n", __LINE__, (char *)readDataFromHM10);
  
  if(hm10drv.setAdvertisingInterval(0))
    uartdrv.CreateAndSendData(USART3, (char *)"OK LINE: %d\r\n", __LINE__);
  
  readUBFromHM10 = hm10drv.qetAdvertisingInterval();
  uartdrv.CreateAndSendData(USART3, (char *)"OK LINE: %d and dt: %X\r\n", __LINE__, readUBFromHM10);
  
  if(hm10drv.openCloseWhiteListProperty(false))
    uartdrv.CreateAndSendData(USART3, (char *)"OK LINE: %d\r\n", __LINE__);
  
  if(hm10drv.isWhiteListPropertyOpen())
    uartdrv.CreateAndSendData(USART3, (char *)"OPEN  LINE: %d\r\n", __LINE__);
  else
    uartdrv.CreateAndSendData(USART3, (char *)"CLOSE LINE: %d\r\n", __LINE__);
  
  if(hm10drv.setWhiteListIndexWithSpecifiedMACAddress(2, (char *)"1122334467"))
    uartdrv.CreateAndSendData(USART3, (char *)"OK LINE: %d\r\n", __LINE__);
    
  memset(readDataFromHM10, 0, 64);
  hm10drv.getWhiteListSelectedDeviceMACAddress(2, (char *)readDataFromHM10);
  uartdrv.CreateAndSendData(USART3, (char *)"OK LINE: %d, %d. index mac address: %s\r\n", __LINE__, 2, readDataFromHM10);
  
  uartdrv.CreateAndSendData(USART3, (char *)"OK LINE: %d, battery: %d\r\n", __LINE__, (int)hm10drv.getBatteryPercentage());
  
  if(hm10drv.setBaudRate(0))
    uartdrv.CreateAndSendData(USART3, (char *)"OK LINE: %d\r\n", __LINE__);
      
  if(hm10drv.setBLECharacteristic(0xFFE1))
    uartdrv.CreateAndSendData(USART3, (char *)"OK LINE: %d\r\n", __LINE__);
  
  readUSFromHM10 = hm10drv.getBLECharacteristic();
  uartdrv.CreateAndSendData(USART3, (char *)"OK LINE: %d and data: %X\r\n", __LINE__, readUSFromHM10);
  
  hm10drv.tryToConnectDeviceUseMAC((char *)"341513D9A655");
  if(hm10drv.isDeviceConnected())
    uartdrv.CreateAndSendData(USART3, (char *)"CONNECTION OCCURED\r\n");
  else
    uartdrv.CreateAndSendData(USART3, (char *)"NOT CONNECTED\r\n");
  
  if(hm10drv.clearLastConnectedDeviceAddress())
    uartdrv.CreateAndSendData(USART3, (char *)"Cleared Buffer\r\n");
    
  if(hm10drv.scanIBeaconDiscoveryScan())
    uartdrv.CreateAndSendData(USART3, (char *)"IBeacon Scan Succeed\r\n");
    
  memset(readDataFromHM10, 0, 64);
  hm10drv.getModuleName((char *)readDataFromHM10);
  uartdrv.CreateAndSendData(USART3, (char *)"NAME: %s\r\n", readDataFromHM10);
  
  if(hm10drv.setModuleName((char *)"dodo\x00"))
    uartdrv.CreateAndSendData(USART3, (char *)"NAME set ok\r\n");
      
  if(hm10drv.setPinCode(0))
    uartdrv.CreateAndSendData(USART3, (char *)"pin set ok\r\n");
  
  hm10drv.getPinCode(&readUIFromHM10);
  uartdrv.CreateAndSendData(USART3, (char *)"pin is: %d\r\n", readUIFromHM10);
  
  memset(readDataFromHM10, 0, 64);
  hm10drv.getModuleVersion((char *)readDataFromHM10);
  uartdrv.CreateAndSendData(USART3, (char *)"ver is: %s\r\n", readDataFromHM10);
  
  readUIFromHM10 = hm10drv.getModuleConnectRemoteDeviceTimeout();
  uartdrv.CreateAndSendData(USART3, (char *)"time is: %d\r\n", readUIFromHM10);
  
  if(hm10drv.setModuleConnectRemoteDeviceTimeout(0))
    uartdrv.CreateAndSendData(USART3, (char *)"time is set\r\n");*/
//END OF USAGE EXAMPLES OF THESE FUNCTIONS///////////////////////////




