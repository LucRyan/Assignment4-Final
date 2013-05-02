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
	SampleApp(){}

protected:


    // Just override the mandatory create scene method
    void createScene(void)
    {
		// Set default ambient light
		mSceneMgr->setAmbientLight(ColourValue(1, 1, 1));

		// Set the Shadow
		mSceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED);

        // Create the SkyBox
        mSceneMgr->setSkyBox(true, mSkyBoxes[mCurrentSkyBox], 99999*3, true);

		// Set some camera params
        mCamera->setFarClipDistance(999999*6);
		mCamera->setPosition(312.902,306.419,1024.02);
		mCamera->setOrientation(Ogre::Quaternion(0.998, -0.0121, -0.0608, -0.00074));

	    // Light
		Ogre::Light *mLight = mSceneMgr->createLight("Light0");
		mLight->setPosition(mSunPosition[mCurrentSkyBox]);
		mLight->setDiffuseColour(1, 1, 1);
		mLight->setSpecularColour(mSunColor[mCurrentSkyBox].x,
			                      mSunColor[mCurrentSkyBox].y,
								  mSunColor[mCurrentSkyBox].z);

		// Load island
		TerrainBuilder *terrain = new TerrainBuilder();
		terrain->configureTerrain(mSceneMgr);      

		// Create Water
		WaterBuilder *water = new WaterBuilder();
		water->configureWater(mSceneMgr, mCamera, mWindow);

		// Create palms
		WorldBuilder *worldBuilder = new WorldBuilder();
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