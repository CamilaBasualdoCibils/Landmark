//
// Created by camila on 10/29/24.
//

#ifndef LENS_H
#define LENS_H
#include <pch.h>

#include <VK/Rendering/Pipeline.h>
#include "structs.h"
#include "Frame.h"
#include "Film.hpp"

class Stage;
class Act;
class Lens {

    static inline uint64_t ID_Assigner = 0;
    const uint64_t ID = ID_Assigner++;

    std::optional<Pipeline> pipeline;
    static inline Log logger = Log("Stage");
    lens_properties properties;
public:


    

    Lens(const lens_properties& internal,Stage& st,Act& act);
    ~Lens();
     
     Pipeline& getPipeline() {return pipeline.value();}
     const lens_properties& Properties() const {return properties;}
};



#endif //LENS_H
