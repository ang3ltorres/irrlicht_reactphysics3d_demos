#include "assets.hpp"
#include <irrlicht/ISceneManager.h>
#include <irrlicht/IMeshSceneNode.h>
#include <reactphysics3d/reactphysics3d.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

Entity::Entity(reactphysics3d::PhysicsCommon* physicsCommon, reactphysics3d::PhysicsWorld* world)
: remove(false), physicsCommon(physicsCommon), world(world)
{
	// Aux
	pos = new reactphysics3d::Vector3();
	rot = new irr::core::vector3df();
	quad = new reactphysics3d::Quaternion();
}

Entity::~Entity(void)
{
	this->world->destroyRigidBody(body);
	this->physicsCommon->destroyBoxShape(shape);
	this->node->remove();

	// Aux
	delete pos;
	delete rot;
	delete quad;
}

void Entity::Update(void)
{
	// Position
	*pos = body->getTransform().getPosition();
	node->setPosition(vector3df(pos->x, pos->y, pos->z));

	// Rotation
	*quad = body->getTransform().getOrientation();
	quaternion(quad->x, quad->y, quad->z, quad->w).toEuler(*rot);
	node->setRotation((*rot) * RADTODEG);
}

Floor::Floor(irr::scene::ISceneManager* smgr, rp3d::PhysicsCommon* physicsCommon,
	rp3d::PhysicsWorld* world, irr::video::ITexture* txr, reactphysics3d::Vector3 pos)
: Entity(physicsCommon, world)
{
	body = world->createRigidBody(rp3d::Transform(pos, rp3d::Quaternion::identity()));
	shape = physicsCommon->createBoxShape(rp3d::Vector3(300 / 2.0f, 20 / 2.0f, 300 / 2.0f));
	collider = body->addCollider(shape, rp3d::Transform::identity());
	body->setType(rp3d::BodyType::STATIC);
	rp3d::Material& material = collider->getMaterial();
	material.setBounciness(0);
	material.setFrictionCoefficient(20);
	material.setMassDensity(0);

	this->node = smgr->addCubeSceneNode(1, nullptr, -1, vector3df(0, 0, 0), vector3df(0, 0, 0), vector3df(300, 20, 300));
	this->node->setMaterialFlag(EMF_LIGHTING, true);
	this->node->setMaterialFlag(EMF_BILINEAR_FILTER, false);
	this->node->setMaterialFlag(EMF_BACK_FACE_CULLING, true);
	this->node->getMaterial(0).EmissiveColor.set(255, 255, 255, 255);
	this->node->setMaterialTexture(0, txr);

	Entity::Update();
}

Box::Box(irr::scene::ISceneManager* smgr, rp3d::PhysicsCommon* physicsCommon,
	rp3d::PhysicsWorld* world, irr::video::ITexture* txr, reactphysics3d::Vector3 pos,
	reactphysics3d::Vector3 size)
: Entity(physicsCommon, world)
{
	body = world->createRigidBody(rp3d::Transform(pos, rp3d::Quaternion::identity()));
	shape = physicsCommon->createBoxShape(rp3d::Vector3(size.x / 2.0f, size.y / 2.0f, size.z / 2.0f));
	body->addCollider(shape, rp3d::Transform::identity());
	body->setType(rp3d::BodyType::DYNAMIC);

	this->node = smgr->addCubeSceneNode(1, nullptr, -1, vector3df(0, 0, 0), vector3df(0, 0, 0), vector3df(size.x, size.y, size.z));
	this->node->setMaterialFlag(EMF_LIGHTING, true);
	this->node->setMaterialFlag(EMF_BILINEAR_FILTER, false);
	this->node->setMaterialFlag(EMF_BACK_FACE_CULLING, true);
	this->node->getMaterial(0).EmissiveColor.set(255, 255, 255, 255);
	this->node->setMaterialTexture(0, txr);

	this->time = 60 * 10;
	Entity::Update();
}

void Box::Update(void)
{
	Entity::Update();
	
	if (time == 0)
		remove = true;
	else
		time--;
}