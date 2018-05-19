IFS='='
while read line; do
	read -r -a array <<< "$line";
	if [ "${array[0]}" = "ExecStart" ]; then
		echo -ne "$line -p=$PORT -i=$IP\n";
	else
		echo -ne "$line\n"
	fi
done < etc/virtkvm.service
