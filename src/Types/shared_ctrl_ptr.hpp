#pragma once

template <typename T>
class shared_ctrl_ptr
{
    using Callback_function = std::function<void(int,int)>;
private:
struct alloc_data {
    T t;
    uint64_t ref_count;
    Callback_function ref_count_change_callback;
};
alloc_data* data;
    /* data */
public:
    template <typename... ARGS>
    shared_ctrl_ptr(ARGS&&... args,Callback_function callback) {
        data = new alloc_data(std::forward<ARGS>(args)...,1, callback);
    }
    shared_ctrl_ptr(const shared_ctrl_ptr& o);
    ~shared_ctrl_ptr();
    static shared_ctrl_ptr Make_shader_ctrl_ptr();
};
