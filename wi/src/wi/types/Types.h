#pragma once
#include "wipch.h"

namespace wi
{
	// -- INTEGERS -------------------------------------------


	// -- FLOATING POINT -------------------------------------
	typedef float  f32_t;
	typedef double f64_t;

	// -- COMPLEX --------------------------------------------
	typedef std::complex<f32_t> c32_t;
	typedef std::complex<f64_t> c64_t;

    // -- VECTORS --------------------------------------------
    typedef glm::dvec2 vec2;
    typedef glm::dvec4 vec4;

    // define extra conversion here before including imgui, don't do it in the imconfig.h
#define IM_VEC2_CLASS_EXTRA \
    constexpr ImVec2(glm::vec2& f) : x(f.x), y(f.y) {} \
    operator glm::vec2() const { return glm::vec2(x, y); }

#define IM_VEC4_CLASS_EXTRA \
    constexpr ImVec4(const glm::vec4& f) : x(f.x), y(f.y), z(f.z), w(f.w) {} \
    operator glm::vec4() const { return glm::vec4(x,y,z,w); }

}
