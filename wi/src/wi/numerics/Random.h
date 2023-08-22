#pragma once

#include "wipch.h"
#include "wi/types/Types.h"

namespace wi {


	class Random32
	{
	public:
		static void Init()
		{
			s_RandomEngine.seed(std::random_device()());
		}

		static uint32_t UInt()
		{
			return s_Distribution(s_RandomEngine);
		}

		static uint32_t UInt(uint32_t min, uint32_t max)
		{
			return min + (s_Distribution(s_RandomEngine) % (max - min + 1));
		}

		static float Float()
		{
			return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
		}

		static glm::vec3 Vec3()
		{
			return glm::vec3(Float(), Float(), Float());
		}

		static glm::vec3 Vec3(float min, float max)
		{
			return glm::vec3(Float() * (max - min) + min, Float() * (max - min) + min, Float() * (max - min) + min);
		}

		static glm::vec3 InUnitSphere()
		{
			return glm::normalize(Vec3(-1.0f, 1.0f));
		}
	private:
		static std::mt19937 s_RandomEngine;
		static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
	};

	class Random64
	{
	public:
		static void Init()
		{
			s_RandomEngine.seed(std::random_device()());
		}

		static uint64_t UInt64()
		{
			return s_Distribution(s_RandomEngine);
		}

		static uint64_t UInt64(uint64_t min, uint64_t max)
		{
			return min + (s_Distribution(s_RandomEngine) % (max - min + 1));
		}

		static f64_t Double()
		{
			return (f64_t)s_Distribution(s_RandomEngine) / (f64_t)std::numeric_limits<uint64_t>::max();
		}

		static f64_t Double(f64_t min, f64_t max)
		{
			return (f64_t)s_Distribution(s_RandomEngine) / (f64_t)std::numeric_limits<uint64_t>::max() * (max - min) + min;
		}


		static glm::dvec3 dVec3()
		{
			return glm::dvec3(Double(), Double(), Double());
		}

		static glm::dvec3 dVec3(f64_t min, f64_t max)
		{
			return glm::vec3(Double() * (max - min) + min, Double() * (max - min) + min, Double() * (max - min) + min);
		}

	private:
		static std::mt19937 s_RandomEngine;
		static std::uniform_int_distribution<uint64_t> s_Distribution;

	};
	class Random
	{
	public:
		static void Init()
		{
			Random32::Init();
			Random64::Init();
		}
	};


}
