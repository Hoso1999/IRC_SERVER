#ifndef BOT_H
#define BOT_H

#include <string>
#include <sstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <cmath>
#define MAX_CONNECTIONS 1000


namespace irc
{
	class bot
	{
		int									_sock;
		const std::string					_host;
		const std::string					_port;
		const std::string					_password;
		std::string 						_name;
		std::string 						_nick;
		std::map<std::string, std::string>	_options;
		std::map<std::string, int>			_selects;

		public:
			enum commands
			{
				UNDEFINED,
				RADTODEG,
				DEGTORAD,
				DECTOBIN,
				SEND
			};
			bot(const std::string& host = "127.0.0.1", const std::string& port = "80", const std::string& password = "pass", const std::string& name = "irc_bot", const std::string& nick = "irc_bot" );
			void connect();
			void run();
			void flush(const std::string& msg);
			void commandReply(const std::string& source, const std::string& cmd, const std::vector<std::string>& args);
			void sendMessage(const std::string& source, const std::string& cmd);
			void sendFile(const std::string& source, const std::string& filename, const std::string& nick);
			void commandHandler(const std::string& cmdline);
			void recieve();
	};
}
	




#endif
