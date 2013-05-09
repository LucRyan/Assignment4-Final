#ifndef __WorldBuilder_h_
#define __WorldBuilder_h_

#include "Final.h"
#include "WaterBuilder.h"

class WorldBuilder{
public:
	WorldBuilder() : seed_(801){

	}

	void createPalms(Ogre::SceneManager *mSceneMgr)
	{
		const int NumberOfPalms = 30;

		Ogre::SceneNode* mPalmsSceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();

		for (int k = 0; k < NumberOfPalms; k++)
		{
			Ogre::Vector3 RandomPos;

			RandomPos = Ogre::Vector3(rnd_(100,3000),   rnd_(60,125),   rnd_(100,3000));

			Ogre::Entity *mPalmEnt = mSceneMgr->createEntity("Palm"+Ogre::StringConverter::toString(k), "Palm.mesh");
			Ogre::SceneNode *mPalmSN = mPalmsSceneNode->createChildSceneNode();

			mPalmSN->rotate(Ogre::Vector3(-1,0,rnd_(-0.3,0.3)), Ogre::Degree(90));
			mPalmSN->attachObject(mPalmEnt);
			Ogre::Real Scale = rnd_(50,75);
			mPalmSN->scale(Scale,Scale,Scale);
			mPalmSN->setPosition(RandomPos);
		}
	}

	void createHouse(Ogre::SceneManager *mSceneMgr)
	{
		Ogre::Vector3 Pos;

		Pos = Ogre::Vector3(1000, 500, 400);

		Ogre::Entity *mHouseEnt = mSceneMgr->createEntity("House", "tudorhouse.mesh");
		Ogre::SceneNode* mHouseSN = mSceneMgr->getRootSceneNode()->createChildSceneNode()->createChildSceneNode("House");
		mHouseSN->attachObject(mHouseEnt);
		Ogre::Real Scale = 0.6;
		mHouseSN->scale(Scale,Scale,Scale);
		mHouseSN->setPosition(Pos);
	}

	void createBasic(Ogre::SceneManager *mSceneMgr, Ogre::Camera *mCamera){
		// Set default ambient light
		mSceneMgr->setAmbientLight(ColourValue(1, 1, 1));

		// Set the Shadow
		mSceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED);

		// Set some camera params
		mCamera->setFarClipDistance(999999*6);
		mCamera->setPosition(312.902,306.419,1024.02);
		mCamera->setOrientation(Ogre::Quaternion(0.998, -0.0121, -0.0608, -0.00074));

		WaterBuilder::getSingleton().setupSky(mSceneMgr);
	}

protected:

	float seed_;
		
	/** 
	 * Just to locate palms with a pseudo-random algorithm
	 */

	float rnd_(const float& min, const float& max)
	{
		seed_ += Ogre::Math::PI*2.8574f + seed_*(0.3424f - 0.12434f + 0.452345f);
		if (seed_ > 10000000000) seed_ -= 10000000000;
		return ((max-min)*Ogre::Math::Abs(Ogre::Math::Sin(Ogre::Radian(seed_))) + min);
	}

};

#endif