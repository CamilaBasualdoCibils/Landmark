#pragma once
#include <pch.h>
#include <ECS/Defines.hpp>
struct Component_Info
{
    std::string Name;
    bool Disablable = true;
};

struct Component_Info_Extended : Component_Info
{
    std::function<void(void *, SceneID, ObjectID)> constructor_where;
    std::function<void(void *)> deconstructor;
    std::function<void(void *, void *)> copy_construct; // dst,src
    std::function<void(void *, void *)> move_construct; // dst,src
    size_t size;

    Component_Info_Extended(const Component_Info &b) : Component_Info(b) {}
    Component_Info_Extended() = default;
    template <typename T>
    static Component_Info_Extended FromComponentInfo(const Component_Info &info)
    {
        Component_Info_Extended info_ext = info;
        info_ext.size = sizeof(T);
        info_ext.constructor_where = [](void *where, SceneID scene_id, ObjectID object_id)
        { new (where) T(scene_id, object_id); };
        info_ext.deconstructor = [](void *what)
        {T* p = reinterpret_cast<T*>(what); p->~T(); };
        info_ext.copy_construct = [](void *dst, void *src)
        { new (dst) T(*reinterpret_cast<T *>(src)); };
        info_ext.move_construct = [](void *dst, void *src)
        { new (dst) T(std::move(*reinterpret_cast<T *>(src))); };
        return info_ext;
    }
};