
import argparse
import codecs
import select
import sys
import time

import serial

import utils

VERSION = "0.0.1"

role = 'peripheral'

parser = argparse.ArgumentParser(
    prog='hm10_hm11_ctl',
    description='a control tool to derive hm10_hm11 ble devices',
    epilog='hm10_hm11_ctl Help')
parser.add_argument('-v', '--version',
                    action='store_true',
                    help='show version number and return')
parser.add_argument('-d', '--verbose',
                    action='store_true',
                    help='verbosity setting')
parser.add_argument('-i', '--interface',
                    action='store',
                    help='serial interface name')
parser.add_argument('-r', '--role',
                    action='store',
                    help='\"peripheral\" or \"central\" whould be typed')
parser.add_argument('-p', '--power',
                    action='store',
                    help='\"sleep\" or \"wakeup\" whould be typed')
parser.add_argument('-e', '--remotecontrol',
                    action='store_true',
                    help='enable remote AT command control')
parser.add_argument('-c', '--customcommand',
                    action='store',
                    help='to pass the argument directly to the module')
parser.add_argument('-m', '--setmac',
                    action='store',
                    help='set MAC address')
parser.add_argument('-a', '--setadv',
                    action='store',
                    help='set advertising value')
parser.add_argument('-w', '--setwhitemac',
                    action='store',
                    help='add the mac address into white list')
parser.add_argument('-l', '--listnumber',
                    action='store',
                    help='specify the list number (1-2-3) if setwhitemac is used')
parser.add_argument('-b', '--battmonitor',
                    action='store_true',
                    help='enable batter monitor')
parser.add_argument('-n', '--name',
                    action='store',
                    help='set module name, max len 12')
parser.add_argument('-x', '--connectmac',
                    action='store',
                    help='type mac to be connected, max len 12')
args = parser.parse_args()


def serial_open(port, wait_time):
    ser = serial.Serial(port, 9600, timeout=wait_time)  # open serial port
    return ser


def serial_close(ser):
    ser.close()  # close port
    return ser


def execute_string_command(ser, command, lenght):
    command_b = bytes(command, 'utf-8')
    ser.write(command_b)  # write a string
    response = ser.read(lenght)  # read up to lenght bytes (timeout)
    response = codecs.decode(response, encoding='utf-8')
    if args.verbose is True:
        print('RESPONSE:   ', response)
    return response


if args.version is True:
    print('version', VERSION)
    sys.exit(0)

u = utils.Utils(args)
if u.arg_check() is False:
    sys.exit(1)

serial_handle = serial_open(args.interface, 10)

print('EXECUTED:    enabled uart wakeup')
response = execute_string_command(serial_handle, 'AT+UART0', 8)

if args.power is None or args.power.__contains__('wakeup'):
    # you can send a long string (Length > 1024 or more),
    # that string can caused module wake up, and you will be received “OK+WAKE” string from module UART
    print('EXECUTED:    wake up from sleep')
    execute_string_command(serial_handle, 'I am iron man, I am iron man, I am iron man I am iron man, \
    I am iron man, I am iron man, I am iron man I am iron man, I am iron man, I am iron man, I am iron \
    man I am iron man, I am iron man, I am iron man, I am iron man I am iron manI am iron man, I am iron \
    man, I am iron man I am iron man, I am iron man, I am iron man, I am iron man I am iron manI am iron man, \
    I am iron man, I am iron man I am iron man, I am iron man, I am iron man, I am iron man I am iron mann \
    I am iron man, I am iron man, I am iron man I am iron man, I am iron man, I am iron man, I am iron man \
    I am iron manI am iron man, I am iron man, I am iron man I am iron man, I am iron man, I am iron man, \
    I am iron man I am iron manI am iron man, I am iron man, I am iron man I am iron man, I am iron man, \
    I am iron man, I am iron man I am iron manI am iron man, I am iron man, I am iron man I am iron man, \
    I am iron man, I am iron man, I am iron man I am iron manI am iron man, I am iron man, I am iron man \
    I am iron man, I am iron man, I am iron man, I am iron man I am iron manI am iron man, I am iron man, \
    I am iron man I am iron man, I am iron man, I am iron man, I am iron man I am iron manI am iron man, \
    I am iron man, I am iron man I am iron man, I am iron man, I am iron man, I am iron man I am iron man', 7)
