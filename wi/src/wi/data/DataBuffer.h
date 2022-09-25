#pragma once

#include "wipch.h"
#include "wi/types/Types.h"

namespace wi
{
    template<typename T>
    struct _DataBuffer
    {
        std::valarray<T> x;
        std::valarray<T> y;
        uint64_t     index = 0;
        uint64_t     MaxSize = 0;
        void AddPoint(_Vec2<T> _v)
        {
            x[index] = _v.x;
            y[index] = _v.y;
            index = (index + 1) % MaxSize;
        }
    };

    typedef _DataBuffer<f64_t> DataBuffer;
}