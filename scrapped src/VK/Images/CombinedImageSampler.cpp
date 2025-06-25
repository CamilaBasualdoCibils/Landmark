#include "CombinedImageSampler.h"

CombinedImageSampler::CombinedImageSampler(const Image & _image, const ImageView::ImageViewProperties &ivp, const Sampler::SamplerProperties &sam):
image(_image),imageView(image,ivp),sampler(sam)
{
}