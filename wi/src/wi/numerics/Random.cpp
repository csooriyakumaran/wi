#include "wipch.h"
#include "Random.h"

namespace wi {

	std::mt19937 Random32::s_RandomEngine;
	std::uniform_int_distribution<std::mt19937::result_type> Random32::s_Distribution;
	std::mt19937 Random64::s_RandomEngine;
	std::uniform_int_distribution<uint64_t> Random64::s_Distribution;

}