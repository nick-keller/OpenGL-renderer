#pragma once
#include <iostream>
#include <SDL2\SDL_thread.h>
#include "Scene.h"
#include <thread>



using namespace std;



/**
*
* Must load async. -> .Obj, Textures, Coordinates on the map
*
*
*
**/


inline bool operator |(const MeshList& a, const string& value) {
	if (a.at(value) != nullptr) return true;
	return false;
}

class Loader

{
private:
	void loadMeshes(string const& pFilePath);
	void loadTextures(string & pFilePath);
	void loadMap(string pFilePath);
	void loadShaders(string pFilePath);

public:
	Loader(Scene* pScene);
	void load();
	~Loader(void);



private:
	Scene *  m_scene;

};


