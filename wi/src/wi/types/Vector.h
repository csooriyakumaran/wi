#pragma once

#include "wipch.h"

namespace wi
{
    // -- INTERNAL
    template<typename T>
    struct _Vec2
    {
        T x, y;
        constexpr _Vec2() : x(0), y(0) { }
        constexpr _Vec2(T _x, T _y) : x(_x), y(_y)   { }
    };

    template<typename T>
    struct _Vec3
    {
        T x, y, z;
        constexpr _Vec3() : x(0), y(0), z(0) { }
        constexpr _Vec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) { }
    };


    template<typename T>
    struct _Vec4
    {
        T x, y, z, w;
        constexpr _Vec4() : x(0), y(0), z(0), w(0) { }
        constexpr _Vec4(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) { }
    };







}
