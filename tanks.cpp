#include "tanks.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m1;

#include "transform2D.h"
#include "object2D.h"

Tanks::Tanks()
{
    X1 = 100;
    X2 = 1000;
    change = false;
    barrelAngle1 = M_PI / 4.f;
    barrelAngle2 = M_PI - M_PI / 4.f;
    hitCount1 = 0;
    hitCount2 = 0;
    maxHitCount = 10;
    cloud1 = 0;
    cloud2 = 0;
    cloud3 = 0;
    barrelWidth = 4;
    explosionRadius = 20;
    force = 40;
}


Tanks::~Tanks()
{
}


void Tanks::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 1;
    
    Mesh* square1 = object2D::CreateSquare("square1", corner, squareSide, glm::vec3(0, 0.8f, 0.35f), true);
    AddMeshToList(square1);
    Mesh* square2 = object2D::CreateSquare("square2", corner, squareSide, glm::vec3(3/255.f, 34/255.f, 76/255.f) , true);
    AddMeshToList(square2);
    Mesh* tank = object2D::CreateTank("tank", glm::vec3(1, 111/255.f, 125/255.f), glm::vec3(254/255.f, 150/255.f, 160/255.f));
    AddMeshToList(tank);
    Mesh* tank1 = object2D::CreateTank("tank1", glm::vec3(219/255.f, 0, 115/255.f), glm::vec3(218/255.f, 112/255.f, 214/255.f));
    AddMeshToList(tank1);
    Mesh* circle1 = object2D::CreateCircle("circle1", corner, barrelWidth, glm::vec3(0, 0, 0), true);
    AddMeshToList(circle1);
    Mesh* circle2 = object2D::CreateCircle("circle2", corner, 2, glm::vec3(1, 1, 1), true);
    AddMeshToList(circle2);
    Mesh* sun = object2D::CreateCircle("sun", corner, barrelWidth, glm::vec3(252/255.f, 210/255.f, 28/255.f), true);
    AddMeshToList(sun);
    Mesh* rectangle = object2D::CreateRectangle("rectangle", corner, 22.5f, barrelWidth, glm::vec3(0, 0, 0), true);
    AddMeshToList(rectangle);
    Mesh* rectangle1 = object2D::CreateRectangle("rectangle1", corner, 22.5f, barrelWidth, glm::vec3(0, 0, 0), false);
    AddMeshToList(rectangle1);
    Mesh* dayCloud = object2D::CreateCloud("dayCloud", corner, glm::vec3(1, 1, 1));
    AddMeshToList(dayCloud);
    Mesh* nightCloud = object2D::CreateCloud("nightCloud", corner, glm::vec3(103/255.f, 113/255.f, 121/255.f));
    AddMeshToList(nightCloud);

    GenerateTerrain();
}

struct Projectile {
    glm::vec2 P;
    glm::vec2 v;
    float time;
    int who;
};

float Tanks::Terrain(float x) {
    return 32 * sin(x / 64) + 64 * sin(x / 128) + 32 * sin(x / 32);
}

void Tanks::FrameStart()
{
    if (change == false) backgroundColor = glm::vec3(0.55f, 0.8f, 1);
    else backgroundColor = glm::vec3(34 / 255.f, 66 / 255.f, 124 / 255.f);
    glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tanks::GenerateTerrain() {
    heightMap.clear();
    float terrainWidth = window->GetResolution().x;
    float terrainHeight = window->GetResolution().y;
    float pointSpacing = 1;

    for (float x = 0; x <= terrainWidth + 1; x += pointSpacing) {
        float y = Terrain(x);
        heightMap.push_back(glm::vec2(x, y + 300));
        originalHeightMap.push_back(glm::vec2(x, y + 300));
    }
}

void Tanks::RenderTerrain() {

    for (size_t i = 0; i < heightMap.size() - 1; i++) {
        glm::vec2 A = heightMap[i];
        glm::vec2 B = heightMap[i + 1];

        float width = B.x - A.x;
        float height = max(A.y, B.y);

        glm::mat3 modelMatrix = glm::mat3(1);

        modelMatrix *= transform2D::Translate(A.x, A.y);

        float shear = (B.y - A.y) / width;
        glm::mat3 shearMatrix = glm::mat3(1, shear, 0,
                                            0, 1, 0,
                                            0, 0, 1);
        modelMatrix *= shearMatrix;
        modelMatrix *= transform2D::Scale(1, -height);

        if (change == false)
            RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
        else
            RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);
    }

    for (size_t i = 1; i < heightMap.size() - 1; i++) {
        float point = (heightMap[i - 1].y + heightMap[i].y + heightMap[i + 1].y) / 3.0f;
        if (fabs(point - heightMap[i].y) > 0.01f) {
            heightMap[i].y = point;
        }
    }
}

