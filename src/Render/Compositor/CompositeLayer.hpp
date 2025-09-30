#pragma once

#include <pch.h>
#include "CompositeObject.hpp"
namespace Render
{

class CompositeLayer : public CompositeObject
{

    public:
    void Compose(CompositePayload& Payload)override {}

};
}
