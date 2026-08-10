#include "frustum.h"
#include "glm/gtc/type_ptr.hpp"

void Frustum::extractFromMatrix(const glm::mat4 &projview)
{
    const float* m = glm::value_ptr(projview);

    // Left clipping plane
    planes[0].normal.x = m[3] + m[0];
    planes[0].normal.y = m[7] + m[4];
    planes[0].normal.z = m[11] + m[8];
    planes[0].distance = m[15] + m[12];

    // Right clipping plane
    planes[1].normal.x = m[3] - m[0];
    planes[1].normal.y = m[7] - m[4];
    planes[1].normal.z = m[11] - m[8];
    planes[1].distance = m[15] - m[12];

    // Bottom clipping plane
    planes[2].normal.x = m[3] + m[1];
    planes[2].normal.y = m[7] + m[5];
    planes[2].normal.z = m[11] + m[9];
    planes[2].distance = m[15] + m[13];

    // Top clipping plane
    planes[3].normal.x = m[3] - m[1];
    planes[3].normal.y = m[7] - m[5];
    planes[3].normal.z = m[11] - m[9];
    planes[3].distance = m[15] - m[13];

    // Near clipping plane
    planes[4].normal.x = m[3] + m[2];
    planes[4].normal.y = m[7] + m[6];
    planes[4].normal.z = m[11] + m[10];
    planes[4].distance = m[15] + m[14];

    // Far clipping plane
    planes[5].normal.x = m[3] - m[2];
    planes[5].normal.y = m[7] - m[6];
    planes[5].normal.z = m[11] - m[10];
    planes[5].distance = m[15] - m[14];

    // Нормализация всех плоскостей
    for (auto& plane : planes) {
        float len = glm::length(plane.normal);
        if (len > 0.0f) {
            plane.normal /= len;
            plane.distance /= len;
        }
    }
}

bool Frustum::isAABBVisible(const glm::vec3 &min, const glm::vec3 &max) const
{
    // Проверяем каждую плоскость фрустума
    for (const auto& plane : planes) {
        // Находим ближайшую к плоскости точку AABB
        glm::vec3 p = min;
        if (plane.normal.x >= 0.0f) p.x = max.x;
        if (plane.normal.y >= 0.0f) p.y = max.y;
        if (plane.normal.z >= 0.0f) p.z = max.z;

        // Если эта точка всё ещё за плоскостью, AABB не виден
        if (plane.signedDistance(p) < 0.0f) {
            return false;
        }
    }
    return true; // все тесты пройдены – AABB внутри фрустума
}
