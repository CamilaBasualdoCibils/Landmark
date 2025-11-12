//
// Created by camila on 11/11/25.
//

#ifndef LANDMARK_GLTEXTURE_HPP
#define LANDMARK_GLTEXTURE_HPP
#include "Math/Math.hpp"

class GLTexture {


public:
    struct Desc {
        uvec2 size;
    };
    GLTexture() = default;
    void create(const Desc& desc,void*);
    void destroy(const Desc& desc,void*);
    void pre_read(const Desc& desc,uint32_t flags,void*);
    void pre_write(const Desc& desc,uint32_t flags,void*);
    std::string to_string(const Desc& desc);

};


#endif //LANDMARK_GLTEXTURE_HPP