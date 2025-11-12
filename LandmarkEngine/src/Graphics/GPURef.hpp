//
// Created by camila on 11/10/25.
//

#ifndef LANDMARK_GPUREF_HPP
#define LANDMARK_GPUREF_HPP
#include <memory>

template <typename T>
class GPU {
    std::weak_ptr<T> _ptr;
public:
    explicit GPU(std::weak_ptr<T> ptr) : _ptr(ptr) {}
    GPU(const GPU& gpu) = default;
    GPU& operator=(const GPU& gpu)  = default;


    bool isExpired() const {return _ptr.expired();}
    T* get() const {return _ptr.lock().get();}
    T* operator->() const {return _ptr.lock().get();}
    explicit operator T*() const {return _ptr.lock().get();}

};


#endif //LANDMARK_GPUREF_HPP