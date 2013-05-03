#ifndef __PhysicsBuilder_h_
#define __PhysicsBuilder_h_

#include "Final.h"
#include <assert.h>



class PhysicsBuilder /* : public Singleton<PhysicsBuilder>*/
{

private:


protected:
	static PhysicsBuilder* PhysicsBuilder::pbSingleton;

	//Bullet pointers
	OgreBulletDynamics::DynamicsWorld *mWorld;
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

	void setupPhysics(SceneManager *mSceneMgr){
		mWorld = new OgreBulletDynamics::DynamicsWorld(mSceneMgr, 
			AxisAlignedBox (Ogre::Vector3 (-10000, -10000, -10000), Ogre::Vector3 (10000,  10000,  10000)), 
			Vector3(0,-9.81,0));
	}

};



#endif