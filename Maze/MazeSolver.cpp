#include "MazeSolver.h"
int32_t MazeSolver::shortestPathIndex = INT_MAX;

MazeSolver::MazeSolver(MazeGenerator& mazeGenerator)
	:mazeGenerator(mazeGenerator),
	 adjacencyMatrix(mazeGenerator.getAdjacencyMatrix()),
	 mazeWidth(mazeGenerator.getMazeWidth()),
	 mazeHeight(mazeGenerator.getMazeHeight()),
	 startNode(Node(1,1)),
	 endNode(Node(1,1)),
	 pathCost(1),
	 vertices(mazeGenerator.getVertices()),
	 isInitialCall(true),
	 start(true)
{
	for (uint32_t i = 0; i < MAX_MAZE_WIDTH * MAX_MAZE_HEIGHT; i++)
	{
		visitedNode[i] = 0;
	}
}

void MazeSolver::initializeDijkstra()
{
	mazeWidth = mazeGenerator.getMazeWidth();
	mazeHeight = mazeGenerator.getMazeHeight();

	shortestDistance.resize(mazeWidth * mazeHeight);

	for (uint32_t i = 0; i < mazeWidth * mazeHeight; i++)
	{
		if (i == (startNode.location.s * mazeWidth + startNode.location.t))
		{
			shortestDistance[i] = 0;
		}
		else
		{
			shortestDistance[i] = UINT_MAX;
		}
	
	}
	
}

void MazeSolver::DijkstrasAlgorithm()
{
	Node currNode = startNode;

	

	bool isEveryNodeVisited = false;
	while(!isEveryNodeVisited)
	{	
		std::vector<uint32_t> neighbours;

		//Checking which nodes are connected to currently active node
		for (uint32_t j = 0; j < mazeWidth * mazeHeight; j++)
		{
			if (adjacencyMatrix[currNode.location.s * mazeWidth + currNode.location.t][j]
				&& !visitedNode[j])
			{
				neighbours.push_back(j);
			}

		}

		//updating distance of neighbours
		for(uint32_t i=0; i<neighbours.size(); i++)
		{
			uint32_t currNeighbour = neighbours[i];
			uint32_t currDistance = shortestDistance[currNode.location.s * mazeWidth + currNode.location.t] + pathCost;

			if (currDistance < shortestDistance[currNeighbour])
			{
				shortestDistance[currNeighbour] = currDistance;

				previousNode[currNeighbour] = currNode.location.s * mazeWidth + currNode.location.t;
			}

		}

		visitedNode[currNode.location.s * mazeWidth + currNode.location.t] = 1;


		Node prevNode = currNode;

		uint32_t shortestNextNodeDistance = UINT_MAX;

		for (uint32_t i = 0; i < mazeWidth * mazeHeight; i++)
		{
			if ((shortestDistance[i] < shortestNextNodeDistance) && !visitedNode[i])
			{
				shortestNextNodeDistance = shortestDistance[i];
				currNode = Node(floor(i / (float)mazeWidth), (i % mazeWidth));
				
			}
		}

		


		for (uint32_t i = 0; i < mazeWidth * mazeHeight; i++)
		{
			if (visitedNode[i] == 0)
			{
				isEveryNodeVisited = false;
				break;
			}
			isEveryNodeVisited = true;
		}

	}
}

void MazeSolver::solve(Node startNode, Node endNode)
{
	this->startNode = startNode;
	this->endNode = endNode;

	reset();
	initializeDijkstra();
	DijkstrasAlgorithm();

	uint32_t currNode = endNode.location.s * mazeWidth + endNode.location.t;
	
	while (currNode!=(startNode.location.s * mazeWidth + startNode.location.t))
	{
		shortestPath.push_back(currNode);
		currNode = previousNode[currNode];
	}
	shortestPath.push_back(currNode);

	if (isInitialCall)
	{
		shortestPathIndex = shortestPath.size() - 1;
		isInitialCall = false;
	}

}

