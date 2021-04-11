// Agent.cc
//
// This code works only for the testworld that comes with the simulator.
//Joseph
//Lewis
//HW 9
//I tried moving my global variables to the .h file. I had some errors doing that.

#include <iostream>
#include <list>
#include "Agent.h"

using namespace std;

int stench[11][11]; //array of stench locations
int breeze[11][11];//breeze locations
int frontier[11][11];//possible pit locations not close to stench
int wumpusX = 0; //wumpus locations
int wumpusY = 0;
int goldX = 0;//gold location
int goldY = 0;
int pits[11][11]; //array of pit locations
int visited[11][11]; //array of visited locations 
int safe[11][11]; //array of safe locations
int noWumpus[11][11];//key areas by stenches that has no wumpus
int maxSize = 3; //assume size 3 and then adjust
int locx = 1; //agents location
int locy = 1;
int rightF = 1;//agent direction facing
int leftF = 0;
int upF = 0;
int downF = 0;
int hasGold = 0;//having gold
int foundMaxSize = 0;//bumped into the highest x or Y wall
int Danger = 0;//if breeze or stench Danger = 1
int nextx = 0;//The target safe location to go to
int nexty = 0;
int hailx = 0;//stench locations to shoot a hail marry shot
int haily = 0;
int firstMove = 0;//first move of a game
int firstTry = 1;//first try of multiple tries
int hasToShoot = 0;//does the user have to shoot
int hasArrow = 1; //does the user have the arrow
int inPit = 0; //determine if agent fell in the pit
int deadWumpus = 0;
int takeTheShot = 0;//take the shot this turn
int hailMarry = 0;//had to shoot to find wumpus


Action action;

//constructor
Agent::Agent ()
{

}

//deconstructor
Agent::~Agent ()
{

}

//initailizes the agent
void Agent::Initialize ()
{
	if(firstTry == 1)//set up arrays if first try
	{
		for(int x = 0; x < 10; x++)
		{
			for(int y = 0; y < 10; y++)
			{
				stench[x][y] = 0;
				pits[x][y] = 0;
				visited[x][y] = 0;
				safe[x][y] = 0;
				breeze[x][y] = 0;
				frontier[x][y] = 0;
				noWumpus[x][y] = 0;
			}
		}
	}
	//if  not first try, then have all safe locations marked
	//defensive programing
	if(firstTry == 0)
	{
		for(int x = 1; x < 10; x++)
		{
			for(int y = 0; y < 10; y++)
			{
				searchEngine.RemoveSafeLocation(locx,locy-1);
			}
		}
		for(int x = 1; x < 10; x++)
		{
			for(int y = 0; y < 10; y++)
			{
				if(visited[x][y] == 1 || safe[x][y] == 1)
				{
					searchEngine.AddSafeLocation(x,y);
				}
			}
		}
	}
	if(wumpusX != 0 && wumpusY != 0)
	{
		searchEngine.RemoveSafeLocation(wumpusX, wumpusY);
		safe[wumpusX][wumpusY] = 0;
	}
	//set defualt values
	searchEngine.AddSafeLocation(1,1);
	firstMove = 1;
	firstTry = 0;
	visited[1][1] = 1;
	actionList.clear();
	hasGold = 0;
	locx = 1;
	locy = 1;
	rightF = 1;//set facing
	leftF = 0;
	upF = 0;
	downF = 0;
	deadWumpus = 0;//wumpus is back alive
	hasArrow = 1;//set arrow
	inPit = 0;
}



