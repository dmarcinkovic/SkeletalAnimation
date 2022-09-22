#ifndef SKELETALANIMATION_IMAGEVIEW_H
#define SKELETALANIMATION_IMAGEVIEW_H

#include <vulkan/vulkan.h>

namespace Animation
{
	class ImageView final
	{
	private:
		VkDevice m_Device;
		VkImageView m_ImageView{};
		VkFormat m_Format;
		VkImage m_Image;

	public:
		ImageView(VkDevice logicalDevice, VkFormat format, VkImage image);

		~ImageView();

		ImageView(const ImageView &imageView);

		VkImageView getView() const;

	private:
		void createImageView(const VkImageViewCreateInfo &imageViewInfo);

		static VkImageViewCreateInfo getImageViewInfo(VkFormat format, VkImage image);
	};
}

#endif //SKELETALANIMATION_IMAGEVIEW_H
