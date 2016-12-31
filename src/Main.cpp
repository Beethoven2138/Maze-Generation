#include <iostream>
#include <SFML/graphics.hpp>
#include <vector>
#include <time.h>
#include <stdlib.h>

#define boardWidth 10
#define boardHeight 10

class Node;

Node* nodes[boardWidth][boardHeight];

Node* currentNode;
Node* tempNode;

vector<Node> stack;

float timeTillNextMove = 0.5;

Node FindUnvisitedNeighbour();

void RecursiveBacktracker();

sf::Clock clock;

int main(){

  srand(time(NULL));
  
  for (int y = 0; y < boardHeight; y++)
    {
      for (int x = 0; x < boardWidth; x++)
	{
	  nodes[x][y] = new Node(x, y);
	}
    }

  currentNode = nodes[0][0];

  sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

  // run the program as long as the window is open
  while (window.isOpen())
    {
      sf::Event event;
      while (window.pollEvent(event))
        {
	  if (event.type == sf::Event::Closed)
	    window.close();
        }
      
      window.clear(sf::Color::Black);

      window.display();
    }
  
  return 0;
}

class Node{
private:
  int xPos, yPos;
  bool visited = false;
  enum Side {Left, Right, Up, Down};
public:
  Node(int xPos, int yPos){
    this->xPos = xPos;
    this->yPos = yPos;
  }
  
  bool IsVisited(){
    return visited;
  }

  void SetVisited(bool visited){
    this->visited = visited;
  }

  int GetXPos(){
    return xPos;
  }
  int GetYPos(){
    return yPos;
  }
  void RemoveWall(int wall){
    if(wall > 3)
      std::cout<<"Error in Remove Wall"<<std::endl;
    else{
      walls[wall] = 0;
    }
};

void RecursiveBacktracker(){
  
  stack.push_back(currentNode);

  tempNode = FindUnvisitedNeighbour();

  if(tempNode != NULL){
    //Remove the walls
    if(tempNode->getXPos == currentNode->getXPos + 1){//Right
      currentNode->RemoveWall(1);
      tempNode->RemoveWall(0);
    }
    if(tempNode->getXPos == currentNode->getXPos - 1){//Left
      currentNode->RemoveWall(0);
      tempNode->RemoveWall(1);
    }
    if(tempNode->getYPos == currentNode->geYPos + 1){//Up
      currentNode->RemoveWall(2);
      tempNode->RemoveWall(3);
    }
    if(tempNode->getYPos == currentNode->getYPos - 1){//Down
      currentNode->RemoveWall(3);
      tempNode->RemoveWall(2);
    }

    currentNode = tempNode;
    currentNode->SetVisited(true);
  }
  else if(stack.size() > 0){
    currentNode = stack.end();
    stack.pop_back();
  }

  if(clock >= timeTillNextMove){
    RecursiveBacktracker();
    clock.restart();
  }
}

Node FindUnvisitedNeighbour(){

  vector<Node> neighbours;

  //TODO add a condition to make sure that it doesn't try to access a node that's outside the board
  
  for (int y = -1; y < 1; y++)
    {
      if(y + currentNode->getYPos() < boardHeight && y + currentNode->getYPos() >= 0){
	  for (int x = -1; x < 1; x++)
	    {
	      if(x + currentNode->getXPos() < boardWidth && x + currentNode->getXPos() >= 0){
		if(abs(x - y) == 1 && !nodes[currentNode->GetXPos() + x][currentNode->getYPos() + y]->IsVisited())
		  neighbours.push_back(nodes[currentNode->GetXPos() + x][currentNode->getYPos() + y]);
	      }
	    }
	}
    }

  if(neighbours.size() == 0)
    return NULL;

  return neighbours.at(rand() % neighbours.size());
}
