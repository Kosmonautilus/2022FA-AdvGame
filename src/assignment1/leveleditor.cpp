#include "assignment1/levelmanager.cpp"

enum DrawMode
{
	DrawMode_Square,
	DrawMode_Line,
	DrawMode_Single,
	DrawMode_Delete
};

enum GameMode
{
	GameMode_Edit,
	GameMode_Test
};
enum EntityCategory
{
	EntityCategory_Terrain,
	EntityCategory_Normans,
	EntityCategory_Items,
	EntityCategory_Count
};

struct EditorButton
{
	bool isActive;

	Sprite buttonSprite;
	Sprite clickedSprite;
};

struct PortraitButton
{
	EntityType portraitEntity;

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

	EditorButton UI_SaveButton;
	EditorButton UI_LoadButton;
	EditorButton UI_ClearButton;
	EditorButton UI_PlayButton;

	PortraitButton UI_Portraits[11];

	PortraitButton UI_ActivePortraits[4];
	vec2i UI_PortraitPositions[4];

	Sprite UI_Number[10];
	Sprite UI_Hyphen;
	Sprite UI_EntityCategory[EntityCategory_Count];

	//Entity Management
	EntityManager entityManager;

	LevelGrid levelGrid;
};

MyData* editorData = {};

GameMode gameMode;

vec2i mousePosition;

bool onGrid;
vec2i gridPosition;

EntityType selectedEntityType;
vec2 currentLevel;
EntityCategory currentEntityCategory;


EntityHandle selectedEntities[1000];
int32 selectedEntitiesCount;

void InitializeUI()
{
	//Initialize sprites
	LoadSprite(&editorData->UI_Background, "data/ui/UI_PanelBackground.png");

	LoadSprite(&editorData->UI_UpButton[0].buttonSprite,   "data/ui/UI_ButtonUp.png");
	LoadSprite(&editorData->UI_UpButton[1].buttonSprite,   "data/ui/UI_ButtonUp.png");
	LoadSprite(&editorData->UI_DownButton[0].buttonSprite, "data/ui/UI_ButtonDown.png");
	LoadSprite(&editorData->UI_DownButton[1].buttonSprite, "data/ui/UI_ButtonDown.png");

	LoadSprite(&editorData->UI_LeftButton.buttonSprite,    "data/ui/UI_ButtonLeft.png");
	LoadSprite(&editorData->UI_RightButton.buttonSprite,   "data/ui/UI_ButtonRight.png");

	LoadSprite(&editorData->UI_SaveButton.buttonSprite,    "data/ui/UI_SaveLevelButton.png");
	LoadSprite(&editorData->UI_LoadButton.buttonSprite,    "data/ui/UI_LoadLevelButton.png");
	LoadSprite(&editorData->UI_ClearButton.buttonSprite,   "data/ui/UI_ClearLevelButton.png");

	LoadSprite(&editorData->UI_PlayButton.buttonSprite, "data/ui/UI_PlayButton.png");

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

	LoadSprite(&editorData->UI_EntityCategory[0], "data/ui/UI_Category_Terrain.png");
	LoadSprite(&editorData->UI_EntityCategory[1], "data/ui/UI_Category_Normans.png");
	LoadSprite(&editorData->UI_EntityCategory[2], "data/ui/UI_Category_Items.png");

  //LoadSprite(&editorData->UI_Portraits[0].activeSprite,  "data/ui/UI_Portrait_Wall.png");
  //LoadSprite(&editorData->UI_Portraits[1].activeSprite,  "data/ui/UI_Portrait_Water.png");
  //LoadSprite(&editorData->UI_Portraits[2].activeSprite,  "data/ui/UI_Portrait_Tree.png");
  //LoadSprite(&editorData->UI_Portraits[3].activeSprite,  "data/ui/UI_Portrait_Rock.png");
  
  //Initialize Normans
    LoadSprite(&editorData->UI_Portraits[4].activeSprite,  "data/ui/UI_Portrait_Footman.png");
	editorData->UI_Portraits[4].portraitEntity = EntityType_Footman;

	LoadSprite(&editorData->UI_Portraits[5].activeSprite,  "data/ui/UI_Portrait_Crossbowman.png");
	editorData->UI_Portraits[5].portraitEntity = EntityType_Crossbowman;

	LoadSprite(&editorData->UI_Portraits[6].activeSprite,  "data/ui/UI_Portrait_Knight.png");
	editorData->UI_Portraits[6].portraitEntity = EntityType_Knight;

  //LoadSprite(&editorData->UI_Portraits[7].activeSprite,  "data/ui/UI_Portrait_Wizard.png");
  //editorData->UI_Portraits[7].portraitEntity = EntityType_Wizard; 
  
  //LoadSprite(&editorData->UI_Portraits[8].activeSprite,  "data/ui/UI_Portrait_Chicken.png");
  //LoadSprite(&editorData->UI_Portraits[9].activeSprite,  "data/ui/UI_Portrait_Potion.png");
  //LoadSprite(&editorData->UI_Portraits[10].activeSprite, "data/ui/UI_Portrait_Coin.png");
  //LoadSprite(&editorData->UI_Portraits[11].activeSprite, "data/ui/UI_Portrait_Bomb.png");

	
	for (int i = 0; i < EntityType_Count; i++)
	{
		editorData->UI_Portraits[i].portraitEntity = (EntityType)i;
	}
	
}

