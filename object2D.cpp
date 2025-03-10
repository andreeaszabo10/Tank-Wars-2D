#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquare(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    float width,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, width, 0), color),
        VertexFormat(corner + glm::vec3(0, width, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateCircle(
    const std::string& name,
    glm::vec3 center,
    float radius,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    vertices.emplace_back(center, color);

    int numSegments = 50;
    for (int i = 0; i <= numSegments; i++) {
        float angle = 2 * i * M_PI / numSegments;
        vertices.emplace_back(center + glm::vec3(cos(angle) * radius, sin(angle) * radius, 0), color);
    }

    for (int i = 1; i <= numSegments; i++) {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    Mesh* circle = new Mesh(name);
    circle->SetDrawMode(GL_TRIANGLE_FAN);
    circle->InitFromData(vertices, indices);

    return circle;
}

Mesh* object2D::CreateCloud(
    const std::string& name,
    glm::vec3 corner,
    glm::vec3 color) {


    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    int indexOffset = 0;
    Mesh* circle = CreateCircle("circle", glm::vec3(25, 25, 0), 27, color, true);
    for (const auto& vertex : circle->vertices) {
        vertices.push_back(vertex);
    }
    for (const auto& index : circle->indices) {
        indices.push_back(index);
        
    }
    indexOffset = vertices.size();
    circle = CreateCircle("circle2", glm::vec3(70, 50, 0), 30, color, true);
    for (auto& vertex : circle->vertices) {
        vertices.push_back(vertex);
    }
    for (const auto& index : circle->indices) {
        indices.push_back(index + indexOffset);
    }
    indexOffset = vertices.size();
    circle = CreateCircle("circle2", glm::vec3(120, 80, 0), 40, color, true);
    for (auto& vertex : circle->vertices) {
        vertices.push_back(vertex);
    }
    for (const auto& index : circle->indices) {
        indices.push_back(index + indexOffset);
    }
    indexOffset = vertices.size();
    circle = CreateCircle("circle2", glm::vec3(150, 34, 0), 35, color, true);
    for (auto& vertex : circle->vertices) {
        vertices.push_back(vertex);
    }
    for (const auto& index : circle->indices) {
        indices.push_back(index + indexOffset);
    }
    indexOffset = vertices.size();
    Mesh* r = CreateRectangle("r", glm::vec3(25, -2, 0), 125, 50, color, true);
    for (auto& vertex : r->vertices) {
        vertices.push_back(vertex);
    }
    for (const auto& index : r->indices) {
        indices.push_back(index + indexOffset);
    }
    Mesh* cloud = new Mesh(name);
    cloud->InitFromData(vertices, indices);
    return cloud;

}

Mesh* object2D::CreateTank(
    const std::string& name,
    glm::vec3 color1,
    glm::vec3 color2)
{
    std::vector<VertexFormat> vertices = {
    VertexFormat(glm::vec3(-17.5f, 0, 0), color2),
    VertexFormat(glm::vec3(17.5f, 0, 0), color2),
    VertexFormat(glm::vec3(-20, 5, 0), color2),
    VertexFormat(glm::vec3(20, 5, 0), color2),
    VertexFormat(glm::vec3(-25, 5, 0), color1),
    VertexFormat(glm::vec3(-17.5f, 15, 0), color1),
    VertexFormat(glm::vec3(17.5f, 15, 0), color1),
    VertexFormat(glm::vec3(25, 5, 0), color1)
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,
        1, 3, 2,
        4, 5, 6,
        4, 6, 7
    };
    int indexOffset = indices.size();
    Mesh* circle = object2D::CreateCircle("circle", glm::vec3(0, 15, 0), 10, color1, 1);
    for (const auto& vertex : circle->vertices) {
        vertices.push_back(vertex);
    }
    for (const auto& index : circle->indices) {
        indices.push_back(index + indexOffset);
    }
    Mesh* tank = new Mesh(name);
    tank->InitFromData(vertices, indices);
    return tank;
}
