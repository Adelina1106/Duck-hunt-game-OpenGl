#pragma once

#include "components/simple_scene.h"
#include "components/text_renderer.h"

namespace m1
{
    class tema1 : public gfxc::SimpleScene
    {
    public:


     public:
         tema1();
        ~tema1();

        
        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void RenderScene();
        void InitSpeed();
        void DrawTree();
        void drawDuck();
        void drawCloud();


     protected:
        gfxc::TextRenderer* tr;
        glm::mat3 modelMatrix, duck, forWings;
        glm::vec3 centerOrigin, center, centerNeck, centerTail, centerHead, cornerPeak, cornerWing, centerEye;
        glm::ivec2 resolution, realResolution, lastRes;
        std::vector<glm::ivec2> cornerCollision;
        bool rotateWings, mirror, down, up, gameOver, win, power, shotsDone;
        int countHitLeftRight, ducks, shots, lives, level, clicksForPower, cloudRadius, oxLimit, oyLimit;
        int bodyRadius, neckLength, tailLength, headRadius, peakLength, wingLength, grassPart;
        float translateX, translateY, scaleYBody, angularStep, wingAngle;
        float speedOX, speedOY, speed, turnSpeedCoef, initialSpeed, tetha;
        float playTime, winTime, hitCounter, score, lastClick, showLevelTime, gameOverTime, scalePower;
        float lengthCol, heightCol;
        
    };
}   // namespace m1
