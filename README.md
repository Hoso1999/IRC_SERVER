# IRC Server

## Description

This project is an IRC server implemented in C++, which supports various IRC commands such as JOIN, KICK, MODE, NICK, NOTICE, PART, PASS, PING, PONG, PRIVMSG, QUIT, and USER. In addition to the IRC server, the project also includes an FTP server and a bot application.

## Features

- IRC server implementation with support for the following commands:
  - JOIN: Allows users to join channels.
  - KICK: Allows channel operators to kick users from a channel.
  - MODE: Allows users to change channel modes or user modes.
  - NICK: Allows users to change their nicknames.
  - NOTICE: Allows users to send notices to other users or channels.
  - PART: Allows users to leave channels.
  - PASS: Allows users to set a password for authentication.
  - PING: Allows users to check if the server is still active.
  - PONG: Responds to PING commands from users.
  - PRIVMSG: Allows users to send private messages to other users or channels.
  - QUIT: Allows users to disconnect from the server gracefully.
  - USER: Allows users to specify their username and other details when connecting.

- FTP server implementation: Provides basic FTP functionality for file transfer.

- Bot application: Includes a bot that performs automated tasks or responds to specific commands.

## Usage
Clone the repository:

```bash
git clone https://github.com/Hoso1999/IRC_SERVER.git && cd IRC_SERVER
```
### Build project
```bash
cmake -B build
```
### Run Server
```bash
cd build && ./ft_irc <PORT> <PASSWORD>
```
### How to connect
You can connect to server with netcat or custom client application, but you must register the client.
#### Example
```bash
hohayrap@hohayrap-GF63-Thin-11UD:~$ nc localhost 6666
PASS pass
NICK nickname
USER nickname ignore ignore real_name
```
If you get output like this it means that client is connected
```bash
:nickname!nickname@localhost 001 nickname :Welcome nickname to the ft_irc network
```
#### Example of Channels
Client 1
```bash
hohayrap@hohayrap-GF63-Thin-11UD:~$ nc localhost 6666
PASS pass
NICK hello
USER hello ignore ignore hello
:hello!hello@localhost 001 hello :Welcome hello to the ft_irc network
JOIN chan
:hello!hello@localhost 353 hello = chan :@hello 
:hello!hello@localhost 366 hello chan :End of /NAMES list.
:hello!hello@localhost JOIN :chan
:nickname!nickname@localhost JOIN :chan
PRIVMSG #chan :hi all
```
Client 2
```bash
hohayrap@hohayrap-GF63-Thin-11UD:~$ nc localhost 6666
PASS pass
NICK nickname
USER nickname ignore ignore real_name
:nickname!nickname@localhost 001 nickname :Welcome nickname to the ft_irc network
JOIN chan
:nickname!nickname@localhost 353 nickname = chan :@hello nickname 
:nickname!nickname@localhost 366 nickname chan :End of /NAMES list.
:nickname!nickname@localhost JOIN :chan
:hello!hello@localhost PRIVMSG #chan :hi all 
:nick!nick@localhost JOIN :chan
```
## Contributing

Contributions to Minishell are welcome!.

Before submitting a pull request, please ensure that you follow these guidelines:

- Describe the changes you've made in detail.
- Test your changes thoroughly.
- Follow the existing coding style and conventions.
- Include relevant documentation updates if necessary.
