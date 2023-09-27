#include "menu.h"

static int menu;
static int subMenuReferenceSystem;
static int subMenuRendering;
static int subMenuCulling;
static int subMenuGridAxis;
static int subMenuTransformInfo;
static int materialSubmenu;
static int shaderSubmenu;


static void updateMenu(int option);
static void setMeshRenderingMode(int option);
static void setCulling(int option);
static void setReferenceSystem(ReferenceSystem option);
static void setGridAxis(int option);
static void setTransformInfo(int option);
static void setMaterial(MaterialType option);
static void setShader(ShadingType option);


static void updateMenu(int option)
{
	switch (option)
	{
	case CHANGE_TO_OCS:
		glutChangeToSubMenu(3, "Reference System: OCS", subMenuReferenceSystem);
		break;
	case CHANGE_TO_WCS:
		glutChangeToSubMenu(3, "Reference System: WCS", subMenuReferenceSystem);
		break;
	case FACE_FILL:
		glutChangeToSubMenu(4, "Rendering: Face fill", subMenuRendering);
		break;
	case WIRE_FRAME:
		glutChangeToSubMenu(4, "Rendering: Wireframe", subMenuRendering);
		break;
	case CULLING_OFF:
		glutChangeToSubMenu(5, "Culling: OFF", subMenuCulling);
		break;
	case CULLING_ON:
		glutChangeToSubMenu(5, "Culling: ON", subMenuCulling);
		break;
	case GRID_AXIS_OFF:
		glutChangeToSubMenu(6, "Grid & Axis: OFF", subMenuGridAxis);
		break;
	case GRID_AXIS_ON:
		glutChangeToSubMenu(6, "Grid & Axis: ON", subMenuGridAxis);
		break;
	case TRANSFORM_INFO_OFF:
		glutChangeToSubMenu(8, "Transform Info: OFF", subMenuTransformInfo);
		break;
	case TRANSFORM_INFO_ON:
		glutChangeToSubMenu(8, "Transform Info: ON", subMenuTransformInfo);
		break;

	default:
		break;
	}
}

static void setReferenceSystem(ReferenceSystem option)
{
	app.coordinateSystem = option;
	glutSetMenu(menu);
	glutTimerFunc(1, updateMenu, option);
}

static void setMeshRenderingMode(int option)
{
	if (option == FACE_FILL)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glutSetMenu(menu);
	glutTimerFunc(1, updateMenu, option);
}

static void setCulling(int option)
{
	if (option == CULLING_OFF)
		glDisable(GL_CULL_FACE);
	else glEnable(GL_CULL_FACE);
	glutSetMenu(menu);
	glutTimerFunc(1, updateMenu, option);
}

static void setGridAxis(int option)
{
	app.showGridAxis = option == GRID_AXIS_ON;
	glutSetMenu(menu);
	glutTimerFunc(1, updateMenu, option);
}

static void setTransformInfo(int option)
{
	app.showTransformInfo = option == TRANSFORM_INFO_ON;
	glutTimerFunc(1, updateMenu, option);
}

static void setMaterial(MaterialType option)
{
	objects.at(selectedObj).material = materials.at(option);
}

static void setShader(ShadingType option)
{
	objects.at(selectedObj).shader = shaders.at(option);
}

void buildOpenGLMenu()
{
	subMenuReferenceSystem = glutCreateMenu(setReferenceSystem);
	glutAddMenuEntry("OCS", CHANGE_TO_OCS);
	glutAddMenuEntry("WCS", CHANGE_TO_WCS);

	subMenuRendering = glutCreateMenu(setMeshRenderingMode);
	glutAddMenuEntry("Face fill", FACE_FILL);
	glutAddMenuEntry("Wireframe", WIRE_FRAME);

	subMenuCulling = glutCreateMenu(setCulling);
	glutAddMenuEntry("OFF", CULLING_OFF);
	glutAddMenuEntry("ON", CULLING_ON);

	subMenuGridAxis = glutCreateMenu(setGridAxis);
	glutAddMenuEntry("OFF", GRID_AXIS_OFF);
	glutAddMenuEntry("ON", GRID_AXIS_ON);

	subMenuTransformInfo = glutCreateMenu(setTransformInfo);
	glutAddMenuEntry("OFF", TRANSFORM_INFO_OFF);
	glutAddMenuEntry("ON", TRANSFORM_INFO_ON);

	materialSubmenu = glutCreateMenu(setMaterial);
	// add Material menu entries
	for (int i = 0; i < NUM_MATERIALS; i++)
	{
		glutAddMenuEntry(materials[i].name.c_str(), i);
	}

	shaderSubmenu = glutCreateMenu(setShader);
	// add Shading menu entries
	for (int i = 0; i < NUM_SHADERS; i++)
	{
		glutAddMenuEntry(shaders.at(i).name.c_str(), i);
	}

	menu = glutCreateMenu(NULL); // richiama main_menu_func() alla selezione di una voce menu
	glutAddMenuEntry("Options", -1); // -1 significa che non si vuole gestire questa riga
	glutAddMenuEntry("", -1);
	glutAddSubMenu(app.coordinateSystem == WCS ? "Reference System: WCS" : "Reference System: OCS", subMenuReferenceSystem);
	glutAddSubMenu("Rendering: Face fill", subMenuRendering);
	glutAddSubMenu("Culling: OFF", subMenuCulling);
	glutAddSubMenu(app.showGridAxis ? "Grid & Axis: ON" : "Grid & Axis: OFF", subMenuGridAxis);
	glutAddMenuEntry("", -1);
	glutAddSubMenu(app.showTransformInfo ? "Transform Info: ON" : "Transform Info: OFF", subMenuTransformInfo);
	glutAddSubMenu("Material", materialSubmenu);
	glutAddSubMenu("Shader", shaderSubmenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
