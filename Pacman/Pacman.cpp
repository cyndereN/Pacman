#include "Pacman.h"
#include "Drawer.h"
#include "SDL.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "Avatar.h"
#include "World.h"
#include "Ghost.h"



// Creatw Pacman with reference to Drawer
Pacman* Pacman::Create(Drawer* aDrawer)
{
	Pacman* pacman = new Pacman(aDrawer);

	// MVC pattern, init world
	if (!pacman->Init())
	{
		delete pacman;
		pacman = NULL;
	}

	return pacman;
}

// Constructor
Pacman::Pacman(Drawer* aDrawer)
: myDrawer(aDrawer)
, myTimeToNextUpdate(0.f)
, myNextMovement(-1.f,0.f)
, myCurrentMovement(-1.f,0.f)
, myScore(0)
, myFps(0)
, myLives(3)
{
	// For initialise entities
	Sprite* newSprite = NULL;
	std::map<std::string, std::string> assetPaths;

	// Avatar
	assetPaths["closedRight"] = "closed_right_32.png";
	assetPaths["openRight"] = "open_right_32.png";
	assetPaths["closedDown"] = "closed_down_32.png";
	assetPaths["openDown"] = "open_down_32.png";
	assetPaths["closedLeft"] = "closed_left_32.png";
	assetPaths["openLeft"] = "open_left_32.png";
	assetPaths["closedUp"] = "closed_up_32.png";
	assetPaths["openUp"] = "open_up_32.png";
	myAvatar = new Avatar(Vector2f(13 * TILESIZE, 22 * TILESIZE), Sprite::Create(assetPaths, myDrawer, 32, 32));
	assetPaths.clear();

	// Cyan ghost (TL)
	assetPaths.clear();
	assetPaths["vulnerable"] = "Ghost_Vulnerable_32.png";
	assetPaths["dead"] = "Ghost_Dead_32.png";
	assetPaths["normal"] = "ghost_32_cyan.png";

	Ghost* ghost = new Ghost(Vector2f(15 * TILESIZE, 12 * TILESIZE),
		Sprite::Create(assetPaths, myDrawer, 32, 32),
		Vector2f(6* TILESIZE, 4* TILESIZE)
	);

	ghosts.push_back(ghost);
	assetPaths.clear();
	
	// Orange Ghost (BR)
	assetPaths.clear();
	assetPaths["vulnerable"] = "Ghost_Vulnerable_32.png";
	assetPaths["dead"] = "Ghost_Dead_32.png";
	assetPaths["normal"] = "ghost_32_orange.png";

	ghost = new Ghost(Vector2f(14 * TILESIZE, 12 * TILESIZE),
		Sprite::Create(assetPaths, myDrawer, 32, 32),
		Vector2f(20* TILESIZE, 23* TILESIZE)
	);

	ghosts.push_back(ghost);
	assetPaths.clear();

	// Pink Ghost  (BL)
	assetPaths.clear();
	assetPaths["vulnerable"] = "Ghost_Vulnerable_32.png";
	assetPaths["dead"] = "Ghost_Dead_32.png";
	assetPaths["normal"] = "ghost_32_pink.png";

	ghost = new Ghost(Vector2f(10 * TILESIZE, 12 * TILESIZE),
		Sprite::Create(assetPaths, myDrawer, 32, 32),
		Vector2f(5* TILESIZE, 23* TILESIZE)
	);
	assetPaths.clear();
	ghosts.push_back(ghost);
	
	// Red Ghost (TR)
	assetPaths["vulnerable"] = "Ghost_Vulnerable_32.png";
	assetPaths["dead"] = "Ghost_Dead_32.png";
	assetPaths["normal"] = "ghost_32_red.png";

	ghost = new Ghost(Vector2f(11 * TILESIZE, 12 * TILESIZE),
		Sprite::Create(assetPaths, myDrawer, 32, 32),
		Vector2f(19* TILESIZE, 4* TILESIZE)
	);
	ghosts.push_back(ghost);
	assetPaths.clear();

	myWorld = new World(myAvatar, myDrawer);
	
}

Pacman::~Pacman(void)
{
}

bool Pacman::Init()
{
	myWorld->Init();

	// Init the state of ghosts
	std::list<Ghost*>::iterator ghostIterator;
	for (ghostIterator = ghosts.begin(); ghostIterator != ghosts.end(); ghostIterator++)
		(*ghostIterator)->RespawningBehavior(myWorld);
	return true;
}


// After losing lifes, restart
void Pacman::Restart(std::list<Ghost*>::iterator ghostIterator)
{
	myAvatar->Respawn(myAvatar->respawnPoint);
	myNextMovement = Vector2f(-1.f, 0.f);
	myCurrentMovement = Vector2f(-1.f, 0.f);

	(*ghostIterator)->Respawn((*ghostIterator)->respawnPoint);
	(*ghostIterator)->RespawningBehavior(myWorld);

}

