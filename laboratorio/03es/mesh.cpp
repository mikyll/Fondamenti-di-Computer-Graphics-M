#include "mesh.h"

static int loadObjFile(std::string file_path, Mesh* mesh, bool vertices_normals);
static void generateAndLoadBuffers(bool generate, Mesh* mesh);



std::vector<Object> objects;


/*
* Load a set of vertices and save it in a Mesh data structure
*/
static int loadObjFile(std::string file_path, Mesh* mesh, bool vertices_normals)
{
	FILE* file = fopen(file_path.c_str(), "r");
	if (file == NULL) {
		std::cerr << "\nFailed to open obj file! --> " << file_path << std::endl;
		return -1;
	}
	// tmp data structures
	std::vector<GLuint> vertexIndices, normalIndices, uvIndices;
	std::vector<glm::vec3> tmp_vertices, tmp_normals;
	std::vector<glm::vec2> tmp_uvs;

	char lineHeader[128];
	while (fscanf(file, "%s", lineHeader) != EOF) {
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, " %f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			tmp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, " %f %f %f\n", &normal.x, &normal.y, &normal.z);
			tmp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, " %f %f\n", &uv.x, &uv.y);
			uv.y = 1 - uv.y;
			tmp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			GLuint v_a, v_b, v_c; // index in position array
			GLuint n_a, n_b, n_c; // index in normal array
			GLuint t_a, t_b, t_c; // index in UV array

			fscanf(file, "%s", lineHeader);
			if (strstr(lineHeader, "//")) { // case: v//n v//n v//n
				sscanf(lineHeader, "%d//%d", &v_a, &n_a);
				fscanf(file, "%d//%d %d//%d\n", &v_b, &n_b, &v_c, &n_c);
				n_a--, n_b--, n_c--;
				normalIndices.push_back(n_a); normalIndices.push_back(n_b); normalIndices.push_back(n_c);
			}
			else if (strstr(lineHeader, "/")) {// case: v/t/n v/t/n v/t/n
				sscanf(lineHeader, "%d/%d/%d", &v_a, &t_a, &n_a);
				fscanf(file, "%d/%d/%d %d/%d/%d\n", &v_b, &t_b, &n_b, &v_c, &t_c, &n_c);
				n_a--, n_b--, n_c--;
				t_a--, t_b--, t_c--;
				normalIndices.push_back(n_a); normalIndices.push_back(n_b); normalIndices.push_back(n_c);
				uvIndices.push_back(t_a); uvIndices.push_back(t_b); uvIndices.push_back(t_c);
			}
			else {// case: v v v
				sscanf(lineHeader, "%d", &v_a);
				fscanf(file, "%d %d\n", &v_b, &v_c);
			}
			v_a--; v_b--; v_c--;
			vertexIndices.push_back(v_a); vertexIndices.push_back(v_b); vertexIndices.push_back(v_c);
		}
	}
	fclose(file);

	// If normals and uvs are not loaded, we calculate normals for vertex
	if (tmp_normals.size() == 0) {
		tmp_normals.resize(vertexIndices.size() / 3, glm::vec3(0.0, 0.0, 0.0));
		// normal of each face saved 1 time PER FACE!
		for (int i = 0; i < vertexIndices.size(); i += 3)
		{
			GLushort ia = vertexIndices[i];
			GLushort ib = vertexIndices[i + 1];
			GLushort ic = vertexIndices[i + 2];
			glm::vec3 normal = glm::normalize(glm::cross(
				glm::vec3(tmp_vertices[ib]) - glm::vec3(tmp_vertices[ia]),
				glm::vec3(tmp_vertices[ic]) - glm::vec3(tmp_vertices[ia])));

			if (vertices_normals)
			{
				// Vertices normals
				tmp_normals[ia] += normal;
				tmp_normals[ib] += normal;
				tmp_normals[ic] += normal;
				// Put an index to the normal for all 3 vertex of the face
				normalIndices.push_back(ia);
				normalIndices.push_back(ib);
				normalIndices.push_back(ic);
			}
			else
			{
				// Faces normals
				tmp_normals[i / 3] = normal;
				// Put an index to the normal for all 3 vertex of the face
				normalIndices.push_back(i / 3);
				normalIndices.push_back(i / 3);
				normalIndices.push_back(i / 3);
			}
		}
	}

	// We prepare the data for glDrawArrays calls, this is a simple but non optimal way of storing mesh data.
	// However, you could optimize the mesh data using a index array for both vertex positions, 
	// normals and textures and later use glDrawElements
	int i = 0;
	// Now following the index arrays, we build the final arrays that will contain the data for glDrawArray...
	for (int i = 0; i < vertexIndices.size(); i++) {
		mesh->vertices.push_back(tmp_vertices[vertexIndices[i]]);
		//mesh->normals.push_back(tmp_normals[normalIndices[i]]);
		mesh->normals.push_back(glm::normalize(tmp_normals[normalIndices[i]]));
	}
}

static void generateAndLoadBuffers(bool generate, Mesh* mesh)
{
	if (generate) {
		// Genero 1 Vertex Array Object
		glGenVertexArrays(1, &mesh->vertexArrayObjID);
		// Genero 1 Vertex Buffer Object per i vertici
		glGenBuffers(1, &mesh->vertexBufferObjID);
		// Genero 1 Buffer Object per le normali
		glGenBuffers(1, &mesh->normalBufferObjID);
	}

	glBindVertexArray(mesh->vertexArrayObjID);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(glm::vec3), &mesh->vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,					// attribute index in the shader
		3,                  // size
		GL_FLOAT,           // type
		false,              // normalized 
		0,					// stride
		(void*)0            // array buffer offset
	);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->normalBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, mesh->normals.size() * sizeof(glm::vec3), mesh->normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,					// attribute index in the shader
		3,                  // size
		GL_FLOAT,           // type
		false,              // normalized 
		0,					// stride
		(void*)0            // array buffer offset
	);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void initMesh(
	std::string filename,
	std::string name,
	bool verticesNormals,
	glm::vec3 position,
	glm::vec3 rotation,
	glm::vec3 scale,
	MaterialType material,
	ShadingType shader)
{
	Mesh sphereS = {};
	if (!loadObjFile(MESH_DIR + filename, &sphereS, verticesNormals))
	{
		std::getchar();
		exit(EXIT_FAILURE);
	}
	generateAndLoadBuffers(true, &sphereS);

	Object obj = {};
	obj.mesh = sphereS;
	obj.material = material;
	obj.shader = shader;
	obj.name = name;

	// Transformations
	obj.M = glm::translate(glm::mat4(1), position);
	if (rotation != glm::vec3(0.0f, 0.0f, 0.0f))
	{
		obj.M = glm::rotate(obj.M, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		obj.M = glm::rotate(obj.M, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		obj.M = glm::rotate(obj.M, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	obj.M = glm::scale(obj.M, scale);
	objects.push_back(obj);
}