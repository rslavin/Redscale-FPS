#pragma once

#include "glut.h"
#include <stdlib.h>
#include <stdarg.h>

#include <iostream>
#include <memory.h>
#include <time.h>
#include <fstream>

#include <IL\il.h>
#include <fstream>
#include <map>

#include "assimp.hpp"	//OO version Header!
#include "aiPostProcess.h"
#include "aiScene.h"
#include "DefaultLogger.h"
#include "LogStream.h"

class Loader{
private:


	// images / texture
	std::map<std::string, GLuint*> textureIdMap;	// map image filenames to textureIds
	GLuint*		textureIds;							// pointer to texture Array

	// Create an instance of the Importer class
	Assimp::Importer importer;

	std::string basepath;
	std::string modelname;

	bool loadModel, loadTextures;

	float scale;

	void init();
	bool Import3DFromFile();
	bool LoadGLTextures();
	void Color4f(const struct aiColor4D *color);
	void set_float4(float f[4], float a, float b, float c, float d);
	void color4_to_float4(const struct aiColor4D *c, float f[4]);
	void apply_material(const struct aiMaterial *mtl);
	void recursive_render (const struct aiScene *sc, const struct aiNode* nd);

public:
		// the global Assimp scene object
	const aiScene* scene;	//Model being loaded
	struct aiVector3D scene_min, scene_max, scene_center;
	Loader(std::string dir, std::string name,float scale){basepath = dir, modelname = name;init(); this->scale = scale;}
	~Loader();
	void setScale(float scale){this->scale = scale;}
	void drawModel(){drawAiScene();}
	void drawAiScene();
	GLboolean abortGLInit(const char* abortMessage);
};