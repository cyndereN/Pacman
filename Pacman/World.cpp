#include "World.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "PathmapTile.h"
#include "Dot.h"
#include "BigDot.h"
#include "Cherry.h"
#include "Drawer.h"


World::World(Avatar* avatar, Drawer* gameDrawer)
{
	myDrawer = gameDrawer;
	myAvatar = avatar;

	// Allocate
	for (int i = 0; i < 26; i++)
		for (int j = 0; j < 29; j++)
			worldMap[i][j] = new PathmapTile();

}

World::~World(void)
{
	// De-Allocate so we don't have memory leak
	for (int i = 0; i < 26; i++)
		delete[] worldMap[i];
	delete[] worldMap;
}

void World::Init()
{
	std::map<std::string, std::string> assetPaths;
	assetPaths["playfield"] = "playfield.png";
	boardBackground = Sprite::Create(assetPaths, myDrawer, 1024, 768);
	boardBackground->SetFrame("playfield");
	InitPathmap();
}

bool World::InitPathmap()
{
	std::map<std::string, std::string> assetPaths;
	Sprite* newSprite = NULL;

	std::string line;
	std::ifstream myfile("map.txt");

	if (myfile.is_open())
	{
		int lineIndex = 0;
		while (!myfile.eof())
		{
			std::getline(myfile, line);
			for (unsigned int i = 0; i < line.length(); i++)
			{
				PathmapTile* tile = new PathmapTile(i, lineIndex, (line[i] == 'x'));


				worldMap[tile->myX][tile->myY] = tile;

				if (line[i] == '.')
				{
					assetPaths.clear();
					assetPaths["smallDot"] = "Small_Dot_32.png";
					newSprite = Sprite::Create(assetPaths, myDrawer, 32, 32);
					newSprite->SetFrame("smallDot");
					Dot* dot = new Dot(Vector2f(i * 22, lineIndex * 22), newSprite);
					myDots.push_back(dot);
				}
				else if (line[i] == 'o')
				{
					assetPaths.clear();
					assetPaths["bigDot"] = "Big_Dot_32.png";
					newSprite = Sprite::Create(assetPaths, myDrawer, 32, 32);
					newSprite->SetFrame("bigDot");
					BigDot* dot = new BigDot(Vector2f(i * 22, lineIndex * 22), newSprite);
					myBigDots.push_back(dot);
				}
				else if (line[i] == 'c')
				{
					assetPaths.clear();
					assetPaths["cherry"] = "cherry.png";
					newSprite = Sprite::Create(assetPaths, myDrawer, 32, 32);
					newSprite->SetFrame("cherry");
					Cherry* cherry = new Cherry(Vector2f(i * 22, lineIndex * 22), newSprite);
					myCherries.push_back(cherry);
				}
			}

			lineIndex++;
		}
		myfile.close();
	}

	return true;
}



void World::Draw(Drawer* aDrawer)
{
	boardBackground->Draw(aDrawer, 0, 0);

	for(std::list<Dot*>::iterator list_iter = myDots.begin(); list_iter != myDots.end(); list_iter++)
	{
		Dot* dot = *list_iter;
		dot->Draw(aDrawer);
	}

	for(std::list<BigDot*>::iterator list_iter = myBigDots.begin(); list_iter != myBigDots.end(); list_iter++)
	{
		BigDot* dot = *list_iter;
		dot->Draw(aDrawer);
	}

	for (std::list<Cherry*>::iterator list_iter = myCherries.begin(); list_iter != myCherries.end(); list_iter++)
	{
		Cherry* cherry = *list_iter;
		cherry->Draw(aDrawer);
	}
}

bool World::TileIsValid(int anX, int anY)
{
	if (anX < 0 || anX >= 26)
		return false;
	if (anY < 0 || anY >= 29)
		return false;


	return !worldMap[anX][anY]->myIsBlockingFlag;
}


bool World::HasIntersectedDot(const Vector2f& aPosition)
{
	for(std::list<Dot*>::iterator list_iter = myDots.begin(); list_iter != myDots.end(); list_iter++)
	{
		Dot* dot = *list_iter;
		if ((dot->GetPosition() - aPosition).Length() < 5.f)
		{
			myDots.remove(dot);
			delete dot;
			return true;
		}
	}

	return false;
}

bool World::HasIntersectedBigDot(const Vector2f& aPosition)
{
	for(std::list<BigDot*>::iterator list_iter = myBigDots.begin(); list_iter != myBigDots.end(); list_iter++)
	{
		BigDot* dot = *list_iter;
		if ((dot->GetPosition() - aPosition).Length() < 5.f)
		{
			myBigDots.remove(dot);
			delete dot;
			return true;
		}
	}

	return false;
}

