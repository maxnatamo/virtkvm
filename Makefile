# virtkvm - Simple virtual KVM switch, controlled via Arduino / serial ports.

CC=g++
CFLAGS=-I. -w

virtkvm:
	$(CC) main.cpp rs232.c misc.cpp -o virtkvm $(CFLAGS)

hotkey:
	$(CC) main.cpp rs232.c misc.cpp -o virtkvm -Dwatch_port $(CFLAGS)
	echo "[Unit]" > etc/virtkvm_alt.service
	echo "Description=Virtual KVM" >> etc/virtkvm_alt.service
	echo "" >> etc/virtkvm_alt.service
	echo "[Service]" >> etc/virtkvm_alt.service
	echo "ExecStart=/usr/local/bin/virtkvm -p=$PORT -i=$IP --daemon" >> etc/virtkvm_alt.service
	echo "[Install]" >> etc/virtkvm_alt.service
	echo "WantedBy=multi-user.target" >> etc/virtkvm_alt.service

clean:
	rm virtkvm -f

install:
	cp -f virtkvm /usr/local/bin
	cp -f etc/virtkvm.service /etc/systemd/system/
	chmod 755 /usr/local/bin/virtkvm

install_hotkey:
	cp -f virtkvm /usr/local/bin
	cp -f etc/virtkvm_alt.service /etc/systemd/system/virtkvm.service
	chmod 755 /usr/local/bin/virtkvm

uninstall:
	rm -f /usr/local/bin/virtkvm
	rm -f /etc/systemd/system/virtkvm.service