void Tanks::RenderTank(int hitCount, float* X, float* Y, float* angle, float barrelAngle, int ok) {
    glm::mat3 modelMatrix = glm::mat3(1);
    if (*X < 0) *X = 0;
    if (*X > 1280) *X = 1280;
    glm::vec2 A = heightMap[floor(*X)];
    glm::vec2 B = heightMap[ceil(*X) + 1];

    if (*X >= A.x && *X <= B.x && hitCount < maxHitCount) {
        *Y = A.y + (*X - A.x) / (B.x - A.x) * (B.y - A.y);
        if (*Y < 0) {
            *Y = 0;
            *angle = 0;
        }
        modelMatrix *= transform2D::Translate(*X, *Y);
        *angle = atan((B.y - A.y) / (B.x - A.x));
        if (*Y <= 0) {
            *angle = 0;
        }
        modelMatrix *= transform2D::Rotate(*angle);

        string name;
        if (ok == 1) {
            name = "tank";
        }
        else {
            name = "tank1";
        }
        RenderMesh2D(meshes[name], shaders["VertexColor"], modelMatrix);

        modelMatrix *= transform2D::Translate(-10, 40);
        RenderMesh2D(meshes["rectangle1"], shaders["VertexColor"], modelMatrix);
        float a = (maxHitCount - hitCount) / maxHitCount;
        modelMatrix *= transform2D::Scale(a, 1);
        RenderMesh2D(meshes["rectangle"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(*X, *Y);
        modelMatrix *= transform2D::Rotate(*angle);
        modelMatrix *= transform2D::Translate(1, 15);
        modelMatrix *= transform2D::Rotate(barrelAngle);
        RenderMesh2D(meshes["rectangle"], shaders["VertexColor"], modelMatrix);
    }
}

void Tanks::RenderTrajectory(float X, float Y, float angle, float barrelAngle, int hitCount) {
    float len = 22;
    float offsetY = 15;
    std::vector<glm::vec2> trajectoryPoints;
    float timeStep = 0.05f;
    glm::vec2 P = glm::vec2(X - sin(angle) * offsetY + cos(angle + barrelAngle) * len, Y + cos(angle) * offsetY + sin(angle + barrelAngle) * len);
    glm::vec2 v = glm::vec2(cos(barrelAngle + angle) * force, sin(barrelAngle + angle) * force);
    float numPoints = 3000;

    while (trajectoryPoints.size() < numPoints) {
        P += v * timeStep;
        v.y -= 2 * timeStep;
        trajectoryPoints.push_back(glm::vec2(P.x, P.y));
    }
    for (size_t i = 0; i < trajectoryPoints.size() - 1; ++i) {
        glm::vec2 p1 = trajectoryPoints[i];
        glm::vec2 p2 = trajectoryPoints[i + 1];

        glm::mat3 modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(p1.x, p1.y);

        if (hitCount < maxHitCount) {
            RenderMesh2D(meshes["circle2"], shaders["VertexColor"], modelMatrix);
        }
    }
}

float rotationAngle = 0;

void Tanks::RenderClouds(float deltaTimeSeconds) {
    glm::mat3 modelMatrix = glm::mat3(1);
    cloud1 += deltaTimeSeconds * 40;
    if (cloud1 + 100 > 1280) cloud1 = -290;
    modelMatrix *= transform2D::Translate(cloud1 + 100, 500);
    string name;
    if (change == false) name = "dayCloud";
    else name = "nightCloud";
    RenderMesh2D(meshes[name], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    cloud2 += deltaTimeSeconds * 40;
    if (cloud2 + 500 > 1280) cloud2 = -690;
    modelMatrix *= transform2D::Translate(cloud2 + 500, 600);
    RenderMesh2D(meshes[name], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    cloud3 += deltaTimeSeconds * 40;
    if (cloud3 + 1000 > 1280) cloud3 = -1190;
    modelMatrix *= transform2D::Translate(cloud3 + 1000, 450);
    RenderMesh2D(meshes[name], shaders["VertexColor"], modelMatrix);
}

void Tanks::RenderSunMoon(float deltaTimeSeconds) {
    glm::mat3 modelMatrix = glm::mat3(1);
    rotationAngle += deltaTimeSeconds * 0.1f;
    if (rotationAngle > 2 * M_PI) {
        rotationAngle = 0;
    }
    if (rotationAngle < M_PI)
        change = false;
    else
        change = true;

    float centerX = 640;
    float centerY = 0;

    modelMatrix *= transform2D::Translate(centerX, centerY);
    modelMatrix *= transform2D::Rotate(-rotationAngle);
    modelMatrix *= transform2D::Translate(-centerX, -centerY);
    modelMatrix *= transform2D::Scale(20, 20);

    RenderMesh2D(meshes["sun"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    rotationAngle += M_PI;
    modelMatrix *= transform2D::Translate(centerX, centerY);
    modelMatrix *= transform2D::Rotate(-rotationAngle);
    modelMatrix *= transform2D::Translate(-centerX, -centerY);
    modelMatrix *= transform2D::Scale(30, 30);
    rotationAngle -= M_PI;

    RenderMesh2D(meshes["circle2"], shaders["VertexColor"], modelMatrix);
}

void Tanks::RenderProjectiles(float deltaTimeSeconds) {
    for (size_t i = 0; i < projectiles.size(); ) {
        struct Projectile& proj = projectiles[i];
        proj.P += proj.v * proj.time;
        proj.v.y -= 2 * proj.time;
        proj.time += deltaTimeSeconds * 0.25f;

        if ((proj.P.y) < 0) {
            projectiles.erase(projectiles.begin() + i);
        }
        else {
            int impactRadius = 20;
            if (glm::distance(proj.P, glm::vec2(X1, Y1)) < (barrelWidth + impactRadius) && hitCount1 < maxHitCount && proj.who == 2) {
                hitCount1++;
                projectiles.erase(projectiles.begin() + i);
                continue;
            }

            if (glm::distance(proj.P, glm::vec2(X2, Y2)) < (barrelWidth + impactRadius) && hitCount2 < maxHitCount && proj.who == 1) {
                hitCount2++;
                projectiles.erase(projectiles.begin() + i);
                continue;
            }
            if (proj.P.x < 0) proj.P.x = 0;
            if (proj.P.x > 1280) proj.P.x = 1280;
            glm::vec2 A = heightMap[floor(proj.P.x)];
            glm::vec2 B = heightMap[ceil(proj.P.x) + 1];

            float terrainY = B.y;
            if (proj.P.y - terrainY <= 0) {
                for (size_t i = 0; i < heightMap.size(); ++i) {
                    glm::vec2& point = heightMap[i];
                    float dx = fabs(proj.P.x - point.x);
                    if (dx < explosionRadius) {
                        float newY = proj.P.y - sqrt(pow(explosionRadius, 2) - pow(dx, 2));
                        if (point.y > newY) {
                            point.y = newY;
                        }
                    }
                }
                projectiles.erase(projectiles.begin() + i);
            }
            else {
                glm::mat3 modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(proj.P.x, proj.P.y);
                RenderMesh2D(meshes["circle1"], shaders["VertexColor"], modelMatrix);
            }
            i++;
        }
    }
}

void Tanks::Update(float deltaTimeSeconds) {

    RenderClouds(deltaTimeSeconds);
    RenderTerrain();

    int ok = 1;
    RenderTank(hitCount1, &X1, &Y1, &angle1, barrelAngle1, ok);
    ok = 2;
    RenderTank(hitCount2, &X2, &Y2, &angle2, barrelAngle2, ok);

    RenderProjectiles(deltaTimeSeconds);

    RenderTrajectory(X1, Y1, angle1, barrelAngle1, hitCount1);
    RenderTrajectory(X2, Y2, angle2, barrelAngle2, hitCount2);

    RenderSunMoon(deltaTimeSeconds);
}


void Tanks::FrameEnd()
{
}

void Tanks::OnInputUpdate(float deltaTime, int mods)
{
    if (window->KeyHold(GLFW_KEY_A)) {
        X1 -= deltaTime * 100;
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        X1 += deltaTime * 100;
    }

    if (window->KeyHold(GLFW_KEY_J)) {
        X2 -= deltaTime * 100;
    }

    if (window->KeyHold(GLFW_KEY_L)) {
        X2 += deltaTime * 100;
    }

    if (window->KeyHold(GLFW_KEY_W)) {
        barrelAngle1 += deltaTime;
        if (barrelAngle1 > M_PI / 2)
            barrelAngle1 = M_PI / 2;
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        barrelAngle1 -= deltaTime;
        if (barrelAngle1 < 0)
            barrelAngle1 = 0;
    }
    if (window->KeyHold(GLFW_KEY_I)) {
        barrelAngle2 -= deltaTime;
        if (barrelAngle2 < M_PI / 2)
            barrelAngle2 = M_PI / 2;
    }

    if (window->KeyHold(GLFW_KEY_K)) {
        barrelAngle2 += deltaTime;
        if (barrelAngle2 > M_PI)
            barrelAngle2 = M_PI;
    }
}

void Tanks::startProjectile(float X, float Y, float barrelAngle, float angle, int who) {
    float len = 22;
    float offsetY = 15;

    struct Projectile proj;
    proj.P = glm::vec2(X - sin(angle) * offsetY + cos(angle + barrelAngle) * len, Y + cos(angle) * offsetY + sin(angle + barrelAngle) * len);
    proj.v = glm::vec2(cos(barrelAngle + angle) * force, sin(barrelAngle + angle) * force);
    proj.time = 0;
    proj.who = who;

    projectiles.push_back(proj);
}

void Tanks::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_SPACE && hitCount1 < maxHitCount) {
        startProjectile(X1, Y1, barrelAngle1, angle1, 1);
    }
    if (key == GLFW_KEY_ENTER && hitCount2 < maxHitCount) {
        startProjectile(X2, Y2, barrelAngle2, angle2, 2);
    }
}


void Tanks::OnKeyRelease(int key, int mods)
{

}


void Tanks::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{

}


void Tanks::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{

}


void Tanks::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{

}


void Tanks::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tanks::OnWindowResize(int width, int height)
{
}
