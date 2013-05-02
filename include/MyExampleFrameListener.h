#include "Final.h"

class MyExampleFrameListener : public ExampleFrameListener
{
public:
	SceneManager *mSceneMgr;
	Real mKeyBuffer;

	MyExampleFrameListener(RenderWindow* win, Camera* cam, SceneManager *sm)
		: ExampleFrameListener(win,cam)
		, mSceneMgr(sm)
		, mKeyBuffer(-1)
	{
		mMoveSpeed = 100;
	}

	bool frameStarted(const FrameEvent &e)
	{
		// Check camera height
		Ogre::RaySceneQuery *raySceneQuery = 
			mSceneMgr->
			createRayQuery(Ogre::Ray(mCamera->getPosition() + Ogre::Vector3(0,1000000,0), 
			Vector3::NEGATIVE_UNIT_Y));
		Ogre::RaySceneQueryResult& qryResult = raySceneQuery->execute();
		Ogre::RaySceneQueryResult::iterator i = qryResult.begin();
		if (i != qryResult.end() && i->worldFragment)
		{
			if (mCamera->getPosition().y < i->worldFragment->singleIntersection.y + 30)
			{
				mCamera->
					setPosition(mCamera->getPosition().x, 
					i->worldFragment->singleIntersection.y + 30, 
					mCamera->getPosition().z);
			}
		}

		delete raySceneQuery;

		// Update Hydrax
		mHydrax->update(e.timeSinceLastFrame);

		// Check for skyboxes switch
		mKeyboard->capture();

		if (mKeyboard->isKeyDown(OIS::KC_M) && mKeyBuffer < 0)
		{
			mCurrentSkyBox++;

			if(mCurrentSkyBox > (_def_SkyBoxNum-1))
			{
				mCurrentSkyBox = 0;
			}

			changeSkyBox();

			mKeyBuffer = 0.5f;
		}

		mKeyBuffer -= e.timeSinceLastFrame;

		return true;
	}

	void changeSkyBox()
	{
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
};