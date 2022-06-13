#pragma once
#include <iostream>
#include <Windows.h>
#include <stack>
#include <array>
#include <vector>
#include <ctime>
#include <cstdio>
#include "Generator.h"
#include "Node.h"
#include "Vertex.h"
#include "Renderer.h"

const int MAX_MAZE_WIDTH = 26;
const int MAX_MAZE_HEIGHT = 21;

class MazeGenerator
{

	private:
		uint32_t mazeWidth;
		uint32_t mazeHeight;
		uint32_t nVisitedNodes;
		Node startNode;

		uint32_t oldMazeHeight;

		std::stack<Node> stack;

		std::vector<Node> traversalOrder;

		std::vector<float> vertice;
		std::vector<uint32_t> indices;

		bool** maze;// keeps track of visited nodes
		uint32_t** adjacencyMatrix;
		bool isInitialRenderCall;
		bool start;
		int traversalIndex; //traversal index
		size_t size; //size of each quad
		mutable bool isAdjacencyMatrixSet;

	private:
		void fillAdjacencyMatrix(Node firstNode, Node secondNode);

	public:
		MazeGenerator(uint32_t mazeWidth, uint32_t mazeHeight);
		~MazeGenerator();

		void generate(uint32_t shaderProgram, uint32_t mazeWidth, uint32_t mazeHeight, uint32_t oldMazeHeight);
		void render(uint32_t shaderProgram, uint32_t speed);
		void reset();

		std::vector<float>& getVertices();
		uint32_t** getAdjacencyMatrix() const;
		uint32_t getMazeWidth() const;
		uint32_t getMazeHeight() const;
		size_t getQuadSize() const;
		int isMatrixSet() const;




};

