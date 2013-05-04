#include "WaterBuilder.h"

class MyExampleFrameListener : public ExampleFrameListener , public RenderTargetListener
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
		
		mKeyboard->capture();

		// Press M to Change the Skybox
		if (mKeyboard->isKeyDown(OIS::KC_M) && mKeyBuffer < 0)
		{
			WaterBuilder::getSingleton().changeSkyBox(mSceneMgr); 

			mKeyBuffer = 0.5f;
		}
		// Press M to add Box
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
		
		// Update Physics
		PhysicsBuilder::getSingleton().getWorld()->stepSimulation(e.timeSinceLastFrame); // update Bullet Physics animation
		// Update Hydrax
		WaterBuilder::getSingleton().update(e);
		// Update CubeMap
		CubeMapping::getSingleton().update(e);
		// Update SphereMap
		SphereMapping::getSingleton().update(e);

		return ret;
	}


	bool frameEnded(const FrameEvent &e){
		bool ret = ExampleFrameListener::frameEnded(e);

		// Update Physics
		PhysicsBuilder::getSingleton().getWorld()->stepSimulation(e.timeSinceLastFrame); // update Bullet Physics animation
		// Update Hydrax
		WaterBuilder::getSingleton().update(e);
		// Update CubeMap
		CubeMapping::getSingleton().update(e);

		return ret;
	}


	void preRenderTargetUpdate(const RenderTargetEvent& evt)
	{
		CubeMapping::getSingleton().preRenderTargetUpdate(evt);
		SphereMapping::getSingleton().preRenderTargetUpdate(evt, mCamera);
	}

	void postRenderTargetUpdate(const RenderTargetEvent& evt)
	{
		CubeMapping::getSingleton().postRenderTargetUpdate(evt);
	}

};