void PlaceEntity(vec2 gridPosition, EntityType type)
{
	if (onGrid == false)
	{
		return;
	}
	else
	{
		EntityHandle entityHandle = AddEntity(&editorData->entityManager, selectedEntityType);
		Entity* e = (Entity*)GetEntity(&editorData->entityManager, entityHandle);
		e->position.x = gridPosition.x;
		e->position.y = gridPosition.y;
		
	}
}

void RemoveEntity(vec2 cursorPosition)
{

}

void LoadLevel(char* path)
{

}

void SaveLevel()
{

}

void NewLevel()
{

}

void UpdateCategory()
{
	if (currentEntityCategory == EntityCategory_Terrain)
	{
		editorData->UI_ActivePortraits[0] = editorData->UI_Portraits[0];
		editorData->UI_ActivePortraits[1] = editorData->UI_Portraits[1];
		editorData->UI_ActivePortraits[2] = editorData->UI_Portraits[2];
		editorData->UI_ActivePortraits[3] = editorData->UI_Portraits[3];
	}
	else if (currentEntityCategory == EntityCategory_Normans)
	{
		editorData->UI_ActivePortraits[0] = editorData->UI_Portraits[4];
		editorData->UI_ActivePortraits[1] = editorData->UI_Portraits[5];
		editorData->UI_ActivePortraits[2] = editorData->UI_Portraits[6];
		editorData->UI_ActivePortraits[3] = editorData->UI_Portraits[7];
	}
	else if (currentEntityCategory == EntityCategory_Items)
	{
		editorData->UI_ActivePortraits[0] = editorData->UI_Portraits[8];
		editorData->UI_ActivePortraits[1] = editorData->UI_Portraits[9];
		editorData->UI_ActivePortraits[2] = editorData->UI_Portraits[10];
		editorData->UI_ActivePortraits[3] = editorData->UI_Portraits[11];
	}
}

