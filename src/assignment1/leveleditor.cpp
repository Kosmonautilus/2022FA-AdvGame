#include "assignment1/levelmanager.cpp"
#include "assignment1/collisiondetection.cpp"

enum DrawMode
{
	DrawMode_Select,
	DrawMode_Delete,
	DrawMode_Single,
	DrawMode_Multi
};

enum GameMode
{
	GameMode_Edit,
	GameMode_PlacePlayer,
	GameMode_Test
};

enum EntityCategory
{
	EntityCategory_Walls,
	EntityCategory_Floors,
	EntityCategory_Blockers,
	EntityCategory_Normans,
	EntityCategory_Items,
	EntityCategory_Count
};

struct EditorButton
{
	bool isActive;
	Rect position;

	Sprite buttonSprite;
	Sprite clickedSprite;

	float timeSincePress;
};

struct PortraitButton
{
	EntityType portraitEntity;
	Rect position;

	bool isActive;

	Sprite inactiveSprite;
	Sprite activeSprite;
};

struct MyData
{
	//Editor UI
	Sprite UI_Background;

	EditorButton UI_UpButton[2];
	EditorButton UI_DownButton[2];
	EditorButton UI_LeftButton;
	EditorButton UI_RightButton;

	EditorButton UI_DrawMode_SingleButton;
	EditorButton UI_DrawMode_LineButton;
	EditorButton UI_DrawMode_MultiButton;
	EditorButton UI_DrawMode_DeleteButton;

	EditorButton UI_DrawMode_SelectButton;

	EditorButton UI_SaveButton;
	EditorButton UI_LoadButton;
	EditorButton UI_ClearButton;
	EditorButton UI_PlayButton;

	PortraitButton UI_Portraits[EntityType_Count];

	PortraitButton UI_ActivePortraits[4];
	vec2 UI_PortraitPositions[4];

	Sprite UI_Number[10];
	Sprite UI_Hyphen;
	Sprite UI_EntityCategory[EntityCategory_Count];
	Sprite UI_SelectBox;

	Sprite staticEntitySprites[EntityType_Footman * 10];

	//Entity Management
	EntityManager entityManager;

	EntityType selectedEntityType;

	LevelGrid levelGrid;
};

MyData* editorData = {};

GameMode gameMode;
DrawMode drawMode;

vec2 mousePosition;

bool onGrid;
vec2i gridPosition;
vec2 gridWorldPosition;

vec2 placeEntityDirection;
EntityCategory currentEntityCategory;

vec2 currentLevel;


EntityHandle selectedEntities[1000];
int32 selectedEntitiesCount;



void LoadStaticEntitySprites()
{
	/*
	for (int i = 0; i < 10; i++)
	{
		LoadSprite(&editorData->entitySprites[0 + i],  "data/sprites/tiles/Wall_Rock_" + (i + 1) + ".png");
		LoadSprite(&editorData->entitySprites[10 + i], "data/sprites/tiles/Wall_Wood_" + (i + 1) + ".png");
		LoadSprite(&editorData->entitySprites[20 + i], "data/sprites/tiles/Wall_Cobble_" + (i + 1) + ".png");
		LoadSprite(&editorData->entitySprites[30 + i], "data/sprites/tiles/Wall_Bush_" + (i + 1) + ".png");

		LoadSprite(&editorData->entitySprites[40 + i], "data/sprites/tiles/Floor_Dirt_" + (i + 1) + ".png");
		LoadSprite(&editorData->entitySprites[50 + i], "data/sprites/tiles/Floor_Grass_" + (i + 1) + ".png");
		LoadSprite(&editorData->entitySprites[60 + i], "data/sprites/tiles/Floor_Cobble_" + (i + 1) + ".png");
		LoadSprite(&editorData->entitySprites[70 + i], "data/sprites/tiles/Floor_Wood_" + (i + 1) + ".png");

		//Need to figure this out. How to index items? Do we leave the index null?
		LoadSprite(&editorData->entitySprites[80 + i], "data/sprites/items/Item_" + (i + 1) + ".png");
		LoadSprite(&editorData->entitySprites[90 + i], "data/sprites/items/Item_" + (i + 1) + ".png");
		LoadSprite(&editorData->entitySprites[100 + i], "data/sprites/items/Item_" + (i + 1) + ".png");
		LoadSprite(&editorData->entitySprites[110 + i], "data/sprites/items/Item_" + (i + 1) + ".png");

		LoadSprite(&editorData->entitySprites[120 + i], "data/sprites/tiles/Floor_Water_" + (i + 1) + ".png");
		LoadSprite(&editorData->entitySprites[130 + i], "data/sprites/tiles/Object_Decor_" + (i + 1) + ".png");
		LoadSprite(&editorData->entitySprites[140 + i], "data/sprites/tiles/Object_Boulder_" + (i + 1) + ".png");
		LoadSprite(&editorData->entitySprites[150 + i], "data/sprites/tiles/Object_Door_" + (i + 1) + ".png");
	}
	*/


	LoadSprite(&editorData->staticEntitySprites[40], "data/sprites/tiles/floor/Floor_Dirt.png");
	LoadSprite(&editorData->staticEntitySprites[50], "data/sprites/tiles/floor/Floor_Grass.png");
	LoadSprite(&editorData->staticEntitySprites[60], "data/sprites/tiles/floor/Floor_Cobble.png");
	LoadSprite(&editorData->staticEntitySprites[70], "data/sprites/tiles/floor/Floor_Wood.png");

}

