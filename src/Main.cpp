#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <time.h>
#include <stdlib.h>

#define boardWidth 70
#define boardHeight 50
#define lineLength 10
#define lineWidth 2

class Node;

sf::RenderWindow window(sf::VideoMode(boardWidth * lineLength, boardHeight * lineLength), "Maze");

Node* nodes[boardWidth][boardHeight];

Node* currentNode;
Node* tempNode;

std::vector <Node*> stack;

float timeTillNextMove = 0.5f;

Node* FindUnvisitedNeighbour();

void RecursiveBacktracker();

void DrawBoard();

void FreeMemory();

sf::Clock timer;

class Node{
private:
  int xPos, yPos;
  bool visited = false;
  bool walls[4] =  {1,1,1,1};
  sf::RectangleShape lines[4];
  void InitLines(){
    for (int i = 0; i < 4; i++)
      {
	lines[i].setSize(sf::Vector2f(lineWidth, lineLength));
	lines[i].setFillColor(sf::Color::White);
	//lines[i].setPosition(0,0);
	//lines[i].setOrigin(lineWidth/2,lineLength/2); 
	if(i == 0){
	  lines[i].setRotation(90);
	  lines[i].setPosition(xPos * lineLength, yPos * lineLength);
	}
	if(i == 1){
	  lines[i].setRotation(90);
	  lines[i].setPosition(xPos * lineLength + lineLength, yPos * lineLength);
	}
	if(i == 2){
	  lines[i].setRotation(0);
	  lines[i].setPosition(xPos * lineLength, yPos * lineLength);
	}
	if(i == 3){
	  lines[i].setRotation(0);
	  lines[i].setPosition(xPos * lineLength, yPos * lineLength + lineLength);
	}
      }
  }
public:
  Node(int xPos, int yPos){
    this->xPos = xPos;
    this->yPos = yPos;

    InitLines();
  }
  
  bool IsVisited(){
    return visited;
  }

  void SetVisited(bool visited){
    this->visited = visited;
  }

  bool HasWall(int wall){
    if(walls[wall] == 1)
      return true;
    return false;
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
  }
  void DrawLines(){
    if(currentNode == this){
      for (int i = 0; i < 4; i++)
	{
	  lines[i].setFillColor(sf::Color::Red);
	}
    }
    
    for (int i = 0; i < 4; i++)
      {
	if(walls[i] == 0)
	  continue;
	window.draw(lines[i]);
      }
  }
};

int main(){

  window.setVerticalSyncEnabled(true);
  
  srand(time(NULL));
  
  for (int y = 0; y < boardHeight; y++)
    {
      for (int x = 0; x < boardWidth; x++)
	{
	  nodes[x][y] = new Node(x, y);
	}
    }

  currentNode = nodes[0][0];

  RecursiveBacktracker();
    
  while (window.isOpen())
    {
      sf::Event event;
      while (window.pollEvent(event))
        {
	  if (event.type == sf::Event::Closed){
	    FreeMemory();
	    window.close();
	  }
        }
      
      window.clear(sf::Color::Black);

      DrawBoard();

      window.display();
    }
  
  return 0;
}



void RecursiveBacktracker(){
  
  stack.push_back(currentNode);

  tempNode = FindUnvisitedNeighbour();

  if(tempNode != currentNode){
    std::cout<<"tempNode != currentNode"<<std::endl;
    //Remove the walls
    if(tempNode->GetXPos() == currentNode->GetXPos() + 1){//Right
      std::cout<<"Right"<<std::endl;
      currentNode->RemoveWall(1);
      tempNode->RemoveWall(0);
    }
    if(tempNode->GetXPos() == currentNode->GetXPos() - 1){//Left
      currentNode->RemoveWall(0);
      tempNode->RemoveWall(1);
    }
    if(tempNode->GetYPos() == currentNode->GetYPos() - 1){//Up
      std::cout<<"Up"<<std::endl;
      currentNode->RemoveWall(2);
      tempNode->RemoveWall(3);
    }
    if(tempNode->GetYPos() == currentNode->GetYPos() + 1){//Down
      std::cout<<"Down"<<std::endl;
      currentNode->RemoveWall(3);
      tempNode->RemoveWall(2);
    }

    currentNode = tempNode;
    currentNode->SetVisited(true);
  }
  else if(stack.size() > 1){
    std::cout<<"tempNode == currentNode"<<std::endl;
    currentNode = stack.back();
    stack.pop_back();
  }

  else{
    
    //system("PAUSE");
  }

  //system("PAUSE");
  //RecursiveBacktracker();
  
}

Node* FindUnvisitedNeighbour(){

  std::vector <Node*> neighbours;
  
  for (int y = -1; y <= 1; y++)
    {
      if(y + currentNode->GetYPos() < boardHeight && y + currentNode->GetYPos() >= 0){
	  for (int x = -1; x <= 1; x++)
	    {
	      if(x + currentNode->GetXPos() < boardWidth && x + currentNode->GetXPos() >= 0){
		if(abs(x - y) == 1 && !nodes[currentNode->GetXPos() + x][currentNode->GetYPos() + y]->IsVisited())
		  neighbours.push_back(nodes[currentNode->GetXPos() + x][currentNode->GetYPos() + y]);
	      }
	    }
	}
    }

  if(neighbours.size() == 0)
    return currentNode;

  return neighbours.at(rand() % neighbours.size());
}

void DrawBoard(){
  for (int y = 0; y < boardHeight; y++)
    {
      for (int x = 0; x < boardWidth; x++)
	{
	  nodes[x][y]->DrawLines();
	}
    }
}

void FreeMemory(){
  delete [] nodes;
  nodes = NULL;

  stack.clear();

  currentNode = NULL;
  tempNode = NULL;
}
