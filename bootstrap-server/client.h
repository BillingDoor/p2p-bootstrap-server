#pragma once
#ifndef _H_CLIENT_
#define _H_CLIENT_

#include <iostream>

#include <WinSock2.h>

#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

class c_client
{
public:
	struct params
	{
		SOCKET client_socket;
		SOCKADDR_IN client_info;
	};

	static unsigned __stdcall handle_session(void *data);

private:
	static bool is_valid_request(std::string request);
};

#endif