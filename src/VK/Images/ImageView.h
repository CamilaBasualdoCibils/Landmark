#include <pch.h>

#include <VK/Devices/DeviceResource.h>
class Image;
class ImageView : DeviceResource
{
public:
    struct ImageViewProperties
    {
    };

private:
    vk::ImageView imageView;
    ImageViewProperties properties;
public:
    ImageView(Image &image, const ImageViewProperties &_info) : properties(_info)
    {
        Create(image);
    }
    void Destroy() override {
        GetvkDevice().destroyImageView(imageView);
    }
    vk::ImageView GetVkImageView() const {return imageView;}
    operator vk::ImageView() const {return imageView;}
    private:
    void Create(Image &image);
};