#include "assignment1/entitymanager.cpp"

struct GameData {
    EntityManager* entityManager;

    //Sprites
    Sprite playerSprite;
    Sprite enemySprite;
    Sprite bulletSprite;
};

GameData *Data = NULL;
EntityManager* EManager = NULL;


void LoadGameSprites()
{
    //This is hardcoded, probably better to rework it in the future through a SpriteData struct with a string path and Sprite name declaration.
    //Get a for loop of sprites in SpriteData array, iterate over and declare each.
    LoadSprite(&Data->playerSprite, "");
    LoadSprite(&Data->enemySprite, "");
    LoadSprite(&Data->bulletSprite, "");
}

void PlayerInput(Player* p)
{
    vec2 P_Direction = V2(0, 0);

    if (InputHeld(Input, Input_Up))
    {
        P_Direction.y = 1;
    }
    if (InputHeld(Input, Input_Down))
    {
        P_Direction.y = -1;
    }
    if (InputHeld(Input, Input_Left))
    {
        P_Direction.x = -1;
    }
    if (InputHeld(Input, Input_Right))
    {
        P_Direction.x = 1;
    }

}

void MoveEntities()
{

}

void CollisionDetection()
{

}

void RenderGame()
{
    //Input rendering code here.
}

void MyInit() {
    //Initialize game data
    Game->myData = malloc(sizeof(GameData));
    memset(Game->myData, 0, sizeof(GameData));

    Data = (GameData *)Game->myData;

    EManager = (EntityManager*)Data->entityManager;

    EntityManagerInit(EManager);

    //Load sprites
    LoadGameSprites();
}

void MyGameUpdate() {
    ClearColor(RGB(0.0f, 0.0f, 0.0f));
    
    //PlayerInput();
    //CollisionDetection(); ||NOTE: This could be combined with or called by MoveEntities. 
    //MoveEntities();
    //DeleteEntities(EManager); ||NOTE: Keep this just after all logic calls are made? We don't want to end up with null references JIC.
    //RenderGame();
}
