#include "Ghost.h"
#include "World.h"
#include "PathmapTile.h"
#include "Drawer.h"
#include "Avatar.h"

// Constructor
Ghost::Ghost(const Vector2f& aPosition, Sprite* sprite, const Vector2f& aPatrolPosition)
: MovableGameEntity(aPosition, sprite),
myBehavior(Respawning),
patrolPoint(aPatrolPosition),
myGhostGhostCounter(0.f)
{
	myIsClaimableFlag = false;
	myIsDeadFlag = false;

	myDesiredMovementX = 0;
	myDesiredMovementY = -1;

	SetSpeed(100.f);
}

Ghost::~Ghost(void)
{
}

void Ghost::PatrolBehavior(World* aWorld) 
{
	myPath.clear();

	// Random wandering around patrol point
	
	int dX = 0, dY = 0;
	do {
		dX = (rand() % (patrolBorderMax - patrolBorderMin + 1)) + patrolBorderMin;
		dY = (rand() % (patrolBorderMax - patrolBorderMin + 1)) + patrolBorderMin;
	} while (!aWorld->TileIsValid(dX + patrolPoint.myX/ TILESIZE, dY + patrolPoint.myY/ TILESIZE));

	targetPosition.myX = dX* TILESIZE + patrolPoint.myX;
	targetPosition.myY = dY* TILESIZE + patrolPoint.myY;

	aWorld->GetPath(this, targetPosition.myX, targetPosition.myY);
	SetBehavior(Patrol);
}

void Ghost::ChaseBehavior(World* aWorld, Avatar* avatar) 
{
	myPath.clear();

	targetPosition.myX = TILESIZE * avatar->GetCurrentTileX();
	targetPosition.myY = TILESIZE * avatar->GetCurrentTileY();

	aWorld->GetPath(this, targetPosition.myX, targetPosition.myY);
	SetBehavior(Chase);
}

void Ghost::RespawningBehavior(World* aWorld) 
{
	
	myPath.clear();
	aWorld->GetPath(this, patrolPoint.myX, patrolPoint.myY);
	SetBehavior(Respawning);
}

void Ghost::DieBehavior(World* aWorld)
{
	
	myPath.clear();

	// Reverse the path to make it looks more "smart"
	PathmapTile* tile = nullptr;
	for (int i = 0; i < 26; i++)
	{
		for (int j = 0; j < 29; j++)
		{
			tile = aWorld->GetTile(i, j);
			tile->myIsVisitedFlag = false;
		}
	}
	auto respawnPointTile = aWorld->GetTile(respawnPoint.myX / TILESIZE, respawnPoint.myY / TILESIZE);

	myPath.push_back(respawnPointTile);
	aWorld->Pathfind(respawnPointTile, aWorld->GetTile(myCurrentTileX, myCurrentTileY), *(GetPath()));
	SetBehavior(Dead);
}

// Exiting Fear state
void Ghost::ContinuePreviousBehavior() 
{
	SetBehavior(previousBehavior);
}

// For changing or setting behavior (state)
void Ghost::SetBehavior(GhostBehavior _behavior) 
{
	
	if(myBehavior != Fear)
		previousBehavior = myBehavior;
	myBehavior = _behavior;

	if (myBehavior == Respawning) {
		sprite->SetFrame("normal");
		SetSpeed(100.f);
	}
	else if (myBehavior == Patrol) {
		sprite->SetFrame("normal");
		SetSpeed(100.f);
	}
	else if (myBehavior == Chase)
	{
		sprite->SetFrame("normal");
		SetSpeed(100.f); 
	}
	else if (myBehavior == Fear)
	{
		myIsClaimableFlag = true;
		myGhostGhostCounter = 5.f;
		sprite->SetFrame("vulnerable");
		SetSpeed(50.f);
		
	}
	else if (myBehavior == Dead)
	{
		sprite->SetFrame("dead");
		myGhostGhostCounter = 0.f;
		SetSpeed(120.f);
		myIsDeadFlag = true;
		myIsClaimableFlag = false;
	}
}

// Trigger chase state
bool Ghost::SensedAvatar(Avatar* avatar)
{
	int avatarX = avatar->GetCurrentTileX();
	int avatarY = avatar->GetCurrentTileY();

	if (patrolBorderMin + patrolPoint.myX/ TILESIZE <= avatarX && avatarX <= patrolBorderMax + patrolPoint.myX/ TILESIZE
		&& patrolBorderMin + patrolPoint.myY/ TILESIZE <= avatarY && avatarY <= patrolBorderMax + patrolPoint.myY/ TILESIZE)
		return true;

	return false;
}


void Ghost::Update(float aTime, World* aWorld, Avatar* avatar)
{
	myGhostGhostCounter -= aTime;

	switch (myBehavior)
	{
		case Respawning:
			if (myPosition == patrolPoint) {
				PatrolBehavior(aWorld);
			}
			break;
		case Patrol:
			if (SensedAvatar(avatar)) {
				ChaseBehavior(aWorld, avatar);
			} 
			if (myPosition == targetPosition) {
				PatrolBehavior(aWorld);
			}
			break;
		case Chase:
			if (myPosition == targetPosition) {
				PatrolBehavior(aWorld);
			}

			break;
		case Fear:
			if (myGhostGhostCounter <= 0)
			{
				myIsClaimableFlag = false;
				ContinuePreviousBehavior();
			}
			break;

		case Dead:
			if (myPosition == respawnPoint) {
				myIsDeadFlag = false;
				RespawningBehavior(aWorld);
			}
			break;
		default:
			break;
	}


	if (IsAtDestination())
	{

		//This is to get at least 1 element inpath
		if (!myPath.empty()){
			PathmapTile* nextTile = nullptr;
			if (myBehavior == Dead)
			{
				nextTile = myPath.back();
				myPath.pop_back();
			}
			else
			{
				nextTile = myPath.front();
				myPath.pop_front();
			}
			SetNextTile(nextTile->myX, nextTile->myY);
		}
		
		
	}

	Vector2f destination(myNextTileX * TILESIZE, myNextTileY * TILESIZE);
	Vector2f direction = destination - myPosition;

	float distanceToMove = aTime * speed;

	if (distanceToMove > direction.Length())
	{
		myPosition = destination;

		myPreviousTileX = myCurrentTileX;
		myPreviousTileY = myCurrentTileY;

		myCurrentTileX = myNextTileX;
		myCurrentTileY = myNextTileY;
	}
	else
	{
		direction.Normalize();
		myPosition += direction * distanceToMove;
	}
}

