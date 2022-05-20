#include "camera.hpp"
#include <irrlicht/ICameraSceneNode.h>
#include <irrlicht/ISceneManager.h>

using namespace irr;
using namespace core;
using namespace scene;

Camera::Camera(ISceneManager* smgr)
: radius(100), angle(0), height(50)
{
	this->cam = smgr->addCameraSceneNode(0, vector3df(0, 20, -80), vector3df(0, 0, 0), -1, false);
	this->cam->setFOV(PI / 2.0);
	this->cam->setUpVector(vector3df(0, 1, 0));
	this->cam->setPosition(vector3df(radius * cos(degToRad(angle)), height, radius * sin(degToRad(angle))));
	
	smgr->setActiveCamera(this->cam);
}

void Camera::Rotate(int dir) {angle += dir; Update();}
void Camera::UpDown(int dir) {height += dir; Update();}
void Camera::Zoom(int zoom) {radius += zoom; Update();}

void Camera::Update(void)
{
	cam->setPosition(vector3df(radius * cos(degToRad(angle)), height, radius * sin(degToRad(angle))));
}