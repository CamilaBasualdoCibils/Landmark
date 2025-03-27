#pragma once

template <typename T>
class shared_ctrl_ptr
{
private:
struct alloc_data {
    T t;
    uint64_t ref_count;
    std::function<void(int,int)> ref_count_change_callback;
};
alloc_data* data;
    /* data */
public:
    shared_ctrl_ptr();
    shared_ctrl_ptr(const shared_ctrl_ptr& o);
    ~shared_ctrl_ptr();
    static shared_ctrl_ptr Make_shader_ctrl_ptr();
};
