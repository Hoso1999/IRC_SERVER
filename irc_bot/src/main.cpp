#include "bot.h"



int main(int argc, char **argv) try
{
	if (argc != 5)
		throw std::logic_error("./bot [host] [port] [password] [nick]");
	irc::bot bot(argv[1], argv[2], argv[3], argv[4], argv[4]);
	bot.run();
}
catch(std::exception& ex)
{
	std::cerr << ex.what() << "\n";
}
