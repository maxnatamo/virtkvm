# virtkvm - Simple virtual KVM switch, controlled via Arduino / serial ports.

CC=g++
CFLAGS=-I. -w

virtkvm:
	$(CC) main.cpp rs232.c misc.cpp -o virtkvm $(CFLAGS)

clean:
	rm virtkvm -f

install:
	cp -f virtkvm /usr/local/bin
	chmod 755 /usr/local/bin/virtkvm

uninstall:
	rm -f /usr/local/bin/virtkvm
