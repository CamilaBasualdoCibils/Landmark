//
// Created by camila on 11/11/25.

#ifndef LANDMARK_RESOURCEENTRY_HPP
#define LANDMARK_RESOURCEENTRY_HPP
#include "Math/Math.hpp"
#include <memory>
/**
 * Holds the actual resource and lists who produces it and who uses it last
 */
class ResourceEntry final {
    friend class FrameGraph;

    enum class Type :uint8 { eTransient, eImported };
    
private:

    struct IResource {

    };
    template <typename T>
    struct Resource : IResource {};

    const Type m_type;
    std::unique_ptr<IResource> resource;
    uint32_t id = -1,version = -1;
    public:
    bool is_imported() const {return m_type == Type::eImported;}
};


#endif //LANDMARK_RESOURCEENTRY_HPP