void MazeSolver::render(uint32_t shaderProgram, Node startNode, Node endNode, int32_t speed)
{
	mazeWidth = mazeGenerator.getMazeWidth();
	mazeHeight = mazeGenerator.getMazeHeight();

	uint32_t vertexLength = 7;
	uint32_t quadVertexLength = 4;

	Renderer renderer(shaderProgram);


	Node currNode;
	Node prevNode;
	uint32_t topOfStack;
	bool isStartNode = false;
	bool isEndNode = false;



	if (this->startNode.location.s!=startNode.location.s   ||
		this->startNode.location.t != startNode.location.t ||
		this->endNode.location.s != endNode.location.s     ||
		this->endNode.location.t != endNode.location.t        )
	{
		ClearMaze();
		std::vector<Vertex> prevStartQuad = Generator::generateQuad(this->startNode.location.s, this->startNode.location.t, glm::vec4(1.f, 1.f, 1.f, 0.f), mazeGenerator.getQuadSize());
		std::vector<Vertex> prevEndQuad = Generator::generateQuad(this->endNode.location.s, this->endNode.location.t, glm::vec4(1.f, 1.f, 1.f, 0.f), mazeGenerator.getQuadSize());

		for (uint32_t i = 0; i < prevStartQuad.size(); i++)
		{
			uint32_t startNodeLocation = (this->startNode.location.s * 2) * vertexLength * quadVertexLength * (mazeWidth) * 2 + (this->startNode.location.t * 2) * vertexLength * quadVertexLength + vertexLength * i + 3;
			uint32_t endNodeLocation = (this->endNode.location.s * 2) * vertexLength * quadVertexLength * (mazeWidth) * 2 + (this->endNode.location.t * 2) * vertexLength * quadVertexLength + vertexLength * i + 3;

			vertices[startNodeLocation] = prevStartQuad[i].color.r;
			vertices[startNodeLocation + 1] = prevStartQuad[i].color.g;
			vertices[startNodeLocation + 2] = prevStartQuad[i].color.b;
			vertices[startNodeLocation + 3] = prevStartQuad[i].color.a;


			vertices[endNodeLocation] = prevEndQuad[i].color.r;
			vertices[endNodeLocation + 1] = prevEndQuad[i].color.g;
			vertices[endNodeLocation + 2] = prevEndQuad[i].color.b;
			vertices[endNodeLocation + 3] = prevEndQuad[i].color.a;
		}


		std::vector<Vertex> startQuad = Generator::generateQuad(startNode.location.s, startNode.location.t, glm::vec4(0.f, 0.f, 1.f, 0.f), mazeGenerator.getQuadSize());
		std::vector<Vertex> endQuad = Generator::generateQuad(endNode.location.s, endNode.location.t, glm::vec4(0.f, 1.f, 0.f, 0.f), mazeGenerator.getQuadSize());

		for (uint32_t i = 0; i < startQuad.size(); i++)
		{
			uint32_t startNodeLocation = (startNode.location.s * 2) * vertexLength * quadVertexLength * (mazeWidth) * 2 + (startNode.location.t * 2) * vertexLength * quadVertexLength + vertexLength * i + 3;
			uint32_t endNodeLocation = (endNode.location.s * 2) * vertexLength * quadVertexLength * (mazeWidth) * 2 + (endNode.location.t * 2) * vertexLength * quadVertexLength + vertexLength * i + 3;

			vertices[startNodeLocation] = startQuad[i].color.r;
			vertices[startNodeLocation + 1] = startQuad[i].color.g;
			vertices[startNodeLocation + 2] = startQuad[i].color.b;
			vertices[startNodeLocation + 3] = startQuad[i].color.a;


			vertices[endNodeLocation] = endQuad[i].color.r;
			vertices[endNodeLocation + 1] = endQuad[i].color.g;
			vertices[endNodeLocation + 2] = endQuad[i].color.b;
			vertices[endNodeLocation + 3] = endQuad[i].color.a;
		}

		this->startNode = startNode;
		this->endNode = endNode;
	}
	
		if (shortestPathIndex >= 0 && shortestPathIndex< shortestPath.size())
		{
			Sleep(500 / speed);

			if (shortestPathIndex == 0)
			{
				isEndNode = true;
			}
			if (shortestPathIndex == shortestPath.size() - 1)
			{
				isStartNode = true;
			}

			currNode = (Node(floor(shortestPath[shortestPathIndex] / (float)mazeWidth), (shortestPath[shortestPathIndex] % mazeWidth)));
			
			if(!start)
			{
				prevNode = (Node(floor(shortestPath[shortestPathIndex + 1] / (float)mazeWidth), (shortestPath[shortestPathIndex + 1] % mazeWidth)));
			}

			std::vector<Vertex> quad = Generator::generateQuad(currNode.location.s, currNode.location.t, glm::vec4(0.952, 0.815, 0.247, 1.f), mazeGenerator.getQuadSize());
			std::vector<Vertex> prevQuad = Generator::generateQuad(currNode.location.s, currNode.location.t, glm::vec4(0.952, 0.815, 0.247, 1.f), mazeGenerator.getQuadSize());
			
			
			if (isStartNode)
			{
				quad = Generator::generateQuad(currNode.location.s, currNode.location.t, glm::vec4(0.f, 0.f, 1.f, 1.f), mazeGenerator.getQuadSize());
				isStartNode = false;
			}

			if (isEndNode)
			{
				quad = Generator::generateQuad(currNode.location.s, currNode.location.t, glm::vec4(0.f, 1.f, 0.f, 1.f), mazeGenerator.getQuadSize());
				isEndNode = false;
			}

			for (uint32_t i = 0; i < quad.size(); i++)
			{
				uint32_t currentLocation = (currNode.location.s * 2) * vertexLength * quadVertexLength * (mazeWidth) * 2 + (currNode.location.t * 2) * vertexLength * quadVertexLength + vertexLength * i + 3;

				vertices[currentLocation] = quad[i].color.r;
				vertices[currentLocation + 1] = quad[i].color.g;
				vertices[currentLocation + 2] = quad[i].color.b;
				vertices[currentLocation + 3] = quad[i].color.a;

				uint32_t noWallLocation = vertices.size() + 1;

				if (!start)
				{
					int32_t condition = currNode.location.s - prevNode.location.s;
					switch (condition)
					{
						case 1: //no wall at top
							noWallLocation = ((prevNode.location.s * 2) + 1) * vertexLength * quadVertexLength * mazeWidth * 2 + (prevNode.location.t * 2) * vertexLength * quadVertexLength + vertexLength * i + 3;
							break;
						case -1://no wall at bottom
							noWallLocation = ((prevNode.location.s * 2) - 1) * vertexLength * quadVertexLength * mazeWidth * 2 + (prevNode.location.t * 2) * vertexLength * quadVertexLength + vertexLength * i + 3;
							break;
						case 0:
							break;

					}


					condition = currNode.location.t - prevNode.location.t;
					switch (condition)
					{
						case 1: //no wall at left
							noWallLocation = (prevNode.location.s * 4) * vertexLength * quadVertexLength * mazeWidth + ((prevNode.location.t * 2) + 1) * vertexLength * quadVertexLength + vertexLength * i + 3;

							break;
						case -1://no wall at right
							noWallLocation = (prevNode.location.s * 4) * vertexLength * quadVertexLength * mazeWidth + ((prevNode.location.t * 2) - 1) * vertexLength * quadVertexLength + vertexLength * i + 3;
						case 0:
							break;

					}
					if (noWallLocation < vertices.size())
					{
						vertices[noWallLocation]     = prevQuad[i].color.r;
						vertices[noWallLocation + 1] = prevQuad[i].color.g;
						vertices[noWallLocation + 2] = prevQuad[i].color.b;
						vertices[noWallLocation + 3] = prevQuad[i].color.a;


					}
				}

			}
			shortestPathIndex--;
			start = false;
		}

	renderer.render(&vertices[0]);
	
}

