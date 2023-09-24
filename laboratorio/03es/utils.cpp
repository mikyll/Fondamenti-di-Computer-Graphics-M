#include "utils.h"

std::string getShaderName(ShadingType shadingType)
{
	std::string result = "Unknown";

	switch (shadingType)
	{
	case PASS_THROUGH: result = "Pass Through"; break;
	case GOURAUD: result = "Gouraud"; break;
	case PHONG: result = "Phong"; break;
	case BLINN: result = "Blinn"; break;
	case WAVE: result = "Wave"; break;
	case WAVE_COLOR: result = "Wave Color"; break;
	case WAVE_LIGHT: result = "Wave Light"; break;
	case TOON: result = "Toon"; break;
	case TOON_V2: result = "Toon v2"; break;
	default:
		break;
	}

	return result;
}