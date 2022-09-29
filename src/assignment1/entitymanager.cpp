enum EntityType
{
    EntityType_Player,
    EntityType_Enemy,
    EntityType_Count,
};

struct EntityInfo
{
    EntityType type;
    int32 generation;
    int32 indexInBuffer;

    bool markedForDeletion;
};

struct EntityHandle
{
    int32 id;
    int32 generation;

    EntityType type;
};

struct Entity
{
    vec2 position;
    vec2 lastPosition;
    vec2 scale;
    vec2 rotation;

};

struct Player
{
    Entity entity;
    uint8 lives;
    vec4 color;
};

struct Enemy
{
    Entity entity;
    int8 direction;
};

struct Bullet
{
    Entity entity;
    int8 speed;
    real32 lifeTime;
};

struct EntityTypeBuffer
{
    int32 count;
    int32 entitySizeInBytes;
    int32 capacity;
    void* entities;
};

struct EntityManager
{
    int32 entityCount;
    int32 entityCapacity;
    int32 nextID;
    EntityInfo* entities;

    EntityTypeBuffer buffers[EntityType_Count];
};

void EntityManagerInit(EntityManager* em)
{
    em->entityCount = 0;
    em->entityCapacity = 1000;
    em->entities = (EntityInfo*)malloc(sizeof(EntityInfo) * em->entityCapacity);
    memset(em->entities, 0, sizeof(EntityInfo) * em->entityCapacity);

    em->nextID = 0;

    EntityTypeBuffer* playerBuffer = &em->buffers[EntityType_Player]; //PLAYER BUFFER || NOTE: Do we need this? There will only ever be ONE player.
    playerBuffer->entitySizeInBytes = sizeof(Player);
    playerBuffer->capacity = 4;
    playerBuffer->count = 0;
    playerBuffer->entities = malloc(playerBuffer->entitySizeInBytes * playerBuffer->capacity);

    EntityTypeBuffer* enemyBuffer = &em->buffers[EntityType_Enemy]; //ENEMY BUFFER
    enemyBuffer->entitySizeInBytes = sizeof(Enemy);
    enemyBuffer->capacity = 20;
    enemyBuffer->count = 0;
    enemyBuffer->entities = malloc(enemyBuffer->entitySizeInBytes * enemyBuffer->capacity);

}

EntityInfo* GetEntityInfo(EntityManager* em, EntityHandle h)
{
    if (h.id >= em->entityCapacity) //Check if entity ID is over entity capacity, if true return null info
    {
        return NULL;
    }

    EntityInfo* info = &em->entities[h.id]; //Create EntityInfo pointer equal to the passed-in EntityHandle's ID.

    if (info->generation != h.generation) //If the info generation is not the same, return null
    {
        return NULL;
    }

    if (info->type != h.type) //If the type is not the same, return null
    {
        return NULL;
    }

    return info; //Otherwise return the EntityInfo
}

void* GetEntity(EntityManager* em, EntityHandle h)
{
    EntityInfo* info = GetEntityInfo(em, h); //Get entity info 
    if (info == NULL)
    {
        return NULL;
    } //If entity info is not found/null return NULL

    EntityTypeBuffer* buffer = &em->buffers[info->type]; //set entity type buffer to whatever the entity info's type is

    return ((u8*)buffer->entities + (buffer->entitySizeInBytes * info->indexInBuffer)); //get first 256 bytes of memory, increment by index * size in bytes to return memory address)

}

EntityHandle AddEntity(EntityManager* em, EntityType type) //We need to increment the generation based on the index and incorporate a free lsit
{
    EntityHandle h = {};

    EntityInfo* info = &em->entities[em->nextID];
    info->type = type;

    EntityTypeBuffer* buffer = &em->buffers[type];
    info->indexInBuffer = buffer->count;
    buffer->count++;
    em->entityCount++;


    h.id = em->nextID;
    h.generation = info->generation;

    em->nextID++;

    return h;
};

void DeleteEntities(EntityManager* em) //Adjust for freelist, this uses basic removal by swap.
{
    for (int i = 0; i < em->entityCount; i++)
    {
        EntityInfo* info = &em->entities[i];
        if (info->markedForDeletion == true)
        {
            EntityTypeBuffer* buffer = &em->buffers[info->type];
            
            memcpy((u8*)buffer->entities + sizeof(buffer->entitySizeInBytes) * (info->indexInBuffer), 
                (u8*)buffer->entities + sizeof(buffer->entitySizeInBytes) * (buffer->count - 1),
                                       sizeof(buffer->entitySizeInBytes));
            buffer->count--;
            em->entityCount--;
            info->generation++; //NOTE: Do we put generation increment here ?
        }
    }
}

void* MarkForDeletion(EntityManager* em, EntityHandle h)
{
    EntityInfo* info = GetEntityInfo(em, h);
    if (info == NULL)
    {
        return NULL;
    }
    EntityTypeBuffer* buffer = &em->buffers[info->type];

    info->markedForDeletion = true;

    return info;
}