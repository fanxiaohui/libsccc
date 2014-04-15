/*
 * misc.cpp
 * Misc util
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <syscall.h>
#include <cstdint>

#include <libsc/com/uart_device.h>

namespace libutil
{

namespace
{

libsc::UartDevice *g_uart = nullptr;

int MyFwriteHandler(int, char *ptr, int len)
{
	if (g_uart)
	{
		g_uart->SendBuffer(reinterpret_cast<const uint8_t*>(ptr), len);
	}
	return len;
}

}

void InitDefaultFwriteHandler(libsc::UartDevice *uart)
{
	g_uart = uart;
	__g_fwrite_handler = MyFwriteHandler;
}

void UninitDefaultFwriteHandler()
{
	g_uart = nullptr;
	__g_fwrite_handler = nullptr;
}

}