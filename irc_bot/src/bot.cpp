#include "bot.h"


namespace irc
{
	bot::bot(const std::string& host, const std::string& port, const std::string& password, const std::string& name, const std::string& nick)
		:	_host(host),
			_port(port),
			_password(password),
			_name(name),
			_nick(nick)
	{
		int sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0)
			throw std::runtime_error("Error while opening socket.");

		struct sockaddr_in serv_address = {};

		// Clear address structure, should prevent some segmentation fault and artifacts
		bzero((char *) &serv_address, sizeof(serv_address));

		/*
	 	 * htons() convert unsigned short int to big-endian network byte order as expected from TCP protocol standards
	 	 */
	 	serv_address.sin_family = AF_INET;
    	serv_address.sin_port = htons(std::stoi(_port));
	 	if (inet_pton(AF_INET, "127.0.0.1", &serv_address.sin_addr)
        <= 0) {
        throw std::runtime_error(
            "Invalid address/ Address not supported");
    }
		//serv_address.sin_family = AF_INET;
		//serv_address.sin_addr.s_addr = INADDR_ANY;
		//serv_address.sin_port = htons(std::stoi(_port));

		// Bind the socket to the current IP address on selected port
		if (::connect(sockfd, (struct sockaddr *) &serv_address, sizeof(serv_address)) < 0)
			throw std::runtime_error("Error while connecting socket.");

		_sock = sockfd;
	}
	void bot::connect()
	{
		flush("PASS " + _password);
		flush("NICK " + _nick);
		flush("USER " + _nick + " " + _host + " " + _host + " " + _name);
	}
	void bot::flush(const std::string& message)
	{
		std::string normalized = message + "\r\n";
		send(_sock, normalized.c_str(), normalized.size(), 0);
	}
	void bot::recieve()
	{
		char buffer[256];
		int length;

		while ((length = recv(_sock, buffer, 256, 0)) > 0)
		{
			buffer[length] = 0;
			commandHandler(std::string(buffer, length));
		}
		close(_sock);
	}
	void bot::commandHandler(const std::string& cmdline)
	{
		std::cout << "commandHandler()\n";
		typedef std::vector<std::string> param_type;
		param_type args;
		std::string arg;
		std::stringstream ss(cmdline);
		while(std::getline(ss, arg, ' '))
			args.push_back(arg);
		if (args.size() < 4)
			return ;
		std::string source = args.at(0);
		std::string cmd = args.at(1);
		commandReply(source, cmd, args);
	}

	void bot::sendMessage(const std::string &source, const std::string &message)
	{
		flush("PRIVMSG " + source + " :" + message);
	}


	void bot::sendFile(const std::string& source, const std::string& filename, const std::string& nick)
	{
		std::ifstream is(filename);
		
		std::string str((std::istreambuf_iterator<char>(is)),
                 std::istreambuf_iterator<char>());
		std::stringstream ss;
		ss << "PRIVMSG " + source + " :" + "\x01" + "DCC SEND " + nick + " 0 " + _port + " ";
		ss << str.size();
		ss << "\x01";
		flush(ss.str());
	}
	void bot::commandReply(const std::string& source, const std::string& cmd, const std::vector<std::string>& args)
	{
		std::string nickname = source;
		std::string::size_type pos = nickname.find('!');
		if (pos != std::string::npos)
			nickname = nickname.substr(1, pos - 1);
		std::cout << cmd << "\n";
		if (cmd == "PRIVMSG")
		{
			std::string msg = args.at(3).substr(1);
			std::cout << msg << "\n";
			std::cout << "op: " << _selects[nickname] << "\n";
			if (msg == "bot")
			{
				_options[nickname] = "bot";
				_selects[nickname] = UNDEFINED;
				sendMessage(nickname, "Select an option");
				sleep(1);
				sendMessage(nickname, "1. Radian to degree");
				sleep(1);
				sendMessage(nickname, "2. Degree to radian");
				sleep(1);
				sendMessage(nickname, "3. Decimal to binary");
				sleep(1);
				sendMessage(nickname, "4. Send file");

			}
			else if (!msg.empty() && _options[nickname] == "bot" && _selects[nickname] == UNDEFINED)
			{
				int option = atoi(msg.c_str());
				switch (option)
				{
					case RADTODEG:
					{
						_selects[nickname] = RADTODEG;
						sendMessage(nickname, "Radian");
						break;
					}
					case DEGTORAD:
					{
						_selects[nickname] = DEGTORAD;
						sendMessage(nickname, "Degree");
						break;
					}
					case DECTOBIN:
					{
						_selects[nickname] = DECTOBIN;
						sendMessage(nickname, "Decimal");
						break;
					}
					case SEND:
					{
						_selects[nickname] = SEND;
						sendMessage(nickname, "Send file");
						break;
					}
					default:
					{
						sendMessage(nickname, "No such option");
						break;
					}
				}
			}
			else if (!msg.empty() && _options[nickname] == "bot" && _selects[nickname])
			{
				int option = _selects[nickname];
				switch(option)
				{
					case RADTODEG:
					{
						long double res = std::stod(msg) * 180.0 / M_PI;
						std::ostringstream ss;
						ss << res;
						sendMessage(nickname, ss.str());
						break;
					}
					case DEGTORAD:
					{
						long double res = std::stod(msg) *  M_PI / 180.0;
						std::ostringstream ss;
						ss << res;
						sendMessage(nickname, ss.str());
						break;
					}
					case DECTOBIN:
					{
						int res = std::stoi(msg);
						std::ostringstream ss;
						for (int i = 31; i >= 0; --i)
						{
        					int k = res >> i;
        					if (k & 1)
            					ss << "1";
        					else
            					ss << "0";
    					}
						sendMessage(nickname, ss.str());
						break;
					}
					case SEND:
					{
						sendFile(nickname, "./file.txt", "file.txt");
						sleep(1);
						sendFile(nickname, "./durex.txt", "durex.txt");
						break;
					}
					default:
					{
						sendMessage(nickname, "No such option");
						break;
					}
				}
			}
		}
	}
	void bot::run()
	{
		connect();
		
		recieve();
		std::string input;
		std::getline(std::cin, input);
		while (input != "exit")
			std::getline(std::cin, input);
		flush("QUIT :goodbye");
	}

}
