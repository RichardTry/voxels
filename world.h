#ifndef WORLD_H
#define WORLD_H

#include "chunk.h"

class World
{
    private:
        unordered_map<Chunk> chunks;

    public:
        World();
        virtual ~World();
};

#endif // WORLD_H