void InitializeUI()
{
	//Initialize sprites
	LoadSprite(&editorData->UI_Background, "data/ui/UI_PanelBackground.png");


	LoadSprite(&editorData->UI_LeftButton.buttonSprite, "data/ui/UI_ButtonLeft.png"); //CATEGORY LEFT
	LoadSprite(&editorData->UI_LeftButton.clickedSprite, "data/ui/UI_ButtonLeft_Active.png");
	editorData->UI_LeftButton.position.min = V2(-7.52f, 2.98f);
	editorData->UI_LeftButton.position.max = V2(-7.32f, 3.26f);

	LoadSprite(&editorData->UI_RightButton.buttonSprite, "data/ui/UI_ButtonRight.png"); //CATEGORY RIGHT
	LoadSprite(&editorData->UI_RightButton.clickedSprite, "data/ui/UI_ButtonRight_Active.png");
	editorData->UI_RightButton.position.min = V2(-5.68f, 2.98f);
	editorData->UI_RightButton.position.max = V2(-5.48f, 3.26f);


	LoadSprite(&editorData->UI_DrawMode_SelectButton.buttonSprite, "data/ui/UI_DrawMode_SelectButton.png"); //SELECT BUTTON
	LoadSprite(&editorData->UI_DrawMode_SelectButton.clickedSprite, "data/ui/UI_DrawMode_SelectButton_Active.png");
	editorData->UI_DrawMode_SelectButton.position.min = V2(-7.0f, -0.46f);
	editorData->UI_DrawMode_SelectButton.position.max = V2(-6.6f, -0.06f);

	LoadSprite(&editorData->UI_DrawMode_DeleteButton.buttonSprite, "data/ui/UI_DrawMode_DeleteButton.png"); //DELETE BUTTON
	LoadSprite(&editorData->UI_DrawMode_DeleteButton.clickedSprite, "data/ui/UI_DrawMode_DeleteButton_Active.png");
	editorData->UI_DrawMode_DeleteButton.position.min = V2(-6.4f, -0.46f);
	editorData->UI_DrawMode_DeleteButton.position.max = V2(-6.0f, -0.06f);

	LoadSprite(&editorData->UI_DrawMode_SingleButton.buttonSprite, "data/ui/UI_DrawMode_SingleButton.png"); //SINGLE DRAW BUTTON
	LoadSprite(&editorData->UI_DrawMode_SingleButton.clickedSprite, "data/ui/UI_DrawMode_SingleButton_Active.png");
	editorData->UI_DrawMode_SingleButton.position.min = V2(-7.0f, -1.06f);
	editorData->UI_DrawMode_SingleButton.position.max = V2(-6.6f, -0.66f);

	LoadSprite(&editorData->UI_DrawMode_MultiButton.buttonSprite, "data/ui/UI_DrawMode_MultiButton.png"); //MULTI DRAW BUTTON
	LoadSprite(&editorData->UI_DrawMode_MultiButton.clickedSprite, "data/ui/UI_DrawMode_MultiButton_Active.png");
	editorData->UI_DrawMode_MultiButton.position.min = V2(-6.4f, -1.06f);
	editorData->UI_DrawMode_MultiButton.position.max = V2(-6.0f, -0.66f);


	LoadSprite(&editorData->UI_UpButton[0].buttonSprite,   "data/ui/UI_ButtonUp.png"); //X UP
	LoadSprite(&editorData->UI_UpButton[0].clickedSprite, "data/ui/UI_ButtonUp_Active.png");
	editorData->UI_UpButton[0].position.min = V2(-6.92f, -1.62f);
	editorData->UI_UpButton[0].position.max = V2(-6.64f, -1.42f);

	LoadSprite(&editorData->UI_UpButton[1].buttonSprite,   "data/ui/UI_ButtonUp.png"); //Y UP
	LoadSprite(&editorData->UI_UpButton[1].clickedSprite, "data/ui/UI_ButtonUp_Active.png");
	editorData->UI_UpButton[1].position.min = V2(-6.36f, -1.62f);
	editorData->UI_UpButton[1].position.max = V2(-6.08f, -1.42f);


	LoadSprite(&editorData->UI_DownButton[0].buttonSprite, "data/ui/UI_ButtonDown.png"); //X DOWN
	LoadSprite(&editorData->UI_DownButton[0].clickedSprite, "data/ui/UI_ButtonDown_Active.png");
	editorData->UI_DownButton[0].position.min = V2(-6.92f,-2.7f);
	editorData->UI_DownButton[0].position.max = V2(-6.64f,-2.5f);

	LoadSprite(&editorData->UI_DownButton[1].buttonSprite, "data/ui/UI_ButtonDown.png"); //Y DOWN
	LoadSprite(&editorData->UI_DownButton[1].clickedSprite, "data/ui/UI_ButtonDown_Active.png");
	editorData->UI_DownButton[1].position.min = V2(-6.36f, -2.7f);
	editorData->UI_DownButton[1].position.max = V2(-6.08f, -2.5f);


	LoadSprite(&editorData->UI_SaveButton.buttonSprite,    "data/ui/UI_SaveLevelButton.png"); //SAVE 
	LoadSprite(&editorData->UI_SaveButton.clickedSprite, "data/ui/UI_SaveLevelButton_Active.png");
	editorData->UI_SaveButton.position.min = V2(-7.08f, -3.54f);
	editorData->UI_SaveButton.position.max = V2(-6.60f, -3.06f);

	LoadSprite(&editorData->UI_LoadButton.buttonSprite,    "data/ui/UI_LoadLevelButton.png"); //LOAD
	LoadSprite(&editorData->UI_LoadButton.clickedSprite, "data/ui/UI_LoadLevelButton_Active.png");
	editorData->UI_LoadButton.position.min = V2(-6.40f, -3.54f);
	editorData->UI_LoadButton.position.max = V2(-5.92f, -3.06f);

	LoadSprite(&editorData->UI_ClearButton.buttonSprite,   "data/ui/UI_ClearLevelButton.png"); //CLEAR
	LoadSprite(&editorData->UI_ClearButton.clickedSprite, "data/ui/UI_ClearLevelButton_Active.png");
	editorData->UI_ClearButton.position.min = V2(-5.72f, -3.50f);
	editorData->UI_ClearButton.position.max = V2(-5.32f, -3.10f);


	LoadSprite(&editorData->UI_PlayButton.buttonSprite, "data/ui/UI_PlayButton.png"); //PLAY BUTTON
	LoadSprite(&editorData->UI_PlayButton.clickedSprite, "data/ui/UI_PlayButton_Active.png");
	editorData->UI_PlayButton.position.min = V2(-7.24f, -4.38f);
	editorData->UI_PlayButton.position.max = V2(-5.76f, -3.78f);

	LoadSprite(&editorData->UI_Number[0], "data/ui/UI_Font_0.png");
	LoadSprite(&editorData->UI_Number[1], "data/ui/UI_Font_1.png");
	LoadSprite(&editorData->UI_Number[2], "data/ui/UI_Font_2.png");
	LoadSprite(&editorData->UI_Number[3], "data/ui/UI_Font_3.png");
	LoadSprite(&editorData->UI_Number[4], "data/ui/UI_Font_4.png");
	LoadSprite(&editorData->UI_Number[5], "data/ui/UI_Font_5.png");
	LoadSprite(&editorData->UI_Number[6], "data/ui/UI_Font_6.png");
	LoadSprite(&editorData->UI_Number[7], "data/ui/UI_Font_7.png");
	LoadSprite(&editorData->UI_Number[8], "data/ui/UI_Font_8.png");
	LoadSprite(&editorData->UI_Number[9], "data/ui/UI_Font_9.png");
	LoadSprite(&editorData->UI_Hyphen, "data/ui/UI_Font_Hyphen.png");

	LoadSprite(&editorData->UI_SelectBox, "data/ui/UI_SelectBox.png");

	LoadSprite(&editorData->UI_EntityCategory[0], "data/ui/UI_Category_Walls.png");
	LoadSprite(&editorData->UI_EntityCategory[1], "data/ui/UI_Category_Floors.png");
	LoadSprite(&editorData->UI_EntityCategory[2], "data/ui/UI_Category_Blockers.png");
	LoadSprite(&editorData->UI_EntityCategory[3], "data/ui/UI_Category_Normans.png");
	LoadSprite(&editorData->UI_EntityCategory[4], "data/ui/UI_Category_Items.png");

  //LoadSprite(&editorData->UI_Portraits[0].activeSprite,  "data/ui/UI_Portrait_Wall.png");
  //LoadSprite(&editorData->UI_Portraits[1].activeSprite,  "data/ui/UI_Portrait_Water.png");
  //LoadSprite(&editorData->UI_Portraits[2].activeSprite,  "data/ui/UI_Portrait_Tree.png");
  //LoadSprite(&editorData->UI_Portraits[3].activeSprite,  "data/ui/UI_Portrait_Rock.png");

	//Initialize floors

	LoadSprite(&editorData->UI_Portraits[4].activeSprite, "data/sprites/tiles/floor/Floor_Dirt.png");
	editorData->UI_Portraits[4].portraitEntity = EntityType_Floor_Dirt;
	LoadSprite(&editorData->UI_Portraits[5].activeSprite, "data/sprites/tiles/floor/Floor_Grass.png");
	editorData->UI_Portraits[5].portraitEntity = EntityType_Floor_Grass;
	LoadSprite(&editorData->UI_Portraits[6].activeSprite, "data/sprites/tiles/floor/Floor_Cobble.png");
	editorData->UI_Portraits[6].portraitEntity = EntityType_Floor_Cobble;
	LoadSprite(&editorData->UI_Portraits[7].activeSprite, "data/sprites/tiles/floor/Floor_Wood.png");
	editorData->UI_Portraits[7].portraitEntity = EntityType_Floor_Wood;
  
  //Initialize Normans
    LoadSprite(&editorData->UI_Portraits[16].activeSprite,  "data/ui/UI_Portrait_Footman.png");
	editorData->UI_Portraits[16].portraitEntity = EntityType_Footman;

	LoadSprite(&editorData->UI_Portraits[17].activeSprite,  "data/ui/UI_Portrait_Crossbowman.png");
	editorData->UI_Portraits[17].portraitEntity = EntityType_Crossbowman;

	LoadSprite(&editorData->UI_Portraits[18].activeSprite,  "data/ui/UI_Portrait_Knight.png");
	editorData->UI_Portraits[18].portraitEntity = EntityType_Knight;

    LoadSprite(&editorData->UI_Portraits[19].activeSprite,  "data/ui/UI_Portrait_Wizard.png");
    editorData->UI_Portraits[19].portraitEntity = EntityType_Wizard; 
  
  //LoadSprite(&editorData->UI_Portraits[8].activeSprite,  "data/ui/UI_Portrait_Chicken.png");
  //LoadSprite(&editorData->UI_Portraits[9].activeSprite,  "data/ui/UI_Portrait_Potion.png");
  //LoadSprite(&editorData->UI_Portraits[10].activeSprite, "data/ui/UI_Portrait_Coin.png");
  //LoadSprite(&editorData->UI_Portraits[11].activeSprite, "data/ui/UI_Portrait_Bomb.png");

	
	for (int i = 0; i < EntityType_Count; i++)
	{
		editorData->UI_Portraits[i].portraitEntity = (EntityType)i;
	}
	
}


