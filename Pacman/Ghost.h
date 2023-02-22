#ifndef GHOST_H
#define GHOST_H

#include <list>
#include "MovableGameEntity.h"
#include "settings.h"

class World;
class PathmapTile;
class Avatar;

class Ghost : public MovableGameEntity
{
public:
	enum GhostBehavior
	{
		Respawning,
		Patrol,
		Chase,
		Dead,
		Fear,
	};

	Ghost(const Vector2f& aPosition, Sprite* sprite, const Vector2f& aPatrolPosition);
	
	~Ghost(void);

	void PatrolBehavior(World* aWorld);

	void ChaseBehavior(World* aWorld, Avatar* avatar);

	void RespawningBehavior(World* aWorld);

	void DieBehavior(World* aWorld);

	void Update(float aTime, World* aWorld, Avatar* avatar);

	bool myIsClaimableFlag;

	bool myIsDeadFlag;

	void ContinuePreviousBehavior();

	GhostBehavior GetBehavior() { return myBehavior; }

	void SetBehavior(GhostBehavior _behavior);

	bool SensedAvatar(Avatar*);

	std::list<PathmapTile*>* GetPath() { return &myPath; };

protected:
	std::list<PathmapTile*> myPath;

	float myGhostGhostCounter;

	int myDesiredMovementX;

	int myDesiredMovementY;

private:
	int patrolBorderMin = -6;
	int patrolBorderMax = 6;

	GhostBehavior myBehavior;
	GhostBehavior previousBehavior;

	Vector2f patrolPoint;
	Vector2f targetPosition;

};

#endif // GHOST_H