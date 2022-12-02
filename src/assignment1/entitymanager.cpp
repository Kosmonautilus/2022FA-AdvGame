#include "assignment1/animation.cpp"

enum EntityType
{
    EntityType_Wall_Rock,      // Walls
    EntityType_Wall_Wood,
    EntityType_Wall_Cobble,
    EntityType_Wall_Bush, 
    EntityType_Floor_Dirt,     //Floors
    EntityType_Floor_Grass,
    EntityType_Floor_Cobble,
    EntityType_Floor_Wood, 
    EntityType_Chicken,        //Items
    EntityType_Potion,
    EntityType_Bomb,
    EntityType_Key,
    EntityType_Water,          //Blockers
    EntityType_Decor,
    EntityType_Boulder,
    EntityType_Door,
    EntityType_Footman,        //Normans
    EntityType_Crossbowman,
    EntityType_Knight,
    EntityType_Wizard,
    EntityType_Player,        //Not in Editor
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
    vec2 direction;

    Sprite sprite;

    EntityHandle handle;
};

struct Boulder : Entity
{
    bool destroyable;
    bool destroyed;
};

struct Door : Entity
{
    bool locked;
    bool closed;
};

struct EntityTypeBuffer
{
    int32 count;
    int32 entitySizeInBytes;
    int32 capacity;
    void* entities;
};

struct LivingEntity : Entity
{
    uint8 health;
    vec2 lastPosition;

    Animation animations[10];
};

struct Crossbowman : LivingEntity
{
    uint8 shotCooldown;
    uint8 ammo;
};

struct Wizard : LivingEntity
{
    uint8 shotCooldown;
    uint8 teleportTimer;
};

struct Player : LivingEntity
{
    uint8 maxHealth;
    uint8 bombCount;
    uint8 keyCount;
    uint8 potionCount;
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
   // if (entityFreeListCount > 0)
   // {
       // ID = entityFreeList[entityFreeListCount];
        //entityFreeListCount--;
   // }
   // else
    //{
       ID = em->nextID++;
    //}

    EntityInfo* info = &em->entities[ID];
    info->type = type;

    if (info->generation == 0)
    {
        info->generation++;
    }


    EntityTypeBuffer* buffer = &em->buffers[type];
    info->indexInBuffer = buffer->count;
    buffer->count++;

    Entity* e = (Entity*)((u8*)buffer->entities + (buffer->entitySizeInBytes * info->indexInBuffer));

    EntityHandle h = {};
    h.generation = info->generation;
    h.id = ID;
    h.type = type;

    e->handle = h;

    return h;
};

void DeleteEntities(EntityManager* em)
{
    for (int i = 0; i < entityDeleteCount; i++)
    {
        EntityInfo* info = &em->entities[entitiesToDelete[i]];
        if (info->markedForDeletion == true)
        {
            EntityTypeBuffer* buffer = &em->buffers[info->type];
            

            Entity* a = (Entity*)((u8*)buffer->entities + (buffer->entitySizeInBytes * info->indexInBuffer)); //initialize A
            Entity* b = (Entity*)((u8*)buffer->entities + (buffer->entitySizeInBytes * (buffer->count - 1))); //initialize B

            /*
            entityFreeList[entityFreeListCount] = entitiesToDelete[i];
            entityFreeListCount++;
            */

            EntityInfo* swappedInfo = &em->entities[b->handle.id]; //initialize C's handle

            memcpy(((u8*)buffer->entities + (buffer->entitySizeInBytes * info->indexInBuffer)), //swap data from B's location to A
                   ((u8*)buffer->entities + (buffer->entitySizeInBytes * (buffer->count - 1))),
                                             buffer->entitySizeInBytes);


            swappedInfo->indexInBuffer = info->indexInBuffer; // Set B's index to A's index

            buffer->count--; //subtract count of buffer by 1
            info->generation++; //NOTE: Do we put generation increment here ?
            info->markedForDeletion = false;
        }
    }
    entityDeleteCount = 0;
}

void MarkForDeletion(EntityManager* em, EntityHandle h)
{
    EntityInfo* info = GetEntityInfo(em, h);
    if (info == NULL)
    {
        return;
    }
    info->markedForDeletion = true;

    entitiesToDelete[entityDeleteCount] = h.id;
    entityDeleteCount++;
}

bool IsEntityTypeAtLocation(EntityManager* em, EntityType type, vec2 location)
{
    EntityTypeBuffer* buffer = &em->buffers[type];

    bool entityAtLocation = false;

    for (int i = 0; i < buffer->count; i++)
    {
        Entity* e = (Entity*)((u8*)buffer->entities + (buffer->entitySizeInBytes * i));

        if (e->position == location)
        {
            entityAtLocation = true;
            break;
        }
        else
        {
            continue;
        }
    }

    return entityAtLocation;
}

EntityHandle GetEntityAtPosition(EntityManager* em, vec2 location)
{
    for (int i = 0; i < EntityType_Count; i++)
    {
        EntityTypeBuffer* buffer = &em->buffers[i];

        for (int n = 0; n < buffer->count; n++)
        {
            Entity* posEntity = (Entity*)((u8*)buffer->entities + (buffer->entitySizeInBytes * n));

            if (posEntity->position == location)
            {
                return posEntity->handle;
            }
        }
    }
}


void RemoveEntityAtPosition(EntityManager* em, vec2 position)
{
    EntityHandle removeHandle = GetEntityAtPosition(em, position);
    EntityInfo* info = &em->entities[removeHandle.id];

    MarkForDeletion(em, removeHandle);
}