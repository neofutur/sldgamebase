/* Body.h
Copyright (c) 2016 by Michael Zahniser

Endless Sky is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

Endless Sky is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef BODY_H_
#define BODY_H_

#include "Angle.h"
#include "ConditionsStore.h"
#include "Point.h"
#include "SpriteParameters.h"

#include <cstdint>
#include <map>
#include <string>

class DataNode;
class DataWriter;
class Government;
class Mask;
class Sprite;



// Class representing any object in the game that has a position, velocity, and
// facing direction and usually also has a sprite.
class Body {
public:
	// States for animation purposes
	enum BodyState{FLYING, FIRING, LAUNCHING, LANDING, JUMPING, DISABLED, NUM_STATES, CURRENT, TRIGGER};
	// Minimum/Default frame rate of animations
	static constexpr float MIN_FRAME_RATE = 2. / 60.;
public:
	// Constructors.
	Body() = default;
	Body(const Sprite *sprite, Point position, Point velocity = Point(), Angle facing = Angle(), double zoom = 1.);
	Body(const Body &sprite, Point position, Point velocity = Point(), Angle facing = Angle(), double zoom = 1.);

	// Check that this Body has a sprite and that the sprite has at least one frame.
	bool HasSprite() const;
	bool HasSpriteFor(BodyState state) const;

	// Access the underlying Sprite object.
	const Sprite *GetSprite(Body::BodyState state = Body::BodyState::CURRENT) const;
	Body::BodyState GetState() const;
	// Get the dimensions of the sprite.
	double Width() const;
	double Height() const;
	// Get the farthest a part of this sprite can be from its center.
	double Radius() const;
	// Which color swizzle should be applied to the sprite?
	int GetSwizzle() const;
	// Get the sprite frame and mask for the given time step.
	float GetFrame(int step = -1) const;
	const Mask &GetMask(int step = -1) const;

	// Positional attributes.
	const Point &Position() const;
	const Point &Velocity() const;
	const Angle &Facing() const;
	Point Unit() const;
	double Zoom() const;
	double Scale() const;

	// Check if this object is marked for removal from the game.
	bool ShouldBeRemoved() const;

	// Store the government here too, so that collision detection that is based
	// on the Body class can figure out which objects will collide.
	const Government *GetGovernment() const;

	// Sprite serialization. Return true if sprite is successfully loaded
	bool LoadSprite(const DataNode &node);
	void LoadTriggerSprite(const DataNode &node, Body::BodyState state,
						   SpriteParameters::AnimationParameters params, int index);
	void SaveSprite(DataWriter &out, const std::string &tag = "sprite", bool allStates = false) const;
	void SaveSpriteParameters(DataWriter &out, SpriteParameters *state, int index) const;
	// Set the sprite.
	void SetSprite(const Sprite *sprite, Body::BodyState state = BodyState::FLYING);
	void SetState(Body::BodyState state);
	// Set the color swizzle.
	void SetSwizzle(int swizzle);


protected:
	// Adjust the frame rate.
	void SetFrameRate(float framesPerSecond);
	void AddFrameRate(float framesPerSecond);
	void PauseAnimation();
	void ShowDefaultSprite(bool defaultSprite);
	// Check if any triggers should be activated in this state
	void CheckTriggers();
	// Ready to perform the desired action
	bool ReadyForAction() const;
	// Finish transitioning between states
	void FinishStateTransition() const;
	// Mark this object to be removed from the game.
	void MarkForRemoval();
	// Mark that this object should not be removed (e.g. a launched fighter).
	void UnmarkForRemoval();


protected:
	// Basic positional attributes.
	Point position;
	Point velocity;
	Angle angle;
	// A zoom of 1 means the sprite should be drawn at half size. For objects
	// whose sprites should be full size, use zoom = 2.
	float zoom = 1.f;
	mutable float scale = 1.f;
	mutable bool debug = false;

	// Government, for use in collision checks.
	const Government *government = nullptr;

	// Conditions registered from bodies
	ConditionsStore conditions;


private:
	// Set what animation step we're on and indicate whether a state transition has finished.
	// This affects future calls to GetMask() and GetFrame().
	bool SetStep(int step) const;


private:
	// Animation parameters.
	mutable SpriteParameters sprites[Body::BodyState::NUM_STATES] = {SpriteParameters(), SpriteParameters(),
																SpriteParameters(), SpriteParameters(),
																SpriteParameters(), SpriteParameters()};
	mutable Body::BodyState currentState = Body::BodyState::FLYING,
							transitionState = Body::BodyState::FLYING;
	mutable bool stateTransitionRequested = false;
	bool returnDefaultSprite = false;
	// Allow objects based on this one to adjust their frame rate and swizzle.
	int swizzle = 0;

	mutable SpriteParameters::AnimationParameters anim;

	mutable int pause = 0;

	// Record when this object is marked for removal from the game.
	bool shouldBeRemoved = false;

	// Cache the frame calculation so it doesn't have to be repeated if given
	// the same step over and over again.
	mutable int currentStep = -1;
	mutable float frameRate = 0.f;
	mutable float frame = 0.f;
	mutable float integratedFrame = 0.f;
	mutable float reversedFrame = 0.f;
	mutable float rewindFrame = 0.f;
	mutable float randomFrame = 0.f;
	mutable float framePercentage = 0.f;
	mutable float delayed = 0.f;
	mutable bool stateReady = false;
};



#endif
