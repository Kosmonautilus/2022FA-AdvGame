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
	GameMode_Test,
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
};

MyData* editorData = {};

GameMode gameMode;

vec2i mousePosition;

bool onGrid;
vec2 gridPosition;

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
  // 
  //LoadSprite(&editorData->UI_Portraits[4].activeSprite,  "data/ui/UI_Portrait_Footman.png");
	LoadSprite(&editorData->UI_Portraits[5].activeSprite,  "data/ui/UI_Portrait_Crossbowman.png");
	LoadSprite(&editorData->UI_Portraits[6].activeSprite,  "data/ui/UI_Portrait_Knight.png");
  //LoadSprite(&editorData->UI_Portraits[7].activeSprite,  "data/ui/UI_Portrait_Wizard.png");
  // 
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

void ClickPositionCheck()
{
	if (!onGrid) //There is a better way to do this but it would require restructuring how buttons work. Could have made an array of buttons and looped through.
	{
		//category incrementer
		if (mousePosition.x >= 48 && mousePosition.x <= 68 && mousePosition.y >= 748 && mousePosition.y <= 776)
		{
			editorData->UI_LeftButton.isActive = true;
			//move an index back on editor category
		}
		if (mousePosition.x >= 232 && mousePosition.x <= 252 && mousePosition.y >= 748 && mousePosition.y <= 776)
		{
			editorData->UI_RightButton.isActive = true;
			//move an index forward on editor category
		}


	}
	/*
	if (mousePosition.x >= && mousePosition.x <= && mousePosition.y >= && mousePosition.y <= )
	{

	}
	*/
}

void InputLogic()
{
	//Get mouse position
	mousePosition = Input->mousePos;
	if (mousePosition.x <= 300.0f && gameMode == GameMode_Edit)
	{
		onGrid = false;
	}
	else
	{
		onGrid = true;
	}

	if (InputPressed(Mouse, Input_MouseLeft) || (InputReleased(Mouse, Input_MouseLeft)))
	{
		ClickPositionCheck();
	}
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

void DrawUI()
{
	//SCALE IS IN 50(PIXEL?)
	//DrawSprite(V2(-6.5f,0), V2(1.5, 4.5), &editorData->UI_Background);

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

	DrawSpritePixel(V2i(150, 450), V2(75, 225), &editorData->UI_Background);

	switch (currentEntityCategory)
	{
	case EntityCategory_Terrain:
		break;
	case EntityCategory_Normans:
		DrawSprite(V2(-7.06f, 2.32f), V2(0.46f, 0.46f), &editorData->UI_Portraits[7].activeSprite);
		break;
	case EntityCategory_Items:
		break;
	};

	/*
	for (int i = 0; i < 4; i++)
	{
		DrawSprite(V2(-7.06f, 2.32f), V2(0.46f, 0.46f), &editorData->UI_ActivePortraits[i].activeSprite);
	}
	*/
	

}



void LogicPhase()
{
	InputLogic();
}

void RenderPhase()
{
	DrawUI();
}

void MyInit()
{
	Game->myData = malloc(sizeof(MyData));
	memset(Game->myData, 0, sizeof(MyData));

	editorData = (MyData*)Game->myData;

	currentLevel.x = 8;
	currentLevel.y = 4;
	currentEntityCategory = EntityCategory_Normans;

	gameMode = GameMode_Edit;

	editorData->UI_PortraitPositions[0] = V2i(0, 0);
	editorData->UI_PortraitPositions[1] = V2i(1, 0);
	editorData->UI_PortraitPositions[2] = V2i(0, 1);
	editorData->UI_PortraitPositions[3] = V2i(1, 1);


	InitializeUI();
}

void MyGameUpdate()
{
	ClearColor(RGB(0.0f, 0.0f, 0.0f));
	LogicPhase();
	RenderPhase();
}