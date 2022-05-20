#include <irrlicht/irrlicht.h>
#include <reactphysics3d/reactphysics3d.h>
#include <new>
#include <cstring>
#include <ctime>
#include <cstdlib>

#include "input.hpp"
#include "camera.hpp"
#include "assets.hpp"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

IrrlichtDevice* device;
IVideoDriver* driver;
ISceneManager* smgr;
IGUIEnvironment* guienv;

int main(void)
{
	srand(time(nullptr));

	Input input;
	device = createDevice(EDT_OPENGL, dimension2d<u32>(1920, 1080), 32, true, false, true, &input);

	device->setWindowCaption(L"Test");
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();

	driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, false);

	/* Physics */
	rp3d::PhysicsCommon physicsCommon;

	rp3d::PhysicsWorld::WorldSettings settings;
	settings.worldName = "TEST";
	settings.gravity = rp3d::Vector3(0, -9.81f, 0);
	settings.persistentContactDistanceThreshold = 0.03f;
	settings.defaultFrictionCoefficient = 20.0f;
	settings.defaultBounciness = 0.0f;
	settings.restitutionVelocityThreshold = 1.0f;
	settings.isSleepingEnabled = true;
	settings.defaultVelocitySolverNbIterations = 32;
	settings.defaultPositionSolverNbIterations = 3;
	settings.defaultTimeBeforeSleep = 1.5f;
	settings.defaultSleepLinearVelocity = 5.0f;
	settings.defaultSleepAngularVelocity = 5.0f * (rp3d::PI_RP3D / 180.0f);
	settings.cosAngleSimilarContactManifold = 0.95f;

	rp3d::PhysicsWorld* world = physicsCommon.createPhysicsWorld(settings);

	// Floor
	ITexture* txrFloor = driver->getTexture("res/Tile_13-128x128.png");
	Floor* floor = new Floor(smgr, &physicsCommon, world, txrFloor, rp3d::Vector3(0, 0, 0));

	// Box
	ITexture* txrBox = driver->getTexture("res/Brick_11-128x128.png");

	unsigned int boxMax = 512;
	unsigned int boxCount = 0;

	Box* box = (Box*)::operator new(sizeof(Box) * boxMax);

	// Camera
	Camera camera(smgr);

	while(device->run())
	{

		/* Update */

		// Physics
		world->update(1.0f / 15.0f);

		// Entities
		if ((input.key[KEY_SPACE]) and (boxCount < boxMax))
		{
			//num = (rand() % (upper - lower + 1)) + lower;
			unsigned char x, y, z;
			x = (rand() % 14) + 5;
			y = (rand() % 14) + 5;
			z = (rand() % 14) + 5;

			signed short posX, posZ;
			posX = (rand() % (200 + 1)) - 100;
			posZ = (rand() % (200 + 1)) - 100;

			new(&box[boxCount]) Box(smgr, &physicsCommon, world, txrBox, rp3d::Vector3(posX, 200, posZ), rp3d::Vector3(x, y, z));
			boxCount++;
		}

		for (unsigned int i = 0; i < boxCount; i++)
		{
			box[i].Update();

			if (box[i].remove)
			{
				boxCount--;
				
				box[i].~Box();
				if (i != boxCount)
					memcpy((void*)&box[i], (void*)&box[boxCount], sizeof(Box));
				
				i--;
			}
		}

		// Camera
		if (input.key[KEY_KEY_Q])
			camera.Rotate(-1);
		else if (input.key[KEY_KEY_E])
			camera.Rotate(1);

		if (input.key[KEY_KEY_S])
			camera.UpDown(-1);
		else if (input.key[KEY_KEY_W])
			camera.UpDown(1);

		if (input.key[KEY_PLUS])
			camera.Zoom(-1);
		else if (input.key[KEY_MINUS])
			camera.Zoom(1);

		/* Draw */
		driver->beginScene(true, true, SColor(255, 0, 0, 0));

		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();
	}

	delete floor;

	for (unsigned int i = 0; i < boxCount; i++)
		box[i].~Box();
	::operator delete(box);

	physicsCommon.destroyPhysicsWorld(world);

	driver->removeAllTextures();
	device->drop();

	return 0;
}