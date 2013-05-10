#include "ControllerMgr.h"

using namespace Ogre;

ControllerMgr::ControllerMgr(SceneManager * scnMgr, Camera * cam, btPairCachingGhostObject * ghostObject,
	btConvexShape * convexShape, btScalar stepHeight, btCollisionWorld * collisionWorld,
	Vector3 & origin, int upAxis
	)
{
	mCCPlayer = new Player(cam->getSceneManager(), origin);
	mCCPhysics = new ControllerPhysics(ghostObject, convexShape, stepHeight, collisionWorld, upAxis);

	mSceneManager = scnMgr;

	mBodyNode = mCCPlayer->getBodySceneNode();

	setupCamera(cam);

	mIsFalling = mCCPhysics->onGround();
	mWalkDirection = Vector3::ZERO;
	mKeyDirection = Vector3::ZERO;
	mJumped = false;
}

ControllerMgr::ControllerMgr(SceneManager * scnMgr,
	Player * ccPlayer, ControllerPhysics * ccPhysics
	)
{
	mCCPlayer = ccPlayer;
	mCCPhysics = ccPhysics;

	mSceneManager = scnMgr;
	mJumped = false;
}

Player * ControllerMgr::getCCPlayer()
{
	return mCCPlayer;
}
ControllerPhysics * ControllerMgr::getCCPhysics()
{
	return mCCPhysics;
}


void ControllerMgr::injectKeyDown(const OIS::KeyEvent & evt)
{
	// we want the animation to be played only if we press w/s/a/d and we haven't jumped
	// (if we jumped and while we're in the air (we'd have the ANIM_JUMP_LOOP animation), it wouldn't look quite real
	// if we suddenly started to run. Well, actually it is not that real to be suddenly moving while in the air too...
	if (((evt.key == OIS::KC_W) || (evt.key == OIS::KC_S) || (evt.key == OIS::KC_A) || (evt.key == OIS::KC_D)) && mCCPhysics->onGround())
	{
		if (mKeyDirection.isZeroLength())
			mCCPlayer->animRunStart();
	}

	switch (evt.key)
	{
	case OIS::KC_W:
		mKeyDirection.z = -1;
		break;

	case OIS::KC_S:
		mKeyDirection.z = 1;
		break;

	case OIS::KC_A:
		mKeyDirection.x = -1;
		break;

	case OIS::KC_D:
		mKeyDirection.x = 1;
		break;

	case OIS::KC_SPACE:
		if (mCCPhysics->canJump())
		{
			mCCPhysics->jump();
			mCCPlayer->animJumpStart();

			mJumped = true;
		}

		break;

	case OIS::KC_LCONTROL:
		mCCPhysics->duck();
		break;
	}
}

void ControllerMgr::injectKeyUp(const OIS::KeyEvent & evt)
{
	switch (evt.key)
	{
	case OIS::KC_W:
	case OIS::KC_S:
		mKeyDirection.z = 0;
		break;

	case OIS::KC_A:
	case OIS::KC_D:
		mKeyDirection.x = 0;
		break;

	case OIS::KC_LCONTROL:
		mCCPhysics->stand();
		break;
	}

	// pretty much the same case as above (the !mJumped if statement) - if we jumped while holding w/s/a/d and then released
	// it, it really wouldn't look very good if the ANIM_JUMP_LOOP would be replaced by ANIM_IDLE_TOP and ANIM_IDLE_BASE
	if (((evt.key == OIS::KC_W) || (evt.key == OIS::KC_S) || (evt.key == OIS::KC_A) || (evt.key == OIS::KC_D)) && mCCPhysics->onGround())
	{
		if (mKeyDirection.isZeroLength())
			mCCPlayer->animRunEnd();
	}
}

void ControllerMgr::injectMouseMove(const OIS::MouseEvent & evt)
{
	// update camera goal based on mouse movement
	updateCameraGoal(-0.05f * evt.state.X.rel, -0.05f * evt.state.Y.rel, -0.0005f * evt.state.Z.rel);
}

