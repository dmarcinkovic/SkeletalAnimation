#ifndef SKELETALANIMATION_SYNCOBJECTS_H
#define SKELETALANIMATION_SYNCOBJECTS_H

#include <vulkan/vulkan.h>
#include <vector>

namespace Animation
{
	class SyncObjects final
	{
	private:
		VkDevice m_Device;

		std::vector<VkSemaphore> m_ImageSemaphores;
		std::vector<VkSemaphore> m_RenderSemaphores;
		std::vector<VkFence> m_Fences;

		std::uint32_t m_CurrentFrame{};
		int m_NumberOfSyncObjects;

	public:
		SyncObjects(VkDevice device, int numberOfSyncObjects);

		~SyncObjects();

		[[nodiscard]] VkFence getCurrentFence() const;

		[[nodiscard]] VkSemaphore getCurrentImageSemaphore() const;

		[[nodiscard]] VkSemaphore getCurrentRenderSemaphore() const;

		void update();

	private:
		void createSemaphore(VkSemaphore &semaphore, const VkSemaphoreCreateInfo &semaphoreInfo);

		void createFence(VkFence &fence, const VkFenceCreateInfo &fenceInfo);
	};
}

#endif //SKELETALANIMATION_SYNCOBJECTS_H
