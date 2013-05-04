#ifndef __CubeMapping_H__
#define __CubeMapping_H__
#include "Final.h"


using namespace Ogre;

class CubeMapping : public Singleton<CubeMapping>
{
public:

	//Singleton Function
	static CubeMapping &getSingleton(){
		if(!msSingleton){
			msSingleton = new CubeMapping();
		}
		assert( msSingleton );
		return (*msSingleton);
	}


	void update(const FrameEvent& evt)
	{
		mPivot->yaw(Radian(evt.timeSinceLastFrame));      // spin the fishy around the cube mapped one
		mFishSwim->addTime(evt.timeSinceLastFrame * 3);   // make the fishy swim
	}

	void preRenderTargetUpdate(const RenderTargetEvent& evt)
	{
		mHead->setVisible(false);  // hide the head

		// point the camera in the right direction based on which face of the cubemap this is
		mCubeCamera->setOrientation(Quaternion::IDENTITY);
		if (evt.source == mTargets[0]) mCubeCamera->yaw(Degree(-90));
		else if (evt.source == mTargets[1]) mCubeCamera->yaw(Degree(90));
		else if (evt.source == mTargets[2]) mCubeCamera->pitch(Degree(90));
		else if (evt.source == mTargets[3]) mCubeCamera->pitch(Degree(-90));
		else if (evt.source == mTargets[5]) mCubeCamera->yaw(Degree(180));
	}

	void postRenderTargetUpdate(const RenderTargetEvent& evt)
	{
		mHead->setVisible(true);  // unhide the head
	}

	void setupContent(SceneManager *mSceneMgr, RenderTargetListener *mListener)
	{


		createCubeMap(mSceneMgr, mListener);

		// create an ogre head, give it the dynamic cube map material, and place it at the origin
		mHead = mSceneMgr->createEntity("CubeMappedHead", "chest_rare.MESH");
		mHead->setMaterialName("Examples/DynamicCubeMap");
		SceneNode* headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		headNode->setScale(50, 50, 50);
		headNode->setPosition(position);
		headNode->attachObject(mHead);
		Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName("Examples/DynamicCubeMap");
		Ogre::TextureUnitState* texus = mat->getTechnique(0)->getPass(0)->getTextureUnitState(0);
		texus->setTextureName("dyncubemap", Ogre::TEX_TYPE_2D);
		texus->setTextureUScale (-1);

		mPivot = mSceneMgr->getRootSceneNode()->createChildSceneNode();  // create a pivot node

		Entity* fish = mSceneMgr->createEntity("Fish", "fish.mesh");
		mFishSwim = fish->getAnimationState("swim");
		mFishSwim->setEnabled(true);

		// create a child node at an offset and attach a regular ogre head and a nimbus to it
		mPivot->setPosition(position);
		SceneNode* node = mPivot->createChildSceneNode(Vector3(-80, 10, 0));
		node->setScale(7, 7, 7);
		node->yaw(Degree(90));
		node->attachObject(fish);

	}


protected:

	CubeMapping()
	{
		position = Vector3(1500, 10, 1700);
	}

	~CubeMapping()
	{
		delete msSingleton;
	}

	

	void createCubeMap(SceneManager *mSceneMgr, RenderTargetListener *mListener)
	{
		mSceneMgr->setAmbientLight(ColourValue(0.3, 0.3, 0.3));
		// create the camera used to render to our cubemap
		mCubeCamera = mSceneMgr->createCamera("CubeMapCamera");
		mCubeCamera->setPosition(Vector3(position.x, position.y , position.z));
		mCubeCamera->setFOVy(Degree(90));
		mCubeCamera->setAspectRatio(1);
		mCubeCamera->setFixedYawAxis(false);
		mCubeCamera->setNearClipDistance(10);
		mCubeCamera->setFarClipDistance(999999*6);

		// create our dynamic cube map texture
		TexturePtr tex = TextureManager::getSingleton().createManual("dyncubemap",
			ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_CUBE_MAP, 128, 128, 0, PF_R8G8B8, TU_RENDERTARGET);

		// assign our camera to all 6 render targets of the texture (1 for each direction)
		for (unsigned int i = 0; i < 6; i++)
		{
			mTargets[i] = tex->getBuffer(i)->getRenderTarget();
			mTargets[i]->addViewport(mCubeCamera)->setOverlaysEnabled(false);
			mTargets[i]->addListener(mListener);
		}
	}

	void cleanupContent(SceneManager *mSceneMgr)
	{
		mSceneMgr->destroyCamera(mCubeCamera);
		TextureManager::getSingleton().remove("dyncubemap");
	}

	Entity* mHead;
	Camera* mCubeCamera;
	RenderTarget* mTargets[6];
	SceneNode* mPivot;
	AnimationState* mFishSwim;
	Vector3 position;
};

#endif
