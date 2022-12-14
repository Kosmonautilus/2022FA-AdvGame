#include "assignment1/entitymanager.cpp"
#include "assignment1/filereadwrite.cpp"

//Create level tile map of 17 x 10 
//If player moves past coordinate boundary then they are transported to the next level in that direction.
//Correlates to a 2D vector of levels (9x9? Probably want to be extensible.)

//How do we save our levels? Do we want to read from a file or set it at runtime?
//Can we map entities based on their position rather than their index?


//We want player position to transfer over to the new level (to stop from getting stuck in walls!)
/*      L1               L2
		#########        #########
		#       #        ##   ####
		#     ->@      ->@  X   ##
		#       #        #    ####
		#########        #########
	
	Get level transition direction
	player should be on opposite side of transition direction axis

	ex: moving left to right = player exits on right, starts on left
		moving down to up = player exits top, starts bottom


*/

//MOST OF THIS IS JUST MOSAIC GRID DESIGN

struct LevelGrid 
{
	real32 tileSize;

	vec2 gridSize;
	vec2 gridOrigin;

	uint8 gridWidth;
	uint8 gridHeight;

	bool toggleGrid;

	real32 lineThickness;
};

void DrawLevelGrid(LevelGrid* grid)
{
	for (int y = 0; y < grid->gridHeight + 1; y++)
	{
		vec2 rowLineCenter = grid->gridOrigin + V2((grid->gridSize.x * 0.5f), 0) + V2(0, -y * grid->tileSize);
		vec2 scale = V2(grid->gridSize.x * 0.5f, grid->lineThickness);
		if (y == 0 || y == grid->gridHeight)
		{
			scale = V2(grid->gridSize.x * 0.5f + grid->lineThickness, grid->lineThickness);
		}

		DrawRect(rowLineCenter, scale, V4(1, 1, 1, 0.5f));
	}

	for (int x = 0; x < grid->gridWidth + 1; x++)
	{
		vec2 colLineCenter = grid->gridOrigin + V2(0, (-grid->gridSize.y * 0.5f)) + V2(x * grid->tileSize, 0);

		vec2 scale = V2(grid->lineThickness, grid->gridSize.y * 0.5f);

		if (x == 0 || x == grid->gridWidth)
		{
			scale = V2(grid->lineThickness, grid->gridSize.y * 0.5f + grid->lineThickness);
		}

		DrawRect(colLineCenter, scale, V4(1, 1, 1, 0.5f));
	}
}

void DrawLevelBorder(LevelGrid* grid)
{
	for (int y = 0; y < grid->gridHeight + 1; y++)
	{
		if (y > 0 && y < grid->gridHeight) { continue; }
		vec2 rowLineCenter = grid->gridOrigin + V2((grid->gridSize.x * 0.5f), 0) + V2(0, -y * grid->tileSize);
		DrawRect(rowLineCenter, V2(grid->gridSize.x * 0.5f + (grid->lineThickness), grid->lineThickness), V4(1, 1, 1, 0.5f));
	}

	for (int x = 0; x < grid->gridWidth + 1; x++)
	{
		if (x > 0 && x < grid->gridWidth) { continue; }

		vec2 colLineCenter = grid->gridOrigin + V2(0, (-grid->gridSize.y * 0.5f)) + V2(x * grid->tileSize, 0);
		DrawRect(colLineCenter, V2(grid->lineThickness, grid->gridSize.y * 0.5f + (grid->lineThickness)), V4(1, 1, 1, 0.5f));
	}
}

void SetGridSize(LevelGrid* grid, float tileSize, uint32 newWidth, uint32 newHeight, vec2 gridOffset)
{
	grid->gridWidth = Clamp(newWidth, 1, 255);
	grid->gridHeight = Clamp(newHeight, 1, 255);

	grid->tileSize = tileSize;

	grid->lineThickness = 0.01f;

	grid->gridSize.x = grid->tileSize * grid->gridWidth;
	grid->gridSize.y = grid->tileSize * grid->gridHeight;

	grid->gridOrigin = gridOffset + V2(-grid->gridSize.x * 0.5f, grid->gridSize.y * 0.5f);
}

vec2i GetGridPosition(LevelGrid* grid)
{
	Camera* cam = &Game->camera;

	vec2 mousePos = Input->mousePosNormSigned;
	mousePos.x *= cam->width * 0.5f;
	mousePos.y *= cam->height * 0.5f;

	real32 xDist = mousePos.x - grid->gridOrigin.x;
	real32 yDist = grid->gridOrigin.y - mousePos.y;

	if (xDist < 0 || yDist < 0)
	{
		return V2i(-1,-1);
	}

	int32 xCoord = xDist / grid->tileSize;
	int32 yCoord = yDist / grid->tileSize;

	if (xCoord >= grid->gridWidth || yCoord >= grid->gridHeight)
	{
		return V2i(-1, -1);
	}

	return V2i(xCoord, yCoord);

}


vec2 GridToWorldPosition(LevelGrid* grid, vec2i gridPosition)
{
	vec2 worldPos = grid->gridOrigin;
	worldPos.x += (grid->tileSize * gridPosition.x) + (grid->tileSize * 0.5f);
	worldPos.y += (-grid->tileSize * gridPosition.y) + (-grid->tileSize * 0.5f);

	return worldPos;
}

void LevelTransition(vec2 ToLevelPosition, vec2 playerPosition)
{

}