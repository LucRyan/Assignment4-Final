#ifndef __PhysicsBuilder_h_
#define __PhysicsBuilder_h_

#include "Final.h"
#include <assert.h>

//------------------------------------------------------------------------------
// Include all the Physics Header
//------------------------------------------------------------------------------
#include "OgreBulletDynamicsRigidBody.h"	
#include "OgreBulletCollisions.h"
#include "Shapes/OgreBulletCollisionsTerrainShape.h"
#include "Shapes/OgreBulletCollisionsBoxShape.h"
#include "Shapes/OgreBulletCollisionsCapsuleShape.h"


class PhysicsBuilder 
{

private:


protected:
	static PhysicsBuilder* PhysicsBuilder::pbSingleton;

	//Bullet pointers
	OgreBulletDynamics::DynamicsWorld *mWorld;
	OgreBulletCollisions::DebugDrawer *debugDrawer;
	int mNumEntitiesInstanced;
	std::deque<OgreBulletDynamics::RigidBody *>         mBodies;
	std::deque<OgreBulletCollisions::CollisionShape *>  mShapes;
	
	
	PhysicsBuilder(){
		pbSingleton = 0;
	}

	~PhysicsBuilder()
	{
		assert( pbSingleton );  pbSingleton = 0;
		mWorld = 0;
		// OgreBullet physic delete - RigidBodies
		std::deque<OgreBulletDynamics::RigidBody *>::iterator itBody = mBodies.begin();
		while (mBodies.end() != itBody)
		{   
			delete *itBody; 
			++itBody;
		}	
		// OgreBullet physic delete - Shapes
		std::deque<OgreBulletCollisions::CollisionShape *>::iterator itShape = mShapes.begin();
		while (mShapes.end() != itShape)
		{   
			delete *itShape; 
			++itShape;
		}
		mBodies.clear();
		mShapes.clear();
		delete mWorld->getDebugDrawer();
		mWorld->setDebugDrawer(0);
		delete mWorld;
		delete pbSingleton;
	}

public:
	//Singleton Function
	static PhysicsBuilder &getSingleton(){
		if(!pbSingleton){
			pbSingleton = new PhysicsBuilder();
		}
		assert( pbSingleton );
		return (*pbSingleton);
	}

	OgreBulletDynamics::DynamicsWorld *getWorld(){
		return mWorld;
	}

	std::deque<OgreBulletDynamics::RigidBody *> getBoides(){
		return mBodies;
	}

	std::deque<OgreBulletCollisions::CollisionShape *> getShapes(){
		return mShapes;
	}

	void addCollisionShape(OgreBulletCollisions::CollisionShape* colShape){
		mShapes.push_back(colShape);
	};

	void addBox(SceneManager *mSceneMgr, Camera *mCamera);
	void setupPhysics(SceneManager *mSceneMgr);
	
};


class MyMotionState : public btDefaultMotionState
{
protected:
	SceneNode * mVisibleObj;
	btTransform mTransform;
	btTransform mCOM;

public:
	MyMotionState(const btTransform & initialPos, SceneNode * node)
	{
		mVisibleObj = node;
		mTransform = initialPos;
		mCOM = btTransform::getIdentity();
	}

	MyMotionState(const btTransform & initialPos)
	{
		mVisibleObj = NULL;
		mTransform = initialPos;
	}

	~MyMotionState() {}

	void setNode(SceneNode * node)
	{
		mVisibleObj = node;
	}

	btTransform getWorldTransform() const
	{
		return mCOM.inverse() * mTransform;
	}

	void getWorldTransform(btTransform & worldTrans) const
	{
		worldTrans = mCOM.inverse() * mTransform;
	}

	void setWorldTransform(const btTransform & worldTrans)
	{
		if (mVisibleObj == NULL)
			return;

		mTransform = worldTrans;
		btTransform transform = mTransform * mCOM;
		btQuaternion rot = transform.getRotation();
		btVector3 pos = transform.getOrigin();
		mVisibleObj->setOrientation(rot.w(), rot.x(), rot.y(), rot.z());
		mVisibleObj->setPosition(pos.x(), pos.y(), pos.z());
	}
};

#endif