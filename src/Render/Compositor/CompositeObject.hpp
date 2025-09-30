#pragma once
#include <pch.h>

#include "CompositePayload.hpp"
class CompositeObject
{

    virtual void Compose(CompositePayload& Payload) = 0;
};