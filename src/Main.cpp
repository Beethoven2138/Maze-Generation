/*
    This file is part of Maze-Generation.

    Maze-Generation is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Maze-Generation is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Maze-Generation.  If not, see <http://www.gnu.org/licenses/>.
*/

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

float timeTillNextMove = 0.5;

Node* FindUnvisitedNeighbour();

void RecursiveBacktracker();

void DrawBoard();

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
	lines[i].setFillColor(sf::Color::Black);
	if(i == 0){
	  lines[i].setRotation(90);
	  lines[i].setPosition(xPos * lineLength, yPos * lineLength);
	}
	if(i == 1){
	  lines[i].setRotation(90);
	  lines[i].setPosition(xPos * lineLength + lineLength, yPos * lineLength);
	}
	if(i == 0){
	  lines[i].setRotation(0);
	  lines[i].setPosition(xPos * lineLength, yPos * lineLength);
	}
	if(i == 0){
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

  // run the program as long as the window is open
  while (window.isOpen())
    {
      sf::Event event;
      while (window.pollEvent(event))
        {
	  if (event.type == sf::Event::Closed)
	    window.close();
        }
      
      window.clear(sf::Color::White);

      DrawBoard();

      window.display();
    }
  
  return 0;
}



void RecursiveBacktracker(){
  
  stack.push_back(currentNode);

  tempNode = FindUnvisitedNeighbour();

  if(tempNode != currentNode){
    //Remove the walls
    if(tempNode->GetXPos() == currentNode->GetXPos() + 1){//Right
      currentNode->RemoveWall(1);
      tempNode->RemoveWall(0);
    }
    if(tempNode->GetXPos() == currentNode->GetXPos() - 1){//Left
      currentNode->RemoveWall(0);
      tempNode->RemoveWall(1);
    }
    if(tempNode->GetYPos() == currentNode->GetYPos() + 1){//Up
      currentNode->RemoveWall(2);
      tempNode->RemoveWall(3);
    }
    if(tempNode->GetYPos() == currentNode->GetYPos() - 1){//Down
      currentNode->RemoveWall(3);
      tempNode->RemoveWall(2);
    }

    currentNode = tempNode;
    currentNode->SetVisited(true);
  }
  else if(stack.size() > 0){
    currentNode = *(stack.end());
    stack.pop_back();
  }

  if(timer.getElapsedTime().asSeconds() >= timeTillNextMove){
    RecursiveBacktracker();
    timer.restart();
  }
}

Node* FindUnvisitedNeighbour(){

  std::vector <Node*> neighbours;
  
  for (int y = -1; y < 1; y++)
    {
      if(y + currentNode->GetYPos() < boardHeight && y + currentNode->GetYPos() >= 0){
	  for (int x = -1; x < 1; x++)
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
