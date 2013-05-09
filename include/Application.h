#ifndef __Application_h_
#define __Application_h_

#include "Final.h"
#include "WaterBuilder.h"
#include "TerrainBuilder.h"
#include "WorldBuilder.h"
#include "CubeMapping.h"
#include "SphereMapping.h"
#include "MyExampleFrameListener.h"
#include "ControllerMgr.h"

class SampleApp : public ExampleApplication
{
public:
	// Basic constructor
	SampleApp(){
		worldBuilder = new WorldBuilder();
		terrain  = new TerrainBuilder();	
	}

protected:
	WorldBuilder *worldBuilder;
	TerrainBuilder *terrain;
	ControllerMgr * mCharacter;

    // Just override the mandatory create scene method
    void createScene(void)
    {
		// Setup Physics
		PhysicsBuilder::getSingleton().setupPhysics(mSceneMgr);
		// Create Basic Secne - Light, Camer, Shadow
		worldBuilder->createBasic(mSceneMgr, mCamera);
		// Load island
		terrain->configureTerrain(mSceneMgr);      
		// Create palms
		worldBuilder->createPalms(mSceneMgr);
		// Create House
		worldBuilder->createHouse(mSceneMgr);
		// Add Actor
		setupCharacter();
		// Create CubeMap
		MyExampleFrameListener *mFrameListener = new MyExampleFrameListener(mWindow, mCamera, mSceneMgr,mCharacter);
		CubeMapping::getSingleton().setupContent(mSceneMgr,mFrameListener);
		// Create SphereMap
		SphereMapping::getSingleton().setupContent(mSceneMgr,mFrameListener,mWindow);
		// Create Water
		WaterBuilder::getSingleton().configureWater(mSceneMgr, mCamera, mWindow);

		// Add frame listener
		mRoot->addFrameListener(mFrameListener);
    }

	void chooseSceneManager()
	{       
		// Create the SceneManager, in this case a generic one
		mSceneMgr = mRoot->createSceneManager(ST_GENERIC, "ExampleSMInstance");
	}

	
	void createFrameListener()override{
		
	}

	void setupCharacter(){
		btTransform startTransform;
		startTransform.setIdentity();
		startTransform.setOrigin(btVector3(10, 500, 500));
		Vector3 origin(10, 500, 500);

		btPairCachingGhostObject * characterGhostObject = new btPairCachingGhostObject();
		characterGhostObject->setWorldTransform(startTransform);

		btScalar characterHeight = 80.f;
		btScalar characterWidth = 10.f;

		//OgreBulletCollisions::CapsuleCollisionShape * capsule = new OgreBulletCollisions::CapsuleCollisionShape(characterWidth, characterHeight, Vector3(0,0,0));
		btConvexShape * capsule = new btCapsuleShape(characterWidth, characterHeight);

		PhysicsBuilder::getSingleton().addCollisionShape((OgreBulletCollisions::CapsuleCollisionShape*)capsule);
		characterGhostObject->setCollisionShape(capsule);
		characterGhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

		// duck setup
		//OgreBulletCollisions::CapsuleCollisionShape *  duck = new OgreBulletCollisions::CapsuleCollisionShape(characterWidth, characterHeight / 3, Vector3(0,0,0));
		btConvexShape * duck = new btCapsuleShape(characterWidth, characterHeight/3);
		PhysicsBuilder::getSingleton().addCollisionShape((OgreBulletCollisions::CapsuleCollisionShape*)duck);

		btScalar stepHeight = 3.5f;
		mCharacter = new ControllerMgr(mSceneMgr, mCamera, characterGhostObject, capsule, stepHeight, 
									   PhysicsBuilder::getSingleton().getWorld()->getBulletCollisionWorld(), origin);
		mCharacter->getCCPhysics()->setDuckingConvexShape(duck);

		PhysicsBuilder::getSingleton().getWorld()->getBulletDynamicsWorld()->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

		PhysicsBuilder::getSingleton().getWorld()->getBulletDynamicsWorld()->addCollisionObject(characterGhostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
		PhysicsBuilder::getSingleton().getWorld()->getBulletDynamicsWorld()->addAction(mCharacter->getCCPhysics());
	}

};


#endif // #ifndef __Application_h_