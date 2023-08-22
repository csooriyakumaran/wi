#pragma once
#include "wipch.h"
#include "wi/core/Ref.h"
#include "wi/core/Log.h"
#include "wi/data/DataBuffer.h"

namespace wi
{
    class DataIO : public RefCounted
    {
    public:
        virtual DataBuffer Load(std::atomic<bool>& streaming, size_t buffsize) = 0;
        virtual bool Write() = 0;  /// maybe something like write(fname, filetype, etc.) or write(io_options)
    };
}
