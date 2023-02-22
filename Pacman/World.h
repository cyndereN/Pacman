#ifndef WORLD_H
#define WORLD_H

#include <list>
#include <map>
#include "Vector2f.h"
#include "Sprite.h"
#include "Ghost.h"
#include "settings.h"

class Drawer;
class PathmapTile;
class Dot;
class BigDot;
class Cherry;
class Avatar;
class Ghost;

class World
{
public:
	World(Avatar* avatar, Drawer* gameDrawer);
	~World(void);

	void Init();

	void Draw(Drawer* aDrawer);
	bool TileIsValid(int anX, int anY);

	bool HasIntersectedDot(const Vector2f& aPosition);
	bool HasIntersectedBigDot(const Vector2f& aPosition);
	bool HasIntersectedCherry(const Vector2f& aPosition);

	int GetDotCount() { return myDots.size() + myBigDots.size(); };

	void GetPath(Ghost* ghost, int aToX, int aToY);

	PathmapTile* GetTile(int aFromX, int aFromY);

	bool Pathfind(PathmapTile* aFromTile, PathmapTile* aToTile, std::list<PathmapTile*>& aList);

private:

	bool ListDoesNotContain(PathmapTile* aFromTile, std::list<PathmapTile*>& aList);


	bool InitPathmap();

	PathmapTile* worldMap[26][29];
	std::list<Dot*> myDots;
	std::list<BigDot*> myBigDots;
	std::list<Cherry*> myCherries;

	Drawer* myDrawer;
	Sprite* boardBackground;
	Avatar* myAvatar;

};

#endif // WORLD_H