void LeftClickPositionCheck()
{
	if (!onGrid) //There is a DEFINITIVELY better way to do this but it would require restructuring how buttons work. 
		//Could have made an array of buttons and looped through.
	{
		/////////////////////////CATEGORY COUNTER///////////////////////////////////////////////////////////////////////////////
		if (mousePosition.x >= 48 && mousePosition.x <= 68 && mousePosition.y >= 748 && mousePosition.y <= 776)
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
		if (mousePosition.x >= 232 && mousePosition.x <= 252 && mousePosition.y >= 748 && mousePosition.y <= 776)
		{
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

			editorData->UI_RightButton.isActive = true;
			//move an index forward on editor category
		}

		/////////////////////////PORTRAITS//////////////////////////////////////////////////////////////////////////////////////
		if (mousePosition.x >= 44 && mousePosition.x <= 144 && mousePosition.y >= 632 && mousePosition.y <= 732)
		{
			editorData->UI_ActivePortraits[0].isActive = true;
			editorData->UI_ActivePortraits[1].isActive = false;
			editorData->UI_ActivePortraits[2].isActive = false;
			editorData->UI_ActivePortraits[3].isActive = false;
			//top left portrait
		}
		if (mousePosition.x >= 156 && mousePosition.x <= 256 && mousePosition.y >= 632 && mousePosition.y <= 732)
		{
			editorData->UI_ActivePortraits[0].isActive = false;
			editorData->UI_ActivePortraits[1].isActive = true;
			editorData->UI_ActivePortraits[2].isActive = false;
			editorData->UI_ActivePortraits[3].isActive = false;
			//top right portrait
		}
		if (mousePosition.x >= 44 && mousePosition.x <= 144 && mousePosition.y >= 516 && mousePosition.y <= 616)
		{
			editorData->UI_ActivePortraits[0].isActive = false;
			editorData->UI_ActivePortraits[1].isActive = false;
			editorData->UI_ActivePortraits[2].isActive = true;
			editorData->UI_ActivePortraits[3].isActive = false;
			//bot left portrait
		}
		if (mousePosition.x >= 156 && mousePosition.x <= 256 && mousePosition.y >= 516 && mousePosition.y <= 616)
		{
			editorData->UI_ActivePortraits[0].isActive = false;
			editorData->UI_ActivePortraits[1].isActive = false;
			editorData->UI_ActivePortraits[2].isActive = false;
			editorData->UI_ActivePortraits[3].isActive = true;
			//bot right portrait
		}

		if (mousePosition.x >= 108 && mousePosition.x <= 136 && mousePosition.y >= 288 && mousePosition.y <= 308)
		{
			if (currentLevel.x >= 9)
			{
				currentLevel.x = 1;
			}
			else
			{
				currentLevel.x++;
			}

		}
		if (mousePosition.x >= 164 && mousePosition.x <= 192 && mousePosition.y >= 288 && mousePosition.y <= 308)
		{
			if (currentLevel.y >= 9)
			{
				currentLevel.y = 1;
			}
			else
			{
				currentLevel.y++;
			}
		}
		if (mousePosition.x >= 108 && mousePosition.x <= 136 && mousePosition.y >= 180 && mousePosition.y <= 200)
		{
			if (currentLevel.x <= 1)
			{
				currentLevel.x = 9;
			}
			else
			{
				currentLevel.x--;
			}
		}
		if (mousePosition.x >= 164 && mousePosition.x <= 192 && mousePosition.y >= 180 && mousePosition.y <= 200)
		{
			if (currentLevel.y <= 1)
			{
				currentLevel.y = 9;
			}
			else
			{
				currentLevel.y--;
			}
		}


	}
	else if (onGrid && mousePosition.x > 300.0f)
	{
		//add entity at grid position
	}
}

void UpdateGridPosition()
{
	if (!onGrid)
	{
		gridPosition = V2i(0, 0);
	}
	else
	{
		gridPosition = GetGridPosition(&editorData->levelGrid, mousePosition);
	}
}

void InputLogic()
{
	//Get mouse position
	mousePosition = Input->mousePos;
	if (mousePosition.x < 300.0f && gameMode == GameMode_Edit)
	{
		onGrid = false;
	}
	else if (mousePosition.x >= 310.0f && mousePosition.x <= 1586 && mousePosition.y >= 0 && mousePosition.y <= 750)
	{
		onGrid = true;
	}
	else
	{
		onGrid = false;
	}

	if (InputPressed(Mouse, Input_MouseLeft))
	{
		LeftClickPositionCheck();
	}
	UpdateGridPosition();
}

