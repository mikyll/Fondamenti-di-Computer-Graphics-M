#include "mesh.h"

static int loadObjFile(std::string file_path, Mesh* mesh, bool vertices_normals);
static void generateAndLoadBuffers(bool generate, Mesh* mesh);


/*
* Reads a .obj file and initializes the mesh vectors of the Mesh data structure.
*/
static int loadObjFile(std::string file_path, Mesh* mesh, bool vertices_normals)
{
	FILE* file;
	if (fopen_s(&file, file_path.c_str(), "r") != 0)
	{
		std::cerr << "\nFailed to open obj file! --> " << file_path << std::endl;
		return -1;
	}
	// tmp data structures
	std::vector<GLuint> vertexIndices, normalIndices, uvIndices;
	std::vector<glm::vec3> tmp_vertices, tmp_normals;
	std::vector<glm::vec2> tmp_uvs;

	const int MAX_SIZE = 128;
	char lineHeader[MAX_SIZE];
	try {
		while (fscanf_s(file, "%s", lineHeader, MAX_SIZE) != EOF) {
			if (strcmp(lineHeader, "v") == 0) {
				glm::vec3 vertex;
				fscanf_s(file, " %f %f %f\n", &vertex.x, &vertex.y, &vertex.z, MAX_SIZE);
				tmp_vertices.push_back(vertex);
			}
			else if (strcmp(lineHeader, "vn") == 0) {
				glm::vec3 normal;
				fscanf_s(file, " %f %f %f\n", &normal.x, &normal.y, &normal.z, MAX_SIZE);
				tmp_normals.push_back(normal);
			}
			else if (strcmp(lineHeader, "vt") == 0) {
				glm::vec2 uv;
				fscanf_s(file, " %f %f\n", &uv.x, &uv.y, MAX_SIZE);
				uv.y = 1 - uv.y;
				tmp_uvs.push_back(uv);
			}
			else if (strcmp(lineHeader, "f") == 0) {
				GLuint v_a, v_b, v_c; // index in position array
				GLuint n_a, n_b, n_c; // index in normal array
				GLuint t_a, t_b, t_c; // index in UV array

				fscanf_s(file, "%s", lineHeader, MAX_SIZE);
				if (strstr(lineHeader, "//")) { // case: v//n v//n v//n
					sscanf_s(lineHeader, "%d//%d", &v_a, &n_a, MAX_SIZE);
					fscanf_s(file, "%d//%d %d//%d\n", &v_b, &n_b, &v_c, &n_c, MAX_SIZE);
					n_a--, n_b--, n_c--;
					normalIndices.push_back(n_a); normalIndices.push_back(n_b); normalIndices.push_back(n_c);
				}
				else if (strstr(lineHeader, "/")) {// case: v/t/n v/t/n v/t/n
					sscanf_s(lineHeader, "%d/%d/%d", &v_a, &t_a, &n_a, MAX_SIZE);
					fscanf_s(file, "%d/%d/%d %d/%d/%d\n", &v_b, &t_b, &n_b, &v_c, &t_c, &n_c, MAX_SIZE);
					n_a--, n_b--, n_c--;
					t_a--, t_b--, t_c--;
					normalIndices.push_back(n_a); normalIndices.push_back(n_b); normalIndices.push_back(n_c);
					uvIndices.push_back(t_a); uvIndices.push_back(t_b); uvIndices.push_back(t_c);
				}
				else {// case: v v v
					sscanf_s(lineHeader, "%d", &v_a, MAX_SIZE);
					fscanf_s(file, "%d %d\n", &v_b, &v_c, MAX_SIZE);
				}
				v_a--; v_b--; v_c--;
				vertexIndices.push_back(v_a); vertexIndices.push_back(v_b); vertexIndices.push_back(v_c);
			}
		}
		fclose(file);
	}
	catch (...) {
		return -2;
	}
	
	// If normals and uvs are not loaded, we calculate normals for vertex
	if (tmp_normals.size() == 0) {
		tmp_normals.resize(vertexIndices.size() / 3, glm::vec3(0.0, 0.0, 0.0));
		// normal of each face saved 1 time PER FACE!
		for (int i = 0; i < vertexIndices.size(); i += 3)
		{
			GLushort ia = vertexIndices.at(i);
			GLushort ib = vertexIndices.at(i + 1);
			GLushort ic = vertexIndices.at(i + 2);
			glm::vec3 normal = glm::normalize(glm::cross(
				glm::vec3(tmp_vertices.at(ib)) - glm::vec3(tmp_vertices.at(ia)),
				glm::vec3(tmp_vertices.at(ic)) - glm::vec3(tmp_vertices.at(ia))));

			if (vertices_normals)
			{
				// Vertices normals
				tmp_normals.at(ia) += normal;
				tmp_normals.at(ib) += normal;
				tmp_normals.at(ic) += normal;
				// Put an index to the normal for all 3 vertex of the face
				normalIndices.push_back(ia);
				normalIndices.push_back(ib);
				normalIndices.push_back(ic);
			}
			else
			{
				// Faces normals
				tmp_normals.at(i / 3) = normal;
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
		mesh->vertices.push_back(tmp_vertices.at(vertexIndices.at(i)));
		//mesh->normals.push_back(tmp_normals.at(normalIndices.at(i)));
		mesh->normals.push_back(glm::normalize(tmp_normals.at(normalIndices.at(i))));
	}

	return 0;
}

/*
* Generates buffers for the Mesh structure and saves its OpenGL pointers.
*/
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

/*
* verticesNormals specifies if the normals must be set:
*	- on vertices (true), -> smooth surface;
*	- on faces (false), -> checkered surface;
* rotation is a vec3 containing the angle the object must be
*	rotated (in degrees) for each axis (X, Y, Z)
*/
int loadMesh(std::string filename, bool verticesNormals, Mesh* mesh)
{
	*mesh = {};
	if (int res = loadObjFile(MESH_DIR + filename, mesh, verticesNormals) < 0)
	{
		if (res == -1)
			std::cout << "File '" << MESH_DIR + filename << "' not found." << std::endl;
		if (res == -2)
			std::cout << "Error while loading file '" << MESH_DIR + filename << "'." << std::endl;

		return -1;
	}
	generateAndLoadBuffers(true, mesh);

	return 0;
}





// ====================================================================
// ====================================================================
// EXTRA ==============================================================
static int loadBrokenObjFile(std::string file_path, Mesh* mesh, bool vertices_normals)
{
	FILE* file;
	if (fopen_s(&file, file_path.c_str(), "r") < 0) {
		std::cerr << "\nFailed to open obj file! --> " << file_path << std::endl;
		return -1;
	}
	// tmp data structures
	std::vector<GLuint> vertexIndices, normalIndices, uvIndices;
	std::vector<glm::vec3> tmp_vertices, tmp_normals;
	std::vector<glm::vec2> tmp_uvs;

	char lineHeader[128];
	while (fscanf_s(file, "%s", lineHeader) != EOF) {
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf_s(file, " %f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			tmp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf_s(file, " %f %f %f\n", &normal.x, &normal.y, &normal.z);
			tmp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf_s(file, " %f %f\n", &uv.x, &uv.y);
			uv.y = 1 - uv.y;
			tmp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			GLuint v_a, v_b, v_c; // index in position array
			GLuint n_a, n_b, n_c; // index in normal array
			GLuint t_a, t_b, t_c; // index in UV array

			fscanf_s(file, "%s", lineHeader);
			if (strstr(lineHeader, "//")) { // case: v//n v//n v//n
				sscanf_s(lineHeader, "%d//%d", &v_a, &n_a);
				fscanf_s(file, "%d//%d %d//%d\n", &v_b, &n_b, &v_c, &n_c);
				n_a--, n_b--, n_c--;
				normalIndices.push_back(n_a); normalIndices.push_back(n_b); normalIndices.push_back(n_c);
			}
			else if (strstr(lineHeader, "/")) {// case: v/t/n v/t/n v/t/n
				sscanf_s(lineHeader, "%d/%d/%d", &v_a, &t_a, &n_a);
				fscanf_s(file, "%d/%d/%d %d/%d/%d\n", &v_b, &t_b, &n_b, &v_c, &t_c, &n_c);
				n_a--, n_b--, n_c--;
				t_a--, t_b--, t_c--;
				normalIndices.push_back(n_a); normalIndices.push_back(n_b); normalIndices.push_back(n_c);
				uvIndices.push_back(t_a); uvIndices.push_back(t_b); uvIndices.push_back(t_c);
			}
			else {// case: v v v
				sscanf_s(lineHeader, "%d", &v_a);
				fscanf_s(file, "%d %d\n", &v_b, &v_c);
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

int loadBrokenMesh(std::string filename, bool verticesNormals, Mesh* mesh)
{
	*mesh = {};
	if (int res = loadBrokenObjFile(MESH_DIR + filename, mesh, verticesNormals) < 0)
	{
		if (res == -1)
			std::cout << "File '" << MESH_DIR + filename << "' not found." << std::endl;
		if (res == -2)
			std::cout << "Error while loading file '" << MESH_DIR + filename << "'." << std::endl;

		return -1;
	}
	generateAndLoadBuffers(true, mesh);

	return 0;
}