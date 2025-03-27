#pragma once
#include <pch.h>
#include "Image.h"
#include "Samplers.h"
#include "ImageView.h"
class CombinedImageSampler: public DeviceResource {
    public:
struct CISProperties {
    Image::ImageProperties image;
    ImageView::ImageViewProperties imageView;
    Sampler::SamplerProperties sampler;
};
    private:
    CISProperties properties;
    Image image;
    Sampler sampler;
    ImageView imageView;
    

public:

    CombinedImageSampler(const CISProperties& p) : properties(p),
    image(properties.image),
    imageView(image,properties.imageView),
    sampler(properties.sampler) {

    }
    //Creates a wrapper around an existing image and created an imageview and sampler for it. Used in swapchain retrieve images
    CombinedImageSampler(const Image&,const ImageView::ImageViewProperties& ivp, const Sampler::SamplerProperties& sam);

    void Destroy() override {
        sampler.Destroy();
        imageView.Destroy(); //order probably matters 
        image.Destroy();
    }
    
    const Image& GetImage() const {return image;}
    Image& GetImage() {return image;}

    const ImageView& GetImageView() const {return imageView;}
    const Sampler& GetSampler() const {return sampler;}
};