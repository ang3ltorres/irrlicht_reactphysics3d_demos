#ifndef CAMERA_HPP
#define CAMERA_HPP

namespace irr::scene
{
	class ISceneManager;
	class ICameraSceneNode;
}

class Camera
{
public:
	Camera(irr::scene::ISceneManager* smgr);
	~Camera(void) = default;

	void Rotate(int dir);
	void UpDown(int dir);
	void Zoom(int zoom);


private:
	void Update(void);
	
	irr::scene::ICameraSceneNode* cam;
	float radius;
	float angle;
	float height;
};

#endif