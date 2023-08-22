#pragma once

#include "wipch.h"
#include "wi/types/Types.h"

namespace wi
{
    struct DataBuffer : public RefCounted
    {
        std::vector<f64_t> x;
        std::vector<f64_t> y;

        DataBuffer() 
        {
            //x.reserve(0);
            //y.reserve(0);
        }

        ~DataBuffer()
        {
        }

        DataBuffer(const DataBuffer& other) = delete;
        DataBuffer& operator=(const DataBuffer& other) = delete;

        DataBuffer(DataBuffer&& other) noexcept
        {
            //x.reserve(x.size() + other.x.size());
            //y.reserve(y.size() + other.y.size());
            //std::move(other.x.begin(), other.x.end(), x.begin());
            //std::move(other.y.begin(), other.y.end(), y.begin());
            x = std::move(other.x);
            y = std::move(other.y);
            other.x.clear();
            other.y.clear();
        }

        DataBuffer& operator=(DataBuffer&& other) noexcept
        {
            if (this != &other)
            {
                //std::move(other.x.begin(), other.x.end(), x.begin());
                //std::move(other.y.begin(), other.y.end(), y.begin());
                x = std::move(other.x);
                y = std::move(other.y);
                other.x.clear();
                other.y.clear();
            }
            return *this;
        }

        void append(DataBuffer& buff)
        {
            if (x.empty())
            {
                x = std::move(buff.x);
                y = std::move(buff.y);
                return;
            }

            x.reserve(x.size() + buff.x.size());
            y.reserve(y.size() + buff.y.size());

            std::move(std::begin(buff.x), std::end(buff.x), std::back_inserter(x));
            std::move(std::begin(buff.y), std::end(buff.y), std::back_inserter(y));
            buff.clear();
            return;
        }

        void reserve(size_t n)
        {
            x.reserve(n);
            y.reserve(n);
        }


        void clear()
        {
            x.clear();
            y.clear();
        }
    };
}