#include "TestMap.h"



TestMap::TestMap()
{
	vertices.push_back(10010);
	vertices.push_back(20020);
	vertices.push_back(30030);
	vertices.push_back(40040);
	vertices.push_back(50050);
	vertices.push_back(60060);

	edges.push_back({ 10010, 30030 });
	//edges.push_back({ 30030, 50050 });
	edges.push_back({ 20020, 30030 });
	edges.push_back({ 30030, 40040 });
	edges.push_back({ 10010, 50050 });
	edges.push_back({ 40040, 60060 });
}

TestMap::TestMap(Image * inputMap)
{
	map = inputMap;

	vertices.push_back(10010);
	vertices.push_back(20020);
	vertices.push_back(30030);
	vertices.push_back(40040);
	vertices.push_back(50050);
	vertices.push_back(60060);

	edges.push_back({ 10010, 30030 });
	//edges.push_back({ 30030, 50050 });
	edges.push_back({ 20020, 30030 });
	edges.push_back({ 30030, 40040 });
	edges.push_back({ 10010, 50050 });
	edges.push_back({ 40040, 60060 });
}


TestMap::~TestMap()
{
}


void TestMap::createGraph(int start)
{
	int inf = 10000;

	graph.clear();

	for (int i = 0; i < vertices.size(); i++) {
		graph.push_back({ inf, start });
	}

	graph[start][0] = 0;

	std::queue<int> q;
	std::queue<int> adjecentVertices;
	int graphIndex = 0;
	int vertex;

	q.push(start);

	while (!q.empty()) {
		vertex = q.front();
		q.pop();

		for (int i = 0; i < edges.size(); i++) {
			if (edges[i][0] == vertices[vertex])
				adjecentVertices.push(findIndexOfVertex(edges[i][1]));
			if (edges[i][1] == vertices[vertex])
				adjecentVertices.push(findIndexOfVertex(edges[i][0]));
		}

		while (!adjecentVertices.empty()) {
			if (graph[adjecentVertices.front()][0] == inf)
				q.push(adjecentVertices.front());

			if (graph[adjecentVertices.front()][0] > (graph[vertex][0] + 1)) { // substitute 1 with cost
				graph[adjecentVertices.front()][0] = (graph[vertex][0] + 1);
				graph[adjecentVertices.front()][1] = vertex;
			}

			adjecentVertices.pop();
		}
	}
}

void TestMap::printGraph()
{
	std::cout << "Graph is: " << std::endl;
	for (size_t i = 0; i < graph.size(); i++)
	{
		std::cout << "vNr:" << i << "   Dist:" << graph[i][0] << "   From:" << graph[i][1] << std::endl;
	}
	std::cout << "-- End of graph --" << std::endl;
}


int TestMap::findClosestValidVertex(int posX, int posY)
{
	double dist;
	double closestsDist = 100000;
	int closestVertex = 100000;

	for (size_t i = 0; i < vertices.size(); i++)
	{
		//std::cout << "vNr" << i << std::endl;

		if (isPathClear(posX, posY, (vertices[i] / 1000), (vertices[i] % 1000))) {
			dist = sqrt(pow(posX - (vertices[i] / 1000), 2) + pow(posY - (vertices[i] % 1000), 2)); // Pythagoras

			if (dist < closestsDist) {
				closestsDist = dist;
				closestVertex = i;
			}
		}
	}

	return closestVertex;
}

void TestMap::createPath(int startX, int startY, int goalX, int goalY)
{
	int startVertex = findClosestValidVertex(startX, startY);
	if (startVertex == 100000) {
		std::cout << "No path from start to a node in map" << std::endl;
		return;
	}
		
	int goalVertex = findClosestValidVertex(goalX, goalY);
	if (goalVertex == 100000) {
		std::cout << "No path from goal to a node in map" << std::endl;
		return;
	}

	while (!path.empty())
		path.pop();

	createGraph(startVertex);

	int pointer = goalVertex;

	path.push({ goalX, goalY });

	while (pointer != startVertex) {
		path.push({ vertices[pointer] / 1000, vertices[pointer] % 1000 });
		pointer = graph[pointer][1];
	}

	path.push({ vertices[pointer] / 1000, vertices[pointer] % 1000 });

}

void TestMap::printPath()
{
	std::cout << "Path is: " << std::endl;
	while (!path.empty()) {
		std::cout << path.top()[0] << ", " << path.top()[1] << std::endl;
		path.pop();
	}
	std::cout << "--End of path--" << std::endl;
}

void TestMap::makeDot()
{
	pathMap = map->copyFlip(0, 0);

	pathMap->setPixel8U(1, 1, 255, 0, 0);

	pathMap->saveAsPPM("pathMap.ppm");
}

int TestMap::findIndexOfVertex(int vertex)
{
	for (int i = 0; i < vertices.size(); i++) {
		if (vertices[i] == vertex)
			return i;
	}
}

