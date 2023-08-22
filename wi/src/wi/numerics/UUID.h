#pragma once

#include "wipch.h"
#include "Random.h"

namespace wi
{

    class UUID
    {
    public:

        UUID() : m_UUID(Random64::UInt64()) {}
        UUID(uint64_t uuid) : m_UUID(uuid) {}

        operator uint64_t() { return m_UUID; };
        operator const uint64_t() const { return m_UUID; }

    private:
        uint64_t m_UUID;

    };

}

namespace std
{
    // this allows uuid to be hashed and for example used as a key in a map
    template<>
    struct hash<wi::UUID>
    {
        std::size_t operator()(const wi::UUID& uuid) const
        {
            // uuid is already a randomly generated 64 bit int, and is suitable for a hash
            return uuid;
            //return hash<uint64_t>()((uint64_t)uuid);
        }
    };
}