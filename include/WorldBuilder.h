#ifndef __WorldBuilder_h_
#define __WorldBuilder_h_

#include "Final.h"
#include "WaterBuilder.h"
#include "OpenSteer/OpenSteerDemo.h"

class WorldBuilder{
public:
	WorldBuilder() : seed_(801){

	}
	~WorldBuilder()
	{
		delete mFishSwimMap;
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
		//mCamera->setFarClipDistance(999999*6);
		//mCamera->setPosition(312.902,306.419,1024.02);
		//mCamera->setOrientation(Ogre::Quaternion(0.998, -0.0121, -0.0608, -0.00074));

		WaterBuilder::getSingleton().setupSky(mSceneMgr);
	}

	void addBoidsToScene(SceneManager* mSceneMgr){
		OpenSteer::OpenSteerDemo::initialize();
		OpenSteer::OpenSteerDemo::updateSimulationAndRedraw ();  //steer
		const OpenSteer::AVGroup& all = OpenSteer::OpenSteerDemo::allVehiclesOfSelectedPlugIn();
		mFishSwimMap = OGRE_NEW std::map<std::string, AnimationState*>();

		int count = 0;
		Ogre::Entity* fishEnt;
		Ogre::SceneNode* Bird;
		for (OpenSteer::AVIterator i = all.begin(); i != all.end(); i++)
		{
			// Create an Entity
			fishEnt = mSceneMgr->createEntity("BoidsFish"+Ogre::StringConverter::toString(count), "fish.mesh");
			AnimationState* mFishSwim;
			mFishSwim = fishEnt->getAnimationState("swim");
			mFishSwim->setEnabled(true);
			mFishSwimMap->insert(std::make_pair<std::string, AnimationState*>("FishAnimation" + Ogre::StringConverter::toString(count), mFishSwim));
			

			// Create a SceneNode and attach the Entity to it
			Bird = mSceneMgr->getRootSceneNode()->createChildSceneNode("BoidsFish"+Ogre::StringConverter::toString(count));
			Bird->attachObject(fishEnt);
			Bird->scale(3, 3, 3);
			Ogre::Vector3 facing((**i).forward().x,(**i).forward().y,(**i).forward().z);
			Bird->setDirection(facing.x, facing.y, facing.z, Ogre::Node::TS_WORLD, Ogre::Vector3::UNIT_Z);
			//Bird->showBoundingBox(true);
			count++;
		}
	}

	void updateSteer(SceneManager* mSceneMgr, const FrameEvent & evt){
		OpenSteer::OpenSteerDemo::updateSimulationAndRedraw ();  //steer
		const OpenSteer::AVGroup& all = OpenSteer::OpenSteerDemo::allVehiclesOfSelectedPlugIn();

		int count = 0;
		Ogre::SceneNode* Fishes;
		for (OpenSteer::AVIterator i = all.begin(); i != all.end(); i++) 	{
			Fishes = mSceneMgr->getSceneNode("BoidsFish"+Ogre::StringConverter::toString(count));
			Ogre::Vector3 oldPos = Fishes->getPosition();
			Ogre::Vector3 newPos = Ogre::Vector3( (**i).position().x * 20 + 2400, 
												  (**i).position().y + 50 , 
												  (**i).position().z * 10 + 1600);

			Fishes->setPosition( newPos );
			mFishSwimMap->at("FishAnimation" + Ogre::StringConverter::toString(count))->addTime(evt.timeSinceLastFrame * 3);

			Ogre::Vector3 dir = newPos - oldPos;
			dir.y = 0;
			dir.normalise();

			if ((1.0f + Ogre::Vector3::UNIT_Z.dotProduct(dir)) < 0.0001f)
			{
				Fishes->yaw(Ogre::Degree(180));
			}
			else
			{
				Ogre::Quaternion quat = Ogre::Vector3::UNIT_X.getRotationTo(-dir);
				Fishes->setOrientation(quat);
			}

			count++;
		}
	}


protected:
	std::map<std::string, AnimationState*> * mFishSwimMap;
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