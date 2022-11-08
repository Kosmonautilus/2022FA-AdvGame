#include "assignment1/entitymanager.cpp"

//Footman
/*
	move in a straight line, when blocked by terrain move in the opposite direction 
*/
//Knight
/*
		move in a straight line, when blocked turn in a 90 degree angle to the right and keep moving until blocked again.
		Cannot be attacked from the front.
*/
//Wizard
/*

teleports every 6 seconds to a random position away from the player, fires a shot at them. Will also teleport when hit.

*/
//Crossbowman
/*
Fires a shot every 3 seconds towards the player if parallel with the player.

*/
//Trapper
/*
	Places a bear trap every 10 seconds, if bear trap is hit will attempt to attack the player, otherwise run away
*/


EnemyMove(EntityManager* em, EntityHandle h, EntityType enemyType)
{
	switch (enemyType)
	{
	case EntityType_Footman:
		Footman* f = (Footman*)GetEntity(&em, h);
		//Check collision, otherwise move
		if () //if blocked
		{
			f->direction *= -1;
			break;
		}
		else
		{
			if (f->direction == V2(0, 1))
			{
				f->position.x += 1 * deltaTime;
				//move right
				break;
			}
			if (f->direction == V2(0, -1))
			{
				f->position.x -= 1 * deltaTime;
				//move left
				break;
			}
			if (f->direction == V2(1,0))
			{
				f->position.y += 1 * deltaTime;
				//move down
				break;
			}
			if (f->direction == V2(-1,0))
			{
				f->position.y -= 1 * deltaTime;
				//move up
				break;
			}
		}
		break;
	case EntityType_Knight:
		break;
	case EntityType_Wizard:
		break;
	}
}
EnemyAttack(EntityHandle h, EntityType enemyType)
{
	switch (enemyType)
	{
	case EntityType_Footman:
		break;
	case EntityType_Crossbowman:
		break;
	case EntityType_Knight:
		break;
	case EntityType_Wizard:
		break;
	}
}

EnemyHitCheck(EntityManager* em, EntityHandle h, EntityType enemyType, vec2 attackDirection, int8 damage)
{
	switch (enemyType)
	{
	case EntityType_Footman:
		//take damage
		break;
	case EntityType_Crossbowman:
		//take damage
		break;
	case EntityType_Knight:
		Knight* k = (Knight*)GetEntity(&em, h);

		if ((k->direction.x == (attackDirection.x * -1)) && (k->direction.y == (attackDirection.y * -1))
		{
			//Attack blocked, play effect?
			break;
		}
		else
		{
			//Take damage
			k->health - damage;
		}
		break;
	case EntityType_Wizard:
		//take damage
		//teleport!
		break;
	}
}
