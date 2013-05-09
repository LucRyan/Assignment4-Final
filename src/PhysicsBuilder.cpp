#include "PhysicsBuilder.h"

PhysicsBuilder* PhysicsBuilder::pbSingleton = 0;

void PhysicsBuilder::setupPhysics(SceneManager *mSceneMgr){
	mWorld = new OgreBulletDynamics::DynamicsWorld(mSceneMgr, 
		AxisAlignedBox (Ogre::Vector3 (-10000, -10000, -10000), Ogre::Vector3 (10000,  10000,  10000)), 
		Vector3(0,-9.81,0));
	// add Debug info display tool
	debugDrawer = new OgreBulletCollisions::DebugDrawer();
	//debugDrawer->setDrawWireframe(true);	// we want to see the Bullet containers

	mWorld->setDebugDrawer(debugDrawer);
	//mWorld->setShowDebugShapes(true);		// enable it if you want to see the Bullet containers
	SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode("debugDrawer", Ogre::Vector3::ZERO);
	node->attachObject(static_cast <SimpleRenderable *> (debugDrawer));
}


void PhysicsBuilder::addBox(SceneManager *mSceneMgr, Camera *mCamera){
	Vector3 size = Vector3::ZERO;	// size of the box
	// starting position of the box
	Vector3 position = (mCamera->getDerivedPosition() + mCamera->getDerivedDirection().normalisedCopy() * 10);

	// create an ordinary, Ogre mesh with texture
	Entity *entity = mSceneMgr->createEntity(
		"Box" + StringConverter::toString(mNumEntitiesInstanced),
		"cube.mesh");			    
	entity->setCastShadows(true);
	// we need the bounding box of the box to be able to set the size of the Bullet-box
	AxisAlignedBox boundingB = entity->getBoundingBox();
	size = boundingB.getSize(); size /= 2.0f; // only the half needed
	size *= 0.95f;	// Bullet margin is a bit bigger so we need a smaller size
	// (Bullet 2.76 Physics SDK Manual page 18)
	entity->setMaterialName("Examples/BumpyMetal");
	SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(entity);
	node->scale(0.05f, 0.05f, 0.05f);	// the cube is too big for us
	size *= 0.05f;						// don't forget to scale down the Bullet-box too

	// after that create the Bullet shape with the calculated size
	OgreBulletCollisions::BoxCollisionShape *sceneBoxShape = new OgreBulletCollisions::BoxCollisionShape(size);
	// and the Bullet rigid body
	OgreBulletDynamics::RigidBody *defaultBody = new OgreBulletDynamics::RigidBody(
		"defaultBoxRigid" + StringConverter::toString(mNumEntitiesInstanced), 
		mWorld);
	defaultBody->setShape(	node,
		sceneBoxShape,
		0.6f,			// dynamic body restitution
		0.6f,			// dynamic body friction
		1.0f, 			// dynamic bodymass
		position,		// starting position of the box
		Quaternion(0,0,0,1));// orientation of the box
	mNumEntitiesInstanced++;				

	defaultBody->setLinearVelocity(
		mCamera->getDerivedDirection().normalisedCopy() * 100.0f ); // shooting speed
	// push the created objects to the dequese
	mShapes.push_back(sceneBoxShape);
	mBodies.push_back(defaultBody);				
	//mTimeUntilNextToggle = 0.5;
}