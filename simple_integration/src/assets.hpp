#ifndef ASSETS_HPP
#define ASSETS_HPP

namespace irr
{
	typedef float f32;
}

namespace irr::core
{
	template <class T>
	class vector3d;
}

namespace irr::scene
{
	class ISceneManager;
	class IMeshSceneNode;
}

namespace irr::video
{
	class ITexture;
}

namespace reactphysics3d
{
	class PhysicsCommon;
	class PhysicsWorld;
	class RigidBody;
	class BoxShape;
	class Collider;

	struct Vector3;
	struct Quaternion;
}

class Entity
{
public:
	Entity(reactphysics3d::PhysicsCommon* physicsCommon, reactphysics3d::PhysicsWorld* world);
	virtual ~Entity(void);

	virtual void Update(void);

	bool remove;
	reactphysics3d::PhysicsCommon* physicsCommon;
	reactphysics3d::PhysicsWorld* world;

	reactphysics3d::RigidBody* body;
	reactphysics3d::BoxShape* shape;
	reactphysics3d::Collider* collider;
	irr::scene::IMeshSceneNode* node;

	// Aux
	reactphysics3d::Vector3* pos;
	irr::core::vector3d<irr::f32>* rot;
	reactphysics3d::Quaternion* quad;
};

class Floor : public Entity
{
public:
	Floor(irr::scene::ISceneManager* smgr, reactphysics3d::PhysicsCommon* physicsCommon,
		reactphysics3d::PhysicsWorld* world, irr::video::ITexture* txr, reactphysics3d::Vector3 pos);

	~Floor(void) = default;
};

class Box : public Entity
{
public:
	Box(irr::scene::ISceneManager* smgr, reactphysics3d::PhysicsCommon* physicsCommon,
		reactphysics3d::PhysicsWorld* world, irr::video::ITexture* txr, reactphysics3d::Vector3 pos,
		reactphysics3d::Vector3 size);

	~Box(void) = default;

	virtual void Update(void) override;


	unsigned int time;
};

#endif