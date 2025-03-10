#pragma once

#include "components/simple_scene.h"


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void GenerateTerrain();
        void RenderTerrain();
        float Terrain(float x);
        void RenderTank(int hitCount, float* X, float* Y, float* angle, float barrelAngle, int ok);
        void startProjectile(float X, float Y, float barrelAngle, float angle, int who);
        void RenderTrajectory(float X, float Y, float angle, float barrelAngle, int hitCount);
        void RenderClouds(float deltaTimeSeconds);
        void RenderSunMoon(float deltaTimeSeconds);
        void RenderProjectiles(float deltaTimeSeconds);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        struct Projectile {
            glm::vec2 P;
            glm::vec2 v;
            float time;
            int who;
        };
        std::vector<glm::vec2> heightMap, originalHeightMap;
        float X1, X2, Y1, Y2, angle1, angle2, cloud1, cloud2, cloud3;
        bool change;
        int hitCount1, hitCount2, barrelWidth, explosionRadius, force;
        float maxHitCount;
        float time, barrelAngle1, barrelAngle2;
        std::vector<Projectile> projectiles;
        glm::vec3 backgroundColor;

    };
}   // namespace m1
