#ifndef PACMAN_H
#define PACMAN_H

#include "Vector2f.h"
#include <list>
#include "settings.h"

struct SDL_Surface;
class Drawer;
class Avatar;
class World;
class Ghost;

class Pacman
{
public:
	static Pacman* Create(Drawer* aDrawer);
	~Pacman(void);

	bool Update(float aTime);
	bool Draw();

private:
	Pacman(Drawer* aDrawer);
	bool Init();
	bool UpdateInput();
	void MoveAvatar();
	bool CheckEndGameCondition();
	void MoveIfValid();

	void Restart(std::list<Ghost*>::iterator);

	Drawer* myDrawer;

	void DrawUIText();

	float myTimeToNextUpdate;

	int myLives;
	int myScore;
	int myFps;
	int totalDots;

	Vector2f myNextMovement;
	Vector2f myCurrentMovement;

	Avatar* myAvatar;
	std::list<Ghost*> ghosts;
	World* myWorld;

};

#endif // PACMAN_H