#include "MazeGenerator.h"


MazeGenerator::MazeGenerator(uint32_t mazeWidth, uint32_t mazeHeight)
	:mazeWidth(mazeWidth),
	mazeHeight(mazeHeight),
	nVisitedNodes(0),
	size(16),
	isInitialRenderCall(true),
	start(true),
	traversalIndex(0),
	isAdjacencyMatrixSet(false)

{
	maze = new bool * [MAX_MAZE_HEIGHT];
	for (uint32_t i = 0; i < MAX_MAZE_HEIGHT; i++)
	{
		maze[i] = new bool[MAX_MAZE_WIDTH];
		
		for (uint32_t j = 0; j < MAX_MAZE_WIDTH; j++)
		{
			maze[i][j] = false;
		}
	}

	adjacencyMatrix = new uint32_t* [MAX_MAZE_WIDTH * MAX_MAZE_HEIGHT];
	for (uint32_t i = 0; i < MAX_MAZE_WIDTH * MAX_MAZE_HEIGHT; i++)
	{
		adjacencyMatrix[i] = new uint32_t[MAX_MAZE_WIDTH * MAX_MAZE_HEIGHT];

		for (uint32_t j = 0; j < MAX_MAZE_WIDTH * MAX_MAZE_HEIGHT; j++)
		{
			adjacencyMatrix[i][j] = false;
		}
	}

}

MazeGenerator::~MazeGenerator()
{
	for (uint32_t i = 0; i < MAX_MAZE_HEIGHT; i++)
	{
		delete[] maze[i];
	}
	delete[] maze;

	for (uint32_t i = 0; i < MAX_MAZE_WIDTH * MAX_MAZE_HEIGHT; i++)
	{
		delete adjacencyMatrix[i];

	}
	delete[] adjacencyMatrix;
}

