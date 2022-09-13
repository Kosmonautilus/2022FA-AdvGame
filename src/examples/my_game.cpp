
enum EntityType
{
    EntityType_Count;
};

struct EntityHandle
{
    int32 id;
    int32 generation;

    EntityType type
}

struct EntityInfo
{
    EntityType type;
    int32 generation;
    int32 indexInBuffer;
};

struct EntityManager
{
    int32 count;
    int32 capacity;
    EntityInfo* entities;
};

EntityHandle AddEntity()
{
    EntityInfo newInfo = {}
}

void* GetEntity(EntityHandle handle)
{

}

struct GameData {

};

GameData *Data = NULL;

int freeList[20];
int freeListCount = 0;


void MyInit() {
    Game->gameData = malloc(sizeof(GameData));
    memset(Game->gameData, 0, sizeof(GameData));

    Data = (GameData *)Game->myData;
}

void MyGameUpdate() {
    ClearColor(RGB(0.0f, 0.0f, 0.0f));

}
