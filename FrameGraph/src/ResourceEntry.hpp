//
// Created by camila on 11/11/25.

#ifndef LANDMARK_RESOURCEENTRY_HPP
#define LANDMARK_RESOURCEENTRY_HPP


/**
 * Holds the actual resource and lists who produces it and who uses it last
 */
class ResourceEntry final {
    friend class FrameGraph;

    enum class Type :uint8_t { eTransient, eImported };

private:

    struct IResource {

    };
    template <typename T>
    struct Resource : IResource {};


    std::unique_ptr<IResource> resource;
    uint32_t id = -1,version = -1;
};


#endif //LANDMARK_RESOURCEENTRY_HPP