void ControllerMgr::updateCharacter(Real deltaTime)
{
	Real direction = RUN_SPEED * deltaTime;

	Vector3 playerPos = mCCPlayer->getPosition();

	btVector3 pos = mCCPhysics->getPosition();

	Vector3 position(pos.x(), pos.y(), pos.z());


	if (position != playerPos)
	{
		mBodyNode->translate((position - playerPos) * direction);

		if (!mIsFalling && !mCCPhysics->onGround()) // last frame we were on ground and now we're in "air"
		{
			mIsFalling = true;

			if (!mJumped) // if we jumped, let the Player's updateAnimations take care about this
				mCCPlayer->animJumpLoop();
		}
		else if (mCCPhysics->onGround() && mIsFalling) // last frame we were falling and now we're on the ground
		{
			mIsFalling = false;
			mJumped = false;

			mCCPlayer->animJumpEnd();
		}
	}

	mGoalDirection = Vector3::ZERO;   // we will calculate this

	if (mKeyDirection != Vector3::ZERO)
	{
		// calculate actuall goal direction in world based on player's key directions
		mGoalDirection += mKeyDirection.z * mCameraNode->getOrientation().zAxis();
		mGoalDirection += mKeyDirection.x * mCameraNode->getOrientation().xAxis();
		mGoalDirection.y = 0;
		mGoalDirection.normalise();

		Quaternion toGoal = mBodyNode->getOrientation().zAxis().getRotationTo(mGoalDirection);

		// calculate how much the character has to turn to face goal direction
		Real yawToGoal = toGoal.getYaw().valueDegrees();
		// this is how much the character CAN turn this frame
		Real yawAtSpeed = yawToGoal / Math::Abs(yawToGoal) * deltaTime * TURN_SPEED;

		// turn as much as we can, but not more than we need to
		if (yawToGoal < 0) yawToGoal = std::min<Real>(0, std::max<Real>(yawToGoal, yawAtSpeed));
		else if (yawToGoal > 0) yawToGoal = std::max<Real>(0, std::min<Real>(yawToGoal, yawAtSpeed));

		mBodyNode->yaw(Degree(yawToGoal));

		mCCPhysics->setWalkDirection(mGoalDirection.x * direction, mGoalDirection.y * direction, mGoalDirection.z * direction);
		mCCPlayer->setIsMoving(true);
	}
	else
	{
		mCCPhysics->setWalkDirection(0, 0, 0);
		mCCPlayer->setIsMoving(false);
	}

	updateCamera(deltaTime);
	mCCPlayer->addTime(deltaTime);
}

void ControllerMgr::setupCamera(Camera * cam)
{
	mCameraPivot = mSceneManager->getRootSceneNode()->createChildSceneNode();

	mCameraGoal = mCameraPivot->createChildSceneNode(Vector3(0, 0, 450));

	mCameraNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	mCameraNode->setPosition(mCameraPivot->getPosition() + mCameraGoal->getPosition());

	mCameraPivot->setFixedYawAxis(true);
	mCameraGoal->setFixedYawAxis(true);
	mCameraNode->setFixedYawAxis(true);

	cam->setNearClipDistance(0.1);
	cam->setFarClipDistance(999999*6);
	mCameraNode->attachObject(cam);
	mCameraNode->setAutoTracking(true, mCameraPivot);

	mPivotPitch = 0;
}

void ControllerMgr::updateCamera(Real deltaTime)
{
	mCameraPivot->setPosition(mBodyNode->getPosition() + Vector3::UNIT_Y * CAM_HEIGHT);

	Vector3 goalOffset = mCameraGoal->_getDerivedPosition() - mCameraNode->getPosition();
	mCameraNode->translate(goalOffset * deltaTime * 7);
}

void ControllerMgr::updateCameraGoal(Real deltaYaw, Real deltaPitch, Real deltaZoom)
{
	mCameraPivot->yaw(Degree(deltaYaw), Node::TS_WORLD);

	// bound the pitch
	if (!(mPivotPitch + deltaPitch > 25 && deltaPitch > 0) &&
		!(mPivotPitch + deltaPitch < -60 && deltaPitch < 0))
	{
		mCameraPivot->pitch(Degree(deltaPitch), Node::TS_LOCAL);
		mPivotPitch += deltaPitch;
	}

	Real dist = mCameraGoal->_getDerivedPosition().distance(mCameraPivot->_getDerivedPosition());
	Real distChange = deltaZoom * dist;

	// bound the zoom
	if (!(dist + distChange < 8 && distChange < 0) &&
		!(dist + distChange > 25 && distChange > 0))
	{
		mCameraGoal->translate(0, 0, distChange, Node::TS_LOCAL);
	}
}
