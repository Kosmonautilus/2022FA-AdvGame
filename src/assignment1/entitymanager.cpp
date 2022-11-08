enum EntityType
{
    EntityType_Wall,
    EntityType_Water,
    EntityType_Tree,
    EntityType_Player,
    EntityType_Footman,
    EntityType_Crossbowman,
    EntityType_Knight,
    EntityType_Wizard,
    EntityType_Count
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
    vec2 direction;

    EntityHandle handle;
};

struct EntityTypeBuffer
{
    int32 count;
    int32 entitySizeInBytes;
    int32 capacity;
    void* entities;
};

struct Footman : Entity
{
    int8 health;
};

struct Crossbowman : Entity
{
    int8 health;
    uint8 ammo;
};

struct Knight : Entity
{
    int8 health;
};

struct Wizard : Entity
{
    int8 health;
    uint8 teleportTimer;
};

struct EntityManager
{
    int32 entityCapacity;
    int32 nextID;
    EntityInfo* entities;

    EntityTypeBuffer buffers[EntityType_Count];
};

int32 entityFreeList[1000];
int32 entityFreeListCount = 0;

int32 entitiesToDelete[1000];
int32 entityDeleteCount = 0;


void EntityManagerInit(EntityManager* em)
{
    em->entityCapacity = 1000;
    em->entities = (EntityInfo*)malloc(sizeof(EntityInfo) * em->entityCapacity);
    memset(em->entities, 0, sizeof(EntityInfo) * em->entityCapacity);

    em->nextID = 0;
}

void InitializeEntityBuffer(EntityManager* em, EntityType bufferType, int32 entityCap, size_t sizeInBytes)
{
    EntityTypeBuffer* buffer = &em->buffers[bufferType];

    buffer->entitySizeInBytes = sizeInBytes;
    buffer->capacity = entityCap;
    buffer->count = 0;
    buffer->entities = malloc(buffer->entitySizeInBytes * buffer->capacity);
}

void ClearEntityBuffer(EntityTypeBuffer* buffer)
{
    buffer->count = 0;
}

void ClearAllBuffers(EntityManager* em)
{
    for (int i = 0; i < EntityType_Count; i++)
    {
        em->buffers[i].count = 0;
    }
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

EntityHandle AddEntity(EntityManager* em, EntityType type) //We need to increment the generation based on the index and incorporate a free list
{
    int32 ID = em->nextID;
    if (entityFreeListCount > 0)
    {
        ID = entityFreeList[entityFreeListCount];
        entityFreeListCount--;
    }
    else
    {
       ID = em->nextID++;
    }

    EntityInfo* info = &em->entities[ID];
    info->type = type;

    if (info->generation == 0)
    {
        info->generation++;
    }


    EntityTypeBuffer* buffer = &em->buffers[type];
    info->indexInBuffer = buffer->count;
    buffer->count++;

    Entity* e = (Entity*)&buffer[info->indexInBuffer];

    EntityHandle h = {};
    h.generation = info->generation;
    h.id = ID;
    h.type = type;

    e->handle = h;

    return h;
};

void DeleteEntities(EntityManager* em) //Adjust for freelist, this uses basic removal by swap.
{
    for (int i = 0; i < entityDeleteCount; i++)
    {
        EntityInfo* info = &em->entities[entitiesToDelete[i]];
        if (info->markedForDeletion == true)
        {
            EntityTypeBuffer* buffer = &em->buffers[info->type];
            
            entityFreeList[entityFreeListCount] = info->indexInBuffer;
            entityFreeListCount++;
            
            memcpy((u8*)buffer->entities + sizeof(buffer->entitySizeInBytes) * (info->indexInBuffer), 
                   (u8*)buffer->entities + sizeof(buffer->entitySizeInBytes) * (buffer->count - 1),
                                           sizeof(buffer->entitySizeInBytes));

            Entity* e = (Entity*)&buffer[buffer->count - 1];
            EntityInfo* swappedInfo = &em->entities[e->handle.id];
            swappedInfo->indexInBuffer = info->indexInBuffer;

            buffer->count--;
            info->generation++; //NOTE: Do we put generation increment here ?

        }
    }
    entityDeleteCount = 0;
}

void* MarkForDeletion(EntityManager* em, EntityHandle h)
{
    EntityInfo* info = GetEntityInfo(em, h);
    if (info == NULL)
    {
        return NULL;
    }
    info->markedForDeletion = true;

    entitiesToDelete[entityDeleteCount] = h.id;
    entityDeleteCount++;

    return info;
}