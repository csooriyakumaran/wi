#pragma once

#include "wipch.h"

namespace wi
{
    class Hash
    {
    public:
        static constexpr uint32_t GenerateFNVHash(const char* str)
        {
            constexpr uint32_t FNV_PRIME = 16777619u;
            constexpr uint32_t OFFEST_BASIS = 2166136261u;

            const size_t length = std::string_view(str).length() + 1;
            uint32_t hash = OFFEST_BASIS;
            for (size_t i = 0; i < length; ++i)
            {
                hash ^= *str++;
                hash *= FNV_PRIME;
            }
            return hash;
        }
        static constexpr uint32_t GenerateFNVHash(std::string_view str)
        {
            return GenerateFNVHash(str.data());
        }
    };

}