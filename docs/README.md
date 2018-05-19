# virtkvm
A virtual KVM-switch. Utilizes Arduino to attach and detach devices from libvirt.

### Configure for hotkeys

You can build it as is to respond to a Arduino push button, or you can map it to a hotkey combination using the steps below.

1) Build project with `make hotkey PORT=1234 IP=10.0.0.12`. Port is arbitrary, but IP should be your host's IP address
on the network facing your VM. If you are unsure, using 0.0.0.0 here will always work, but is less secure.
2) Configure the client programs on your host and guest machines to use this port and ip address