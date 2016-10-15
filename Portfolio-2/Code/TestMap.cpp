#include "TestMap.h"



TestMap::TestMap()
{
	vertices.push_back({ 10, 10 });
	vertices.push_back({ 20, 20 });
	vertices.push_back({ 30, 30 });
	vertices.push_back({ 40, 40 });
	vertices.push_back({ 50, 50 });
	vertices.push_back({ 60, 60 });

	edges.push_back({ 0, 2 });
	//edges.push_back({ 2, 4 });
	edges.push_back({ 1, 2 });
	edges.push_back({ 2, 3 });
	edges.push_back({ 0, 4 });
	edges.push_back({ 3, 5 });
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
	int vertex;

	q.push(start);

	while (!q.empty()) {
		vertex = q.front();
		q.pop();

		for (int i = 0; i < edges.size(); i++) {
			if (edges[i][0] == vertex)
				adjecentVertices.push(edges[i][1]);
			if (edges[i][1] == vertex)
				adjecentVertices.push(edges[i][0]);
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
	double closestsDist = 10000;
	int closestVertex;

	for (size_t i = 0; i < vertices.size(); i++)
	{
		dist = sqrt(pow(posX - vertices[i][0], 2) + pow(posY - vertices[i][1], 2)); // Pythagoras

		if (dist < closestsDist) {
			closestsDist = dist;
			closestVertex = i;
		}
	}

	return closestVertex;

}

void TestMap::createPath(int startX, int startY, int goalX, int goalY)
{
	int startVertex = findClosestValidVertex(startX, startY);
	int goalVertex = findClosestValidVertex(goalX, goalY);

	while (!path.empty())
		path.pop();

	createGraph(startVertex);

	int pointer = goalVertex;

	path.push({ goalX, goalY });

	while (pointer != startVertex) {
		path.push({ vertices[pointer][0], vertices[pointer][1] });
		pointer = graph[pointer][1];
	}

	path.push({ vertices[pointer][0], vertices[pointer][1] });

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
