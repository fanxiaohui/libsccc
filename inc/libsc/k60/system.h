/*
 * system.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libbase/k60/reg_file.h"

#include "libsc/sys_tick_timer.h"
#include "libsc/k60/dwt_delay.h"

namespace libsc
{
namespace k60
{

class System
{
public:
	static void Init()
	{
		if (!m_instance)
		{
			m_instance = new System;
		}
	}

	static void DelayUs(const uint16_t us)
	{
		m_instance->m_delay.DelayUs(us);
	}

	static void DelayMs(const uint16_t ms)
	{
		m_instance->m_delay.DelayMs(ms);
	}

	static void DelayS(const uint16_t s)
	{
		m_instance->m_delay.DelayS(s);
	}

	/**
	 * Return the time elapsed, in ms, since Init()
	 *
	 * @return
	 */
	static Timer::TimerInt Time()
	{
		return m_instance->m_timer.Time();
	}

	static libbase::k60::RegFile* GetRegFile()
	{
		return &m_instance->m_reg_file;
	}

private:
	System();

	DwtDelay m_delay;
	SysTickTimer m_timer;
	libbase::k60::RegFile m_reg_file;

	static System *m_instance;
};

}
}
