#include "menu.h"

static int menu;
static int subMenuTransformMode;
static int subMenuRendering;
static int subMenuCulling;
static int materialSubmenu;
static int shaderSubmenu;


static void updateMenu(int option);
static void setMeshRenderingMode(int option);
static void setCulling(int option);
static void setTransformMode(CoordinateSystem option);
static void setMaterial(MaterialType option);
static void setShader(ShadingType option);


static void updateMenu(int option)
{
	switch (option)
	{
	case CHANGE_TO_OCS:
		glutChangeToSubMenu(3, "Transform Mode: OCS", subMenuTransformMode);
		app.coordinateSystem = OCS;
		break;
	case CHANGE_TO_WCS:
		glutChangeToSubMenu(3, "Transform Mode: WCS", subMenuTransformMode);
		app.coordinateSystem = WCS;
		break;
	case FACE_FILL:
		glutChangeToSubMenu(4, "Rendering: Face fill", subMenuRendering);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case WIRE_FRAME:
		glutChangeToSubMenu(4, "Rendering: Wireframe", subMenuRendering);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case CULLING_OFF:
		glutChangeToSubMenu(5, "Culling: OFF", subMenuCulling);
		glDisable(GL_CULL_FACE);
		break;
	case CULLING_ON:
		glutChangeToSubMenu(5, "Culling: ON", subMenuCulling);
		glEnable(GL_CULL_FACE);
		break;

	default:
		break;
	}
}

static void setMeshRenderingMode(int option)
{
	glutSetMenu(menu);
	glutTimerFunc(1, updateMenu, option);
}

static void setCulling(int option)
{
	glutSetMenu(menu);
	glutTimerFunc(1, updateMenu, option);
}

static void setTransformMode(CoordinateSystem option)
{
	glutSetMenu(menu);
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
	subMenuTransformMode = glutCreateMenu(setTransformMode);
	glutAddMenuEntry("OCS", CHANGE_TO_OCS);
	glutAddMenuEntry("WCS", CHANGE_TO_WCS);

	subMenuRendering = glutCreateMenu(setMeshRenderingMode);
	glutAddMenuEntry("Face fill", FACE_FILL);
	glutAddMenuEntry("Wireframe", WIRE_FRAME);

	subMenuCulling = glutCreateMenu(setCulling);
	glutAddMenuEntry("OFF", CULLING_OFF);
	glutAddMenuEntry("ON", CULLING_ON);

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
		glutAddMenuEntry(getShaderName(i).c_str(), i);
	}

	menu = glutCreateMenu(NULL); // richiama main_menu_func() alla selezione di una voce menu
	glutAddMenuEntry("Opzioni", -1); // -1 significa che non si vuole gestire questa riga
	glutAddMenuEntry("", -1);
	glutAddSubMenu("Transform Mode: WCS", subMenuTransformMode);
	glutAddSubMenu("Rendering: Face fill", subMenuRendering);
	glutAddSubMenu("Culling: OFF", subMenuCulling);
	glutAddSubMenu("Material", materialSubmenu);
	glutAddSubMenu("Shader", shaderSubmenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
