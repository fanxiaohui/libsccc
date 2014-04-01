/*
 * ov7725.h
 * OV7725 camera
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_OV7725_H_
#define LIBSC_OV7725_H_

#include <mini_common.h>
#include <cstddef>
#include <cstdint>
#include <memory>

#include "libsc/com/camera.h"

namespace libsc
{

class Ov7725 : public Camera
{
public:
	/**
	 * Construct a camera taking images in @a w * @a h size. Beware not to set
	 * @a w and @a h too high unless you have a plenty of heap available
	 *
	 * @param w (0, 640]
	 * @param h (0, 480]
	 */
	Ov7725(const uint16_t w, const uint16_t h);
	~Ov7725();

	bool Init() override;

	void ShootOnce() override;
	void ShootContinuously() override;
	void StopShoot() override;

	bool IsImageReady() const override
	{
		return m_is_image_ready;
	}

	/**
	 * Return a COPY of the buffer, you are required to delete the buffer. This
	 * is VERY inefficient, use LockBuffer() and UnlockBuffer() instead
	 *
	 * @return
	 */
	const Byte* GetImage();

	/**
	 * Lock and return the buffer. When the buffer is locked, new incoming frame
	 * will be dropped until UnlockBuffer() is called.
	 *
	 * REMEMBER to pair each LockBuffer() call with a UnlockBuffer() call!
	 *
	 * @return The image buffer, 8 pixel/byte
	 */
	const Byte* LockBuffer();
	void UnlockBuffer()
	{
		m_is_buffer_lock = false;
	}

	Uint GetImageW() const
	{
		return m_w;
	}
	Uint GetImageH() const
	{
		return m_h;
	}

private:
	enum State
	{
		START_SHOOT,
		SHOOTING,
		END_SHOOT,
		FAIL_SHOOT
	};

	bool InitCameraConfig();
	void InitPort();

	inline void RegVsyncHandler();
	inline void UnregVsyncHandler();

	void OnVsync();
	static __ISR void VsyncHandler();
	void OnDma();
	static __ISR void DmaHandler();

	uint16_t m_w;
	uint16_t m_h;
	bool m_is_buffer_lock;
	Byte *m_front_buffer;
	volatile Byte *m_back_buffer;
	size_t m_buffer_size;

	volatile State m_state;
	bool m_is_shoot_once;
	volatile bool m_is_image_ready;
};

}

#endif /* LIBSC_OV7725_H_ */
