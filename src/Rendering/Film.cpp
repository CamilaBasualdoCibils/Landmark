#include "Film.hpp"
#include "Act.hpp"
Film::Film(const film_properties &_properties, Act &act) : properties(_properties)
{

    if (!properties.format.has_value())
    {
        properties.format.emplace(act.GetProperties().color_attachments[0].format);
    }

    LASSERT(_properties.surface, "Window Surface must be provided");
    swapchain.emplace(MakeSwapChainProperties(), act.getRenderPass(), _properties.surface);
    image_ready_semaphore.emplace();
}

void Film::Reconstruct(const film_properties &_new_prop)
{
    std::optional<Format> og_format = properties.format;
    properties = _new_prop;
    if (!_new_prop.format.has_value())
        properties.format = og_format;

    swapchain->Recreate(MakeSwapChainProperties());
}

Semaphore &Film::NextFrame()
{
    current_image_index = swapchain->AcquireNextImage({*image_ready_semaphore});
    return image_ready_semaphore.value();
}

void Film::Present(Queue *queue, std::vector<Semaphore> wait_Semaphores)
{
    bool out_of_date;
    queue->Present(current_image_index, &out_of_date, *swapchain, wait_Semaphores);
}

Swapchain::SwapchainProperties Film::MakeSwapChainProperties() const
{
    Swapchain::SwapchainProperties sc_prop;
    sc_prop.colorFormat = *properties.format;
    sc_prop.colorSpace = properties.color_space;
    sc_prop.presentMode = properties.present_mode;
    return sc_prop;
}