else:
    print('EXECUTED:    going into sleep')
    execute_string_command(serial_handle, 'AT+SLEEP', 8)
    serial_close(serial_handle)
    exit(0)

print('EXECUTED:    query software version')
response = execute_string_command(serial_handle, 'AT+VERR?', 20)
print('INFO:        version ', response)

# use AT+BAUD[P1]  to set it where P1 is
# 0: 9600; 1: 19200;2: 38400;
# 3: 57600; 4: 115200; 5: 4800;
# 6: 2400; 7: 1200; 8: 230400;
# Default: 0(9600)
print('EXECUTED:    query baud rate')
response = execute_string_command(serial_handle, 'AT+BAUD?', 13)
interval = response.split(":")
if interval[1].__contains__('0'):
    print('INFO:        baud 9600')
elif interval[1].__contains__('1'):
    print('INFO:        baud 19200')
elif interval[1].__contains__('2'):
    print('INFO:        baud 38400')
elif interval[1].__contains__('3'):
    print('INFO:        baud 57600')
elif interval[1].__contains__('4'):
    print('INFO:        baud 115200')
elif interval[1].__contains__('5'):
    print('INFO:        baud 4800')
elif interval[1].__contains__('6'):
    print('INFO:        baud 2400')
elif interval[1].__contains__('7'):
    print('INFO:        baud 1200')
elif interval[1].__contains__('8'):
    print('INFO:        baud 230400')

print('EXECUTED:    get stop bit')
response = execute_string_command(serial_handle, 'AT+STOP?', 8)
interval = response.split(":")
if interval[1].__contains__('0'):
    print('INFO:        stop bit 1')
elif interval[1].__contains__('1'):
    print('INFO:        stop bit 2')

print('EXECUTED:    get parity')
response = execute_string_command(serial_handle, 'AT+PARI?', 8)
interval = response.split(":")
if interval[1].__contains__('0'):
    print('INFO:        parity NONE')
elif interval[1].__contains__('1'):
    print('INFO:        parity ODD')
elif interval[1].__contains__('2'):
    print('INFO:        parity EVEN')

# use this command AT+UUID[P1] to set it where P1: 0x0001~0xFFFE
print('EXECUTED:    get UUID')
response = execute_string_command(serial_handle, 'AT+UUID?', 13)
interval = response.split(":")
print('INFO:        UUID ', interval[1])

print('EXECUTED:    get temperature')
response = execute_string_command(serial_handle, 'AT+TEMP?', 14)
interval = response.split(":")
print('INFO:        temperature ', interval[1])

print('EXECUTED:    get last connected device mac')
response = execute_string_command(serial_handle, 'AT+RADD?', 20)
interval = response.split(":")
print('INFO:        last device mac ', interval[1])

# use this command AT+TYPE? to get it where P1: 0~2
# 0: Not need PIN Code
# 1: Auth not need PIN
# 2: Auth with PIN
# 3: Auth and bonded
# Default: 0
print('EXECUTED:    set bond mode for no PIN')
response = execute_string_command(serial_handle, 'AT+TYPE0', 8)

print('EXECUTED:    set for reliable advertise')
response = execute_string_command(serial_handle, 'AT+RELI1', 8)

print('EXECUTED:    do not auto sleep')
response = execute_string_command(serial_handle, 'AT+PWRM1', 8)

print('EXECUTED:    maximize power (6 dbm)')
response = execute_string_command(serial_handle, 'AT+POWE3', 8)
response = execute_string_command(serial_handle, 'AT+PCTL1', 8)

print('EXECUTED:    open rx gain')
response = execute_string_command(serial_handle, 'AT+GAIN1', 8)

