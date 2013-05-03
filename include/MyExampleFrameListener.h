#include "WaterBuilder.h"

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

	virtual bool processUnbufferedKeyInput(const FrameEvent& e)
	{
		bool ret = ExampleFrameListener::processUnbufferedKeyInput(e);
		// Check for skyboxes switch
		mKeyboard->capture();

		if (mKeyboard->isKeyDown(OIS::KC_M) && mKeyBuffer < 0)
		{
			PhysicsBuilder::getSingleton().addBox(mSceneMgr,mCamera);
			mCurrentSkyBox++;

			if(mCurrentSkyBox > (_def_SkyBoxNum-1))
			{
				mCurrentSkyBox = 0;
			}

			changeSkyBox();

			mKeyBuffer = 0.5f;
		}

		if (mKeyboard->isKeyDown(OIS::KC_B) && mKeyBuffer < 0)
		{
			PhysicsBuilder::getSingleton().addBox(mSceneMgr,mCamera);

			mKeyBuffer = 0.5f;
		}

		mKeyBuffer -= e.timeSinceLastFrame;

		return ret;

	}

	bool frameStarted(const FrameEvent &e)
	{
		bool ret = ExampleFrameListener::frameEnded(e);

		PhysicsBuilder::getSingleton().getWorld()->stepSimulation(e.timeSinceLastFrame); // update Bullet Physics animation
		
		// Update Hydrax
		mHydrax->update(e.timeSinceLastFrame);

		return ret;
	}


	bool frameEnded(const FrameEvent &e){
		bool ret = ExampleFrameListener::frameEnded(e);

		PhysicsBuilder::getSingleton().getWorld()->stepSimulation(e.timeSinceLastFrame); // update Bullet Physics animation
		
		// Update Hydrax
		mHydrax->update(e.timeSinceLastFrame);
		
		return ret;
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