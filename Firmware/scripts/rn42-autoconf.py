#!/usr/bin/python

import sys
import time
from bluetooth import *

CMD_MODE_CMD =  '$$$';          #Command to enter command mode
CMD_MODE_RET =  'CMD\r\n'       #Response from RN42 upon entering command mode

BAUD_RATE_CMD = 'SU,921K\r'     #Command to set 921600 baud rate
CONFIG_CMD =    'ST,255\r'      #Command to set forever enable command mode locally and remotely
SNIFF_CMD =     'SW,0140\r'     #Command to set 200ms sniff mode
PAGESCAN_CMD =  'SJ,0600\r'     #Command to set 37.5% page scan
AUTHMODE_CMD =  'SA,2\r'        #Command to set authentication mode to 'just works'
NAME_CMD =      'S-,Cellulo\r'  #Command to set the friendly name to 'Cellulo-XXXX' with last 2 digits of MAC address auto-appended by device
RESET_CMD =     'R,1\r'         #Command to force reset

EXCHANGE_RET =  'AOK\r\n'       #Response from RN42 upon regular command

def printAndDie(message):
    print(message);
    sys.exit(-1);

def RN42Exchange(socket, preText, postText, command, expectedResponse):
    print(preText + ': ' + command);
    socket.send(command);
    response = socket.recv(len(expectedResponse));
    if(response == expectedResponse):
        print('Received: ' + response + postText + '.');
    else:
        socket.close();
        printAndDie('Received invalid response: ' + response);

def RN42Command(socket, preText, command):
    print(preText + ': ' + command);
    socket.send(command);
    time.sleep(1);

#Get command line arguments
if(len(sys.argv) <= 1):
    print('No arguments provided, scanning for devices...');
    devices = discover_devices(lookup_names = True);
    for addr, name in devices:
        print(addr + ' ' + name);
    print('To configure, run: rn42-autoconf.py XX:XX:XX:XX:XX:XX');
    sys.exit(0);
elif(sys.argv[1] == '--help' or sys.argv[1] == '-h'):
    print('Configures RN42 module remotely for Cellulo.');
    print('Usage: rn42-autoconf.py XX:XX:XX:XX:XX:XX');
    print('If no MAC address is specified, just scans for devices.');
    sys.exit(0);
addr = sys.argv[1];

#Connect to host
print('Connecting to the RN42 module at ' + addr + ' for autoconfigure...');
sock = BluetoothSocket(RFCOMM);
sock.setblocking(True);
try:
    sock.connect((addr,1));
except btcommon.BluetoothError as error:
    printAndDie('Caught BluetoothError: ' + str(error));

#Do all exchanges
RN42Exchange(sock, 'Entering command mode',     'Entered command mode',     CMD_MODE_CMD,   CMD_MODE_RET);
RN42Exchange(sock, 'Setting baud rate',         'Set baud rate',            BAUD_RATE_CMD,  EXCHANGE_RET);
RN42Exchange(sock, 'Enabling config forever',   'Enabled config forever',   CONFIG_CMD,     EXCHANGE_RET);
RN42Exchange(sock, 'Setting sniff mode',        'Set sniff mode',           SNIFF_CMD,      EXCHANGE_RET);
RN42Exchange(sock, 'Setting page scan window',  'Set page scan window',     PAGESCAN_CMD,   EXCHANGE_RET);
RN42Exchange(sock, 'Setting friendly name',     'Set friendly name',        NAME_CMD,       EXCHANGE_RET);

#This has to be the last SET command as the module stops responding after setting auth mode
RN42Exchange(sock, 'Setting auth mode',         'Set auth mode',            AUTHMODE_CMD,   EXCHANGE_RET);

#Reset module
RN42Command(sock,  'Resetting module',                                      RESET_CMD);

print('RN42 module at ' + addr + ' autoconfigured successfully. Now flash the firmare to the robot.');
sock.close();
