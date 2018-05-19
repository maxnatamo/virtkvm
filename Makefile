# virtkvm - Simple virtual KVM switch, controlled via Arduino / serial ports.

CC=g++
CFLAGS=-I. -w

virtkvm:
	$(CC) main.cpp rs232.c misc.cpp -o virtkvm $(CFLAGS)

hotkey:
	$(CC) main.cpp rs232.c misc.cpp -o virtkvm -Dwatch_port $(CFLAGS)

clean:
	rm virtkvm -f

install:
	cp -f virtkvm /usr/local/bin
	chmod 755 /usr/local/bin/virtkvm
	if [ ${IP} ]; then \
		if [ ! ${PORT} ]; then "PORT not defined"; exit; fi; \
		export PORT=${PORT}; \
		export IP=${IP}; \
		bash add_args_to_service.sh > /etc/systemd/system/virtkvm.service; \
	else \
		cp -f etc/virtkvm.service /etc/systemd/system/; \
	fi

uninstall:
	rm -f /usr/local/bin/virtkvm
	rm -f /etc/systemd/system/virtkvm.service