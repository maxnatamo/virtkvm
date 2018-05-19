# virtkvm
A virtual KVM-switch. Utilizes Arduino to attach and detach devices from libvirt.

### Configure for hotkeys

You can build it as is to respond to a Arduino push button, or you can map it to a hotkey combination using the steps below.

1) Build project with `make hotkey`. Compiling like this will disable the arduino hotkey, and instead the server will
listen on a TCP port (Port 5555 on all networks by default)
2) It's recommended you specify a new IP address on your VM's subnet. Otherwise any device on your LAN can toggle your
mouse and keyboard. Add flags to the command make install command, like `sudo make install IP=10.1.1.10 PORT=56789`,
where 10.1.1.10 is your host's IP address on the network facing your VM, and PORT is an arbitrary number between 1024
and 65535.
3) Set up your client programs to point to the correct IP address your server is using. Even if you skipped the step
above and your server is running on all networks, your client still requires an IP address. If you skipped the step
above, the default port number is 5555.