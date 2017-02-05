#include "Game.h"
#include "util.h"

#include <ctime>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

long long getTimeInMilliSeconds()
{
#ifdef _WIN32
    SYSTEMTIME tv;
    GetSystemTime(&tv);
    return tv.wSecond * 1000 + tv.wMilliseconds;
#else
    timeval tv;
    gettimeofday(&tv, NULL);
    return 1000 * tv.tv_sec + tv.tv_usec / 1000;
#endif
}

Game::Game()
{
    // turnStartTime = -1;
    // map = NULL;
    // lossRate1 = 0;
    // lossRate2 = 0;
    // escape = 0;
    // totalTurns = 0;
    // turn = 0;
    // turnTimeout = 0;
    // myID = 0;
    // firstlvl = 0;
    // secondlvl = 0;
    // nodeBonus = 0;
    // edgeBonus = 0;
}

Game::~Game() {}

void Game::setConstants(Json::Value &msg)
{
    // this->totalTurns = msg["turns"].asInt();
    // this->turnTimeout = msg["turnTimeout"].asInt();
    // this->escape = msg["escape"].asInt();
    // this->nodeBonus = msg["nodeBonus"].asInt();
    // this->edgeBonus = msg["edgeBonus"].asInt();
    // this->firstlvl = msg["firstlvl"].asInt();
    // this->secondlvl = msg["secondlvl"].asInt();
    // this->lossRate1 = msg["lossRate1"].asInt();
    // this->lossRate2 = msg["lossRate2"].asInt();
}

int Game::getTotalTurns()
{
    return this->totalTurns;
}

void Game::handleInitMessage(Message msg)
{
    Json::Value &argsArray = msg.getArray("args");

    Json::UInt I = 0;
    Json::UInt zero = 0;

    this->myID = argsArray[I++].asInt();

    Json::Value &sizeArray = argsArray[I++];
    size = std::make_pair(sizeArray[zero].asInt(), sizeArray[zero + 1].asInt());

    Json::Value &fishArray = argsArray[I++];

    for (Json::UInt i = 0; i < fishArray.size(); i++)
    {
	fishes.push_back(new Fish(fishArray[i][zero + 0].asInt(), fishArray[i][zero + 1].asInt(), fishArray[i][zero + 2].asInt(), fishArray[i][zero + 3].asBool(), fishArray[i][zero + 4].asBool(), fishArray[i][zero + 5].asBool(), fishArray[i][zero + 6].asBool()));
    }

    Json::Value &foodArray = argsArray[I++];
    for (int i = 0; i < foodArray.size(); i++)
    {
	foods.push_back({foodArray[i][zero + 0].asInt(), foodArray[i][zero + 1].asInt()});
    }

    Json::Value &trashArray = argsArray[I++];
    for (int i = 0; i < trashArray.size(); i++)
    {
	trashes.push_back({trashArray[i][zero + 0].asInt(), trashArray[i][zero + 1].asInt()});
    }

    Json::Value &netArray = argsArray[I++];
    for (int i = 0; i < netArray.size(); i++)
    {
	nets.push_back({netArray[i][zero + 0].asInt(), netArray[i][zero + 1].asInt()});
    }

    Json::Value &teleportArray = argsArray[I++];
    for (int i = 0; i < teleportArray.size(); i++)
    {
	teleports.push_back({{teleportArray[i][zero + 0].asInt(), teleportArray[i][zero + 1].asInt()}, {teleportArray[i][zero + 2].asInt(), teleportArray[i][zero + 3].asInt()}});
    }

    Json::Value &constants = argsArray[I++];
    this->setConstants(constants);

    // updateNodesList();
}

void Game::handleTurnMessage(Message msg)
{
    turnStartTime = getTimeInMilliSeconds();

    Json::Value &argsArray = msg.getArray("args");
    Json::UInt I = 0;
    Json::UInt zero = 0;
    turn = argsArray[I++].asInt();

    Json::Value &scores = argsArray[I++];
    score = std::make_pair(scores[zero + 0].asInt(), scores[zero + 1].asInt());

    Json::Value &graphDiff = argsArray[I++]; //TODO: to be defined later
					     // for (int i = 0; i < (int)graphDiff.size(); i++)
					     // {
					     // 	Json::Value &nodeDiff = graphDiff[i];
					     // 	Json::UInt J=0;
					     // 	int nodeIndex = nodeDiff[J++].asInt();
					     // 	map->getNode(nodeIndex)->setOwner(nodeDiff[J++].asInt());
					     // 	map->getNode(nodeIndex)->setArmyCount(nodeDiff[J++].asInt());
					     // }

    // // 	updateNodesList();
}

// // void Game::updateNodesList()
// // {
// // 	for(int i = 0; i < 3; i++)
// // 		this->nodes[i].clear();
// // 	for (Node* n : this->map->getNodes())
// // 	{
// // 		nodes[n->getOwner() + 1].push_back(n);
// // 	}
// // }

long long Game::getTurnTimePassed()
{
    return getTimeInMilliSeconds() - turnStartTime;
}

long long Game::getTurnRemainingTime()
{
	return turnTimeout - getTurnTimePassed();
}

int Game::getMyId()
{
	return myID;
}

Size getSize()
{
	return this->size;
}

std::vector<Roach *> &getRoaches()
{
	return this->roaches;
}

std::vector<Cell *> &getFood()
{
	return this->food;
}

std::vector<Cell *> &getTrash()
{
	return this->trash;
}

std::vector<Cell *> &getSlippers()
{
	return this->slippers;
}

std::vector<Teleport *> &getTeleports()
{
	return this->teleports;
}

int Game::getTurnNumber()
{
	return this->turn;
}

long long Game::getTotalTurnTime()
{
	return this->turnTimeout;
}

void Game::changeStrategy(Antenna t, int i, int j, int k, Move s)
{
	GameEvent* ev = new GameEvent(Constants::TYPE_CHANGE_STRATEGY);
	ev->addArg(static_cast<int>(t));
	ev->addArg(i);
	ev->addArg(j);
	ev->addArg(k);
	ev->addArg(static_cast<int>(s));
	eventHandler->addEvent(ev);
}

void Game::deterministicMove(const Roach &roach, Move s)
{
	GameEvent* ev = new GameEvent(Constants::TYPE_DETERMINISTIC_MOVE);
	ev->addArg(roach.getId());
	ev->addArg(static_cast<int>(s));
	eventHandler->addEvent(ev);
}

void Game::antennaChange(const Roach& roach, Antenna t)
{
	GameEvent* ev = new GameEvent(Constants::TYPE_ANTENNA_CHANGE);
	ev->addArg(roach.getId());
	ev->addArg(static_cast<int>(t));
	eventHandler->addEvent(ev);
}
