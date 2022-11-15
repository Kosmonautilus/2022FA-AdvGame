#include "assignment1/entitymanager.cpp"
#include "assignment1/filereadwrite.cpp"

//Create level tile map of 32 x 16 
//If player moves past coordinate boundary then they are transported to the next level in that direction.
//Correlates to a 2D vector of levels (8x8? Probably want to be extensible.)

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

void LevelTransition(vec2 ToLevelPosition, vec2 playerPosition)
{

}