print('EXECUTED:    enable notify information with address')
response = execute_string_command(serial_handle, 'AT+NOTI1', 8)
response = execute_string_command(serial_handle, 'AT+NOTP1', 8)

if args.name is not None:
    if len(args.name) > 12:
        print('Please be sure the name lenght must be less than 12')
        serial_close(serial_handle)
        exit(0)
    concat_command = "AT+NAME" + args.name
    print('EXECUTED:    setting name command: ', concat_command)
    execute_string_command(serial_handle, concat_command, 18)

print('EXECUTED:    query name')
response = execute_string_command(serial_handle, 'AT+NAME?', 18)
print('INFO:        name: ', response[8:])

# 1 will turn it on
print('EXECUTED:    turn off ibeacon')
response = execute_string_command(serial_handle, 'AT+IBEA0', 8)

if args.role is None or args.role.__contains__('peripheral'):
    # AT+ROLE0” is peripheral role
    print('EXECUTED:    peripheral role selected')
    execute_string_command(serial_handle, 'AT+ROLE0', 8)
else:
    # AT+ROLE1” is central role
    print('EXECUTED:    central role selected')
    execute_string_command(serial_handle, 'AT+ROLE1', 8)

# For central: In AT+IMME0 mode, when power on, module will check if have a device address
# have already success connect before. If have, module will try to connect that device
# for ever, if we setup AT+TCON, when AT+TCON value timeout, module will forget that
# device and try scan devices, then try connect the device what we scanned; If not have
# a device address, then module will try scan devices and try connect them. AT+SAVE
# command will let module know if save devices address or not.

# For peripheral: If AT+IMME0 is setup module will auto into discoverable mode.

# With IMME1: the device will be in AT configuration mode
# If the mode is central, In AT+IMME1 modem, when power on, module do nothing, module will wait your
# command, now you can use AT+DISC? Or AT+DISI? Or AT+DISA? Command to
# scan devices. Also, you can use AT+CO command connect to a device
print('EXECUTED:    we are in AT configuration mode')
execute_string_command(serial_handle, 'AT+IMME1', 8)

if args.customcommand is not None:
    # can be used for and AT commands just to see their outputs
    # eg: AT+ADC[P1]?  where P1: 3,4,5,6,7,8,9, A, B
    # eg: AT+ANCS? or AT+ANCSP1 where P1 is 0 for OFF and 1 for ON, apple ANCS switch defaultly OFF
    # eg: AT+ALLO? or AT+ALLOP1 where P1 is 0 for OFF and 1 for ON, white list switch defaultly OFF
    # eg: AT+RENEW for FACTORY RESET
    # eg: AT+MEAS? to get ibeacon measured power
    # eg: AT+MINO? or AT+MARJ? to get ibeacon minor marjor
    # eg AT+IBE0, AT+IBE1, AT+IBE2, AT+IBE3 are used to set ibeacon UUID (one part at a time)
    # eg: AT+ERASE to remove bond information
    # eg: AT+CHAR?  to read characteristic value (changeable). default: 0xFFE1
    print('EXECUTED:   ', args.customcommand)
    execute_string_command(serial_handle, args.customcommand, 100)
    serial_close(serial_handle)
    exit(0)

# Mode 0(Transmission mode, command string: “AT+MODE0”):
# Before connect, you can configure module parameters with AT Commands
# After connect, you can send and receive Bluetooth data through UART

# Mode 2 (Remote control mode command string: “AT+MODE2”):
# Before connect, you can configure module parameters with AT Commands
# After connect:
# 1) Send and receive Bluetooth data through UART.
# 2) Remote device could configure module parameters with AT Commands
# 3) Remote device could control PIO2~11 output low or high
# HM-11 only has PIO2 and PIO3.
if args.remotecontrol is True:
    print('EXECUTED:    remote control mode selected')
    execute_string_command(serial_handle, 'AT+MODE2', 8)
else:
    print('EXECUTED:    transmission mode selected')
    execute_string_command(serial_handle, 'AT+MODE0', 8)

