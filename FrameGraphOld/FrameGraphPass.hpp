//
// Created by camila on 11/10/25.
//

#ifndef LANDMARK_FRAMEGRAPHPASS_HPP
#define LANDMARK_FRAMEGRAPHPASS_HPP
#include <utility>

class FrameGraphPassResources;

struct IFrameGraphPass {
    IFrameGraphPass() = default;

    IFrameGraphPass(const IFrameGraphPass &) = delete;

    virtual ~IFrameGraphPass() = default;

    IFrameGraphPass &operator=(const IFrameGraphPass &) = delete;

    virtual void operator()(FrameGraphPassResources &, void *) = 0;
};

template<typename Data, typename Execute>
struct FrameGraphPass final : IFrameGraphPass {
    explicit FrameGraphPass(Execute &&exec) : exec(std::forward<Execute>(exec)) {
    }
    void operator()(FrameGraphPassResources &resources, void *context) override {
        exec(data,resources,context);
    }

    Execute exec;
    Data data{};
};


#endif //LANDMARK_FRAMEGRAPHPASS_HPP
