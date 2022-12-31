
#include "commands/Command.hpp"


FileCommand::FileCommand(Server* server)
	:	Command(server)
{}

// format : FILE <nick>:<path_to_file>
FileCommand::~FileCommand() {}


void FileCommand::execute(Client* client, std::vector<std::string> arguments)
{
	if (arguments.size() < 2)
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "FILE"));
		return ;
	}

	std::string target = arguments.at(0);
	

}
