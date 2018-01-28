#pragma once
#include <queue>
#include <vector>
#include <functional>
#include "Game.h"
#include "Map.h"



class nodeCompare
{
public:
	nodeCompare() {}
	bool operator() (const Tile* t1, const Tile* t2) const
	{
		return (t1->node.f > t2->node.f);
	}
};

typedef std::priority_queue<Tile*, std::vector<Tile*>, nodeCompare> nQueue;

class PathFinder
{
public:
	//nQueue open;
	std::vector<Tile*> open;
	std::vector<Tile*> closed;
	std::vector<Tile>* tiles;
	int width, height;
	PathFinder() {}
	PathFinder(Map* map, int width, int height)
	{
		this->tiles = map->getTiles();
		this->width = width;
		this->height = height;
	}

	std::vector<std::pair<int, int>> getNeighbors(int x, int y)
	{
		std::vector<std::pair<int, int>> neighbors;
		neighbors.reserve(8);

		for (int yn = y - 1; yn <= y + 1; yn++)
		{
			if (yn == height || yn < 0)
				continue;
			for (int xn = x - 1; xn <= x + 1; xn++)
			{
				if (xn == width || xn < 0 || (xn == x && yn == y))
					continue;
				neighbors.push_back({ xn,yn });
			}
		}

		return neighbors;
	}

	Tile* getTile(int x, int y)
	{
		return &(tiles->at(y*this->width + x));
	}
	void ClosedToOpen(Tile* tile)
	{
		for (int c = 0; c < closed.size(); c++)
		{
			if (closed[c]->node == tile->node)
			{
				closed.erase(closed.begin() + c);
				break;
			}
		}
		toOpen(tile);
	}
	void toOpen(Tile* tile)
	{
		tile->node.s = Node::set::Open;
		open.push_back(tile);
	}
	void toClosed(Tile* t)
	{
		for (int c = 0; c < open.size(); c++)
		{
			if (open[c]->node == t->node)
			{
				open.erase(open.begin() + c);
				break;
			}
		}
		t->node.s = Node::set::Closed;
		closed.push_back(t);
	}
	std::vector <std::pair<int, int>> getPath(Tile* start, Tile* end)
	{
		std::vector<std::pair<int, int>> path;
		Node current = end->node;
		while (!(current == start->node))
		{
			if (current.parent == nullptr)
				break;
			path.push_back({ current.x,current.y });
			current = *current.parent;
		}
		return path;
	}
	Tile* getLowest()
	{
		float f = std::numeric_limits<int>::max();
		Tile* lowest = nullptr;
		for (auto t : open)
		{
			if (t->node.f < f)
			{
				lowest = t;
				f = t->node.f;
			}
		}
		return lowest;
	}
	std::vector <std::pair<int, int>> findPath(sf::Vector2i s, sf::Vector2i e)
	{
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				getTile(x, y)->node = Node(x,y);
			}
		}
		// Init start / end
		this->open.clear();
		this->closed.clear();
		Tile* start = getTile(s.x, s.y);
		Tile* end = getTile(e.x, e.y);
		std::vector<std::pair<int, int>> neighbors;
		// add start to closed
		toOpen(start);
		start->node.g = 0;
		start->node.H(end->node);
		start->node.F();
		Tile* current = start;

		while (open.size() > 0)
		{
			current = getLowest();
			if (current->node == end->node)
				break;
			toClosed(current);
			neighbors = getNeighbors(current->node.x, current->node.y);
			for (auto n : neighbors)
			{
				bool addOpen = false;
				Tile* nt = getTile(n.first, n.second);
				if (nt->node.inClosed())
					continue;
				if (!nt->node.inOpen())
				{
					addOpen = true;
					nt->node.H(end->node);
				}
				float new_g = nt->node.NewG(current->node);
				if (new_g >= nt->node.g)
					continue;
				nt->node.P(&current->node);
				nt->node.G(current->node);
				nt->node.F();
				if(addOpen)
					toOpen(nt);
			}
		}
		return getPath(start, end);
	}
};