void MazeGenerator::render(uint32_t shaderProgram, uint32_t speed)
{
	const uint32_t numberOfQuads = 4 * (mazeWidth) * (mazeHeight);
	
	uint32_t vertexLength = 7;
	uint32_t quadVertexLength = 4;
	for (uint32_t i = 0; i < traversalOrder.size(); i++)
	{
		if (i + 1 < traversalOrder.size())
		{

			fillAdjacencyMatrix(traversalOrder[i], traversalOrder[i + 1]);
		}
	}

	if (!isInitialRenderCall && traversalIndex < traversalOrder.size())
	{	

		Sleep(500/speed);

		
		std::vector<Vertex> currQuad = (Generator::generateQuad(traversalOrder[traversalIndex].location.s, traversalOrder[traversalIndex].location.t, glm::vec4(1.f, 0.f, 0.f, 1.f),size));
		std::vector<Vertex> prevQuad = (Generator::generateQuad(traversalOrder[traversalIndex].location.s, traversalOrder[traversalIndex].location.t, glm::vec4(1.f, 1.f, 1.f, 1.f),size));
		std::vector<Vertex> wallQuad = (Generator::generateQuad(traversalOrder[traversalIndex].location.s, traversalOrder[traversalIndex].location.t, glm::vec4(0.f, 0.f, 0.f, 1.f),size));

		if (traversalIndex + 1 == traversalOrder.size())
		{
			currQuad = (Generator::generateQuad(traversalOrder[traversalIndex].location.s, traversalOrder[traversalIndex].location.t, glm::vec4(1.f, 1.f, 1.f, 1.f), size));
		}

		for (int k = 0; k < currQuad.size(); k++)
		{
			uint32_t previousLocation;
			if (!start)
			{
				
				previousLocation = (traversalOrder[traversalIndex - 1].location.s * 2) * vertexLength * quadVertexLength * (mazeWidth) * 2 + (traversalOrder[traversalIndex - 1].location.t * 2) * vertexLength * quadVertexLength + vertexLength * k + 3;
				if (previousLocation < vertice.size())
				{
					vertice[previousLocation] = prevQuad[k].color.r;
					vertice[previousLocation + 1] = prevQuad[k].color.g;
					vertice[previousLocation + 2] = prevQuad[k].color.b;
					vertice[previousLocation + 3] = prevQuad[k].color.a;
				}
			}

			uint32_t currentLocation = (traversalOrder[traversalIndex].location.s * 2) * vertexLength * quadVertexLength * (mazeWidth) * 2 + (traversalOrder[traversalIndex].location.t * 2) * vertexLength * quadVertexLength + vertexLength * k + 3;
			
			if (currentLocation < vertice.size())
			{	
				vertice[currentLocation] = currQuad[k].color.r;
				vertice[currentLocation + 1] = currQuad[k].color.g;
				vertice[currentLocation + 2] = currQuad[k].color.b;
				vertice[currentLocation + 3] = currQuad[k].color.a;
			}

			uint32_t noWallLocation = vertice.size() + 5;
			//Determing the direction where there is no wall
			if (!start)
			{
					int32_t condition = traversalOrder[traversalIndex].location.s - traversalOrder[traversalIndex - 1].location.s;
					switch (condition)
					{
					case 1: //no wall at top
						noWallLocation = ((traversalOrder[traversalIndex - 1].location.s * 2) + 1) * vertexLength * quadVertexLength * mazeWidth * 2 + (traversalOrder[traversalIndex - 1].location.t * 2) * vertexLength * quadVertexLength + vertexLength * k + 3;
						break;
					case -1://no wall at bottom
						noWallLocation = ((traversalOrder[traversalIndex - 1].location.s * 2) - 1) * vertexLength * quadVertexLength * mazeWidth * 2 + (traversalOrder[traversalIndex - 1].location.t * 2) * vertexLength * quadVertexLength + vertexLength * k + 3;
						break;
					case 0:
						break;

					}


					condition = traversalOrder[traversalIndex].location.t - traversalOrder[traversalIndex - 1].location.t;
					switch (condition)
					{
					case 1: //no wall at left
						noWallLocation = (traversalOrder[traversalIndex - 1].location.s * 4) * vertexLength * quadVertexLength * mazeWidth + ((traversalOrder[traversalIndex - 1].location.t * 2) + 1) * vertexLength * quadVertexLength + vertexLength * k + 3;

						break;
					case -1://no wall at right
						noWallLocation = (traversalOrder[traversalIndex - 1].location.s * 4) * vertexLength * quadVertexLength * mazeWidth + ((traversalOrder[traversalIndex - 1].location.t * 2) - 1) * vertexLength * quadVertexLength + vertexLength * k + 3;
					case 0:
						break;

					}
					if (noWallLocation < vertice.size())
					{
						vertice[noWallLocation] = prevQuad[k].color.r;
						vertice[noWallLocation + 1] = prevQuad[k].color.g;
						vertice[noWallLocation + 2] = prevQuad[k].color.b;
						vertice[noWallLocation + 3] = prevQuad[k].color.a;


					}
				
				
			}

		}

		start = false;
		traversalIndex++;
		if (traversalIndex + 1 == traversalOrder.size())
		{
			isAdjacencyMatrixSet = true;
		}
	}


	static Renderer renderer(shaderProgram);
	if (isInitialRenderCall)
	{
		//setting indicess
		vertice.reserve(numberOfQuads* vertexLength* quadVertexLength);

		indices.reserve(6 * numberOfQuads);
		uint32_t currQuad = 0;
		int i = 0;
		while (currQuad < numberOfQuads)
		{
			indices.push_back(i);
			indices.push_back(i + 1);
			indices.push_back(i + 2);
			indices.push_back(i + 2);
			indices.push_back(i + 3);
			indices.push_back(i);

			i += 4;
			currQuad++;
		}

		//vertices
		for (uint32_t i = 0; i < 2 * mazeHeight; i++)
		{
			for (int j = 0; j < 2 * mazeWidth; j++)
			{
				std::vector<Vertex> quad = (Generator::generateQuad(20 + size * j + j, 20 + size * i + i, glm::vec4(0.f, 0.f, 0.f, 1.f),size));

				for (int k = 0; k < quad.size(); k++)
				{
					vertice.push_back(quad[k].position.x);
					vertice.push_back(quad[k].position.y);
					vertice.push_back(quad[k].position.z);
					vertice.push_back(quad[k].color.r);
					vertice.push_back(quad[k].color.g);
					vertice.push_back(quad[k].color.b);
					vertice.push_back(quad[k].color.a);
				}
			}
		}



		renderer.create(&vertice[0], vertice.size()*sizeof(float), &indices[0], indices.size()*sizeof(uint32_t), indices.size());
		isInitialRenderCall = false;
	}

	renderer.render(&vertice[0]);
}




