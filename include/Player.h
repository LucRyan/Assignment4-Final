#ifndef _PLAYER_H
#define _PLAYER_H

#include "Final.h"

using namespace Ogre;

class Player
{
private:
	static const int NUM_ANIMS = 13;
	static const float ANIM_FADE_SPEED;

	enum AnimID
	{
		ANIM_IDLE_BASE,
		ANIM_IDLE_TOP,

		ANIM_RUN_BASE,
		ANIM_RUN_TOP,

		ANIM_HANDS_CLOSED,
		ANIM_HANDS_RELAXED,

		ANIM_DRAW_SWORDS,

		ANIM_SLICE_VERTICAL,
		ANIM_SLICE_HORIZONTAL,

		ANIM_DANCE,

		ANIM_JUMP_START,
		ANIM_JUMP_LOOP,
		ANIM_JUMP_END,
		ANIM_NONE
	};

	Camera * mCamera;
	SceneNode * mBodyNode;
	SceneNode * mCameraPivot;
	SceneNode * mCameraGoal;
	SceneNode * mCameraNode;
	Real mPivotPitch;

	Entity * mBodyEnt;
	AnimationState * mAnims[NUM_ANIMS];
	AnimID mBaseAnimID;
	AnimID mTopAnimID;
	bool mFadingIn[NUM_ANIMS];
	bool mFadingOut[NUM_ANIMS];
	Real mTimer; // general timer to see how long animations have been playing
	bool mMoving; // used to decide what animation we want to use after we've landed


public:
	Player(SceneManager * sceneMgr, Vector3 & origin);
	void addTime(Real deltaTime);

	SceneNode * getBodySceneNode();
	Vector3 getPosition() const;

	void animRunStart();
	void animRunEnd();
	void animJumpStart();
	void animJumpLoop();
	void animJumpEnd();
	void setIsMoving(bool isMoving);


private:
	void setupBody(SceneManager * sceneMgr, Vector3 & origin);
	void setupAnimations();
	void updateAnimations(Real deltaTime);
	void fadeAnimations(Real deltaTime);
	void setBaseAnimation(AnimID id, bool reset = false);
	void setTopAnimation(AnimID id, bool reset = false);
};

#endif // CC_PLAYER_H