void DrawUI()
{
	//SCALE IS IN 50(PIXEL?)
	//DrawSprite(V2(-6.5f,0), V2(1.5, 4.5), &editorData->UI_Background);

	DrawSpritePixel(V2i(150, 450), V2i(75, 225), &editorData->UI_Background);

	DrawSprite(V2(-6.5f, -4.08f), V2(0.74, 0.3),    &editorData->UI_PlayButton.buttonSprite);
	DrawSprite(V2(-6.84f, -3.3f), V2(0.24, 0.24),   &editorData->UI_SaveButton.buttonSprite);
	DrawSprite(V2(-6.16f, -3.3f), V2(0.24, 0.24),   &editorData->UI_LoadButton.buttonSprite);
	DrawSprite(V2(-5.52f, -3.3f), V2(0.2, 0.2),     &editorData->UI_ClearButton.buttonSprite);

	DrawSprite(V2(-6.78f, -2.6f), V2(0.14, 0.1),    &editorData->UI_DownButton[0].buttonSprite);
	DrawSprite(V2(-6.22f, -2.6f), V2(0.14, 0.1),    &editorData->UI_DownButton[1].buttonSprite);

	DrawSprite(V2(-6.78f, -1.52f), V2(0.14, 0.1),   &editorData->UI_UpButton[0].buttonSprite);
	DrawSprite(V2(-6.22f, -1.52f), V2(0.14, 0.1),   &editorData->UI_UpButton[1].buttonSprite);

	DrawSprite(V2(-7.42f, 3.12f), V2(0.1,0.14),     &editorData->UI_LeftButton.buttonSprite);
	DrawSprite(V2(-5.58f, 3.12f), V2(0.1, 0.14),    &editorData->UI_RightButton.buttonSprite);

	DrawSprite(V2(-6.78f, -2.06f), V2(0.1, 0.2),    &editorData->UI_Number[(uint8)currentLevel.x]);
	DrawSprite(V2(-6.22f, -2.06f), V2(0.1, 0.2),    &editorData->UI_Number[(uint8)currentLevel.y]);
	DrawSprite(V2(-6.5f, -2.06f), V2(0.1, 0.2),     &editorData->UI_Hyphen);


	DrawSprite(V2(-6.52f, 3.11f), V2(0.54f, 0.1f),  &editorData->UI_EntityCategory[currentEntityCategory]);
	
	DrawSpritePixel(editorData->UI_PortraitPositions[0], V2i(23, 23), &editorData->UI_ActivePortraits[0].activeSprite);
	DrawSpritePixel(editorData->UI_PortraitPositions[1], V2i(23, 23), &editorData->UI_ActivePortraits[1].activeSprite);
	DrawSpritePixel(editorData->UI_PortraitPositions[2], V2i(23, 23), &editorData->UI_ActivePortraits[2].activeSprite);
	DrawSpritePixel(editorData->UI_PortraitPositions[3], V2i(23, 23), &editorData->UI_ActivePortraits[3].activeSprite);

}



void LogicPhase()
{
	InputLogic();
}

void RenderPhase()
{
	DrawUI();

	DrawTextScreenPixel(&Game->monoFont, V2(320, 20), 10.0f, RGB(1, 1, 1), "On grid: %i", onGrid);
	DrawTextScreenPixel(&Game->monoFont, V2(320, 40), 10.0f, RGB(1, 1, 1), "Pixel position: (%i, %i)", mousePosition.x, mousePosition.y);
	DrawTextScreenPixel(&Game->monoFont, V2(320, 60), 10.0f, RGB(1, 1, 1), "Grid position: (%i, %i)", gridPosition.x, gridPosition.y);
}

void MyInit()
{
	Game->myData = malloc(sizeof(MyData));
	memset(Game->myData, 0, sizeof(MyData));

	editorData = (MyData*)Game->myData;

	SetGridSize(&editorData->levelGrid,0.75f, 17, 10, V2(1.5f,-0.75f)); //shifted right for now, in game the x axis will be centered.

	editorData->UI_PortraitPositions[0] = V2i(94, 682);
	editorData->UI_PortraitPositions[1] = V2i(206, 682);
	editorData->UI_PortraitPositions[2] = V2i(94, 566);
	editorData->UI_PortraitPositions[3] = V2i(206, 566);

	InitializeUI();

	currentLevel.x = 1;
	currentLevel.y = 1;
	currentEntityCategory = EntityCategory_Terrain;

	gameMode = GameMode_Edit;

	UpdateCategory();
}

void MyGameUpdate()
{
	ClearColor(RGB(0.0f, 0.0f, 0.0f));
	LogicPhase();
	RenderPhase();
	DrawLevelGrid(&editorData->levelGrid);
}