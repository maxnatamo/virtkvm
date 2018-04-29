#include <iostream>
#include <string>

std::string popenCmd(std::string cmd) {
	std::string data;
	FILE * stream;
	const int max_buffer = 256;
	char buffer[max_buffer];

	cmd.append(" 2>&1");
	stream = popen(cmd.c_str(), "r");

	if( stream ) {
		while ( !feof(stream ) )
			if ( fgets ( buffer, max_buffer, stream ) != NULL ) data.append( buffer );
		pclose(stream);
	}
	return data;
}

bool replace (std::string& str, const std::string& a, const std::string& b)
{
	size_t start_pos = str.find(a);
	if(start_pos == std::string::npos)
		return false;
	str.replace(start_pos, a.length(), b);
	return true;
}

void clearScreen() { std::cout << "\033[2J\033[1;1H"; }
