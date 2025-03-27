#include "pch.h"
#include "Semaphore.h"




Semaphore::Semaphore()
{
	vk::SemaphoreCreateInfo create_info{};
	create_info.sType = vk::StructureType::eSemaphoreCreateInfo;

	semaphore = GetvkDevice().createSemaphore(create_info).value;
	logger.Debug("Semaphore Created");
}

Semaphore::Semaphore(const Semaphore& o):semaphore(o.semaphore)
{
	
}

void Semaphore::Destroy()
{
	GetvkDevice().destroySemaphore(semaphore);
	logger.Debug("Semaphore Destroyed");
}

uint64_t Semaphore::CounterValue() const
{
	return GetvkDevice().getSemaphoreCounterValue(semaphore).value;
}

void Semaphore::Signal(uint64_t counter)
{
	vk::SemaphoreSignalInfo semaphore_signal_info{};
	semaphore_signal_info.value = counter;
	auto result = GetvkDevice().signalSemaphore(semaphore_signal_info);
}

void Semaphore::Wait(uint64_t count)
{
	vk::SemaphoreWaitInfo semaphore_wait_info{};
	semaphore_wait_info.pSemaphores = &semaphore;
	semaphore_wait_info.semaphoreCount = 1;
	semaphore_wait_info.flags = vk::SemaphoreWaitFlagBits::eAny;
	semaphore_wait_info.pValues = &count;

	auto result = GetvkDevice().waitSemaphores(semaphore_wait_info, UINT64_MAX);
}

Semaphore::operator vk::Semaphore() const
{
	return semaphore;
}

