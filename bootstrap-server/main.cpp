#define WIN32_LEAN_AND_MEAN
#define _WINSOCKAPI_

#include <WinSock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include <stdio.h>
#include <process.h>

#pragma comment(lib, "Ws2_32.lib")

#define PORT "22018"

#include "logger.h"
#include "main.h"
#include "client.h"

std::vector<std::string> c_main::bot_ips;
std::vector<std::string> c_main::node_ips;

int main()
{
	WSADATA wsa_data;

	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
	{
		c_logger::error("WSAStartup failed.");
	}
	else
	{
		struct addrinfo *result = NULL, *ptr = NULL, hints;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		if (getaddrinfo(NULL, PORT, &hints, &result) != 0)
		{
			c_logger::error("getaddrinfo failed.");
			WSACleanup();
		}
		else
		{
			SOCKET listen_socket = INVALID_SOCKET;
			listen_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

			if (listen_socket == INVALID_SOCKET)
			{
				c_logger::error("socket().");
				freeaddrinfo(result);
				WSACleanup();
			}
			else
			{
				if (bind(listen_socket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR)
				{
					c_logger::error("bind() failed with: " + WSAGetLastError());
					freeaddrinfo(result);
					closesocket(listen_socket);
					WSACleanup();
				}
				else
				{
					freeaddrinfo(result); // no longer needed

					if (listen(listen_socket, SOMAXCONN) == SOCKET_ERROR)
					{
						c_logger::error("Listen failed with: " + WSAGetLastError());
						closesocket(listen_socket);
						WSACleanup();
					}
					else
					{
						SOCKET client_socket = INVALID_SOCKET;
						SOCKADDR_IN client_address;

						int client_address_size = sizeof(client_address);
						while (client_socket = accept(listen_socket, (SOCKADDR*)&client_address, &client_address_size))
						{
							c_client::params param;

							param.client_info = client_address;
							param.client_socket = client_socket;

							unsigned threadID;
							HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, &c_client::handle_session, (void*)&param, 0, &threadID);
						}
					}
				}
			}
		}
	}

	return 0;
}
