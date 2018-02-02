/*
 * jy_mcu_bt_106.cpp
 * JY-MCU BT Board v1.06
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cstdint>

#include "libbase/k60/pin.h"
#include "libbase/k60/uart.h"

#include "libsc/config.h"
#include "libsc/k60/dk100.h"

using namespace libbase::k60;

namespace libsc
{
namespace k60
{

#ifdef LIBSC_USE_UART

Uart::Config Dk100::Initializer::GetUartConfig() const
{
	Uart::Config product = UartDevice::Initializer::GetUartConfig();
	// On this board, there's a diode connected to the Tx pin, preventing the
	// module to correctly send data to the MCU
	product.baud_rate = libbase::k60::Uart::Config::BaudRate::k9600;
	product.rx_config[Pin::Config::kPullEnable] = true;
	product.rx_config[Pin::Config::kPullUp] = true;
	return product;
}

Dk100::Dk100(const Config &config)
		: UartDevice(Initializer(config))
{
}


#else /* LIBSC_USE_UART */
Dk100::Dk100(const Config&)
		: UartDevice(nullptr)
{}

Dk100::Dk100(nullptr_t)
:
	UartDevice(nullptr)
{}

bool Dk100::Listener(const Byte* data, const size_t size){return false}

#endif /* LIBSC_USE_UART */

}
}
