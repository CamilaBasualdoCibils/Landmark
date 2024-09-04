#pragma once
#include <pch.h>
#include "DescriptorPool.h"
#include "DescriptorSet.h"
#include "DescriptorSetLayout.h"
#include <G_API/Devices/DeviceResource.h>
class DSetManager :  public DeviceResource
{
public:
    struct DSetManagerProrperties
    {
        DescriptorPool::DescriptorPoolProperties poolProperties;
    };

private:
    DSetManagerProrperties properties;
    DescriptorPool pool;
    std::vector<DescriptorSet> sets;

public:
    DSetManager(const DSetManagerProrperties &properties);

    decltype(sets)::iterator push_back(const DescriptorSet &set)
    {
        sets.push_back(set);
        return --sets.end();
    }
    decltype(sets)::iterator emplace_back(const DescriptorSetLayout &layout)
    {
        sets.emplace_back(pool, layout);
        return --sets.end();
    }

    size_t Size() const { return sets.size(); }

    void Destroy() override {
        pool.Destroy();
        //sets are destroyed with the pool
    }
    DescriptorSet& operator[](size_t index) {return sets[index];}
    DescriptorSet& front() {return sets.front();}
};