#include "octree.h"
#include <glad/glad.h>
#include <cassert>
#include <cmath>
#include <iostream>

Octree::Octree()
{
    this->level = 8;
    this->parent = nullptr;
    this->pos = vec3(0,0,0);
    NullChilds();
}

Octree::Octree(Octree* parent, vec3 pos)
{
    this->level = parent->level - 1;
    this->parent = parent;
    this->pos = pos;
    NullChilds();
}

Octree::~Octree()
{
    //dtor
}

void Octree::NullChilds()
{
    for (size_t i = 0; i < 8; ++i)
        childs[i] = nullptr;
}

void Octree::InitChilds()
{
    assert(level >= 0);

    for (unsigned char octant = 0; octant < 8; ++octant) {
        vec3 child_pos;
        if (octant & 0b001)
            child_pos.x = pos.x + pow(2, level)/4.;
        else
            child_pos.x = pos.x - pow(2, level)/4.;
        if (octant & 0b010)
            child_pos.y = pos.y + pow(2, level)/4.;
        else
            child_pos.y = pos.y - pow(2, level)/4.;
        if (octant & 0b100)
            child_pos.z = pos.z + pow(2, level)/4.;
        else
            child_pos.z = pos.z - pow(2, level)/4.;

        childs[octant] = new Octree(this, child_pos);
    }
}

unsigned char Octree::GetOctant()
{
    return  (((int)floor(pos.x) >> level) & 0b1)       |
           ((((int)floor(pos.y) >> level) & 0b1) << 1) |
           ((((int)floor(pos.z) >> level) & 0b1) << 2);
}

Octree* Octree::GetNext(direction dir)
{
    if (dir == LEFT) {
        if ((GetOctant() & 0b001) == 0)
            return parent->GetNext(dir)->childs[GetOctant() | 0b001];
        else
            return parent->childs[GetOctant() & 0b110];
    }
    else if (dir == RIGHT) {
        if ((GetOctant() & 0b001) == 0)
            return parent->childs[GetOctant() | 0b001];
        else
            return parent->GetNext(dir)->childs[GetOctant() & 0b110];
    }
    else if (dir == DOWN) {
        if ((GetOctant() & 0b010) == 0)
            return parent->GetNext(dir)->childs[GetOctant() | 0b010];
        else
            return parent->childs[GetOctant() & 0b101];
    }
    else if (dir == UP) {
        if ((GetOctant() & 0b010) == 0)
            return parent->childs[GetOctant() | 0b010];
        else
            return parent->GetNext(dir)->childs[GetOctant() & 0b101];
    }
    else if (dir == BACK) {
        if ((GetOctant() & 0b100) == 0)
            return parent->GetNext(dir)->childs[GetOctant() | 0b100];
        else
            return parent->childs[GetOctant() & 0b011];
    }
    else if (dir == FRONT) {
        if ((GetOctant() & 0b100) == 0)
            return parent->childs[GetOctant() | 0b100];
        else
            return parent->GetNext(dir)->childs[GetOctant() & 0b011];
    }
}

Octree* Octree::Find(vec3 pos)
{
    if (level == 0) return this;

    unsigned char octant = 0;

    if (pos.x >= this->pos.x) octant |= X_PLUS;
    if (pos.y >= this->pos.y) octant |= Y_PLUS;
    if (pos.z >= this->pos.z) octant |= Z_PLUS;

    InitChilds();
    return childs[octant]->Find(pos);
}

void Octree::Draw()
{
    float top = pos.y + pow(2, level)/2.,
            bot = pos.y - pow(2, level)/2.,
            left = pos.x - pow(2, level)/2.,
            right = pos.x + pow(2, level)/2.,
            near = pos.z - pow(2, level)/2.,
            far = pos.z + pow(2, level)/2.;

    glLineWidth(1.0f);
    glColor3f(1,0,0);
    glBegin(GL_LINES);

        glVertex3f(left, bot, near);
        glVertex3f(right, bot, near);
            glVertex3f(left, top, near);
            glVertex3f(right, top, near);
                glVertex3f(left, bot, far);
                glVertex3f(right, bot, far);
                    glVertex3f(left, top, far);
                    glVertex3f(right, top, far);


        glVertex3f(left, bot, near);
        glVertex3f(left, bot, far);
            glVertex3f(right, bot, near);
            glVertex3f(right, bot, far);
                glVertex3f(left, top, near);
                glVertex3f(left, top, far);
                    glVertex3f(right, top, near);
                    glVertex3f(right, top, far);

        glVertex3f(left, bot, near);
        glVertex3f(left, top, near);
            glVertex3f(right, bot, near);
            glVertex3f(right, top, near);
                glVertex3f(left, bot, far);
                glVertex3f(left, top, far);
                    glVertex3f(right, bot, far);
                    glVertex3f(right, top, far);

    glEnd();
}
