#include "assignment1/entitymanager.cpp"

//Create level tile map of 32 x 16 
//If player moves past coordinate boundary then they are transported to the next level in that direction.
//Correlates to a 2D vector of levels (8x8? Probably want to be extensible.)

//How do we save our levels? Do we want to read from a file or set it at runtime?
//Can we map entities based on their position rather than their index?

struct Level
{
	vec2 levelDimensions; //THIS SHOULD BE 32 x 16

};


void PlaceEntity(vec2 cursorPosition, EntityType type)
{

}

void RemoveEntity(vec2 cursorPosition)
{

}

void LoadLevel()
{

}

void SaveLevel()
{

}
