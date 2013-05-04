#ifndef __SphereMapping_H__
#define __SphereMapping_H__
#include "Final.h"


using namespace Ogre;

class SphereMapping : public Singleton<SphereMapping>
{
public:

	//Singleton Function
	static SphereMapping &getSingleton(){
		if(!msSingleton){
			msSingleton = new SphereMapping();
		}
		assert( msSingleton );
		return (*msSingleton);
	}


	void update(const FrameEvent& evt)
	{
		mPivot->yaw(Radian(evt.timeSinceLastFrame));      // spin the fishy around the Sphere mapped one
	}

	void preRenderTargetUpdate(const RenderTargetEvent& evt, Camera *mCamera)
	{
		// point the camera in the right direction based on which face of the cubemap this is
		mSphereCamera->setOrientation(Ogre::Quaternion::IDENTITY);
		mSphereCamera->setPosition(sphere->_getDerivedPosition());
		if (evt.source == mTarget) {
			mSphereCamera->lookAt(mCamera->getDerivedPosition());
			mSphereCamera->setFixedYawAxis(true, Ogre::Vector3::UNIT_Y);
		}
	}

	void postRenderTargetUpdate(const RenderTargetEvent& evt)
	{
		
	}

	void setupContent(SceneManager *mSceneMgr, RenderTargetListener *mListener, RenderWindow *mWindow)
	{
		createSphereMap(mSceneMgr, mListener, mWindow);

		// create an ogre head, give it the dynamic Sphere map material, and place it at the origin
		mHead = mSceneMgr->createEntity("SphereMappedHead", "sphere.mesh");
		mHead->setMaterialName("Examples/DynamicSphereMap");
		mPivot = mSceneMgr->getRootSceneNode()->createChildSceneNode("mSpherePivot");
		mPivot->setPosition(Vector3(1000, 500, 400));
		sphere = mPivot->createChildSceneNode(Ogre::Vector3(1000, 0, 0));
		sphere->yaw(Ogre::Degree(90));
		sphere->attachObject(mHead);

		Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName("Examples/DynamicSphereMap");
		Ogre::TextureUnitState* texUnState = mat->getTechnique(0)->getPass(0)->getTextureUnitState(0);
		texUnState->setTextureName("dynsphmap", Ogre::TEX_TYPE_2D);
		texUnState->setTextureUScale (-1);
	}


protected:

	SphereMapping()
	{
		position = Vector3(1000, 500, 400);
	}

	~SphereMapping()
	{
		delete msSingleton;
	}



	void createSphereMap(SceneManager *mSceneMgr, RenderTargetListener *mListener, RenderWindow *mWindow)
	{
		// create the camera used to render to our Spheremap
		mSphereCamera = mSceneMgr->createCamera("SphereMapCamera");
		mSphereCamera->setFOVy(Degree(90));
		mSphereCamera->setAspectRatio(1);
		mSphereCamera->setFixedYawAxis(false);
		mSphereCamera->setNearClipDistance(10);
		mSphereCamera->setFarClipDistance(999999*6);

		// create our dynamic Sphere map texture
		TexturePtr tex = TextureManager::getSingleton().createManual("dynsphmap",
			ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
			Ogre::TEX_TYPE_2D, mWindow->getWidth(), mWindow->getHeight(), 0, Ogre::PF_A8R8G8B8, Ogre::TU_RENDERTARGET);

		// assign our camera to 1 render targets of the texture (1 for each direction)
		mTarget = tex->getBuffer(0)->getRenderTarget();
		mTarget->addViewport(mSphereCamera)->setOverlaysEnabled(false);
		mTarget->addListener(mListener);
	}

	void cleanupContent(SceneManager *mSceneMgr)
	{
		mSceneMgr->destroyCamera(mSphereCamera);
		TextureManager::getSingleton().remove("dynsphmap");
	}

	Entity* mHead;
	Camera* mSphereCamera;
	RenderTarget* mTarget;
	SceneNode* mPivot;
	SceneNode* sphere;
	AnimationState* mFishSwim;
	Vector3 position;
};

#endif