# P1: 0 ~ 3
# 0: Advertising
#  ScanResponse, Connectable
# 1: Only allow last device connect
# in 1.28 seconds
# 2: Only allow Advertising and
# ScanResponse.
# 3: Only allow Advertising
print('EXECUTED:    setting advertising type')
execute_string_command(serial_handle, 'AT+ADTY0', 8)

# If module is in sleep mode, module will output “OK+WAKE” and into standby mode.
# If Module is not connected to remote device, you will receive: “OK”
# If Module is connected, module will be disconnected from remote device, if “AT +
# NOTI” is setup to 1, module will output “OK+LOST” through UART.
print('EXECUTED:    test')
response = execute_string_command(serial_handle, 'AT', 7)
if response.__contains__('OK+LOST'):
    print('INFO:        disconnected and ready')
elif response.__contains__('OK+WAKE'):
    print('INFO:        wakeup and ready')
elif response.__contains__('OK'):
    print('INFO:        not connected and ready')

if args.setmac is not None:
    if len(args.setmac) != 12:
        print('Please be sure the MAC address lenght must be 12')
        serial_close(serial_handle)
        exit(0)
    concat_command = "AT+ADDR" + args.setmac
    print('EXECUTED:    setting mac command: ', concat_command)
    execute_string_command(serial_handle, concat_command, 24)

# receive format "OK+ADDR:MAC Address"
print('EXECUTED:    address query')
response = execute_string_command(serial_handle, 'AT+ADDR?', 20)
mac = response.split(":")
print('INFO:        MAC: ', mac[1])

# receive format "OK+Get:[P1]" where P1 is
# P1: 0 ~ F
# 0: 100ms; 1: 252.5ms;
# 2: 211.25ms; 3:318.75ms
# 4: 417.50ms; 5:546.25ms
# 6: 760.00ms; 7:852,50ms;
# 8: 1022.5ms; 9:1285ms
# A: 2000ms; B: 3000ms
# C: 4000ms; D: 5000ms
# E: 6000ms; F: 7000ms
# HMSoft Default: 0
# HMSensor Default: 9
if args.verbose is True:
    print('INFO:        0 means 100ms')
    print('INFO:        1 means 252.5ms')
    print('INFO:        2 means 211.25ms')
    print('INFO:        3 means 318.75ms')
    print('INFO:        4 means 417.50ms')
    print('INFO:        5 means 546.25ms')
    print('INFO:        6 means 760.00ms')
    print('INFO:        7 means 852,50ms')
    print('INFO:        8 means 1022.5ms')
    print('INFO:        9 means 1285ms')
    print('INFO:        A means 2000ms')
    print('INFO:        B means 3000ms')
    print('INFO:        C means 4000ms')
    print('INFO:        D means 5000ms')
    print('INFO:        E means 6000ms')
    print('INFO:        F means 7000ms')
    print('INFO:        The maximum 1285ms recommendations from the IOS system')

if args.setadv is not None:
    if len(args.setadv) != 1:
        print('Please be sure the data lenght must be 1')
        serial_close(serial_handle)
        exit(0)
    concat_command = "AT+ADVI" + args.setadv
    print('EXECUTED:    setting advertise interval: ', concat_command)
    execute_string_command(serial_handle, concat_command, 8)

print('EXECUTED:    advertising interval query')
response = execute_string_command(serial_handle, 'AT+ADVI?', 8)
interval = response.split(":")
print('INFO:        interval: ', interval[1])

if args.setwhitemac is not None:
    if len(args.setwhitemac) != 12:
        print('Please be sure the mac address len')
        serial_close(serial_handle)
        exit(0)
    if args.listnumber is None:
        print('Please be sure the mac address len')
        serial_close(serial_handle)
        exit(0)
    if len(args.listnumber) != 1:
        print('Please be sure list number must be 1, 2 or 3')
        serial_close(serial_handle)
        exit(0)
    concat_command = "AT+AD" + args.listnumber + args.setwhitemac
    print('EXECUTED:    white list adding mac: ', concat_command)
    execute_string_command(serial_handle, concat_command, 19)

