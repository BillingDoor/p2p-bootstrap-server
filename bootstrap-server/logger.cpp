#pragma once

#include "logger.h"

std::string c_logger::prefix = "[~] =";

void c_logger::log(const std::string& message, bool time)
{
#ifndef NDEBUG
	std::cout << (prefix + " " + message).c_str() << std::endl;
#endif
}

void c_logger::error(const  std::string& message)
{
#ifndef NDEBUG
	std::cout << ("[~] [error] = " + message).c_str() << std::endl;
#endif
}