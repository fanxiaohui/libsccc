/*
 * incremental_pid_controller.h
 * Generic incremental PID controller
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#if MK60DZ10 || MK60D10 || MK60F15
#include "libsc/system.h"

#elif MKL26Z4
#include "libsc/kl26/system.h"

#endif

#include "libsc/timer.h"
#include "libutil/pid_controller.h"

namespace libutil
{

template<typename InT_, typename OutT_>
class IncrementalPidController : public PidController<InT_, OutT_>
{
public:
	typedef InT_ InT;
	typedef OutT_ OutT;

	IncrementalPidController(const InT setpoint, const float kp, const float ki,
			const float kd);

	/**
	 * Set the upper bound of i, <= 0 means unlimited i
	 *
	 * @param value
	 */
	void SetILimit(const float value)
	{
		m_i_limit = value;
	}

	void Reset()
	{
		ResetTime();
	}

	void ResetTime()
	{
#if MK60DZ10 || MK60D10 || MK60F15
		m_prev_time = libsc::System::Time();
#elif MKL26Z4
		m_prev_time = libsc::kl26::System::Time();
#endif
	}

protected:
	void OnCalc(const InT error) override;
	OutT GetControlOut() override;

private:
	float m_i_limit;

	InT m_prev_error[2];
	OutT m_prev_output;
	libsc::Timer::TimerInt m_prev_time;
};

}

#include "incremental_pid_controller.tcc"