void PlaceEntity(vec2 position, EntityManager* em, EntityType type)
{
	EntityTypeBuffer* buffer = &em->buffers[type];


	if (onGrid == false)
	{
		
	}
	else if (buffer->count >= buffer->capacity)
	{
		
	}
	else if (IsEntityTypeAtLocation(em, type, position) == true)
	{

	}
	else
	{
		EntityHandle entityHandle = AddEntity(&editorData->entityManager, type);
		Entity* e = (Entity*)GetEntity(&editorData->entityManager, entityHandle);
		e->position.x = position.x;
		e->position.y = position.y;
		e->sprite = editorData->staticEntitySprites[(int)type * 10];
	}
}


void LoadLevel(char* path)
{

}

void SaveLevel()
{

}

void UpdateCategory()
{
	if (currentEntityCategory == EntityCategory_Walls)
	{
		editorData->UI_ActivePortraits[0] = editorData->UI_Portraits[0];
		editorData->UI_ActivePortraits[1] = editorData->UI_Portraits[1];
		editorData->UI_ActivePortraits[2] = editorData->UI_Portraits[2];
		editorData->UI_ActivePortraits[3] = editorData->UI_Portraits[3];
	}
	else if (currentEntityCategory == EntityCategory_Floors)
	{
		editorData->UI_ActivePortraits[0] = editorData->UI_Portraits[4];
		editorData->UI_ActivePortraits[1] = editorData->UI_Portraits[5];
		editorData->UI_ActivePortraits[2] = editorData->UI_Portraits[6];
		editorData->UI_ActivePortraits[3] = editorData->UI_Portraits[7];
	}
	else if (currentEntityCategory == EntityCategory_Blockers)
	{
		editorData->UI_ActivePortraits[0] = editorData->UI_Portraits[8];
		editorData->UI_ActivePortraits[1] = editorData->UI_Portraits[9];
		editorData->UI_ActivePortraits[2] = editorData->UI_Portraits[10];
		editorData->UI_ActivePortraits[3] = editorData->UI_Portraits[11];
	}
	else if (currentEntityCategory == EntityCategory_Items)
	{
		editorData->UI_ActivePortraits[0] = editorData->UI_Portraits[12];
		editorData->UI_ActivePortraits[1] = editorData->UI_Portraits[13];
		editorData->UI_ActivePortraits[2] = editorData->UI_Portraits[14];
		editorData->UI_ActivePortraits[3] = editorData->UI_Portraits[15];
	}
	else if (currentEntityCategory == EntityCategory_Normans)
	{
		editorData->UI_ActivePortraits[0] = editorData->UI_Portraits[16];
		editorData->UI_ActivePortraits[1] = editorData->UI_Portraits[17];
		editorData->UI_ActivePortraits[2] = editorData->UI_Portraits[18];
		editorData->UI_ActivePortraits[3] = editorData->UI_Portraits[19];
	}
}