bool Pacman::Update(float aTime)
{

	if (!UpdateInput())
		return false;

	// Game "Paused", return directly
	if (CheckEndGameCondition())
		return true;
	else if (myLives <= 0)
		return true;

	// Update Avatar
	MoveAvatar();
	myAvatar->Update(aTime);

	// Update Ghosts
	std::list<Ghost*>::iterator ghostIterator;
	for (ghostIterator = ghosts.begin(); ghostIterator != ghosts.end(); ghostIterator++)
		(*ghostIterator)->Update(aTime, myWorld, myAvatar);


	// Check collisions with game entity
	if (myWorld->HasIntersectedDot(myAvatar->GetPosition())) {
		myScore += 10;
	}

	if (myWorld->HasIntersectedCherry(myAvatar->GetPosition())) {
		myScore += 30;
	}

	if (myWorld->HasIntersectedBigDot(myAvatar->GetPosition()))
	{
		myScore += 20;
		for (ghostIterator = ghosts.begin(); ghostIterator != ghosts.end(); ghostIterator++) {
			if ((*ghostIterator)->GetBehavior() != Ghost::GhostBehavior::Dead)
				(*ghostIterator)->SetBehavior(Ghost::GhostBehavior::Fear);
		}
	}

	//Check avatar collision with ghost
	for (ghostIterator = ghosts.begin(); ghostIterator != ghosts.end(); ghostIterator++)
	{
		if (((*ghostIterator)->GetPosition() - myAvatar->GetPosition()).Length() < 16.f)
		{
			if ((*ghostIterator)->GetBehavior() != Ghost::GhostBehavior::Fear && (*ghostIterator)->GetBehavior() != Ghost::GhostBehavior::Dead)
			{
				myLives--;
				
				Restart(ghostIterator);
			}
			else if ((*ghostIterator)->GetBehavior() == Ghost::GhostBehavior::Fear)
			{
				myScore += 50;
				(*ghostIterator)->DieBehavior(myWorld);
			}
		}
	}
	
	
	if (aTime > 0)
		myFps = (int) (1 / aTime);
	
	return true;
}

bool Pacman::UpdateInput()
{
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);
	
	if (keystate[SDL_SCANCODE_UP]) {
		
		myNextMovement = Vector2f(0.f, -1.f);
	}
		
	else if (keystate[SDL_SCANCODE_DOWN]) {

		myNextMovement = Vector2f(0.f, 1.f);
	}
		
	else if (keystate[SDL_SCANCODE_RIGHT]) {

		myNextMovement = Vector2f(1.f, 0.f);
	}
		
	else if (keystate[SDL_SCANCODE_LEFT]) {

		myNextMovement = Vector2f(-1.f, 0.f);
	}
		

	if (keystate[SDL_SCANCODE_ESCAPE])
		return false;

	return true;
}

// Check validity of movement, if so, do move
void Pacman::MoveIfValid() 
{
	int nextTileX = myAvatar->GetCurrentTileX() + myNextMovement.myX;
	int nextTileY = myAvatar->GetCurrentTileY() + myNextMovement.myY;


	// There is a tube in the middle
	if (nextTileY == 13 && nextTileX == -1)
		nextTileX = 25;
	else if (nextTileY == 13 && nextTileX == 26)
		nextTileX = 0;

	if (myWorld->TileIsValid(nextTileX, nextTileY))
	{
		myAvatar->SetNextTile(nextTileX, nextTileY);
		myCurrentMovement = myNextMovement;
	}

		
}

void Pacman::MoveAvatar()
{
	if (myAvatar->IsAtDestination())
	{
		int intendedNextTileX = myAvatar->GetCurrentTileX() + myNextMovement.myX;
		int intendedNextTileY = myAvatar->GetCurrentTileY() + myNextMovement.myY;

		// There is a tube in the middle
		if (intendedNextTileY == 13 && intendedNextTileX == -1)
			intendedNextTileX = 25;
		else if (intendedNextTileY == 13 && intendedNextTileX == 26)
			intendedNextTileX = 0;
		
		// Ignore intended move command if next tile is not valid
		if (!myWorld->TileIsValid(intendedNextTileX, intendedNextTileY))
			myNextMovement = myCurrentMovement;
		
		MoveIfValid();

	}
}

bool Pacman::CheckEndGameCondition()
{
	return myWorld->GetDotCount() == 0;
}

bool Pacman::Draw()
{
	myWorld->Draw(myDrawer);
	myAvatar->Draw(myDrawer);

	std::list<Ghost*>::iterator ghostIterator;
	for (ghostIterator = ghosts.begin(); ghostIterator != ghosts.end(); ghostIterator++)
		(*ghostIterator)->Draw(myDrawer);
	DrawUIText();
	return true;
}

void Pacman::DrawUIText() 
{
	std::string scoreString;
	std::stringstream scoreStream;
	scoreStream << myScore;
	scoreString = scoreStream.str();
	myDrawer->DrawText("Score", "freefont-ttf\\sfd\\FreeMonoBold.ttf", Vector2f(20, 50), { 255,255,255,255 });
	myDrawer->DrawText(scoreString.c_str(), "freefont-ttf\\sfd\\FreeMono.ttf", Vector2f(90, 50), { 255,255,0,255 });

	std::string livesString;
	std::stringstream liveStream;
	liveStream << myLives;
	livesString = liveStream.str();
	myDrawer->DrawText("Lives", "freefont-ttf\\sfd\\FreeMonoBold.ttf", Vector2f(20, 80), { 255,255,255,255 });
	myDrawer->DrawText(livesString.c_str(), "freefont-ttf\\sfd\\FreeMono.ttf", Vector2f(90, 80), { 0 , 255, 0, 255 });

	myDrawer->DrawText("FPS", "freefont-ttf\\sfd\\FreeMonoBold.ttf", Vector2f(880, 50), { 255, 255, 255, 255 });
	std::string fpsString;
	std::stringstream fpsStream;
	fpsStream << myFps;
	fpsString = fpsStream.str();
	myDrawer->DrawText(fpsString.c_str(), "freefont-ttf\\sfd\\FreeMono.ttf", Vector2f(930, 50), { 255,255,255,255 });

	if (CheckEndGameCondition())
		myDrawer->DrawText("You win!", "freefont-ttf\\sfd\\FreeMono.ttf", Vector2f(450, 50), { 255,255,255,255 });

	if (myLives <= 0)
		myDrawer->DrawText("You lose!", "freefont-ttf\\sfd\\FreeMono.ttf", Vector2f(450, 50), { 255,255,255,255 });
}
