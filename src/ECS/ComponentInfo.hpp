#pragma once
#include <pch.h>
#include <ECS/Defines.hpp>
struct Component_Info {
    std::string Name;
};

struct Component_Info_Extended :Component_Info {
    std::function<void(void*,SceneID,ObjectID)> constructor_where;
    size_t size;
    Component_Info_Extended(const Component_Info& b):Component_Info(b) {}
    Component_Info_Extended() = default;
    template <typename T>
    static Component_Info_Extended FromComponentInfo(const Component_Info& info) {
        Component_Info_Extended info_ext = info;
        info_ext.size = sizeof(T);
        info_ext.constructor_where = [](void* where, SceneID scene_id,ObjectID object_id){new (where) T(scene_id,object_id); };
        return info_ext;
    }
};