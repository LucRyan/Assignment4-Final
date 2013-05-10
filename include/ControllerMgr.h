#ifndef CC_MANAGER_H
#define CC_MANAGER_H

#include "ControllerPhysics.h"
#include "Player.h"

using namespace Ogre;

class ControllerMgr
{
private:
	static const int RUN_SPEED = 50;
	static const int TURN_SPEED = 500;
	static const int CAM_HEIGHT = 2;

	ControllerPhysics * mCCPhysics;
	Player * mCCPlayer;

	SceneManager * mSceneManager;

	Camera * mCamera;
	SceneNode * mBodyNode;
	SceneNode * mCameraPivot;
	SceneNode * mCameraGoal;
	SceneNode * mCameraNode;
	Real mPivotPitch;
	Vector3 mWalkDirection;

	bool mIsFalling;
	bool mJumped;

	Vector3 mGoalDirection;
	Vector3 mKeyDirection;


public:
	ControllerMgr(SceneManager * scnMgr, Camera * cam, btPairCachingGhostObject * ghostObject,
		btConvexShape * convexShape, btScalar stepHeight, btCollisionWorld * collisionWorld,
		Vector3 & origin, int upAxis = 1
		);

	ControllerMgr(SceneManager * scnMgr, Player * ccPlayer,
		ControllerPhysics * ccPhysics
		);

	Player * getCCPlayer();
	ControllerPhysics * getCCPhysics();
	void injectKeyDown(const OIS::KeyEvent & evt);
	void injectKeyUp(const OIS::KeyEvent & evt);
	void injectMouseMove(const OIS::MouseEvent & evt);
	void updateCharacter(Real dt);


private:
	void setupCamera(Camera * cam);
	void updateCamera(Real deltaTime);
	void updateCameraGoal(Real deltaYaw, Real deltaPitch, Real deltaZoom);
	Quaternion updateOrientation(Real deltaTime);
};

#endif // CC_MANAGER_H