void TestMap::drawStraightLine(int x0, int y0, int x1, int y1)
{

	pathMap = map->copyFlip(0, 0);

	if (x0 < x1) {
		int deltaX = x1 - x0;
		int deltaY = y1 - y0;

		if (y0 > y1)
			deltaY = y0 - y1;

		if (x0 > x1)
			deltaX = x0 - x1;

		if (deltaY < deltaX) {
			int deltaErr = deltaY - deltaX;
			int y = y0;

			for (int x = x0; x < x1; x++)
			{
				pathMap->setPixel8U(x, y, 150);

				if (deltaErr >= 0) {
					if (y1 > y0)
						y = y + 1;
					else
						y = y - 1;
					deltaErr = deltaErr - deltaX;
				}

				deltaErr = deltaErr + deltaY;

			}
		}

		else {

			int deltaErr = deltaX - deltaY;

			if (y0 > y1) {
				int x = x1;
				for (int y = y1; y < y0; y++)
				{
					if (pathMap->getPixelValuei(x, y, 0) != 0)
						pathMap->setPixel8U(x, y, 150);

					if (deltaErr >= 0) {
						if (x1 > x0)
							x = x - 1;
						else
							x = x + 1;
						deltaErr = deltaErr - deltaY;
					}

					deltaErr = deltaErr + deltaX;

				}
			}
			else {
				int x = x0;
				for (int y = y0; y < y1; y++)
				{
					if (pathMap->getPixelValuei(x, y, 0) != 0)
						pathMap->setPixel8U(x, y, 150);

					if (deltaErr >= 0) {
						if (x1 > x0)
							x = x + 1;
						else
							x = x - 1;
						deltaErr = deltaErr - deltaY;
					}

					deltaErr = deltaErr + deltaX;

				}
			}
		}


	}

	else {
		int deltaX = x0 - x1;
		int deltaY = y0 - y1;
		int y = y1;

		if (y1 > y0)
			deltaY = y1 - y0;

		int deltaErr = deltaY - deltaX;

		if (deltaY < deltaX) {
			for (int x = x1; x < x0; x++)
			{

				if (pathMap->getPixelValuei(x, y, 0) != 0)
					pathMap->setPixel8U(x, y, 150);

				if (deltaErr >= 0) {
					if (y1 > y0)
						y = y - 1;
					else
						y = y + 1;
					deltaErr = deltaErr - deltaX;
				}

				deltaErr = deltaErr + deltaY;

			}
		}

		else {

			int deltaErr = deltaX - deltaY;

			if (y0 > y1) {
				int x = x1;
				for (int y = y1; y < y0; y++)
				{
					if (pathMap->getPixelValuei(x, y, 0) != 0)
						pathMap->setPixel8U(x, y, 150);

					if (deltaErr >= 0) {
						if (x1 > x0)
							x = x - 1;
						else
							x = x + 1;
						deltaErr = deltaErr - deltaY;
					}

					deltaErr = deltaErr + deltaX;

				}
			}
			else {
				int x = x0;
				for (int y = y0; y < y1; y++)
				{
					if (pathMap->getPixelValuei(x, y, 0) != 0)
						pathMap->setPixel8U(x, y, 150);

					if (deltaErr >= 0) {
						if (x1 > x0)
							x = x + 1;
						else
							x = x - 1;
						deltaErr = deltaErr - deltaY;
					}

					deltaErr = deltaErr + deltaX;

				}
			}
		}
	}
}

bool TestMap::isPathClear(int x0, int y0, int x1, int y1)
{
	if (x0 < x1) {
		int deltaX = x1 - x0;
		int deltaY = y1 - y0;

		if (y0 > y1)
			deltaY = y0 - y1;

		if (x0 > x1)
			deltaX = x0 - x1;

		if (deltaY < deltaX) {
			int deltaErr = deltaY - deltaX;
			int y = y0;

			for (int x = x0; x < x1; x++)
			{
				if (map->getPixelValuei(x, y, 0) == 0)
					return 0;

				if (deltaErr >= 0) {
					if (y1 > y0)
						y = y + 1;
					else
						y = y - 1;
					deltaErr = deltaErr - deltaX;
				}

				deltaErr = deltaErr + deltaY;

			}
		}

		else {

			int deltaErr = deltaX - deltaY;

			if (y0 > y1) {
				int x = x1;
				for (int y = y1; y < y0; y++)
				{
					if (map->getPixelValuei(x, y, 0) == 0)
						return 0;

					if (deltaErr >= 0) {
						if (x1 > x0)
							x = x - 1;
						else
							x = x + 1;
						deltaErr = deltaErr - deltaY;
					}

					deltaErr = deltaErr + deltaX;

				}
			}
			else {
				int x = x0;
				for (int y = y0; y < y1; y++)
				{
					if (map->getPixelValuei(x, y, 0) == 0)
						return 0;

					if (deltaErr >= 0) {
						if (x1 > x0)
							x = x + 1;
						else
							x = x - 1;
						deltaErr = deltaErr - deltaY;
					}

					deltaErr = deltaErr + deltaX;

				}
			}
		}


	}

	else {
		int deltaX = x0 - x1;
		int deltaY = y0 - y1;
		int y = y1;

		if (y1 > y0)
			deltaY = y1 - y0;

		int deltaErr = deltaY - deltaX;

		if (deltaY < deltaX) {
			for (int x = x1; x < x0; x++)
			{

				if (map->getPixelValuei(x, y, 0) == 0)
					return 0;

				if (deltaErr >= 0) {
					if (y1 > y0)
						y = y - 1;
					else
						y = y + 1;
					deltaErr = deltaErr - deltaX;
				}

				deltaErr = deltaErr + deltaY;

			}
		}

		else {

			int deltaErr = deltaX - deltaY;

			if (y0 > y1) {
				int x = x1;
				for (int y = y1; y < y0; y++)
				{
					if (map->getPixelValuei(x, y, 0) == 0)
						return 0;

					if (deltaErr >= 0) {
						if (x1 > x0)
							x = x - 1;
						else
							x = x + 1;
						deltaErr = deltaErr - deltaY;
					}

					deltaErr = deltaErr + deltaX;

				}
			}
			else {
				int x = x0;
				for (int y = y0; y < y1; y++)
				{
					if (map->getPixelValuei(x, y, 0) == 0)
						return 0;

					if (deltaErr >= 0) {
						if (x1 > x0)
							x = x + 1;
						else
							x = x - 1;
						deltaErr = deltaErr - deltaY;
					}

					deltaErr = deltaErr + deltaX;

				}
			}
		}
	}

	return 1;
}