void MazeSolver::reset()
{
	isInitialCall = true;
	start = true;

	shortestPath.clear();
	
	for (uint32_t i = 0; i < MAX_MAZE_WIDTH * MAX_MAZE_HEIGHT; i++)
	{
		visitedNode[i] = 0;
	}
}


void MazeSolver::ClearMaze()
{
	uint32_t vertexLength = 7;
	uint32_t quadVertexLength = 4;

	Node currNode;
	Node prevNode;

	shortestPathIndex = shortestPath.size() - 1;
	start = true;

	while (shortestPathIndex >= 0 && shortestPathIndex < shortestPath.size())
	{

		currNode = (Node(floor(shortestPath[shortestPathIndex] / (float)mazeWidth), (shortestPath[shortestPathIndex] % mazeWidth)));

		if (!start)
		{
			prevNode = (Node(floor(shortestPath[shortestPathIndex + 1] / (float)mazeWidth), (shortestPath[shortestPathIndex + 1] % mazeWidth)));
		}

		std::vector<Vertex> quad = Generator::generateQuad(currNode.location.s, currNode.location.t, glm::vec4(1.f, 1.f, 1.f, 1.f), mazeGenerator.getQuadSize());
		std::vector<Vertex> prevQuad = Generator::generateQuad(currNode.location.s, currNode.location.t, glm::vec4(1.f, 1.f, 1.f, 1.f), mazeGenerator.getQuadSize());

		for (uint32_t i = 0; i < quad.size(); i++)
		{
			uint32_t currentLocation = (currNode.location.s * 2) * vertexLength * quadVertexLength * (mazeWidth) * 2 + (currNode.location.t * 2) * vertexLength * quadVertexLength + vertexLength * i + 3;

			vertices[currentLocation] = quad[i].color.r;
			vertices[currentLocation + 1] = quad[i].color.g;
			vertices[currentLocation + 2] = quad[i].color.b;
			vertices[currentLocation + 3] = quad[i].color.a;

			uint32_t noWallLocation = vertices.size() + 1;

			if (!start)
			{
				int32_t condition = currNode.location.s - prevNode.location.s;
				switch (condition)
				{
				case 1: //no wall at top
					noWallLocation = ((prevNode.location.s * 2) + 1) * vertexLength * quadVertexLength * mazeWidth * 2 + (prevNode.location.t * 2) * vertexLength * quadVertexLength + vertexLength * i + 3;
					break;
				case -1://no wall at bottom
					noWallLocation = ((prevNode.location.s * 2) - 1) * vertexLength * quadVertexLength * mazeWidth * 2 + (prevNode.location.t * 2) * vertexLength * quadVertexLength + vertexLength * i + 3;
					break;
				case 0:
					break;

				}


				condition = currNode.location.t - prevNode.location.t;
				switch (condition)
				{
				case 1: //no wall at left
					noWallLocation = (prevNode.location.s * 4) * vertexLength * quadVertexLength * mazeWidth + ((prevNode.location.t * 2) + 1) * vertexLength * quadVertexLength + vertexLength * i + 3;

					break;
				case -1://no wall at right
					noWallLocation = (prevNode.location.s * 4) * vertexLength * quadVertexLength * mazeWidth + ((prevNode.location.t * 2) - 1) * vertexLength * quadVertexLength + vertexLength * i + 3;
				case 0:
					break;

				}
				if (noWallLocation < vertices.size())
				{
					vertices[noWallLocation] = prevQuad[i].color.r;
					vertices[noWallLocation + 1] = prevQuad[i].color.g;
					vertices[noWallLocation + 2] = prevQuad[i].color.b;
					vertices[noWallLocation + 3] = prevQuad[i].color.a;


				}
			}

		}
		shortestPathIndex--;
		start = false;
	}
}

void MazeSolver::resetNode(Node startNode, Node endNode)
{
	this->startNode = startNode;
	this->endNode = endNode;
}