print('EXECUTED:    query white lists')
response = execute_string_command(serial_handle, 'AT+AD1??', 20)
print('INFO:        list 1: ', response)
response = execute_string_command(serial_handle, 'AT+AD2??', 20)
print('INFO:        list 2: ', response)
response = execute_string_command(serial_handle, 'AT+AD3??', 20)
print('INFO:        list 3: ', response)

if args.battmonitor is True:
    # it means it is enabled
    print('EXECUTED:    added battery info into scan response')
    execute_string_command(serial_handle, 'AT+BATC1', 8)
else:
    print('EXECUTED:    NOT added battery info into scan response')
    execute_string_command(serial_handle, 'AT+BATC0', 8)

print('EXECUTED:    query battery monitor')
response = execute_string_command(serial_handle, 'AT+BATC?', 8)
interval = response.split(":")
print('INFO:        enabled: ', interval[1])

print('EXECUTED:    query battery level')
response = execute_string_command(serial_handle, 'AT+BATT?', 10)
interval = response.split(":")
print('INFO:        battery %', interval[1])

# to set it use AT+COMI[P1] command
# where P1 is P1: 0 ~ 9
# 0: 7.5ms; 1: 10ms; 2: 15ms;
# 3: 20ms; 4: 25ms; 5: 30ms;
# 6: 35ms; 7: 40ms; 8: 45ms;
# 9: 4000ms;
# Default: 3(20ms)
print('EXECUTED:    query min Link Layer Connection Interval')
response = execute_string_command(serial_handle, 'AT+COMI?', 8)
interval = response.split(":")
if interval[1].__contains__('0'):
    print('INFO:        interval 7.5ms')
elif interval[1].__contains__('1'):
    print('INFO:        interval 10ms')
elif interval[1].__contains__('2'):
    print('INFO:        interval 15ms')
elif interval[1].__contains__('3'):
    print('INFO:        interval 20ms')
elif interval[1].__contains__('4'):
    print('INFO:        interval 25ms')
elif interval[1].__contains__('5'):
    print('INFO:        interval 30ms')
elif interval[1].__contains__('6'):
    print('INFO:        interval 35ms')
elif interval[1].__contains__('7'):
    print('INFO:        interval 40ms')
elif interval[1].__contains__('8'):
    print('INFO:        interval 45ms')
elif interval[1].__contains__('9'):
    print('INFO:        interval 4000ms')

# to set it use AT+COMA[P1] command
# where P1 is P1: 0 ~ 9
# 0: 7.5ms; 1: 10ms; 2: 15ms;
# 3: 20ms; 4: 25ms; 5: 30ms;
# 6: 35ms; 7: 40ms; 8: 45ms;
# 9: 4000ms;
# Default: 7(40ms)
print('EXECUTED:    query max Link Layer Connection Interval')
response = execute_string_command(serial_handle, 'AT+COMA?', 8)
interval = response.split(":")
if interval[1].__contains__('0'):
    print('INFO:        interval 7.5ms')
elif interval[1].__contains__('1'):
    print('INFO:        interval 10ms')
elif interval[1].__contains__('2'):
    print('INFO:        interval 15ms')
elif interval[1].__contains__('3'):
    print('INFO:        interval 20ms')
elif interval[1].__contains__('4'):
    print('INFO:        interval 25ms')
elif interval[1].__contains__('5'):
    print('INFO:        interval 30ms')
elif interval[1].__contains__('6'):
    print('INFO:        interval 35ms')
elif interval[1].__contains__('7'):
    print('INFO:        interval 40ms')
elif interval[1].__contains__('8'):
    print('INFO:        interval 45ms')
elif interval[1].__contains__('9'):
    print('INFO:        interval 4000ms')

print('EXECUTED:    query supervision timeout')
response = execute_string_command(serial_handle, 'AT+COSU?', 8)
interval = response.split(":")
if interval[1].__contains__('0'):
    print('INFO:        100ms')
elif interval[1].__contains__('1'):
    print('INFO:        1000ms')
elif interval[1].__contains__('2'):
    print('INFO:        2000ms')
