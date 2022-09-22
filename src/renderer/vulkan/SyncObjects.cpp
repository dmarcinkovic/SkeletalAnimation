#include <cassert>
#include <spdlog/spdlog.h>

#include "SyncObjects.h"

namespace Animation
{
	SyncObjects::SyncObjects(VkDevice device, int numberOfSyncObjects)
			: m_Device(device), m_ImageSemaphores(numberOfSyncObjects), m_RenderSemaphores(numberOfSyncObjects),
			  m_Fences(numberOfSyncObjects), m_NumberOfSyncObjects(numberOfSyncObjects)
	{
		assert(m_Device != VK_NULL_HANDLE);
		assert(numberOfSyncObjects > 0);

		VkSemaphoreCreateInfo semaphoreInfo{.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
		VkFenceCreateInfo fenceInfo{.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, .flags = VK_FENCE_CREATE_SIGNALED_BIT};

		for (int i = 0; i < numberOfSyncObjects; ++i)
		{
			createSemaphore(m_ImageSemaphores[i], semaphoreInfo);
			createSemaphore(m_RenderSemaphores[i], semaphoreInfo);
			createFence(m_Fences[i], fenceInfo);
		}
	}

	SyncObjects::~SyncObjects()
	{
		assert(m_Device != VK_NULL_HANDLE);
		assert(m_Fences.size() == m_RenderSemaphores.size() && m_Fences.size() == m_ImageSemaphores.size());

		for (int i = 0; i < m_Fences.size(); ++i)
		{
			vkDestroySemaphore(m_Device, m_ImageSemaphores[i], nullptr);
			vkDestroySemaphore(m_Device, m_RenderSemaphores[i], nullptr);
			vkDestroyFence(m_Device, m_Fences[i], nullptr);
		}
	}

	VkFence SyncObjects::getCurrentFence() const
	{
		assert(m_CurrentFrame < m_Fences.size());
		assert(m_Fences[m_CurrentFrame] != VK_NULL_HANDLE);
		return m_Fences[m_CurrentFrame];
	}

	VkSemaphore SyncObjects::getCurrentImageSemaphore() const
	{
		assert(m_CurrentFrame < m_ImageSemaphores.size());
		assert(m_ImageSemaphores[m_CurrentFrame] != VK_NULL_HANDLE);
		return m_ImageSemaphores[m_CurrentFrame];
	}

	VkSemaphore SyncObjects::getCurrentRenderSemaphore() const
	{
		assert(m_CurrentFrame < m_RenderSemaphores.size());
		assert(m_RenderSemaphores[m_CurrentFrame] != VK_NULL_HANDLE);
		return m_RenderSemaphores[m_CurrentFrame];
	}

	void SyncObjects::createSemaphore(VkSemaphore &semaphore, const VkSemaphoreCreateInfo &semaphoreInfo)
	{
		if (vkCreateSemaphore(m_Device, &semaphoreInfo, nullptr, &semaphore) != VK_SUCCESS)
		{
			spdlog::error("Failed to create semaphore.");
			std::exit(EXIT_FAILURE);
		}

		assert(semaphore != VK_NULL_HANDLE);
	}

	void SyncObjects::createFence(VkFence &fence, const VkFenceCreateInfo &fenceInfo)
	{
		if (vkCreateFence(m_Device, &fenceInfo, nullptr, &fence) != VK_SUCCESS)
		{
			spdlog::error("Failed to create fence.");
			std::exit(EXIT_FAILURE);
		}

		assert(fence != VK_NULL_HANDLE);
	}

	void SyncObjects::update()
	{
		++m_CurrentFrame;
		m_CurrentFrame %= m_NumberOfSyncObjects;
	}
}