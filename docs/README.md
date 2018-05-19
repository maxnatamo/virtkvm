# virtkvm
A virtual KVM-switch. Utilizes Arduino to attach and detach devices from libvirt.

### Configure for hotkeys

You can build it as-is to respond to a Arduino push button, or you can map it to a hotkey combination using the steps below.

1) Build project with `make hotkey`. Compiling like this will disable the arduino hotkey, and instead the server will
listen on a TCP port (Port 5555 on all networks by default)
2) Open virtkvm_client.py and edit the HOST and PORT lines. Port can be any arbitary port, but it's recommended you
change to the IP address on your host-vm subnet. Leaving the IP address as 0.0.0.0 means anyone on your LAN can toggle
your mouse and keyboard.
3) Using the IP address and port above, add flags to the command make install command, like
`sudo make install IP=10.1.1.10 PORT=56789`, where 10.1.1.10 is your host's IP address on the network facing your VM.
4) The installation automatically copies the client file on your host. You can run it by typing virtkvm_client.py in a
bash shell. See documentation for your specific desktop environment for how to set up a hotkey.
5) You will have to manually copy the script to your guest. If you put a shortcut to the script on a Windows desktop,
you can right click the icon, select properties, and under shortcut there will be an option to map a hotkey. You will
also need to install python on your windows machine for it to work. A java alternative should be easy to make, but I'm
having issues.