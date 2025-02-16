#pragma once

class Transform {
    private:
        float _position[3];

    public:
        Transform(float x, float y, float z);
        Transform();

        void setPosition(float x, float y, float z);
        void setPosition(Transform* transform);
        float* getPosition();
};