void MazeGenerator::generate(uint32_t shaderProgram, uint32_t mazeWidth, uint32_t mazeHeight, uint32_t oldMAzeHeight)
{
	this->oldMazeHeight = oldMAzeHeight;
	this->mazeWidth = mazeWidth;
	this->mazeHeight = mazeHeight;
	reset();

	Node currNode;

	srand((unsigned)time(0));
	startNode = Node(rand() % (mazeHeight - 1), rand() % (mazeWidth - 1));
	nVisitedNodes++;

	currNode = startNode;

	stack.push(startNode);
	traversalOrder.push_back(startNode);
	maze[stack.top().location.x][stack.top().location.y] = true;

	while (nVisitedNodes < mazeWidth * mazeHeight)
	{
		
		std::vector<int16_t> neighbour;

		//Here s and t are equivalent to y and x axis respectively

		//up neighbour
		if ((stack.top().location.s < mazeHeight - 1)
			&& (maze[stack.top().location.s + 1][stack.top().location.t] == false))
		{
			neighbour.push_back(up);
		}

		//down neighbour
		if (stack.top().location.s > 0
			&& (maze[stack.top().location.s - 1][stack.top().location.t] == false))
		{
			neighbour.push_back(down);
		}

		//left neighbour
		if (stack.top().location.t > 0
			&& (maze[stack.top().location.s][stack.top().location.t - 1] == false))
		{
			neighbour.push_back(left);
		}

		//Right neighbour
		if ((stack.top().location.t < mazeWidth - 1)
			&& (maze[stack.top().location.s][stack.top().location.t + 1] == false))
		{
			neighbour.push_back(right);
		}

		if (!neighbour.empty())
		{
			int16_t currDirection = neighbour[rand() % neighbour.size()];

			switch (currDirection)
			{
				case up:
					
					currNode = Node(stack.top().location.s + 1, stack.top().location.t);
					maze[currNode.location.s][currNode.location.t] = true;
					stack.push(currNode);
					break;

				case down:
					
					currNode = Node(stack.top().location.s - 1, stack.top().location.t);
					maze[currNode.location.s][currNode.location.t] = true;
					stack.push(currNode);
					break;

				case left:
					
					currNode = Node(stack.top().location.s, stack.top().location.t - 1);
					maze[currNode.location.s][currNode.location.t] = true;
					stack.push(currNode);
					break;


				case right:
					
					currNode = Node(stack.top().location.s, stack.top().location.t + 1);
					maze[currNode.location.s][currNode.location.t] = true;
					stack.push(currNode);
					break;

			}
			if((currNode.location.s < mazeHeight) && (currNode.location.t < mazeWidth))
			traversalOrder.push_back(currNode);	
			nVisitedNodes++;
		}
		else
		{
			
			
				stack.pop();

				if (!stack.empty())
				{
				traversalOrder.push_back(stack.top());
				}
		}
	}
}

void MazeGenerator::reset()
{
	//resetting everything to generate new maze
	nVisitedNodes = 0;
	isInitialRenderCall = true;
	isAdjacencyMatrixSet = false;
	start = true;
	traversalIndex = 0;
	vertice.clear();
	indices.clear();
	traversalOrder.clear();
	while (!stack.empty())
	{
		stack.pop();
	}

	for (uint32_t i = 0; i < MAX_MAZE_HEIGHT; i++)
	{
		for (uint32_t j = 0; j < MAX_MAZE_WIDTH; j++)
		{	

			maze[i][j] = false;
		}
	}

	


	for (uint32_t i = 0; i < MAX_MAZE_WIDTH * MAX_MAZE_HEIGHT; i++)
	{
		for (uint32_t j = 0; j < MAX_MAZE_WIDTH * MAX_MAZE_HEIGHT; j++)
		{
			adjacencyMatrix[i][j] = false;
		}
	}
}


void MazeGenerator::fillAdjacencyMatrix(Node firstNode, Node secondNode)
{
	adjacencyMatrix[firstNode.location.s * mazeWidth + firstNode.location.t]
		[secondNode.location.s * mazeWidth + secondNode.location.t] = true;

	adjacencyMatrix[secondNode.location.s * mazeWidth + secondNode.location.t]
		[firstNode.location.s * mazeWidth + firstNode.location.t] = true;

}

uint32_t** MazeGenerator::getAdjacencyMatrix() const
{	
	isAdjacencyMatrixSet = false;

	return adjacencyMatrix;
}

uint32_t MazeGenerator::getMazeWidth() const
{
	return mazeWidth;
}

uint32_t MazeGenerator::getMazeHeight() const
{
	return mazeHeight;
}

int MazeGenerator::isMatrixSet() const
{
	return isAdjacencyMatrixSet;
}

std::vector<float>& MazeGenerator::getVertices()
{
	return vertice;
}

size_t MazeGenerator::getQuadSize() const
{
	return size;
}