bool World::HasIntersectedCherry(const Vector2f& aPosition)
{
	for (std::list<Cherry*>::iterator list_iter = myCherries.begin(); list_iter != myCherries.end(); list_iter++)
	{
		Cherry* cherry = *list_iter;
		if ((cherry->GetPosition() - aPosition).Length() < 5.f)
		{
			myCherries.remove(cherry);
			delete cherry;
			return true;
		}
	}

	return false;
}

void World::GetPath(Ghost* ghost, int aToX, int aToY)
{
	PathmapTile* tile = nullptr;

	PathmapTile* fromTile = GetTile(ghost->GetCurrentTileX(), ghost->GetCurrentTileY());
	PathmapTile* toTile = GetTile(aToX / TILESIZE, aToY / TILESIZE);

	PathmapTile* previousTile = nullptr;

	// Blank all tiles
	for (int i = 0; i < 26; i++)
		for (int j = 0; j < 29; j++)
		{
			tile = worldMap[i][j];
			tile->myIsVisitedFlag = false;
		}

	// In chasing ghosts con not turn back
	if (ghost->GetBehavior() == Ghost::GhostBehavior::Chase)
	{
		tile = GetTile(ghost->GetPreviousTileX(), ghost->GetPreviousTileY());
		tile->myIsBlockingFlag = true;
		previousTile = tile;
	}

	// Set the ghost path
	Pathfind(fromTile, toTile, *(ghost->GetPath()));

	// Restore the previous tile if needed
	if (previousTile != nullptr)
		previousTile->myIsBlockingFlag = false;

}

PathmapTile* World::GetTile(int aFromX, int aFromY)
{
	if (aFromX < 0)
		aFromX = 0;
	if (aFromX >= 26)
		aFromX = 25;

	if (aFromY < 0)
		aFromY = 0;
	if (aFromY >= 29)
		aFromY = 28;

	return worldMap[aFromX][aFromY];
}

bool World::ListDoesNotContain(PathmapTile* aFromTile, std::list<PathmapTile*>& aList)
{
	for(std::list<PathmapTile*>::iterator list_iter = aList.begin(); list_iter != aList.end(); list_iter++)
	{
		PathmapTile* tile = *list_iter;
		if (tile == aFromTile)
		{
			return false;
		}
	}

	return true;
}

// Greedy search pathfinding, not optimal but decent path for ai
bool World::Pathfind(PathmapTile* aFromTile, PathmapTile* aToTile, std::list<PathmapTile*>& aList)
{
	aFromTile->myIsVisitedFlag = true;

	if (aFromTile->myIsBlockingFlag)
		return false;

	if (aFromTile == aToTile)
		return true;

	std::list<PathmapTile*> neighborList;

	PathmapTile* up = GetTile(aFromTile->myX, aFromTile->myY - 1);
	if (up != nullptr)
		if (up && !up->myIsVisitedFlag && !up->myIsBlockingFlag && ListDoesNotContain(up, aList))
			neighborList.push_front(up);

	PathmapTile* down = GetTile(aFromTile->myX, aFromTile->myY + 1);
	if (down != nullptr)
		if (down && !down->myIsVisitedFlag && !down->myIsBlockingFlag && ListDoesNotContain(down, aList))
			neighborList.push_front(down);

	PathmapTile* right = GetTile(aFromTile->myX + 1, aFromTile->myY);
	if (right != nullptr)
		if (right && !right->myIsVisitedFlag && !right->myIsBlockingFlag && ListDoesNotContain(right, aList))
			neighborList.push_front(right);

	PathmapTile* left = GetTile(aFromTile->myX - 1, aFromTile->myY);
	if (left != nullptr)
		if (left && !left->myIsVisitedFlag && !left->myIsBlockingFlag && ListDoesNotContain(left, aList))
			neighborList.push_front(left);

	neighborList.sort([aToTile](const PathmapTile* a, const PathmapTile* b)
		{
			int la = abs(a->myX - aToTile->myX) + abs(a->myY - aToTile->myY);
			int lb = abs(b->myX - aToTile->myX) + abs(b->myY - aToTile->myY);

			return la < lb;
		});

	for (std::list<PathmapTile*>::iterator list_iter = neighborList.begin(); list_iter != neighborList.end(); list_iter++)
	{
		PathmapTile* tile = *list_iter;

		aList.push_back(tile);

		if (Pathfind(tile, aToTile, aList))
			return true;

		aList.pop_back();
	}

	return false;
}