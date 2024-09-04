#pragma once
#include "G_API/Devices/DeviceResource.h"

#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

class Semaphore final: public DeviceResource
{
	vk::Semaphore semaphore;

public:
	Semaphore();
	Semaphore(const Semaphore& o);
	void Destroy() override;

	uint64_t CounterValue() const;

	void Signal(uint64_t counter);

	void Wait(uint64_t count);
	operator vk::Semaphore() const;

	vk::Semaphore GetVkSemaphore() const { return semaphore; }
};


#endif