void LeftClickPositionCheck()
{
	if (!onGrid) //There is a DEFINITIVELY better way to do this but it would require restructuring how buttons work. 
		//Could have made an array of buttons and looped through.
	{
		/////////////////////////CATEGORY COUNTER///////////////////////////////////////////////////////////////////////////////
		if (RectPointTest(mousePosition, editorData->UI_LeftButton.position))
		{
			editorData->UI_LeftButton.isActive = true;

			if (currentEntityCategory > 0)
			{
				currentEntityCategory = (EntityCategory)(currentEntityCategory - 1);
				UpdateCategory();
			}
			else
			{
				currentEntityCategory = (EntityCategory)(EntityCategory_Count - 1);
				UpdateCategory();
			}
			//move an index back on editor category
		}
		if (RectPointTest(mousePosition, editorData->UI_RightButton.position))
		{
			editorData->UI_RightButton.isActive = true;
			if (currentEntityCategory < EntityCategory_Count - 1)
			{
				currentEntityCategory = (EntityCategory)(currentEntityCategory + 1);
				UpdateCategory();
			}
			else
			{
				currentEntityCategory = (EntityCategory)0;
				UpdateCategory();
			}

			//move an index forward on editor category
		}

		/////////////////////////PORTRAITS//////////////////////////////////////////////////////////////////////////////////////
		if (mousePosition.x >= -7.56f && mousePosition.x <= -6.56f && mousePosition.y >= 1.82f && mousePosition.y <= 2.82f)
		{
			editorData->UI_ActivePortraits[0].isActive = true;
			editorData->UI_ActivePortraits[1].isActive = false;
			editorData->UI_ActivePortraits[2].isActive = false;
			editorData->UI_ActivePortraits[3].isActive = false;

			editorData->selectedEntityType = editorData->UI_ActivePortraits[0].portraitEntity;
			//top left portrait
		}
		if (mousePosition.x >= -6.44f && mousePosition.x <= -5.44f && mousePosition.y >= 1.82f && mousePosition.y <= 2.82f)
		{
			editorData->UI_ActivePortraits[0].isActive = false;
			editorData->UI_ActivePortraits[1].isActive = true;
			editorData->UI_ActivePortraits[2].isActive = false;
			editorData->UI_ActivePortraits[3].isActive = false;

			editorData->selectedEntityType = editorData->UI_ActivePortraits[1].portraitEntity;
			//top right portrait
		}
		if (mousePosition.x >= -7.56f && mousePosition.x <= -6.56f && mousePosition.y >= 0.66f && mousePosition.y <= 1.66f)
		{
			editorData->UI_ActivePortraits[0].isActive = false;
			editorData->UI_ActivePortraits[1].isActive = false;
			editorData->UI_ActivePortraits[2].isActive = true;
			editorData->UI_ActivePortraits[3].isActive = false;

			editorData->selectedEntityType = editorData->UI_ActivePortraits[2].portraitEntity;
			//bot left portrait
		}
		if (mousePosition.x >= -6.44f && mousePosition.x <= -5.44f && mousePosition.y >= 0.66f && mousePosition.y <= 1.66f)
		{
			editorData->UI_ActivePortraits[0].isActive = false;
			editorData->UI_ActivePortraits[1].isActive = false;
			editorData->UI_ActivePortraits[2].isActive = false;
			editorData->UI_ActivePortraits[3].isActive = true;

			editorData->selectedEntityType = editorData->UI_ActivePortraits[3].portraitEntity;
			//bot right portrait
		}

		/////////////////////////LEVEL INCREMENTER//////////////////////////////////////////////////////////////////////////////

		if (RectPointTest(mousePosition, editorData->UI_UpButton[0].position)) //INCREMENT CURRENT LEVEL X UP
		{
			editorData->UI_UpButton[0].isActive = true;
			if (currentLevel.x >= 9)
			{
				currentLevel.x = 1;
			}
			else
			{
				currentLevel.x++;
			}

		}
		if (RectPointTest(mousePosition, editorData->UI_UpButton[1].position)) //INCREMENT CURRENT LEVEL Y UP
		{
			editorData->UI_UpButton[1].isActive = true;
			if (currentLevel.y >= 9)
			{
				currentLevel.y = 1;
			}
			else
			{
				currentLevel.y++;
			}
		}
		if (RectPointTest(mousePosition, editorData->UI_DownButton[0].position)) //INCREMENT CURRENT LEVEL X DOWN
		{
			editorData->UI_DownButton[0].isActive = true;
			if (currentLevel.x <= 1)
			{
				currentLevel.x = 9;
			}
			else
			{
				currentLevel.x--;
			}
		}
		if (RectPointTest(mousePosition, editorData->UI_DownButton[1].position)) //INCREMENT CURRENT LEVEL Y DOWN
		{
			editorData->UI_DownButton[1].isActive = true;
			if (currentLevel.y <= 1)
			{
				currentLevel.y = 9;
			}
			else
			{
				currentLevel.y--;
			}
		}

		/////////////////////////DRAW MODES////////////////////////////////////////////////////////////////////////////

		if (RectPointTest(mousePosition, editorData->UI_DrawMode_SelectButton.position))
		{
			drawMode = DrawMode_Select;
			editorData->UI_DrawMode_SelectButton.isActive = true;
			editorData->UI_DrawMode_DeleteButton.isActive = false;
			editorData->UI_DrawMode_SingleButton.isActive = false;
			editorData->UI_DrawMode_MultiButton.isActive = false;
		}
		if (RectPointTest(mousePosition, editorData->UI_DrawMode_DeleteButton.position))
		{
			drawMode = DrawMode_Delete;
			editorData->UI_DrawMode_SelectButton.isActive = false;
			editorData->UI_DrawMode_DeleteButton.isActive = true;
			editorData->UI_DrawMode_SingleButton.isActive = false;
			editorData->UI_DrawMode_MultiButton.isActive = false;

		}
		if (RectPointTest(mousePosition, editorData->UI_DrawMode_SingleButton.position))
		{
			drawMode = DrawMode_Single;
			editorData->UI_DrawMode_SelectButton.isActive = false;
			editorData->UI_DrawMode_DeleteButton.isActive = false;
			editorData->UI_DrawMode_SingleButton.isActive = true;
			editorData->UI_DrawMode_MultiButton.isActive = false;
		}
		if (RectPointTest(mousePosition, editorData->UI_DrawMode_MultiButton.position))
		{
			drawMode = DrawMode_Multi;
			editorData->UI_DrawMode_SelectButton.isActive = false;
			editorData->UI_DrawMode_DeleteButton.isActive = false;
			editorData->UI_DrawMode_SingleButton.isActive = false;
			editorData->UI_DrawMode_MultiButton.isActive = true;
		}

		/////////////////////////SAVE LOAD CLEAR BUTTONS///////////////////////////////////////////////////////////

		if (RectPointTest(mousePosition, editorData->UI_SaveButton.position))
		{
			editorData->UI_SaveButton.isActive = true;
		}

		if (RectPointTest(mousePosition, editorData->UI_LoadButton.position))
		{
			editorData->UI_LoadButton.isActive = true;
		}

		if (RectPointTest(mousePosition, editorData->UI_ClearButton.position))
		{
			editorData->UI_ClearButton.isActive = true;
			ClearAllBuffers(&editorData->entityManager);
		}

		if (RectPointTest(mousePosition, editorData->UI_PlayButton.position))
		{
			editorData->UI_PlayButton.isActive = true;
		}

	}
	else if (onGrid)
	{
		if (drawMode == DrawMode_Select)
		{

		}
		else if (drawMode == DrawMode_Delete)
		{
			RemoveEntityAtPosition(&editorData->entityManager, gridWorldPosition);
		}
		else if (drawMode == DrawMode_Single)
		{
			PlaceEntity(gridWorldPosition, &editorData->entityManager, editorData->selectedEntityType);
		}
		else if (drawMode == DrawMode_Multi)
		{

		}

	}
}