Action Agent::Process (Percept& percept)
{
	list<Action> actionList2;
	Danger = 0;
	cout << "wumpus \n";
	cout << wumpusX;
	cout << wumpusY;
	cout << "\n";
	if(wumpusX != 0 && wumpusY != 0 && deadWumpus == 0)
	{
		searchEngine.RemoveSafeLocation(wumpusX, wumpusY);
		safe[wumpusX][wumpusY] = 0;
	}
	if(percept.Bump)//if far end bump we know the size of the world
	{
		if(rightF == 1)
		{
			foundMaxSize = 1;
			maxSize = locx;
		}
		else if(upF == 1)
		{
			foundMaxSize = 1;
			maxSize = locy;
		}
	}
	else if(action == GOFORWARD && firstMove == 0)//update cordinates and facing
	{
		if(rightF == 1)
		{
			locx += 1;
		}
		else if(leftF == 1)
		{
			locx -= 1;
		}
		else if(upF == 1)
		{
			locy += 1;
		}
		else if(downF == 1)
		{
			locy -= 1;
		}
	}
	else if(action == TURNRIGHT)
	{
		if(rightF == 1)
		{
			rightF = 0;
			downF = 1;
		}
		else if(leftF == 1)
		{
			leftF = 0;
			upF = 1;
		}
		else if(upF == 1)
		{
			upF = 0;
			rightF = 1;
		}
		else if(downF == 1)
		{
			downF = 0;
			leftF = 1;
		}
	}
	else if(action == TURNLEFT)
	{
		if(rightF == 1)
		{
			rightF = 0;
			upF = 1;
		}
		else if(leftF == 1)
		{
			leftF = 0;
			downF = 1;
		}
		else if(upF == 1)
		{
			upF = 0;
			leftF = 1;
		}
		else if(downF == 1)
		{
			downF = 0;
			rightF = 1;
		}
	}
	else if(action == SHOOT)
	{
		hasArrow = 0;
		if(!percept.Scream)//did the agent miss
		{
			if(rightF == 1)
			{
				noWumpus[locx+1][locy] = 1;
			}
			else if(leftF == 1)
			{
				noWumpus[locx-1][locy] = 1;
			}
			else if(upF == 1)
			{
				noWumpus[locx][locy+1] = 1;
			}
			else if(downF == 1)
			{
				noWumpus[locx][locy-1] = 1;
			}
		}
	}
	
	cout << "cord\n";
	cout << locx;
	cout << ", ";
	cout << locy;
	cout << "\n";
	firstMove = 0;
	visited[locx][locy] = 1;
	frontier[locx][locy] = 0;//not a pit
	if(percept.Scream)//if scream mark wumpus safe for the round
	{
		cout<<"\n marked wumpus safe\n";
		deadWumpus = 1;
		hasToShoot = 1;
		if(wumpusX == 0 && wumpusY == 0)
		{
			if(rightF == 1)
			{
				wumpusX = locx + 1;
				wumpusY = locy;
			}
			else if(leftF == 1)
			{
				wumpusX = locx - 1;
				wumpusY = locy;
			}
			else if(upF == 1)
			{
				wumpusX = locx;
				wumpusY = locy + 1;
			}
			else if(downF == 1)
			{
				wumpusX = locx;
				wumpusY = locy - 1;
			}
		}
		searchEngine.AddSafeLocation(wumpusX,wumpusY);
		safe[wumpusX][wumpusY] = 1;
	}
	
	if(percept.Stench && deadWumpus == 0)//stench mark the location and find wumpus
	{
		Danger = 1;
		
		stench[locx][locy] = 1;
		if(locx > 1)//check left if not by a wall
		{
			if(stench[locx-1][locy+1] == 1)//diagnal
			{
				if(safe[locx][locy+1] == 1)//safe check
				{
					wumpusX = locx - 1;
					wumpusY = locy;
					searchEngine.RemoveSafeLocation(locx-1,locy);
				}
			}
			if(stench[locx-1][locy-1] == 1)
			{
				if(safe[locx][locy-1] == 1)
				{
					wumpusX = locx - 1;
					wumpusY = locy;
					searchEngine.RemoveSafeLocation(locx-1,locy);
				}
			}		
		}
		if(locy > 1)//check down
		{
			if(stench[locx-1][locy-1] == 1)
			{
				if(safe[locx-1][locy] == 1)
				{
					wumpusX = locx;
					wumpusY = locy - 1;
					searchEngine.RemoveSafeLocation(locx,locy-1);
				}
			}
			if(stench[locx+1][locy-1] == 1)
			{
				if(safe[locx+1][locy] == 1)
				{
					wumpusX = locx;
					wumpusY = locy - 1;
					searchEngine.RemoveSafeLocation(locx,locy-1);
				}
			}
		}
		if(locx < maxSize)//check right
		{
			if(stench[locx+1][locy-1] == 1)
			{
				if(safe[locx][locy-1] == 1)
				{
					wumpusX = locx + 1;
					wumpusY = locy;
					searchEngine.RemoveSafeLocation(locx+1,locy);
				}
			}
			if(stench[locx+1][locy+1] == 1)
			{
				if(safe[locx][locy+1] == 1)
				{
					wumpusX = locx + 1;
					wumpusY = locy;
					searchEngine.RemoveSafeLocation(locx+1,locy);
				}
			}
		}
		if(locy < maxSize)//check up
		{
			if(stench[locx+1][locy+1] == 1)
			{
				if(safe[locx+1][locy] == 1)
				{
					wumpusX = locx;
					wumpusY = locy + 1;
					searchEngine.RemoveSafeLocation(locx,locy+1);
				}
			}
			if(stench[locx-1][locy+1] == 1)
			{
				if(safe[locx-1][locy] == 1)
				{
					wumpusX = locx;
					wumpusY = locy + 1;
					searchEngine.RemoveSafeLocation(locx,locy+1);
				}
			}
		}
		
	}
	
	if(wumpusX == 0 && wumpusY == 0)//find wumpus new rule
	{
		//rule 2 if 3 of the ajacent stench locations are safe, pits, or frontier,
		//then we have found the wumpus
		for(int x = 0; x < 10; x++)
		{
			for(int y = 0; y < 10; y++)
			{
				if(stench[x][y] == 1)//if found stench confirm wupus location
				{
					int conUp = 0;
					int conDown = 0;
					int conLeft = 0;
					int conRight = 0;
					if(x > 1)//check left
					{
						if(visited[x-1][y] == 1 || pits[x-1][y] == 1 || frontier[x-1][y] == 1)
						{
							noWumpus[x-1][y] = 1;
							conLeft = 1;
						}
					}
					else
					{
						conLeft = 1;
					}
					if(y > 1)//check down
					{
						if(visited[x][y-1] == 1 || pits[x][y-1] == 1 || frontier[x][y-1] == 1)
						{
							noWumpus[x][y-1] = 1;
							conDown = 1;
						}
					}
					else
					{
						conDown = 1;
					}
					if(foundMaxSize == 0)
					{
						if(x < 9)
						{
							if(visited[x+1][y] == 1 || pits[x+1][y] == 1 || frontier[x+1][y] == 1)
							{
								noWumpus[x+1][y] = 1;
								conRight = 1;
							}
						}
						else
						{
							conRight = 1;
						}
						
						if(y < 9)
						{
							if(visited[x][y+1] == 1 || pits[x][y+1] == 1 || frontier[x][y+1] == 1)
							{
								noWumpus[x][y+1] = 1;
								conUp = 1;
							}
						}
						else
						{
							conUp = 1;
						}	
					}
					else
					{
						if(x < maxSize)
						{
							if(visited[x+1][y] == 1 || pits[x+1][y] == 1 || frontier[x+1][y] == 1)
							{
								noWumpus[x+1][y] = 1;
								conRight = 1;
							}
						}
						else
						{
							conRight = 1;
						}
						
						if(y < maxSize)
						{
							if(visited[x][y+1] == 1 || pits[x][y+1] == 1 || frontier[x][y+1] == 1)
							{
								noWumpus[x][y+1] = 1;
								conUp = 1;
							}
						}
						else
						{
							conUp = 1;
						}	
						
					}
					
					//check to see if we found the wumpus
					if(conUp == 1 && conDown == 1 && conLeft == 1 && conRight == 0)
					{
						//wumpus is found right
						wumpusX = x+1;
						wumpusY = y;
						searchEngine.RemoveSafeLocation(wumpusX, wumpusY);
					}
					else if(conUp == 1 && conDown == 1 && conLeft == 0 && conRight == 1)
					{
						//wumpus is found Left
						wumpusX = x-1;
						wumpusY = y;
						searchEngine.RemoveSafeLocation(wumpusX, wumpusY);
					}
					else if(conUp == 1 && conDown == 0 && conLeft == 1 && conRight == 1)
					{
						//wumpus is found Down
						wumpusX = x;
						wumpusY = y-1;
						searchEngine.RemoveSafeLocation(wumpusX, wumpusY);
					}
					else if(conUp == 0 && conDown == 1 && conLeft == 1 && conRight == 1)
					{
						//wumpus is found Up
						wumpusX = x;
						wumpusY = y+1;
						searchEngine.RemoveSafeLocation(wumpusX, wumpusY);
					}
					
				}
			}
		}
	}
	
	
	if(percept.Breeze && percept.Stench && wumpusX == 0 && wumpusY == 0)//no safe or frontier locations added
	{
		Danger = 1;
		breeze[locx][locy] = 1;
	}
	else if(percept.Breeze)
	{
		cout << "\nmarking possible pits!!\n";
		Danger = 1;
		breeze[locx][locy] = 1;
		//mark possible pits
		if(visited[locx][locy+1] == 0 && pits[locx][locy+1] == 0 && !(wumpusX == locx && wumpusY == locy+1))
		{
			frontier[locx][locy+1] = 1;
		}
		
		if(visited[locx][locy-1] == 0 && pits[locx][locy-1] == 0 && !(wumpusX == locx && wumpusY == locy-1))
		{
			frontier[locx][locy-1] = 1;
		}
		
		if(visited[locx+1][locy] == 0 && pits[locx+1][locy] == 0 && !(wumpusX == locx+1 && wumpusY == locy))
		{
			frontier[locx+1][locy] = 1;
		}
		
		if(visited[locx-1][locy] == 0 && pits[locx-1][locy] == 0 && !(wumpusX == locx-1 && wumpusY == locy))
		{
			frontier[locx-1][locy] = 1;
		}
	}
	
	//new rule to find pits if breeze spot has 3 ajacent safe spots, then
	//the 4th spot has a pit and can't be in frontier
	
	for(int x = 0; x < 10; x++)//
	{
		for(int y = 0; y < 10; y++)
		{
			if(breeze[x][y] == 1)//if found breeze confirm pit location
			{
				int conUp = 0;
				int conDown = 0;
				int conLeft = 0;
				int conRight = 0;
				if(x > 1)//check left
				{
					if(visited[x-1][y] == 1)
					{
						conLeft = 1;
					}
				}
				else
				{
					conLeft = 1;
				}
				if(y > 1)//check down
				{
					if(visited[x][y-1] == 1)
					{
						conDown = 1;
					}
				}
				else
				{
					conDown = 1;
				}
				if(foundMaxSize == 0)
				{
					if(x < 9)
					{
						if(visited[x+1][y] == 1)
						{
							conRight = 1;
						}
					}
					else
					{
						conRight = 1;
					}
						
					if(y < 9)
					{
						if(visited[x][y+1] == 1)
						{
							conUp = 1;
						}
					}
					else
					{
						conUp = 1;
					}	
				}
				else
				{
					if(x < maxSize)
					{
						if(visited[x+1][y] == 1)
						{
							conRight = 1;
						}
					}
					else
					{
						conRight = 1;
					}
					
					if(y < maxSize)
					{
						if(visited[x][y+1] == 1)
						{
							conUp = 1;
						}
					}
					else
					{
						conUp = 1;
					}	
					
				}
				
				//check to see if we found a pit
				if(conUp == 1 && conDown == 1 && conLeft == 1 && conRight == 0)
				{
					//pit is found right
					pits[x+1][y] = 1;
					frontier[x+1][y] = 0;
					searchEngine.RemoveSafeLocation(x+1, y);
				}
				else if(conUp == 1 && conDown == 1 && conLeft == 0 && conRight == 1)
				{
					//pit is found Left
					pits[x-1][y] = 1;
					frontier[x-1][y] = 0;
					searchEngine.RemoveSafeLocation(x-1, y);
				}
				else if(conUp == 1 && conDown == 0 && conLeft == 1 && conRight == 1)
				{
					//pit is found Down
					pits[x][y-1] = 1;
					frontier[x][y-1] = 0;
					searchEngine.RemoveSafeLocation(x, y-1);
				}
				else if(conUp == 0 && conDown == 1 && conLeft == 1 && conRight == 1)
				{
					//pit is found Up
					pits[x][y+1] = 1;
					frontier[x][y+1] = 0;
					searchEngine.RemoveSafeLocation(x, y+1);
				}
				
			}
		}
	}
	//pit rule end
	
	//once wumpus found find safe spots next to stenches
	if(wumpusX != 0 && wumpusY != 0)
	{
		if(wumpusX > 1)//check left if not by a wall
		{
			if(wumpusY > 1)//left bottom
			{
				if((visited[wumpusX][wumpusY-1] == 1 && breeze[wumpusX][wumpusY-1] == 0) || (visited[wumpusX-1][wumpusY] == 1 && breeze[wumpusX-1][wumpusY] == 0))
				{
					safe[wumpusX-1][wumpusY-1] = 1;
					searchEngine.AddSafeLocation(wumpusX-1,wumpusY-1);	
				}
			}
			if(wumpusY < maxSize || (foundMaxSize == 0 && wumpusY < 9))//left top
			{
				if((visited[wumpusX][wumpusY+1] == 1 && breeze[wumpusX][wumpusY+1] == 0) || (visited[wumpusX-1][wumpusY] == 1 && breeze[wumpusX-1][wumpusY] == 0))
				{
					safe[wumpusX-1][wumpusY+1] = 1;
					searchEngine.AddSafeLocation(wumpusX-1,wumpusY+1);	
				}		
			}
		}
		if(wumpusX < maxSize || (foundMaxSize == 0 && wumpusX < 9))//check right
		{
			if(wumpusY > 1)//right down
			{
				if((visited[wumpusX][wumpusY-1] == 1 && breeze[wumpusX][wumpusY-1] == 0) || (visited[wumpusX+1][wumpusY] == 1 && breeze[wumpusX+1][wumpusY] == 0))
				{
					safe[wumpusX+1][wumpusY-1] = 1;
					searchEngine.AddSafeLocation(wumpusX+1,wumpusY-1);	
				}				
			}
			if(wumpusY < maxSize || (foundMaxSize == 0 && wumpusY < 9))//right up
			{
				if((visited[wumpusX][wumpusY+1] == 1 && breeze[wumpusX][wumpusY+1] == 0) || (visited[wumpusX+1][wumpusY] == 1 && breeze[wumpusX+1][wumpusY] == 0))
				{
					safe[wumpusX+1][wumpusY+1] = 1;
					searchEngine.AddSafeLocation(wumpusX+1,wumpusY+1);	
				}			
			}			
		}
	}
	
	
	if(percept.Glitter)//pick up the gold and head home
	{
		goldX = locx;
		goldY = locy;
		hasGold = 1;
		searchEngine.AddSafeLocation(locx,locy);
		actionList.clear();
		action = GRAB;
		return GRAB;
	}
	
	if(takeTheShot == 1 && actionList.empty() && hasArrow == 1)
	{
		cout << "\nFIRE!!!!\n";
		takeTheShot = 0;
		action = SHOOT;
		return SHOOT;
	}
	
	if(maxSize < locx)//update max world size
	{
		maxSize = locx;
	}
	if(maxSize < locy)
	{
		maxSize = locy;
	}
	
	if(Danger == 0)//no breeze or stench add safe locations
	{
		cout<<"\nsafe!!! \n";
		if(locx < 9)
		{
			safe[locx+1][locy] = 1;
			searchEngine.AddSafeLocation(locx+1,locy);
		}
		
		if(locy < 9)
		{
			safe[locx][locy+1] = 1;
			searchEngine.AddSafeLocation(locx,locy+1);
		}
		
		if(locx > 1)
		{
			safe[locx-1][locy] = 1;
			searchEngine.AddSafeLocation(locx-1,locy);
		}
		
		if(locy > 1)
		{
			safe[locx][locy-1] = 1;
			searchEngine.AddSafeLocation(locx,locy-1);
		}		
	}
	
	if (actionList.empty()) {
		cout<<"empty actions\n";
		inPit = 0; //no pit attempt yet
		if(hasGold == 1)//if picked up gold
		{
			cout << "let's go home\n";
			if(rightF == 1)//find facing
			{
				actionList2 = searchEngine.FindPath(Location(locx,locy), RIGHT, Location(1,1), LEFT);
				actionList.splice(actionList.end(), actionList2);
				actionList.push_back(CLIMB);
			}
			else if(leftF == 1)
			{
				actionList2 = searchEngine.FindPath(Location(locx,locy), LEFT, Location(1,1), DOWN);
				actionList.splice(actionList.end(), actionList2);
				actionList.push_back(CLIMB);
			}
			else if(upF == 1)
			{
				actionList2 = searchEngine.FindPath(Location(locx,locy), UP, Location(1,1), DOWN);
				actionList.splice(actionList.end(), actionList2);
				actionList.push_back(CLIMB);
			}
			else if(downF == 1)
			{
				actionList2 = searchEngine.FindPath(Location(locx,locy), DOWN, Location(1,1), LEFT);
				actionList.splice(actionList.end(), actionList2);
				actionList.push_back(CLIMB);
			}		
		}
		else if(hasToShoot == 1 && deadWumpus == 0 && firstTry == 0)//wumpus is alive and needs to die
		{
			if(visited[wumpusX][wumpusY-1] == 1) //shoot up?
			{
				if(rightF == 1)
				{
					actionList2 = searchEngine.FindPath(Location(locx,locy), RIGHT, Location(wumpusX,wumpusY-1), UP);
					actionList.splice(actionList.end(), actionList2);
					takeTheShot = 1;
				}
				else if(leftF == 1)
				{
					actionList2 = searchEngine.FindPath(Location(locx,locy), LEFT, Location(wumpusX,wumpusY-1), UP);
					actionList.splice(actionList.end(), actionList2);
					takeTheShot = 1;
				}
				else if(upF == 1)
				{
					actionList2 = searchEngine.FindPath(Location(locx,locy), UP, Location(wumpusX,wumpusY-1), UP);
					actionList.splice(actionList.end(), actionList2);
					takeTheShot = 1;
				}
				else if(downF == 1)
				{
					actionList2 = searchEngine.FindPath(Location(locx,locy), DOWN, Location(wumpusX,wumpusY-1), UP);
					actionList.splice(actionList.end(), actionList2);
					takeTheShot = 1;
				}	
			}
			else if(visited[wumpusX][wumpusY+1] == 1)//shoot down?
			{
				if(rightF == 1)
				{
					actionList2 = searchEngine.FindPath(Location(locx,locy), RIGHT, Location(wumpusX,wumpusY+1), DOWN);
					actionList.splice(actionList.end(), actionList2);
					takeTheShot = 1;
				}
				else if(leftF == 1)
				{
					actionList2 = searchEngine.FindPath(Location(locx,locy), LEFT, Location(wumpusX,wumpusY+1), DOWN);
					actionList.splice(actionList.end(), actionList2);
					takeTheShot = 1;
				}
				else if(upF == 1)
				{
					actionList2 = searchEngine.FindPath(Location(locx,locy), UP, Location(wumpusX,wumpusY+1), DOWN);
					actionList.splice(actionList.end(), actionList2);
					takeTheShot = 1;
				}
				else if(downF == 1)
				{
					actionList2 = searchEngine.FindPath(Location(locx,locy), DOWN, Location(wumpusX,wumpusY+1), DOWN);
					actionList.splice(actionList.end(), actionList2);
					takeTheShot = 1;
				}	
			}
			else if(visited[wumpusX-1][wumpusY] == 1)//shoot Right?
			{
				if(rightF == 1)
				{
					actionList2 = searchEngine.FindPath(Location(locx,locy), RIGHT, Location(wumpusX-1,wumpusY), RIGHT);
					actionList.splice(actionList.end(), actionList2);
					takeTheShot = 1;
				}
				else if(leftF == 1)
				{
					actionList2 = searchEngine.FindPath(Location(locx,locy), LEFT, Location(wumpusX-1,wumpusY), RIGHT);
					actionList.splice(actionList.end(), actionList2);
					takeTheShot = 1;
				}
				else if(upF == 1)
				{
					actionList2 = searchEngine.FindPath(Location(locx,locy), UP, Location(wumpusX-1,wumpusY), RIGHT);
					actionList.splice(actionList.end(), actionList2);
					takeTheShot = 1;
				}
				else if(downF == 1)
				{
					actionList2 = searchEngine.FindPath(Location(locx,locy), DOWN, Location(wumpusX-1,wumpusY), RIGHT);
					actionList.splice(actionList.end(), actionList2);
					takeTheShot = 1;
				}	
			}
			else if(visited[wumpusX+1][wumpusY] == 1)//shoot Left?
			{
				if(rightF == 1)
				{
					actionList2 = searchEngine.FindPath(Location(locx,locy), RIGHT, Location(wumpusX+1,wumpusY), LEFT);
					actionList.splice(actionList.end(), actionList2);
					takeTheShot = 1;
				}
				else if(leftF == 1)
				{
					actionList2 = searchEngine.FindPath(Location(locx,locy), LEFT, Location(wumpusX+1,wumpusY), LEFT);
					actionList.splice(actionList.end(), actionList2);
					takeTheShot = 1;
				}
				else if(upF == 1)
				{
					actionList2 = searchEngine.FindPath(Location(locx,locy), UP, Location(wumpusX+1,wumpusY), LEFT);
					actionList.splice(actionList.end(), actionList2);
					takeTheShot = 1;
				}
				else if(downF == 1)
				{
					actionList2 = searchEngine.FindPath(Location(locx,locy), DOWN, Location(wumpusX+1,wumpusY), LEFT);
					actionList.splice(actionList.end(), actionList2);
					takeTheShot = 1;
				}	
			}
		}
		else if(goldX != 0 && goldY != 0)//if knows gold location go fetch
		{
			cout<<"gold already known\n";
			if(rightF == 1)//find facing
			{
				actionList2 = searchEngine.FindPath(Location(1,1), RIGHT, Location(goldX,goldY), RIGHT);
				actionList.splice(actionList.end(), actionList2);
			}
			else if(leftF == 1)
			{
				actionList2 = searchEngine.FindPath(Location(locx,locy), LEFT, Location(goldX,goldY), RIGHT);
				actionList.splice(actionList.end(), actionList2);		
			}
			else if(upF == 1)
			{
				actionList2 = searchEngine.FindPath(Location(locx,locy), UP, Location(goldX,goldY), DOWN);
				actionList.splice(actionList.end(), actionList2);
			}
			else if(downF == 1)
			{
				actionList2 = searchEngine.FindPath(Location(locx,locy), DOWN, Location(goldX,goldY), DOWN);
				actionList.splice(actionList.end(), actionList2);
			}		
		}
		else//find a safe location
		{
			nextx = 0;
			nexty = 0;
			if(foundMaxSize == 0)
			{
				for(int x = 1; x<10 ; x++)
				{
					for(int y = 1; y<10 ; y++)
					{
						if(safe[x][y] == 1 && visited[x][y] == 0)
						{
							nextx = x;
							nexty = y;
							cout<<"found solution\n";
						}
					} 
				}
			
			}
			else if(foundMaxSize != 0)
			{
				for(int x = 1; x<=maxSize; x++)
				{
					for(int y = 1; y<=maxSize; y++)
					{
						if(safe[x][y] == 1 && visited[x][y] == 0)
						{
							nextx = x;
							nexty = y;
							cout<<"found solution\n";
						}
					} 
				}
			}
			
			if(nextx != 0 && nexty != 0)//next spot found
			{				
				cout << "nextx = ";
				cout << nextx;
				cout << "\n";
				cout << "nexty = ";
				cout << nexty;
				cout << "\n";
				cout << "x = ";
				cout << locx;
				cout << "\n";
				cout << "y = ";
				cout << locy;
				cout << "\n";
				if(rightF == 1)//find facing
				{
					actionList2 = searchEngine.FindPath(Location(locx,locy), RIGHT, Location(nextx,nexty), RIGHT);
					actionList.splice(actionList.end(), actionList2);
				}
				else if(leftF == 1)
				{
					actionList2 = searchEngine.FindPath(Location(locx,locy), LEFT, Location(nextx,nexty), RIGHT);
				 	actionList.splice(actionList.end(), actionList2);
			
				}
				else if(upF == 1)
				{
					actionList2 = searchEngine.FindPath(Location(locx,locy), UP, Location(nextx,nexty), DOWN);
					actionList.splice(actionList.end(), actionList2);
				}
				else if(downF == 1)
				{
					actionList2 = searchEngine.FindPath(Location(locx,locy), DOWN, Location(nextx,nexty), DOWN);
					actionList.splice(actionList.end(), actionList2);
				}		
			}
			else//no safe spots
			{
				if(wumpusX != 0 && wumpusY != 0 && deadWumpus == 0)//if wumpus needs to die
				{
					hasToShoot = 1;
					if(visited[wumpusX][wumpusY-1] == 1) //shoot up?
					{
						if(rightF == 1)
						{
							actionList2 = searchEngine.FindPath(Location(locx,locy), RIGHT, Location(wumpusX,wumpusY-1), UP);
							actionList.splice(actionList.end(), actionList2);
							takeTheShot = 1;
						}
						else if(leftF == 1)
						{
							actionList2 = searchEngine.FindPath(Location(locx,locy), LEFT, Location(wumpusX,wumpusY-1), UP);
							actionList.splice(actionList.end(), actionList2);
							takeTheShot = 1;
						}
						else if(upF == 1)
						{
							actionList2 = searchEngine.FindPath(Location(locx,locy), UP, Location(wumpusX,wumpusY-1), UP);
							actionList.splice(actionList.end(), actionList2);
							takeTheShot = 1;
						}
						else if(downF == 1)
						{
							actionList2 = searchEngine.FindPath(Location(locx,locy), DOWN, Location(wumpusX,wumpusY-1), UP);
							actionList.splice(actionList.end(), actionList2);
							takeTheShot = 1;
						}	
					}
					else if(visited[wumpusX][wumpusY+1] == 1)//shoot down?
					{
						if(rightF == 1)
						{
							actionList2 = searchEngine.FindPath(Location(locx,locy), RIGHT, Location(wumpusX,wumpusY+1), DOWN);
							actionList.splice(actionList.end(), actionList2);
							takeTheShot = 1;
						}
						else if(leftF == 1)
						{
							actionList2 = searchEngine.FindPath(Location(locx,locy), LEFT, Location(wumpusX,wumpusY+1), DOWN);
							actionList.splice(actionList.end(), actionList2);
							takeTheShot = 1;
						}
						else if(upF == 1)
						{
							actionList2 = searchEngine.FindPath(Location(locx,locy), UP, Location(wumpusX,wumpusY+1), DOWN);
							actionList.splice(actionList.end(), actionList2);
							takeTheShot = 1;
						}
						else if(downF == 1)
						{
							actionList2 = searchEngine.FindPath(Location(locx,locy), DOWN, Location(wumpusX,wumpusY+1), DOWN);
							actionList.splice(actionList.end(), actionList2);
							takeTheShot = 1;
						}	
					}
					else if(visited[wumpusX-1][wumpusY] == 1)//shoot Right?
					{
						if(rightF == 1)
						{
							actionList2 = searchEngine.FindPath(Location(locx,locy), RIGHT, Location(wumpusX-1,wumpusY), RIGHT);
							actionList.splice(actionList.end(), actionList2);
							takeTheShot = 1;
						}
						else if(leftF == 1)
						{
							actionList2 = searchEngine.FindPath(Location(locx,locy), LEFT, Location(wumpusX-1,wumpusY), RIGHT);
							actionList.splice(actionList.end(), actionList2);
							takeTheShot = 1;
						}
						else if(upF == 1)
						{
							actionList2 = searchEngine.FindPath(Location(locx,locy), UP, Location(wumpusX-1,wumpusY), RIGHT);
							actionList.splice(actionList.end(), actionList2);
							takeTheShot = 1;
						}
						else if(downF == 1)
						{
							actionList2 = searchEngine.FindPath(Location(locx,locy), DOWN, Location(wumpusX-1,wumpusY), RIGHT);
							actionList.splice(actionList.end(), actionList2);
							takeTheShot = 1;
						}	
					}
					else if(visited[wumpusX+1][wumpusY] == 1)//shoot Left?
					{
						if(rightF == 1)
						{
							actionList2 = searchEngine.FindPath(Location(locx,locy), RIGHT, Location(wumpusX+1,wumpusY), LEFT);
							actionList.splice(actionList.end(), actionList2);
							takeTheShot = 1;
						}
						else if(leftF == 1)
						{
							actionList2 = searchEngine.FindPath(Location(locx,locy), LEFT, Location(wumpusX+1,wumpusY), LEFT);
							actionList.splice(actionList.end(), actionList2);
							takeTheShot = 1;
						}
						else if(upF == 1)
						{
							actionList2 = searchEngine.FindPath(Location(locx,locy), UP, Location(wumpusX+1,wumpusY), LEFT);
							actionList.splice(actionList.end(), actionList2);
							takeTheShot = 1;
						}
						else if(downF == 1)
						{
							actionList2 = searchEngine.FindPath(Location(locx,locy), DOWN, Location(wumpusX+1,wumpusY), LEFT);
							actionList.splice(actionList.end(), actionList2);
							takeTheShot = 1;
						}	
					}
				}
				else//go to a possible pit. 
				{
					nextx = 0;
					nexty = 0;
					if(foundMaxSize == 0)
					{
						for(int x = 1; x<10 ; x++)
						{
							for(int y = 1; y<10 ; y++)
							{
								if(frontier[x][y] == 1)
								{
									nextx = x;
									nexty = y;
									cout<<"found risky solution\n";
								}
								if(stench[x][y] == 1)
								{
									hailx = x;
									haily = y;
								}
							} 
						}
			
					}
					else if(foundMaxSize != 0)
					{
						for(int x = 1; x<=maxSize; x++)
						{
							for(int y = 1; y<=maxSize; y++)
							{
								if(frontier[x][y] == 1)
								{
									nextx = x;
									nexty = y;
									cout<<"found risky solution\n";
								}
								if(stench[x][y] == 1)
								{
									hailx = x;
									haily = y;
								}
							} 
						}
					}
					if(nextx != 0 && nexty != 0)//next spot found
					{
						searchEngine.AddSafeLocation(nextx,nexty);
						safe[locx][locy] = 1;
						inPit = 1;//found pit if died				
						cout << "nextx = ";
						cout << nextx;
						cout << "\n";
						cout << "nexty = ";
						cout << nexty;
						cout << "\n";
						cout << "x = ";
						cout << locx;
						cout << "\n";
						cout << "y = ";
						cout << locy;
						cout << "\n";
						if(rightF == 1)//find facing
						{
							actionList2 = searchEngine.FindPath(Location(locx,locy), RIGHT, Location(nextx,nexty), RIGHT);
							actionList.splice(actionList.end(), actionList2);
						}
						else if(leftF == 1)
						{
							actionList2 = searchEngine.FindPath(Location(locx,locy), LEFT, Location(nextx,nexty), RIGHT);
				 			actionList.splice(actionList.end(), actionList2);
			
						}
						else if(upF == 1)
						{
							actionList2 = searchEngine.FindPath(Location(locx,locy), UP, Location(nextx,nexty), DOWN);
							actionList.splice(actionList.end(), actionList2);
						}
						else if(downF == 1)
						{
							actionList2 = searchEngine.FindPath(Location(locx,locy), DOWN, Location(nextx,nexty), DOWN);
							actionList.splice(actionList.end(), actionList2);
						}		
					}
					else if(hailx != 0 && haily != 0 && hasArrow == 1 && wumpusX == 0 && wumpusY == 0)//we can hail marry shoot
					{
						cout << "\nHail Marry!!!\n";
						hailMarry = 1;
						//determine direction to shoot
						int shootUp = 1;
						int shootDown = 1;
						int shootLeft = 1;
						int shootRight = 1;//default direction
						
						if(foundMaxSize == 0 && hailx+1 > 9)//check boundaries
						{
							shootRight = 0;
						}
						else if(hailx + 1 > maxSize)
						{
							shootRight = 0;
						}
						
						if(foundMaxSize == 0 && haily+1 > 9)
						{
							shootUp = 0;
						}
						else if(haily + 1 > maxSize)
						{
							shootUp = 0;
						}
						
						if(hailx-1 < 1)
						{
							shootLeft = 0;
						}
						
						if(haily-1 < 1)
						{
							shootDown = 0;
						}
						
						if(noWumpus[hailx+1][haily] == 1)//determine possible shots
						{
							shootRight = 0;
						}
						if(noWumpus[hailx-1][haily] == 1)
						{
							shootLeft = 0;
						}
						if(noWumpus[hailx][haily+1] == 1)
						{
							shootUp = 0;
						}
						if(noWumpus[hailx][haily-1] == 1)
						{
							shootDown = 0;
						}
						
						if(shootRight == 1)
						{
							if(locx == hailx && locy == haily)
							{
								if(rightF == 1)
								{
									action = SHOOT;
									return SHOOT;
								}
								else if(leftF == 1)
								{
									action = TURNLEFT;
									return TURNLEFT;
								}
								else if(downF == 1)
								{
									action = TURNRIGHT;
									return TURNLEFT;
								}
								else
								{
									action = TURNRIGHT;
									return TURNRIGHT;
								}
							}
							
							if(rightF == 1)
							{
								actionList2 = searchEngine.FindPath(Location(locx,locy), RIGHT, Location(hailx,haily), RIGHT);
								actionList.splice(actionList.end(), actionList2);
								takeTheShot = 1;
							}
							else if(leftF == 1)
							{
								actionList2 = searchEngine.FindPath(Location(locx,locy), LEFT, Location(hailx,haily), RIGHT);
								actionList.splice(actionList.end(), actionList2);
								takeTheShot = 1;
							}
							else if(upF == 1)
							{
								actionList2 = searchEngine.FindPath(Location(locx,locy), UP, Location(hailx,haily), RIGHT);
								actionList.splice(actionList.end(), actionList2);
								takeTheShot = 1;
							}
							else if(downF == 1)
							{
								actionList2 = searchEngine.FindPath(Location(locx,locy), DOWN, Location(hailx,haily), RIGHT);
								actionList.splice(actionList.end(), actionList2);
								takeTheShot = 1;
							}	
						}
						else if(shootUp == 1)
						{
						
							if(locx == hailx && locy == haily)
							{
								if(rightF == 1)
								{
									action = TURNLEFT;
									return TURNLEFT;
								}
								else if(leftF == 1)
								{
									action = TURNRIGHT;
									return TURNRIGHT;
								}
								else if(downF == 1)
								{
									action = TURNLEFT;
									return TURNLEFT;
								}
								else
								{
									action = SHOOT;
									return SHOOT;
								}
							}
							
							if(rightF == 1)
							{
								actionList2 = searchEngine.FindPath(Location(locx,locy), RIGHT, Location(hailx,haily), UP);
								actionList.splice(actionList.end(), actionList2);
								takeTheShot = 1;
							}
							else if(leftF == 1)
							{
								actionList2 = searchEngine.FindPath(Location(locx,locy), LEFT, Location(hailx,haily), UP);
								actionList.splice(actionList.end(), actionList2);
								takeTheShot = 1;
							}
							else if(upF == 1)
							{
								actionList2 = searchEngine.FindPath(Location(locx,locy), UP, Location(hailx,haily), UP);
								actionList.splice(actionList.end(), actionList2);
								takeTheShot = 1;
							}
							else if(downF == 1)
							{
								actionList2 = searchEngine.FindPath(Location(locx,locy), DOWN, Location(hailx,haily), UP);
								actionList.splice(actionList.end(), actionList2);
								takeTheShot = 1;
							}	
						}
						else if(shootDown == 1)
						{
						
							if(locx == hailx && locy == haily)
							{
								if(rightF == 1)
								{
									action = TURNRIGHT;
									return TURNRIGHT;
								}
								else if(leftF == 1)
								{
									action = TURNLEFT;
									return TURNLEFT;
								}
								else if(downF == 1)
								{
									action = SHOOT;
									return SHOOT;
								}
								else
								{
									action = TURNRIGHT;
									return TURNRIGHT;
								}
							}
						
							if(rightF == 1)
							{
								actionList2 = searchEngine.FindPath(Location(locx,locy), RIGHT, Location(hailx,haily), DOWN);
								actionList.splice(actionList.end(), actionList2);
								takeTheShot = 1;
							}
							else if(leftF == 1)
							{
								actionList2 = searchEngine.FindPath(Location(locx,locy), LEFT, Location(hailx,haily), DOWN);
								actionList.splice(actionList.end(), actionList2);
								takeTheShot = 1;
							}
							else if(upF == 1)
							{
								actionList2 = searchEngine.FindPath(Location(locx,locy), UP, Location(hailx,haily), DOWN);
								actionList.splice(actionList.end(), actionList2);
								takeTheShot = 1;
							}
							else if(downF == 1)
							{
								actionList2 = searchEngine.FindPath(Location(locx,locy), DOWN, Location(hailx,haily), DOWN);
								actionList.splice(actionList.end(), actionList2);
								takeTheShot = 1;
							}	
						}
						else//shoot left
						{
						
							if(locx == hailx && locy == haily)
							{
								if(rightF == 1)
								{
									action = TURNRIGHT;
									return TURNRIGHT;
								}
								else if(leftF == 1)
								{
									action = SHOOT;
									return SHOOT;
								}
								else if(downF == 1)
								{
									action = TURNRIGHT;
									return TURNRIGHT;
								}
								else
								{
									action = TURNLEFT;
									return TURNLEFT;
								}
							}
						
							if(rightF == 1)
							{
								actionList2 = searchEngine.FindPath(Location(locx,locy), RIGHT, Location(hailx,haily), LEFT);
								actionList.splice(actionList.end(), actionList2);
								takeTheShot = 1;
							}
							else if(leftF == 1)
							{
								actionList2 = searchEngine.FindPath(Location(locx,locy), LEFT, Location(hailx,haily), LEFT);
								actionList.splice(actionList.end(), actionList2);
								takeTheShot = 1;
							}
							else if(upF == 1)
							{
								actionList2 = searchEngine.FindPath(Location(locx,locy), UP, Location(hailx,haily), LEFT);
								actionList.splice(actionList.end(), actionList2);
								takeTheShot = 1;
							}
							else if(downF == 1)
							{
								actionList2 = searchEngine.FindPath(Location(locx,locy), DOWN, Location(hailx,haily), LEFT);
								actionList.splice(actionList.end(), actionList2);
								takeTheShot = 1;
							}	
						}
					}
					else
					{
						
						//last resort if world is not safe at all
						cout << "get out of here \n";
						if(rightF == 1)//find facing
						{
							actionList2 = searchEngine.FindPath(Location(locx,locy), RIGHT, Location(1,1), LEFT);
							actionList.splice(actionList.end(), actionList2);
							actionList.push_back(CLIMB);
						}	
						else if(leftF == 1)
						{
							actionList2 = searchEngine.FindPath(Location(locx,locy), LEFT, Location(1,1), DOWN);
							actionList.splice(actionList.end(), actionList2);
							actionList.push_back(CLIMB);
						}
						else if(upF == 1)
						{
							actionList2 = searchEngine.FindPath(Location(locx,locy), UP, Location(1,1), DOWN);
							actionList.splice(actionList.end(), actionList2);
							actionList.push_back(CLIMB);
						}
						else if(downF == 1)
						{
							actionList2 = searchEngine.FindPath(Location(locx,locy), DOWN, Location(1,1), LEFT);
							actionList.splice(actionList.end(), actionList2);
							actionList.push_back(CLIMB);
						}
					}
				}		
			}
		}

	}
	action = actionList.front();
	actionList.pop_front();
	return action;
}

void Agent::GameOver (int score)
{
	hailMarry = 0;
	cout << "\nend game!!!!!\n";
	if(rightF == 1)
	{
		locx += 1;
	}
	else if(leftF == 1)
	{
		locx -= 1;
	}
	else if(upF == 1)
	{
		locy += 1;
	}
	else if(downF == 1)
	{
		locy -= 1;
	}	
	
	if(inPit == 1)
	{
		cout << "\n oops you fell in a pit!!!!\n";
		pits[locx][locy] = 1;//confirmed pit
		frontier[locx][locy] = 0;//remove from possible pits
		safe[locx][locy] = 0;
		searchEngine.RemoveSafeLocation(locx, locy);
	}
	
	
	if(wumpusX != 0 && wumpusY != 0)//mark wumpus back to unsafe
	{
		searchEngine.RemoveSafeLocation(wumpusX, wumpusY);
		safe[wumpusX][wumpusY] = 0;
	}
}


