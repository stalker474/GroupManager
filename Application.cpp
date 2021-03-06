#include "Game.h"
#include "Obstacle.h"
#include "Group.h"
#include "FormationStateMachine.h"
#include "Squad.h"

using namespace std;
using namespace TestGame;

void populateMap(shared_ptr<Map> M)
{
	float size = static_cast<float>(600 / 20);
	auto obs = make_shared<Obstacle>(size, size);
	auto obs2 = make_shared<Obstacle>(size, size);
	auto obs3 = make_shared<Obstacle>(size, size);
	obs->SetLocation({ 10,10 });
	obs2->SetLocation({ 18,18 });
	obs3->SetLocation({ 1,1 });
	M->AddActor(obs);
	M->AddActor(obs2);
	M->AddActor(obs3);
}

int main()
{
	Game game({600,600});
	auto map = make_shared<Map>("Basic map",150,150);
	//populateMap(map);
	game.LoadMap(map);

	auto blueTeam = make_shared<Squad>(3,ETeam::Blue);
	auto redTeam = make_shared<Squad>(3, ETeam::Red);

	blueTeam->UpdatePosition({ 40,10 }, {0, 1}, true);
	redTeam->UpdatePosition({ 110,70 }, {0, -1}, true);

	blueTeam->Spawn(map);
	redTeam->Spawn(map);

	double lastX, lastY;

	game.Run(
	//mouse pressed
	[&](double x,double y, bool left)
	{
		lastX = x;
		lastY = y;
	},
	//mouse released
	[&](double x, double y, bool left)
	{
		sf::Vector2f orientation{(float)(x-lastX),(float)(y-lastY)};
		if (left)
			blueTeam->Goto({ (float)lastX,(float)lastY }, orientation);
		else
			redTeam->Goto({ (float)lastX,(float)lastY }, orientation);
	},
	[&](shared_ptr<Actor> Target, bool left)
	{
		if (shared_ptr<Unit> unit = dynamic_pointer_cast<Unit>(Target))
		{
			if (unit->GetTeamStatus() == GroupManager::ETeam::ALLY) //blue
			{
				if (!left)
					redTeam->Attack(blueTeam);
			}
			else
				if (left)
					blueTeam->Attack(redTeam);
		}
	},
	[&](sf::Keyboard::Key Key)
	{
		if (Key == sf::Keyboard::R)
		{
			redTeam->AddUnits(1);
			redTeam->Unspawn(map);
			redTeam->Spawn(map);
		}
		else if (Key == sf::Keyboard::B)
		{
			blueTeam->AddUnits(1);
			blueTeam->Unspawn(map);
			blueTeam->Spawn(map);
		}
			
	}
	);
    return 0;
}

