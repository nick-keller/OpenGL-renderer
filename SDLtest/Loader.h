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

inline vec3 operator /(const vec3 a, const float b) {
	vec3 ret = a;
	ret.x /= b;
	ret.y /= b;
	ret.z /= b;
	return ret;
}

inline vec3 operator *(const vec3 a, const float b) {
	vec3 ret = a;
	ret.x *= b;
	ret.y *= b;
	ret.z *= b;
	return ret;
}

inline vec3 operator +(const vec3 a, const float b) {
	vec3 ret = a;
	ret.x += b;
	ret.y += b;
	ret.z += b;
	return ret;
}

inline vec3 operator -(const vec3 a, const float b) {
	vec3 ret = a;
	ret.x -= b;
	ret.y -= b;
	ret.z -= b;
	return ret;
}

class Loader

{
private:
	void loadMeshes(string const& pFilePath);
	void loadTextures(string & pFilePath);
	void loadMap(string pFilePath);
	void loadShaders(string pFilePath);

	void placeObject(vec3& coordinates, string label, string placement, string refObject, vec3& offset);
	double sum(double val, double n);


public:
	Loader(Scene* pScene);
	void load();
	~Loader(void);



private:
	Scene *  m_scene;

};


