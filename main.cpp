#include <stdlib.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "rs232.h"
#include "misc.h"
#include "config.h"

#define BUFLEN 16

// Create .xml for libvirt.
void
CreateXMLFiles(void)
{
	system( ("mkdir " + cacheDir + " -p > /dev/null 2>&1").c_str() );
	for(int i = 0; i < devices.size(); i++)
	{
		Device dev = devices[i];
		std::ofstream f;
		f.open(cacheDir + dev.vendor + ".xml", std::ofstream::out);
		f << "<hostdev mode='subsystem' type='usb' managed='yes'><source><vendor id='0x" + std::string(dev.vendor) + "'/><product id='0x" + std::string(dev.product) + "'/></source></hostdev>";
		f.close();
	}
}

// Attach device.
void
AttachDevice( Device dev )
{
	system(("sudo virsh attach-device " + vmName + " --file " + cacheDir + dev.vendor + ".xml --live").c_str()); 
}

// Detach device.
void
DetachDevice( Device dev )
{
	system(("sudo virsh detach-device " + vmName + " --file " + cacheDir + dev.vendor + ".xml --live").c_str()); 
}

// Returns the state of the specified device, whether it is attached or not.
bool
CheckDeviceStatus( Device dev )
{
	std::string m_Result = popenCmd("sudo virsh dumpxml " + vmName + " | grep vendor");
	return m_Result.find( dev.vendor ) != std::string::npos;
}

// Toggle all devices. Devices are attached or detached symmetrically. They're either all attached or all dettached.
void
ToggleDevices ( )
{
	// We're gonna assume, that all devices has syncronized attachments - they're all attached or all detached.
	// Therefore, we're following the state of the first device in the list.

	bool state = CheckDeviceStatus( devices[0] );
	for(int i = 0; i < devices.size(); i++)
	{
		if(state) {
			DetachDevice( devices[i] );
		} else {
			AttachDevice( devices[i] );
		}
	}
}

int
main(int argc, char *argv[])
{
	int i = 0, size = 32;
	char mode[] = { '8', 'N', '1', '\0' };
	unsigned char buf[size];
	unsigned char ipaddr[] = {0,0,0,0};     // The address to listen on
    unsigned short port = 5555;            // The port to listen on


    if ( popenCmd ( "sudo virsh list --all | grep " + vmName ).find ( "shut off" ) != std::string::npos ) {
        for(int i = 0; i < argc; i++) {
            if(argv[i] == "--daemonize") {
                std::cout << "[*] ERROR: VM \'" + vmName + "\' is not running.";
                return 1;
            }
        }
    }
    clearScreen();
    std::cout << "VIRTUAL KVM" << std::endl;

	CreateXMLFiles();

    // TODO: Listen for argument here to specify IP address or network device to bind to, so only your VM can toggle
    // the KVM and not any rando on your WiFi

#ifdef watch_port
    // Commands to run if you want the virtkvm to respond to a keyboard hotkey
    int socket_info;
    struct sockaddr_in server;
    socklen_t servlen = sizeof(server);
    int recv_len;
    char msg[BUFLEN];

    // Get the correct IP address and port from the arguments
    for(int i = 0; i < 3; i++) {
        // Make a test string and put the first 2 characters of the argument in it
        char teststr[3];
        teststr[0] = argv[i][0];
        teststr[1] = argv[i][1];
        teststr[2] = 0;

        if(strcmp(teststr, "-p") == 0) {
            // If arg is a port, skip ahead to the actual number and parse it
            port = atoi((char*)(&argv[i][3]));
        } else if (strcmp(teststr, "-i") == 0) {
            // If arg is an ip address, split the octets into tokens and parse them individually
            char *octets = strtok((char*)(&argv[i][3]), ".");
            int j = 0;
            while (octets != NULL) {
                ipaddr[j++] = atoi(octets);
                octets = strtok(NULL, ".");
            }
        }
    }

    try {
        if ((socket_info = socket(AF_INET, SOCK_DGRAM, 0)) == 0) {
            perror("Socket failed");
            throw;
        }

        server.sin_addr.s_addr = htonl((int)(*ipaddr));
        server.sin_port = htons(port);
        server.sin_family = AF_INET;

        if (bind(socket_info, (struct sockaddr*)&server, sizeof(server)) == -1) {
            perror("Bind error");
            throw;
        }

        while(1) {
            if ((recv_len = recvfrom(socket_info, msg, BUFLEN, 0, (struct sockaddr*)&server, &servlen)) == -1) {
                perror("Error receiving");
                throw;
            }

            if (strcmp(msg, "toggle") == 0) {
                clearScreen();
                std::cout << "VIRTUAL KVM" << std::endl;

                ToggleDevices();
            }

        }

    } catch (char * err) { }

    return 0;

#else
	if(RS232_OpenComport(port, baudRate, mode))
	{
		std::cout << "Could not access serial port!\nDid you run as root?" << std::endl;
		return 1;
	}

	while(1)
	{
		int n = RS232_PollComport(port, buf, size - 1);
		if (n > 0) {
			buf[n] = 0;
			for(int i = 0; i < n; i++) {
				if(buf[i] < 32) {
					buf[i] = '.';
				}
			}
			i++;
		}
		if (i > 20) {
			clearScreen();
			std::cout << "VIRTUAL KVM" << std::endl;

			ToggleDevices();
			i = 0;
		}
	}
	return 0;
#endif
}
