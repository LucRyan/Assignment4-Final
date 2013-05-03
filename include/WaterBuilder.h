#ifndef __WaterBuilder_h_
#define __WaterBuilder_h_
#include "Final.h"

#define _def_SkyBoxNum 3

class WaterBuilder : public Singleton<WaterBuilder>
{
public:
	//Singleton Function
	static WaterBuilder &getSingleton(){
		if(!msSingleton){
			msSingleton = new WaterBuilder();
		}
		assert( msSingleton );
		return (*msSingleton);
	}

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
	

	void setupSky(SceneManager *mSceneMgr){
		// Create the SkyBox
		mSceneMgr->setSkyBox(true, mSkyBoxes[mCurrentSkyBox], 99999*3, true);
		// Light
		Ogre::Light *mLight = mSceneMgr->createLight("Light0");
		mLight->setPosition(mSunPosition[mCurrentSkyBox]);
		mLight->setDiffuseColour(1, 1, 1);
		mLight->setSpecularColour(mSunColor[mCurrentSkyBox].x,
			mSunColor[mCurrentSkyBox].y,
			mSunColor[mCurrentSkyBox].z);
	}

	void setSunPara(Ogre::Vector3 *mSunPosition, Ogre::Vector3 *mSunColor){
		mHydrax->setSunPosition(mSunPosition[mCurrentSkyBox]);
		mHydrax->setSunColor(mSunColor[mCurrentSkyBox]);
	}

	void changeSkyBox(SceneManager *mSceneMgr)
	{

		mCurrentSkyBox++;

		if(mCurrentSkyBox > (_def_SkyBoxNum-1))
		{
			mCurrentSkyBox = 0;
		}

		// Change skybox
		mSceneMgr->setSkyBox(true, mSkyBoxes[mCurrentSkyBox], 99999*3, true);

		// Update Hydrax sun position and colour
		mHydrax->setSunPosition(mSunPosition[mCurrentSkyBox]);
		mHydrax->setSunColor(mSunColor[mCurrentSkyBox]);

		// Update light 0 light position and colour
		mSceneMgr->getLight("Light0")->setPosition(mSunPosition[mCurrentSkyBox]);
		mSceneMgr->getLight("Light0")->setSpecularColour(mSunColor[mCurrentSkyBox].x,mSunColor[mCurrentSkyBox].y,mSunColor[mCurrentSkyBox].z);

		// Log
		LogManager::getSingleton().logMessage("Skybox " + mSkyBoxes[mCurrentSkyBox] + " selected. ("+Ogre::StringConverter::toString(mCurrentSkyBox+1)+"/"+Ogre::StringConverter::toString(_def_SkyBoxNum)+")");
	}

protected:
	WaterBuilder(){
		mHydrax = 0;

		//SkyBoxes
		mSkyBoxes[0] = "Sky/ClubTropicana";
		mSkyBoxes[1] = "Sky/EarlyMorning";
		mSkyBoxes[2] = "Sky/Clouds";

		mSunPosition[0] = Ogre::Vector3(0,10000,0);
		mSunPosition[1] = Ogre::Vector3(0,10000,90000);
		mSunPosition[2] = Ogre::Vector3(0,10000,0);

		mSunColor[0]= Ogre::Vector3(1, 0.9, 0.6);
		mSunColor[1]= Ogre::Vector3(1,0.6,0.4),
			mSunColor[2]= Ogre::Vector3(0.45,0.45,0.45);

		mCurrentSkyBox = 0;
	}

	~WaterBuilder(){
		delete mHydrax;
		delete mSkyBoxes;
		delete mSunColor;
		delete mSunPosition;
	}

	// Hydrax pointer
	Hydrax::Hydrax *mHydrax; 

	//SkyBoxes
	Ogre::String mSkyBoxes[_def_SkyBoxNum] ;

	Ogre::Vector3 mSunPosition[_def_SkyBoxNum];

	Ogre::Vector3 mSunColor[_def_SkyBoxNum] ;

	int mCurrentSkyBox ;
};


#endif // #ifndef __Water_h_