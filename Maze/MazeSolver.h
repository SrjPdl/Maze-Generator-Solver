#pragma once
#include "MazeGenerator.h"
class MazeSolver
{
	private:
		MazeGenerator& mazeGenerator;
		uint32_t** adjacencyMatrix;
		uint32_t mazeWidth;
		uint32_t mazeHeight;
		uint32_t pathCost;

		std::vector<float>& vertices;

		Node startNode;
		Node endNode;

		uint32_t visitedNode[MAX_MAZE_WIDTH * MAX_MAZE_HEIGHT];
		std::vector<uint32_t> shortestDistance;
		uint32_t previousNode[MAX_MAZE_WIDTH * MAX_MAZE_HEIGHT];

		std::vector<uint32_t> shortestPath;
		static int32_t shortestPathIndex;

		bool isInitialCall;
		bool start;


	private:
		void initializeDijkstra();
		void DijkstrasAlgorithm();
		void ClearMaze();

	public:
		MazeSolver(MazeGenerator& mazeGenerator);
		void solve(Node startNode, Node endNode);
		void reset();
		void resetNode(Node startNode, Node endNode);
		void render(uint32_t shaderProgram, Node startNode, Node endNode, int32_t speed);



};