void UpdateGridPosition()
{
	if (!onGrid)
	{
		gridPosition = V2i(-999, -999);
		gridWorldPosition = V2(-999, -999);
	}
	else
	{
		gridPosition = GetGridPosition(&editorData->levelGrid);
		gridWorldPosition = GridToWorldPosition(&editorData->levelGrid, gridPosition);
	}
}

void InputLogic()
{
	//Get mouse position
	mousePosition.x = Input->mousePosNormSigned.x * 8;
	mousePosition.y = Input->mousePosNormSigned.y * 4.5f;
	if (mousePosition.x < -5.0f && gameMode == GameMode_Edit)
	{
		onGrid = false;
	}
	else if (mousePosition.x >= editorData->levelGrid.gridOrigin.x 
		  && mousePosition.x <= editorData->levelGrid.gridSize.x + editorData->levelGrid.gridOrigin.x
		  && mousePosition.y <= editorData->levelGrid.gridOrigin.y
		  && mousePosition.y >= editorData->levelGrid.gridOrigin.y - editorData->levelGrid.gridSize.y)
	{
		onGrid = true;
	}
	else
	{
		onGrid = false;
	}

	if (InputPressed(Keyboard, Input_G))
	{
		if (editorData->levelGrid.toggleGrid)
		{
			editorData->levelGrid.toggleGrid = false;
		}
		else
		{
			editorData->levelGrid.toggleGrid = true;
		}
	}

	if (InputPressed(Mouse, Input_MouseLeft))
	{
		LeftClickPositionCheck();
	}
	UpdateGridPosition();
}

