#pragma once

#include <QOpenGLExtraFunctions>

#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>

#include "Mesh.h"
#include "GLShader.h"
#include "Object.h"

using namespace std;

class Model : public Object, protected QOpenGLExtraFunctions
{
public:
	/*  Model Data */
	vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	vector<Mesh> meshes;
	std::string directory;
	bool gammaCorrection;

	/*  Functions  */
	// constructor, expects a filepath to a 3D model.
	Model(std::string const &path, bool gamma = false);

	// draws the model, and thus all its meshes
	virtual void draw(GLShader shader);

private:
	/*  Functions  */
	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void loadModel(std::string const &path);

	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode *node, const aiScene *scene);

	Mesh processMesh(aiMesh *mesh, const aiScene *scene);

	// checks all material textures of a given type and loads the textures if they're not loaded yet.
	// the required info is returned as a Texture struct.
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);

	unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);
};


