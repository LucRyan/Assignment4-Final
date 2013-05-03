#ifndef __WaterBuilder_h_
#define __WaterBuilder_h_
#include "Final.h"

#define _def_SkyBoxNum 3

// Hydrax pointer
Hydrax::Hydrax *mHydrax = 0;

//SkyBoxes
Ogre::String mSkyBoxes[_def_SkyBoxNum] = 
{"Sky/ClubTropicana",
"Sky/EarlyMorning",
"Sky/Clouds"};

Ogre::Vector3 mSunPosition[_def_SkyBoxNum] = 
{Ogre::Vector3(0,10000,0),
Ogre::Vector3(0,10000,90000),
Ogre::Vector3(0,10000,0)};

Ogre::Vector3 mSunColor[_def_SkyBoxNum] = 
{Ogre::Vector3(1, 0.9, 0.6),
Ogre::Vector3(1,0.6,0.4),
Ogre::Vector3(0.45,0.45,0.45)};

int mCurrentSkyBox = 0;


class WaterBuilder 
{
public:
	WaterBuilder(){}

	void configureWater(Ogre::SceneManager *mSceneMgr, Ogre::Camera *mCamera, Ogre::RenderWindow *mWindow){
		
		// Create Hydrax object
		mHydrax = new Hydrax::Hydrax(mSceneMgr, mCamera, mWindow->getViewport(0));

		// Create our projected grid module  
		Hydrax::Module::ProjectedGrid *mModule 
			= new Hydrax::Module::ProjectedGrid(// Hydrax parent pointer
			mHydrax,
			// Noise module
			new Hydrax::Noise::Perlin(/*Generic one*/),
			// Base plane
			Ogre::Plane(Ogre::Vector3(0,1,0), Ogre::Vector3(0,0,0)),
			// Normal mode
			Hydrax::MaterialManager::NM_VERTEX,
			// Projected grid options
			Hydrax::Module::ProjectedGrid::Options(/*264 /*Generic one*/));

		// Set our module
		mHydrax->setModule(static_cast<Hydrax::Module::Module*>(mModule));

		// Load all parameters from config file
		// Remarks: The config file must be in Hydrax resource group.
		// All parameters can be set/updated directly by code(Like previous versions),
		// but due to the high number of customizable parameters, since 0.4 version, Hydrax allows save/load config files.
		mHydrax->loadCfg("HydraxDemo.hdx");

		// Create water
		mHydrax->create();

		// Hydrax initialization code end -----------------------------------------
		// ------------------------------------------------------------------------
	}

	void update(const FrameEvent &e){
		mHydrax->update(e.timeSinceLastFrame);
	}
	
	void setSunPara(Ogre::Vector3 *mSunPosition, Ogre::Vector3 *mSunColor){
		mHydrax->setSunPosition(mSunPosition[mCurrentSkyBox]);
		mHydrax->setSunColor(mSunColor[mCurrentSkyBox]);
	}

protected:

};


#endif // #ifndef __Water_h_