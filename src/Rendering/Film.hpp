#pragma once
#include "structs.h"
#include "Canvas.hpp"
#include <VK/Rendering/Swapchain.h>
class Queue;
class Act;
class Film : public Canvas {
    film_properties properties;
    std::optional<Swapchain> swapchain;
    std::optional<Semaphore> image_ready_semaphore;
    uint32_t current_image_index;
public:
    Film(const film_properties& _properties, Act& act) ;
    bool isOutOfDate() const {return swapchain->GetIsOutOfDate();}
    void Reconstruct(const film_properties &_new_prop);
    Semaphore& NextFrame();
    Framebuffer& GetFramebuffer() override {return swapchain->GetFramebuffer(current_image_index);}
    ivec2 GetFramebufferSize() const override {return swapchain->GetExtent();}
    uint32_t GetCurrentImageIndex() const {return current_image_index;}
    void Present(Queue *queue,std::vector<Semaphore> wait_Semaphores);
    const film_properties& GetProperties() const {return properties;}

    private:
    Swapchain::SwapchainProperties MakeSwapChainProperties() const;
};