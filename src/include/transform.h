#pragma once
#include "vec.h"

class Transform {
    private:
        Vec3 _position;

    public:
        Transform(float x, float y, float z);
        Transform();

        void setPosition(float x, float y, float z);
        void setPosition(Vec3* position);
        Vec3* getPosition();
};