/*
 * smart_car.cpp
 * Smart car abstraction
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <cstdint>

#include "libsc/com/config.h"
#include "libsc/com/bluetooth.h"
#include "libsc/com/led.h"
#include "libsc/com/motor.h"
#include "libsc/com/servo.h"
#include "libutil/misc.h"

#include "libsc/smart_car.h"

#define SERVO_MID_DEGREE 90
#define SERVO_AMPLITUDE 40
#define SERVO_MAX_DEGREE (SERVO_MID_DEGREE + SERVO_AMPLITUDE)
#define SERVO_MIN_DEGREE (SERVO_MID_DEGREE - SERVO_AMPLITUDE)

namespace libsc
{

SmartCar::SmartCar()
		: m_encoder{0}, m_leds{Led(0), Led(1), Led(2), Led(3)}, m_motor(0),
		  m_servo(0)
{
	m_motor.SetPower(0);
	SetMotorDirection(true);
	m_servo.SetDegree(SERVO_MID_DEGREE);
	m_bt.StartReceive();
}

void SmartCar::SetMotorDirection(const bool is_forward)
{
	m_motor.SetClockwise(!is_forward);
}

void SmartCar::AddMotorPowerTil(const uint16_t factor, const uint16_t max)
{
	const uint16_t curr_power = GetMotorPower();
	if (curr_power < max)
	{
		SetMotorPower(libutil::Clamp<uint16_t>(0, curr_power + factor, max));
	}
}

void SmartCar::DropMotorPower(const uint16_t factor)
{
	m_motor.AddPower(-factor);
}

void SmartCar::DropMotorPowerTil(const uint16_t factor, const uint16_t min)
{
	const uint16_t curr_power = GetMotorPower();
	if (curr_power > min)
	{
		SetMotorPower(libutil::Clamp<uint16_t>(min, curr_power - factor, 1000));
	}
}

void SmartCar::TurnLeft(const uint8_t percentage)
{
	m_servo.SetDegree(SERVO_MID_DEGREE - (percentage * SERVO_AMPLITUDE / 100));
}

void SmartCar::TurnRight(const uint8_t percentage)
{
	m_servo.SetDegree((percentage * SERVO_AMPLITUDE / 100) + SERVO_MID_DEGREE);
}

void SmartCar::SetRightPercentage(const uint8_t percentage)
{
	m_servo.SetDegree((percentage * (SERVO_AMPLITUDE * 2) / 100)
			+ SERVO_MIN_DEGREE);
}

bool SmartCar::IsMotorForward() const
{
	return !m_motor.IsClockwise();
}

uint8_t SmartCar::GetRightPercentge() const
{
	return (m_servo.GetDegree() - SERVO_MIN_DEGREE) * 100 / (SERVO_AMPLITUDE * 2);
}

}
