#pragma once

#define DEFAULT_WINDOW_WIDTH 1120
#define DEFAULT_WINDOW_HEIGHT 630
#define ASPECT_RATIO 16.0f / 9.0f

#define CAMERA_ZOOM_SPEED 0.1f
#define CAMERA_TRASLATION_SPEED 0.01f

#define SHIFT_WHEEL_UP 11
#define SHIFT_WHEEL_DOWN 12
#define CTRL_WHEEL_UP 19
#define CTRL_WHEEL_DOWN 20

#define LIGHT_SCALE_FACTOR 5

const bool VERTEX_NORMALS = true;
const bool FACE_NORMALS = false;

const std::string MESH_DIR = "Mesh/";
const std::string SHADERS_DIR = "Shaders/";

const glm::vec3 DEFAULT_LIGHT_COLOR = { 1.0f, 1.0f, 1.0f };

// CoordinateSystem
enum {
	OCS = 0,	// Object Coordinate System
	WCS = 1			// World Coordinate System
};

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

// MenuOption
enum {
	CHANGE_TO_OCS,
	CHANGE_TO_WCS,
	WIRE_FRAME,
	FACE_FILL,
	CULLING_OFF,
	CULLING_ON,
	GRID_AXIS_OFF,
	GRID_AXIS_ON,
	TRANSFORM_INFO_OFF,
	TRANSFORM_INFO_ON,
	NUM_OPTIONS,
};

// MaterialType
enum {
	NONE = 0,
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
	WAVE_COLORS,
	WAVE_OCEAN,
	WAVE_LIGHT,
	TOON,
	TOON_V2,
	NUM_SHADERS
};

typedef int MenuOption;
typedef int MaterialType;
typedef int ShadingType;

typedef int ReferenceSystem;
typedef int OperationMode;
typedef int WorkingAxis;