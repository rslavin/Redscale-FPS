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

// assimp include files. These three are usually needed.
#include "assimp.hpp"	//OO version Header!
#include "aiPostProcess.h"
#include "aiScene.h"
#include "DefaultLogger.h"
#include "LogStream.h"

/*// the global Assimp scene object
extern const aiScene* scene;
extern GLuint scene_list;
extern struct aiVector3D scene_min, scene_max, scene_center;

// images / texture
extern std::map<std::string, GLuint*> textureIdMap;	// map image filenames to textureIds
extern GLuint*		textureIds;							// pointer to texture Array

// Create an instance of the Importer class
extern Assimp::Importer importer;

extern std::string basepath;
extern std::string modelname;
*/
void createAILogger();
void destroyAILogger();
void logInfo(std::string logString);
void logDebug(const char* logString);
bool Import3DFromFile( const std::string& pFile);
int LoadGLTextures(const aiScene* scene);
void Color4f(const struct aiColor4D *color);
void set_float4(float f[4], float a, float b, float c, float d);
void color4_to_float4(const struct aiColor4D *c, float f[4]);
void apply_material(const struct aiMaterial *mtl);
void recursive_render (const struct aiScene *sc, const struct aiNode* nd, float scale);
void drawAiScene(const aiScene* scene);
GLboolean abortGLInit(const char* abortMessage);