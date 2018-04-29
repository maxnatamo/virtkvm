#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>

#include "rs232.h"
#include "misc.h"
#include "config.h"

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

	CreateXMLFiles();

	if(RS232_OpenComport(port, baudRate, mode))
	{
		std::cout << "Could not access serial port!\nDid you run as root?" << std::endl;
		return 1;
	}
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
}
