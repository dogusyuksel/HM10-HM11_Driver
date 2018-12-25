# HM10-HM11_Driver
This is a sketch to drive HM10/HM11 with AT commands. Body of the sketch is to search iBeacons around.


These are functions and their explanations to for AT Commands;

		
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
