#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateRectangle(
        const std::string& name,
        glm::vec3 leftBottomCorner,
        float length,
        float width,
        glm::vec3 color,
        bool fill);

    Mesh* CreateCircle(
        const std::string& name,
        glm::vec3 center,
        float radius,
        glm::vec3 color,
        bool fill);

    Mesh* CreateTank(
        const std::string& name,
        glm::vec3 color1,
        glm::vec3 color2);

    Mesh* CreateCloud(
        const std::string& name,
        glm::vec3 corner,
        glm::vec3 color);
}