elif interval[1].__contains__('3'):
    print('INFO:        3000ms')
elif interval[1].__contains__('4'):
    print('INFO:        4000ms')
elif interval[1].__contains__('5'):
    print('INFO:        5000ms')
elif interval[1].__contains__('6'):
    print('INFO:        6000ms')

print('EXECUTED:    query  Link Layer connection slave latency')
response = execute_string_command(serial_handle, 'AT+COLA?', 8)
interval = response.split(":")
print('INFO:        latency ', interval[1])

if args.role is not None and args.role.__contains__('central'):
    # now you can use AT+DISC? Or AT+DISI? Or AT+DISA? Command to
    # scan devices. Also, you can use AT+CO command connect to a device.

    # where P1 value: 000000~999999
    print('EXECUTED:    connect forever')
    response = execute_string_command(serial_handle, 'AT+TCON000000', 13)

    # 0: none, 1: only name, 2: only RSSI
    print('EXECUTED:    show RSSI and Name at DISC result')
    response = execute_string_command(serial_handle, 'AT+SHOW3', 8)

    # P1: 1 ~ 9  in seconds
    print('EXECUTED:    set module discovery time')
    response = execute_string_command(serial_handle, 'AT+SCAN9', 8)

    # 0 means save
    print('EXECUTED:    do not save connected device mac address')
    response = execute_string_command(serial_handle, 'AT+SAVE1', 8)

    # 0 means save
    print('EXECUTED:    Clear Last Connected device address')
    response = execute_string_command(serial_handle, 'AT+CLEAR', 8)

    # start central here
    execute_string_command(serial_handle, 'AT+START', 8)
    print('EXECUTED:    START executed')

    print('EXECUTED:    Start a device discovery scan')
    response = execute_string_command(serial_handle, 'AT+DISC?', 5000)
    print('INFO:        response', response)

    if args.connectmac is not None:
        if len(args.connectmac) != 12:
            print('Please be sure the MAC address lenght must be 12')
            serial_close(serial_handle)
            exit(0)
        concat_command = "AT+CON" + args.connectmac
        print('EXECUTED:    setting mac command: ', concat_command)
        response = execute_string_command(serial_handle, concat_command, 32)
        if response.__contains__('OK+CONNA'):
            print('INFO:        Accept request, connecting')
            time.sleep(5)
            print('EXECUTED:    get RSSI')
            response = execute_string_command(serial_handle, 'AT+RSSI?', 20)
            print('INFO:        RSSI', response)
        if response.__contains__('OK+CONNE'):
            print('INFO:        Connect error')
        if response.__contains__('OK+CONNF'):
            print('INFO:        Connect failed')

else:
    execute_string_command(serial_handle, 'AT+START', 8)
    print('EXECUTED:    START executed')

serial_close(serial_handle)

serial_handle = serial_open(args.interface, 1)

print('INFO:        start listening...')


def inp_ready(line):
    user_inp_b = bytes(line, 'utf-8')
    serial_handle.write(user_inp_b)


def check_serial():
    response = serial_handle.read(1)
    response = codecs.decode(response, encoding='utf-8')
    return response


t = time.time()
last_packet_get_time = int(t * 1000)
cumulative_packet_content = ""

condition = True
while condition is True:
    # If there's input ready, do inp_ready, else do check_serial. Note timeout is zero so select won't block at all.
    while sys.stdin in select.select([sys.stdin], [], [], 0)[0]:
        line = sys.stdin.readline()
        if line:
            if line.__contains__('exit()'):
                condition = False
            else:
                inp_ready(line)

    resp = check_serial()
    if resp:
        t = time.time()
        last_packet_get_time = int(t * 1000)
        cumulative_packet_content = cumulative_packet_content + resp
    else:
        t = time.time()
        current_ms = int(t * 1000)
        time_diff = current_ms - last_packet_get_time
        if time_diff > 1000:
            if cumulative_packet_content:
                print(cumulative_packet_content)
            cumulative_packet_content = ""
            last_packet_get_time = current_ms

serial_close(serial_handle)
