#include <string>
#include <vector>

struct Device {
	char *vendor;
	char *product;
};

// The name of the VM. This is the name displayed in virt-manager or libvirt.
static const std::string 	vmName = "w10";

// The port of the incoming serial signal. Check https://www.teuniz.net/RS-232/ under 'List of comport numbers...' for more information.
static const unsigned short 	port = 24;

// The baudrate specified on the Arduino. This is usually 9600, but might differ.
static const unsigned short 	baudRate = 9600;

// The array of devices to control. Run lsusb to see IDs. Format: { "VENDOR", "PRODUCT" }. If the element is not the last, append a comma (,).
// Example: "ID 2341:8037 Arduino SA" -> { "2341", "8037" }
static const std::vector<Device> devices = {
	{ "2516", "001a" },
	{ "0738", "1709" }
};

// The cache directory. This will store .xml files for libvirt. Note: Don't use tilde (~), as the command is run as root. Write "/home/$USER/" or similar instead.
static const std::string	cacheDir = "/home/blue/.cache/virtkvm/";
