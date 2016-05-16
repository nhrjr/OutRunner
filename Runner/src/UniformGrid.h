#pragma once
#include "IGameEntity.h"

#include "stdafx.h"
#include <math.h>
#include <iostream>
#include <unordered_set>

#include "UniformGrid.h"
#include "MapHitbox.h"
#include "SAT.h"
#include "IGameEntity.h"

#include "V2Tools.h"


//#define GRID_DIMENSIONS 64.0f // 4.0f


//#include <list>

template<typename T>
class UniformGrid
{
	static_assert(std::is_base_of<IGameEntity, T>::value, "UniformGrid<T> - T must derive from IGameEntity");
public:
	UniformGrid() {};
	UniformGrid(int width, int height, std::vector<T>* o);
	~UniformGrid() {};

	void writeGridToStream(std::ofstream& outputFile);
	void readGridFromStream(std::ifstream& inputFile);

	std::unordered_set<unsigned int> getCollisionObjects(sf::Vector2f coord, int radius=1);
	void constructArrays(int w, int h, std::vector<T>* o);

	std::vector<unsigned int> L;
	std::vector<unsigned int> C;
	int width, height;
	float conversion;

private:
	

	std::vector<T>* objects;	
	std::map<int, bool> object;

	bool intersectObjectsSAT(const T& object, int x, int y);
	bool intersectObjectsSimple(const T& object, int x, int y);
};

template<typename T>
void UniformGrid<T>::writeGridToStream(std::ofstream& outputFile)
{
	outputFile.write((char*)&this->width, sizeof(int));
	outputFile.write((char*)&this->height, sizeof(int));
	outputFile.write((char*)&this->conversion, sizeof(float));

	unsigned int size = this->L.size();
	outputFile.write((char*)&size, sizeof(unsigned int));
	size = this->C.size();
	outputFile.write((char*)&size, sizeof(unsigned int));

	for (auto& l : this->L) {
		outputFile.write((char*)&l, sizeof(unsigned int));
	}
	for (auto& c : this->C) {
		outputFile.write((char*)&c, sizeof(unsigned int));
	}
}

template<typename T>
void UniformGrid<T>::readGridFromStream(std::ifstream& inputFile)
{
	inputFile.read((char*)&this->width, sizeof(int));
	inputFile.read((char*)&this->height, sizeof(int));
	inputFile.read((char*)&this->conversion, sizeof(float));

	unsigned int size;
	inputFile.read((char*)&size, sizeof(unsigned int));
	this->L.resize(size);
	inputFile.read((char*)&size, sizeof(unsigned int));
	this->C.resize(size);

	for (auto& l : this->L) {
		inputFile.read((char*)&l, sizeof(unsigned int));
	}
	for (auto& c : this->C) {
		inputFile.read((char*)&c, sizeof(unsigned int));
	}
}



template<typename T>
UniformGrid<T>::UniformGrid(int w, int h, std::vector<T>* o)
{
	constructArrays(w, h, o);
}

template<typename T>
void UniformGrid<T>::constructArrays(int w, int h, std::vector<T>* o)
{
	objects = o;
	//float grid_dim = sqrt(w*h) / (GAME_TILESIZE);
	//conversion = sqrt(GRID_DIMENSIONS * GRID_DIMENSIONS / (w*h));
	conversion = sqrt(4.0f / (w*h));
	width = conversion * w * GAME_TILESIZE;
	height = conversion * h * GAME_TILESIZE;
	C = std::vector<unsigned int>(width*height + 1);
	for (int y = 0; y < this->height; ++y) {
		for (int x = 0; x < this->width; ++x) {
			int i = y * this->width + x;
			//for (auto& obj : *objects) {
			for (int j = objects->size() - 1; j >= 0; --j) {
				if (intersectObjectsSAT((*objects)[j], x, y))
				{
					++C[i];
				}
			}
		}
	}
	
	for (int i = 1; i <= this->width * this->height; ++i)
	{
		C[i] += C[i - 1];
	}
	L = std::vector<unsigned int>(C.back());

	
		for (int y = 0; y < this->height; ++y) {
			for (int x = 0; x < this->width; ++x) {
				int i = y * this->width + x;
				for (int j = objects->size() - 1; j >= 0; --j) {
				if (intersectObjectsSAT((*objects)[j], x, y))
				{
					L[--C[i]] = j;
				}
			}
		}
	}
}

template<typename T>
std::unordered_set<unsigned int> UniformGrid<T>::getCollisionObjects(sf::Vector2f coord, int radius)
{
	int x = coord.x * conversion;
	int y = coord.y * conversion;
	std::unordered_set<unsigned int> returnValue;
	for (int i = x - radius; i <= x + radius; ++i){
		for (int j = y - radius; j <= y + radius; ++j){
			if (i >= 0 && j >= 0 && i < this->width && j < this->height){
				int k = i * this->width + j;
				for (int l = C[k]; l < C[k + 1]; ++l)
				{
					returnValue.emplace(L[l]);
				}
			}
		}
	}
	return returnValue;
}

template<typename T>
bool UniformGrid<T>::intersectObjectsSAT(const T& object, int y, int x)
{
	bool found = false;
	const float gridlengthWC = 1 / conversion;
	const int distance = 2 * gridlengthWC;
	//const int distance = 3;
	const float x_map = x * gridlengthWC;
	const float y_map = y * gridlengthWC;
	const sf::Vector2f pos = object.getPosition();

	for (int i = 0; i < object.getPointCount(); ++i)
	{
		//if(V2Tools::distance(sf::Vector2i((position.x+object.getPoint(i).x)*conversion, (position.y+object.getPoint(i).y)*conversion),sf::Vector2i(x,y)) < distance)
		if (V2Tools::distance(sf::Vector2f(pos.x+object.getPoint(i).x, pos.y+object.getPoint(i).y), sf::Vector2f(x_map + gridlengthWC/2, y_map + gridlengthWC/2)) < distance)
		{
			found = true;
			break;
		}
	}
	if (!found) return false;
	
	MapHitbox gridbox(sf::Vector2f(gridlengthWC, gridlengthWC));
	gridbox.setPosition(sf::Vector2f(x_map, y_map));

	if (Collision::SATCollision(&object, &gridbox) != sf::Vector2f(0, 0))
	{
		return true;
	}
	return false;
}

template<typename T>
bool UniformGrid<T>::intersectObjectsSimple(const T& object, int y, int x)
{
	float x_map = x / conversion;
	float y_map = y / conversion;
	float x_max = x_map + 1 / conversion;
	float y_max = y_map + 1 / conversion;

	sf::Vector2f p = object.hitbox.getPosition();
	if (p.x >= x_map && p.y >= y_map && p.x < x_max && p.y < y_max)
		return true;
	else
		return false;
}

