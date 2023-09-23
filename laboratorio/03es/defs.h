#pragma once

#define SHIFT_WHEEL_UP 11
#define SHIFT_WHEEL_DOWN 12
#define CTRL_WHEEL_UP 19
#define CTRL_WHEEL_DOWN 20

const std::string MESH_DIR = "Mesh/";

// OperationMode
enum {
	MODE_NAVIGATION,
	MODE_CAMERA_MOVING,
	MODE_TRASLATING,
	MODE_ROTATING,
	MODE_SCALING,
};

// WorkingAxis
enum {
	AXIS_X,
	AXIS_Y,
	AXIS_Z,
};

// TransformMode
enum {
	OCS, // Object Coordinate System
	WCS // World Coordinate System
};

// MenuOption
enum {
	CHANGE_TO_OCS,
	CHANGE_TO_WCS,
	WIRE_FRAME,
	FACE_FILL,
	CULLING_OFF,
	CULLING_ON,
	NUM_OPTIONS,
};

// MaterialType
enum {
	NO_MATERIAL = 0,
	RED_PLASTIC,
	EMERALD,
	BRASS,
	SLATE,
	BLACK_PLASTIC,
	GOLD,
	SILVER,
	BRONZE,
	COPPER,
	TURQUOISE,
	NUM_MATERIALS
};

// ShadingType
enum {
	PASS_THROUGH = 0,
	GOURAUD,
	PHONG,
	BLINN,
	WAVE,
	WAVE_COLOR,
	WAVE_LIGHT,
	TOON,
	TOON_V2,
	NUM_SHADERS
};