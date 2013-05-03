#ifndef __Application_h_
#define __Application_h_

#include "Final.h"
#include "WaterBuilder.h"
#include "TerrainBuilder.h"
#include "WorldBuilder.h"
#include "MyExampleFrameListener.h"

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

    // Just override the mandatory create scene method
    void createScene(void)
    {
		// Setup Physics
		PhysicsBuilder::getSingleton().setupPhysics(mSceneMgr);
		// Create Basic Secne - Light, Camer, Shadow
		worldBuilder->createBasic(mSceneMgr, mCamera);
		// Load island
		terrain->configureTerrain(mSceneMgr);      
		// Create Water
		WaterBuilder::getSingleton().configureWater(mSceneMgr, mCamera, mWindow);
		// Create palms
		worldBuilder->createPalms(mSceneMgr);
		// Create House
		worldBuilder->createHouse(mSceneMgr);
		// Add frame listener
		mRoot->addFrameListener(new MyExampleFrameListener(mWindow, mCamera, mSceneMgr));

    }

	void chooseSceneManager()
	{       
		// Create the SceneManager, in this case a generic one
		mSceneMgr = mRoot->createSceneManager(ST_GENERIC, "ExampleSMInstance");
	}

};


#endif // #ifndef __Application_h_