void UIActiveButtons()
{
	if (editorData->UI_LeftButton.isActive && editorData->UI_LeftButton.timeSincePress < 0.1f)
	{
		DrawSprite(V2(-7.42f, 3.12f), V2(0.1, 0.14), &editorData->UI_LeftButton.clickedSprite);
		editorData->UI_LeftButton.timeSincePress += 1 * DeltaTime;
	}
	else
	{
		DrawSprite(V2(-7.42f, 3.12f), V2(0.1, 0.14), &editorData->UI_LeftButton.buttonSprite);
	}

	if (editorData->UI_RightButton.isActive && editorData->UI_RightButton.timeSincePress < 0.1f)
	{
		DrawSprite(V2(-5.58f, 3.12f), V2(0.1, 0.14), &editorData->UI_RightButton.clickedSprite);
		editorData->UI_RightButton.timeSincePress += 1 * DeltaTime;
	}
	else
	{
		DrawSprite(V2(-5.58f, 3.12f), V2(0.1, 0.14), &editorData->UI_RightButton.buttonSprite);
	}


	if (editorData->UI_DrawMode_SelectButton.isActive)
	{
		DrawSprite(V2(-6.8f, -0.26f), V2(0.2, 0.2), &editorData->UI_DrawMode_SelectButton.clickedSprite);
	}
	else
	{
		DrawSprite(V2(-6.8f, -0.26f), V2(0.2, 0.2), &editorData->UI_DrawMode_SelectButton.buttonSprite);
	}

	if (editorData->UI_DrawMode_DeleteButton.isActive)
	{
		DrawSprite(V2(-6.2f, -0.26f), V2(0.2, 0.2), &editorData->UI_DrawMode_DeleteButton.clickedSprite);
	}
	else
	{
		DrawSprite(V2(-6.2f, -0.26f), V2(0.2, 0.2), &editorData->UI_DrawMode_DeleteButton.buttonSprite);
	}

	if (editorData->UI_DrawMode_SingleButton.isActive)
	{
		DrawSprite(V2(-6.8f, -0.86f), V2(0.2, 0.2), &editorData->UI_DrawMode_SingleButton.clickedSprite);
	}
	else
	{
		DrawSprite(V2(-6.8f, -0.86f), V2(0.2, 0.2), &editorData->UI_DrawMode_SingleButton.buttonSprite);
	}

	if (editorData->UI_DrawMode_MultiButton.isActive)
	{
		DrawSprite(V2(-6.2f, -0.86f), V2(0.2, 0.2), &editorData->UI_DrawMode_MultiButton.clickedSprite);
	}
	else
	{
		DrawSprite(V2(-6.2f, -0.86f), V2(0.2, 0.2), &editorData->UI_DrawMode_MultiButton.buttonSprite);
	}

	if (editorData->UI_DownButton[0].isActive)
	{
		DrawSprite(V2(-6.78f, -2.6f), V2(0.14, 0.1), &editorData->UI_DownButton[0].clickedSprite);
		editorData->UI_DownButton[0].timeSincePress += 1 * DeltaTime;
	}
	else
	{
		DrawSprite(V2(-6.78f, -2.6f), V2(0.14, 0.1), &editorData->UI_DownButton[0].buttonSprite);
	}

	if (editorData->UI_DownButton[1].isActive)
	{
		DrawSprite(V2(-6.22f, -2.6f), V2(0.14, 0.1), &editorData->UI_DownButton[1].clickedSprite);
		editorData->UI_DownButton[1].timeSincePress += 1 * DeltaTime;
	}
	else
	{
		DrawSprite(V2(-6.22f, -2.6f), V2(0.14, 0.1), &editorData->UI_DownButton[1].buttonSprite);
	}


	if (editorData->UI_UpButton[0].isActive)
	{
		DrawSprite(V2(-6.78f, -1.52f), V2(0.14, 0.1), &editorData->UI_UpButton[0].clickedSprite);
		editorData->UI_UpButton[0].timeSincePress += 1 * DeltaTime;
	}
	else
	{
		DrawSprite(V2(-6.78f, -1.52f), V2(0.14, 0.1), &editorData->UI_UpButton[0].buttonSprite);
	}

	if (editorData->UI_UpButton[1].isActive)
	{
		DrawSprite(V2(-6.22f, -1.52f), V2(0.14, 0.1), &editorData->UI_UpButton[1].clickedSprite);
		editorData->UI_UpButton[1].timeSincePress += 1 * DeltaTime;
	}
	else
	{
		DrawSprite(V2(-6.22f, -1.52f), V2(0.14, 0.1), &editorData->UI_UpButton[1].buttonSprite);
	}


	if (editorData->UI_SaveButton.isActive)
	{
		DrawSprite(V2(-6.84f, -3.3f), V2(0.24, 0.24), &editorData->UI_SaveButton.clickedSprite);
		editorData->UI_SaveButton.timeSincePress += 1 * DeltaTime;
	}
	else
	{
		DrawSprite(V2(-6.84f, -3.3f), V2(0.24, 0.24), &editorData->UI_SaveButton.buttonSprite);
	}

	if (editorData->UI_LoadButton.isActive)
	{
		DrawSprite(V2(-6.16f, -3.3f), V2(0.24, 0.24), &editorData->UI_LoadButton.clickedSprite);
		editorData->UI_LoadButton.timeSincePress += 1 * DeltaTime;
	}
	else
	{
		DrawSprite(V2(-6.16f, -3.3f), V2(0.24, 0.24), &editorData->UI_LoadButton.buttonSprite);
	}

	if (editorData->UI_ClearButton.isActive)
	{
		DrawSprite(V2(-5.52f, -3.3f), V2(0.2, 0.2), &editorData->UI_ClearButton.clickedSprite);
		editorData->UI_ClearButton.timeSincePress += 1 * DeltaTime;
	}
	else
	{
		DrawSprite(V2(-5.52f, -3.3f), V2(0.2, 0.2), &editorData->UI_ClearButton.buttonSprite);
	}

	if (editorData->UI_PlayButton.isActive)
	{
		DrawSprite(V2(-6.5f, -4.08f), V2(0.74, 0.3), &editorData->UI_PlayButton.clickedSprite);
		editorData->UI_PlayButton.timeSincePress += 1 * DeltaTime;
	}
	else
	{
		DrawSprite(V2(-6.5f, -4.08f), V2(0.74, 0.3), &editorData->UI_PlayButton.buttonSprite);
	}
}

