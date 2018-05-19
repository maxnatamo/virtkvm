#!/usr/bin/env python3
import socket

# This file connects to the server at the specified port and ip address. After connecting, this program sends the
# command "toggle" and immediately disconnects and terminates. It's reccomeneded to map a hotkey to run this program,
# and then that hotkey would appear to be a kvm toggle switch

HOST = "192.168.122.1"
PORT = 5555

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.connect((HOST, PORT))
s.sendall(b'toggle')
s.close()