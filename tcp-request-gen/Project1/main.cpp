#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

int main()
{
	WSADATA WSAData;
	SOCKET server;
	SOCKADDR_IN addr;

	WSAStartup(MAKEWORD(2, 0), &WSAData);
	server = socket(AF_INET, SOCK_STREAM, 0);

	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // replace the ip with your futur server ip address. If server AND client are running on the same computer, you can use the local ip 127.0.0.1
	addr.sin_family = AF_INET;
	addr.sin_port = htons(22018);

	connect(server, (SOCKADDR *)&addr, sizeof(addr));
	cout << "Connected to server!" << endl;


		std::string request = "{"
			"\"auth\":\"p4AzO|L1jMf=\","
			"\"request\":\"update_ips\","
			"\"ips\":{"
			"\"ip1\":\"144.12.123.11\","
				"\"ip2\":\"122.311.33.87\","
				"\"ip3\":\"91.122.31.633\""
		"},"
			"\"ip_amount\":3,"
			"\"node_replaced\":\"293.192.1.29\""
	"}";

		printf((request + " \n").c_str());

		char buffer[8192];
		strcpy_s(buffer, request.c_str());

		send(server, buffer, sizeof(buffer), 0);
		cout << "Message sent!" << endl;
		Sleep(1000);
	
		char message_buffer[8192];
		recv(server, message_buffer, sizeof(message_buffer), 0);

		std::string resp(message_buffer);
		printf((resp + "\n").c_str());

	closesocket(server);
	WSACleanup();
	cout << "Socket closed." << endl << endl;

	getchar();
}