void DrawUI()
{
	//SCALE IS IN 50(PIXEL?)
	//DrawSprite(V2(-6.5f,0), V2(1.5, 4.5), &editorData->UI_Background);

	DrawSprite(V2(-6.5f, 0.0f), V2(1.5, 4.5), &editorData->UI_Background);

	DrawSprite(V2(-6.52f, 3.11f), V2(0.54f, 0.1f), &editorData->UI_EntityCategory[currentEntityCategory]);
	
	UIActiveButtons();

	/////////////////////////PORTRAIT BUTTONS/////////////////////////////////////////////////////////////////////////////

	if (editorData->UI_ActivePortraits[0].isActive)
	{
		DrawSprite(editorData->UI_PortraitPositions[0], V2(0.5f, 0.5f), &editorData->UI_ActivePortraits[0].activeSprite);
		DrawSprite(editorData->UI_PortraitPositions[0], V2(0.5f, 0.5f), &editorData->UI_SelectBox);
	}
	else
	{
		DrawSprite(editorData->UI_PortraitPositions[0], V2(0.5f, 0.5f), &editorData->UI_ActivePortraits[0].activeSprite);
	}

	if (editorData->UI_ActivePortraits[1].isActive)
	{
		DrawSprite(editorData->UI_PortraitPositions[1], V2(0.5f, 0.5f), &editorData->UI_ActivePortraits[1].activeSprite);
		DrawSprite(editorData->UI_PortraitPositions[1], V2(0.5f, 0.5f), &editorData->UI_SelectBox);
	}
	else
	{
		DrawSprite(editorData->UI_PortraitPositions[1], V2(0.5f, 0.5f), &editorData->UI_ActivePortraits[1].activeSprite);
	}


	if (editorData->UI_ActivePortraits[2].isActive)
	{
		DrawSprite(editorData->UI_PortraitPositions[2], V2(0.5f, 0.5f), &editorData->UI_ActivePortraits[2].activeSprite);
		DrawSprite(editorData->UI_PortraitPositions[2], V2(0.5f, 0.5f), &editorData->UI_SelectBox);
	}
	else
	{
		DrawSprite(editorData->UI_PortraitPositions[2], V2(0.5f, 0.5f), &editorData->UI_ActivePortraits[2].activeSprite);
	}


	if (editorData->UI_ActivePortraits[3].isActive)
	{
		DrawSprite(editorData->UI_PortraitPositions[3], V2(0.5f, 0.5f), &editorData->UI_ActivePortraits[3].activeSprite);
		DrawSprite(editorData->UI_PortraitPositions[3], V2(0.5f, 0.5f), &editorData->UI_SelectBox);
	}
	else
	{
		DrawSprite(editorData->UI_PortraitPositions[3], V2(0.5f, 0.5f), &editorData->UI_ActivePortraits[3].activeSprite);
	}

	/////////////////////////LEVEL INCREMENTER BUTTONS///////////////////////////////////////////////////////////////////////////

	if (editorData->UI_UpButton[0].isActive && editorData->UI_UpButton[0].timeSincePress >= 0.1f)
	{
		editorData->UI_UpButton[0].isActive = false;
		editorData->UI_UpButton[0].timeSincePress = 0.0f;
	}

	if (editorData->UI_UpButton[1].isActive && editorData->UI_UpButton[1].timeSincePress >= 0.1f)
	{
		editorData->UI_UpButton[1].isActive = false;
		editorData->UI_UpButton[1].timeSincePress = 0.0f;
	}

	if (editorData->UI_DownButton[0].isActive && editorData->UI_DownButton[0].timeSincePress >= 0.1f)
	{
		editorData->UI_DownButton[0].isActive = false;
		editorData->UI_DownButton[0].timeSincePress = 0.0f;
	}
	
	if (editorData->UI_DownButton[1].isActive && editorData->UI_DownButton[1].timeSincePress >= 0.1f)
	{
		editorData->UI_DownButton[1].isActive = false;
		editorData->UI_DownButton[1].timeSincePress = 0.0f;
	}

	DrawSprite(V2(-6.78f, -2.06f), V2(0.1, 0.2), &editorData->UI_Number[(uint8)currentLevel.x]);
	DrawSprite(V2(-6.22f, -2.06f), V2(0.1, 0.2), &editorData->UI_Number[(uint8)currentLevel.y]);
	DrawSprite(V2(-6.5f, -2.06f), V2(0.1, 0.2), &editorData->UI_Hyphen);


	/////////////////////////CATEGORY INCREMENTER BUTTONS///////////////////////////////////////////////////////////////////////////

	if (editorData->UI_LeftButton.isActive && editorData->UI_LeftButton.timeSincePress >= 0.1f)
	{
		editorData->UI_LeftButton.isActive = false;
		editorData->UI_LeftButton.timeSincePress = 0.0f;
	}

	if (editorData->UI_RightButton.isActive && editorData->UI_RightButton.timeSincePress >= 0.1f)
	{
		editorData->UI_RightButton.isActive = false;
		editorData->UI_RightButton.timeSincePress = 0.0f;
	}

	/////////////////////////SAVE LOAD CLEAR PLAY///////////////////////////////////////////////////////////////////////////

	if (editorData->UI_SaveButton.isActive && editorData->UI_SaveButton.timeSincePress >= 0.1f)
	{
		editorData->UI_SaveButton.isActive = false;
		editorData->UI_SaveButton.timeSincePress = 0.0f;
	}

	if (editorData->UI_LoadButton.isActive && editorData->UI_LoadButton.timeSincePress >= 0.1f)
	{
		editorData->UI_LoadButton.isActive = false;
		editorData->UI_LoadButton.timeSincePress = 0.0f;
	}

	if (editorData->UI_ClearButton.isActive && editorData->UI_ClearButton.timeSincePress >= 0.1f)
	{
		editorData->UI_ClearButton.isActive = false;
		editorData->UI_ClearButton.timeSincePress = 0.0f;
	}

	if (editorData->UI_PlayButton.isActive && editorData->UI_PlayButton.timeSincePress >= 0.1f)
	{
		editorData->UI_PlayButton.isActive = false;
		editorData->UI_PlayButton.timeSincePress = 0.0f;
	}
	
	/////////////////////////DEBUG TEXT///////////////////////////////////////////////////////////////////////////

	DrawTextScreen(&Game->monoFont, V2(0.19375f, 0.02f), 0.00625f, RGB(1, 1, 1), "On grid: %i", onGrid);
	DrawTextScreen(&Game->monoFont, V2(0.19375f, 0.04f), 0.00625f, RGB(1, 1, 1), "Grid position: (%i, %i)", gridPosition.x, gridPosition.y);
	DrawTextScreen(&Game->monoFont, V2(0.19375f, 0.06f), 0.00625f, RGB(1, 1, 1), "Mouse position: (%.3f, %.3f)", mousePosition.x, mousePosition.y);
	DrawTextScreen(&Game->monoFont, V2(0.19375f, 0.08f), 0.00625f, RGB(1, 1, 1), "Grid world position: (%.3f, %.3f)", gridWorldPosition.x, gridWorldPosition.y);
	DrawTextScreen(&Game->monoFont, V2(0.19375f, 0.08f), 0.00625f, RGB(1, 1, 1), "Grid world position: (%.3f, %.3f)", gridWorldPosition.x, gridWorldPosition.y);
	DrawTextScreen(&Game->monoFont, V2(0.5f, 0.02f), 0.00625f, RGB(1, 1, 1), "Press G to toggle grid.");
	DrawTextScreen(&Game->monoFont, V2(0.5f, 0.04f), 0.00625f, RGB(1, 1, 1), "Toggle grid state: %i", editorData->levelGrid.toggleGrid);

}

