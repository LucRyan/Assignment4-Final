#include "WaterBuilder.h"
#include "ControllerMgr.h"

class MyExampleFrameListener : public FrameListener , public RenderTargetListener, public OIS::KeyListener, public OIS::MouseListener
{
private:
	ControllerMgr * mController;
	Camera * mCamera;
	Timer mTimer;
	RenderWindow * mWindow;
	bool mShutdown;

	OIS::InputManager * mInputManager;
	OIS::Keyboard * mKeyboard;
	OIS::Mouse * mMouse;


public:
	SceneManager *mSceneMgr;
	Real mKeyBuffer;


	MyExampleFrameListener(RenderWindow* win, Camera* cam, SceneManager *sm,  ControllerMgr * ch)
		: mWindow(win),
		  mCamera(cam),
		  mController(ch),
		  mShutdown(false)
	{
		OIS::ParamList pl;
		unsigned int windowHnd = 0;
		std::stringstream windowHndStr;

		win->getCustomAttribute("WINDOW", &windowHnd);
		windowHndStr << windowHnd;

		pl.insert(std::make_pair("WINDOW", windowHndStr.str()));

		mInputManager = OIS::InputManager::createInputSystem(pl);
		mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
		mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));

		mKeyboard->setEventCallback(this);
		mMouse->setEventCallback(this);
	}

	~MyExampleFrameListener()
	{
		mInputManager->destroyInputObject(mKeyboard);
		OIS::InputManager::destroyInputSystem(mInputManager);
	}

	bool frameRenderingQueued(const FrameEvent & evt)
	{
		if (mWindow->isClosed() || mShutdown)
			return false;

		// Update Actor
		mController->updateCharacter(evt.timeSinceLastFrame);

		return true;
	}

	virtual bool processUnbufferedKeyInput(const FrameEvent& e)
	{
		//bool ret = ExampleFrameListener::processUnbufferedKeyInput(e);
		mKeyboard->capture();

		// Press M to Change the Skybox
		if (mKeyboard->isKeyDown(OIS::KC_M) && mKeyBuffer < 0)
		{
			WaterBuilder::getSingleton().changeSkyBox(mSceneMgr); 

			mKeyBuffer = 0.5f;
		}
		// Press B to add Box
		if (mKeyboard->isKeyDown(OIS::KC_B) && mKeyBuffer < 0)
		{
			PhysicsBuilder::getSingleton().addBox(mSceneMgr,mCamera);

			mKeyBuffer = 0.5f;
		}

		mKeyBuffer -= e.timeSinceLastFrame;

		return true;

	}

	bool frameStarted(const FrameEvent &e)
	{
		//bool ret = ExampleFrameListener::frameStarted(e);
		
		mKeyboard->capture();
		mMouse->capture();

		// Update Physics
		PhysicsBuilder::getSingleton().getWorld()->stepSimulation(e.timeSinceLastFrame, 10); // update Bullet Physics animation
		// Update Hydrax
		WaterBuilder::getSingleton().update(e);
		// Update CubeMap
		CubeMapping::getSingleton().update(e);
		// Update SphereMap
		SphereMapping::getSingleton().update(e);


		return true;
	}


	bool frameEnded(const FrameEvent &e){
		//bool ret = ExampleFrameListener::frameEnded(e);

		// Update Physics
		PhysicsBuilder::getSingleton().getWorld()->stepSimulation(e.timeSinceLastFrame, 10); // update Bullet Physics animation
		// Update Hydrax
		WaterBuilder::getSingleton().update(e);
		// Update CubeMap
		CubeMapping::getSingleton().update(e);

		return true;
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


	// Controller
	bool keyPressed(const OIS::KeyEvent & evt)
	{
		switch (evt.key)
		{
		case OIS::KC_ESCAPE:
			mShutdown = true;
			break;

		case OIS::KC_R:
			PolygonMode pm;
			switch (mCamera->getPolygonMode())
			{
			case PM_SOLID:
				pm = PM_WIREFRAME;
				break;

			case PM_WIREFRAME:
				pm = PM_POINTS;
				break;

			case PM_POINTS:
				pm = PM_SOLID;
				break;

			default:
				pm = PM_SOLID;
			}

			mCamera->setPolygonMode(pm);
			break;

			break;
		}
		mController->injectKeyDown(evt);
		return true;
	}

	bool keyReleased(const OIS::KeyEvent & evt)
	{
		mController->injectKeyUp(evt);
		return true;
	}

	bool mouseMoved(const OIS::MouseEvent & evt)
	{
		mController->injectMouseMove(evt);
		return true;
	}

	bool mousePressed(const OIS::MouseEvent & evt, OIS::MouseButtonID id)
	{
		if (evt.state.buttonDown(OIS::MB_Left))
			PhysicsBuilder::getSingleton().addBox(mSceneMgr,mCamera);

		return true;
	}

	bool mouseReleased(const OIS::MouseEvent & evt, OIS::MouseButtonID id)
	{
		return true;
	}
};