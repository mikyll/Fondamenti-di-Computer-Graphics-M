#include "utils.h"

std::string getCoordinateSystemName(ReferenceSystem coordinateSystem)
{
	std::string result = "Unknown";

	switch (coordinateSystem)
	{
		case OCS: result = "OCS"; break;
		case WCS: result = "WCS"; break;
		default: break;
	}

	return result;
}

std::string getOperationModeName(OperationMode operationMode)
{
	std::string result = "Unknown";

	switch (operationMode)
	{
		case MODE_TRASLATING: result = "Translate"; break;
		case MODE_ROTATING: result = "Rotate"; break;
		case MODE_SCALING: result = "Scale"; break;
		case MODE_NAVIGATION: result = "Navigate"; break;
		default: break;
	}

	return result;
}

std::string getWorkingAxisName(WorkingAxis workingAxis)
{
	std::string result = "Unknown";

	switch (workingAxis)
	{
		case AXIS_X: result = "X"; break;
		case AXIS_Y: result = "Y"; break;
		case AXIS_Z: result = "Z"; break;
		default: break;
	}

	return result;
}

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
		default: break;
	}

	return result;
}