void DrawEntityType(EntityManager* em, EntityType type) //ONLY USE WITH ENTITIES THAT HAVE NO ANIMATION
{
	EntityTypeBuffer* buffer = &em->buffers[type];

	for (int i = 0; i < buffer->count; i++)
	{
		Entity* e = (Entity*)((u8*)buffer->entities + (buffer->entitySizeInBytes * i));

		DrawSprite(e->position, V2(0.5 * editorData->levelGrid.tileSize, 0.5 * editorData->levelGrid.tileSize), &e->sprite);
	}
}

void DrawStaticEntities()
{
	DrawEntityType(&editorData->entityManager, (EntityType)4);
	DrawEntityType(&editorData->entityManager, (EntityType)5);
	DrawEntityType(&editorData->entityManager, (EntityType)6);
	DrawEntityType(&editorData->entityManager, (EntityType)7);
}

void DrawLivingEntities()
{

}

void DrawSprites()
{
	DrawStaticEntities();
	DrawLivingEntities();
}

void LogicPhase()
{
	InputLogic();
	if (entityDeleteCount > 0)
	{
		DeleteEntities(&editorData->entityManager);
	}

}

void RenderPhase()
{
	DrawUI();
	DrawSprites();

	if (drawMode == DrawMode_Single)
	{
		DrawSprite(gridWorldPosition, V2(0.375f, 0.375f), 0.6f, &editorData->staticEntitySprites[(int)editorData->selectedEntityType * 10]);
	}

}

void MyInit()
{
	Game->myData = malloc(sizeof(MyData));
	memset(Game->myData, 0, sizeof(MyData));

	editorData = (MyData*)Game->myData;

	//INITIALIZE ENTITY MANAGER AND BUFFERS
	EntityManagerInit(&editorData->entityManager);

	InitializeEntityBuffer(&editorData->entityManager, EntityType_Wall_Rock,    170, sizeof(Entity));
	InitializeEntityBuffer(&editorData->entityManager, EntityType_Wall_Wood,    170, sizeof(Entity));
	InitializeEntityBuffer(&editorData->entityManager, EntityType_Wall_Cobble,  170, sizeof(Entity));
	InitializeEntityBuffer(&editorData->entityManager, EntityType_Wall_Bush,    170, sizeof(Entity));
	InitializeEntityBuffer(&editorData->entityManager, EntityType_Floor_Dirt,   170, sizeof(Entity));
	InitializeEntityBuffer(&editorData->entityManager, EntityType_Floor_Grass,  170, sizeof(Entity));
	InitializeEntityBuffer(&editorData->entityManager, EntityType_Floor_Cobble, 170, sizeof(Entity));
	InitializeEntityBuffer(&editorData->entityManager, EntityType_Floor_Wood,   170, sizeof(Entity));
	InitializeEntityBuffer(&editorData->entityManager, EntityType_Chicken,      170, sizeof(Entity));
	InitializeEntityBuffer(&editorData->entityManager, EntityType_Potion,       170, sizeof(Entity));
	InitializeEntityBuffer(&editorData->entityManager, EntityType_Bomb,         170, sizeof(Entity));
	InitializeEntityBuffer(&editorData->entityManager, EntityType_Key,          170, sizeof(Entity));
	InitializeEntityBuffer(&editorData->entityManager, EntityType_Water,        170, sizeof(Entity));
	InitializeEntityBuffer(&editorData->entityManager, EntityType_Decor,        170, sizeof(Entity));
	InitializeEntityBuffer(&editorData->entityManager, EntityType_Boulder,      170, sizeof(Boulder));
	InitializeEntityBuffer(&editorData->entityManager, EntityType_Door,         170, sizeof(Door));
	InitializeEntityBuffer(&editorData->entityManager, EntityType_Footman,      170, sizeof(LivingEntity));
	InitializeEntityBuffer(&editorData->entityManager, EntityType_Crossbowman,  170, sizeof(Crossbowman));
	InitializeEntityBuffer(&editorData->entityManager, EntityType_Knight,       170, sizeof(LivingEntity));
	InitializeEntityBuffer(&editorData->entityManager, EntityType_Wizard,       170, sizeof(Wizard));
	InitializeEntityBuffer(&editorData->entityManager, EntityType_Player,       1, sizeof(Player));

	SetGridSize(&editorData->levelGrid,0.75f, 17, 10, V2(1.5f,-0.75f));

	editorData->UI_PortraitPositions[0] = V2(-7.06f, 2.32f);
	editorData->UI_PortraitPositions[1] = V2(-5.94f, 2.32f);
	editorData->UI_PortraitPositions[2] = V2(-7.06f, 1.16f);
	editorData->UI_PortraitPositions[3] = V2(-5.94f, 1.16f);

	editorData->UI_DrawMode_SelectButton.isActive = true;

	editorData->levelGrid.toggleGrid = true;

	InitializeUI();
	LoadStaticEntitySprites();

	currentLevel.x = 1;
	currentLevel.y = 1;
	currentEntityCategory = EntityCategory_Walls;


	gameMode = GameMode_Edit;

	UpdateCategory();
}

void MyGameUpdate()
{
	ClearColor(RGB(0.0f, 0.0f, 0.0f));
	LogicPhase();
	RenderPhase();

	if (editorData->levelGrid.toggleGrid)
	{
		DrawLevelGrid(&editorData->levelGrid);
	}
	else
	{
		DrawLevelBorder(&editorData->levelGrid);
	}
}