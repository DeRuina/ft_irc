#ifndef SERVER_H
#define SERVER_H

#include "Client.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <csignal>
#include <cstring>

#define RED "\e[1;31m"
#define WHITE "\e[0;37m"
#define GREEN "\e[1;32m"
#define YELLOW "\e[1;33m"

class Server
{
	private:
		int port;
		const std::string password;
		int server_socket;
		static bool signal;
		std::vector<Client> clients;
		std::vector<struct pollfd> fds;


	public:
		Server(int port, const std::string &password);
		
		void create_server_socket();
		void server_init();
		void close_fds();


};

#endif