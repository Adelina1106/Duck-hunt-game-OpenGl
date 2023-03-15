#include "lab_m1/tema1/tema1.h"

#include <vector>
#include <iostream>

#include "core/engine.h"
#include "utils/gl_utils.h"
#include "lab_m1/tema1/transform2D.h"
#include "components/text_renderer.h"
using namespace std;
using namespace m1;

#define M_PI 3.14159

Mesh* CreateSquare(
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
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* CreateCircle(
    const std::string& name,
    float radius,
    glm::vec3 center,
    glm::vec3 color,
    bool fill)
{
    int count = 3600;
    float z = 0;
    float x, y;
    std::vector<VertexFormat> vertices;
    for (int i = 0; i < count; i++) {
        x = radius * cosf(i * (float)3.145 / 90);
        y = radius * sinf(i * (float)3.145 / 90);
        vertices.push_back(VertexFormat(glm::vec3(center.x + x, center.y + y, center.z + z), color));
    }
    Mesh* sphere = new Mesh(name);
    std::vector<unsigned int> indices;
    for (int i = 1; i < count - 1; i++) {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i + 1);
    }
    if (!fill) {
        sphere->SetDrawMode(GL_LINE_LOOP);
    }
    sphere->InitFromData(vertices, indices);
    return sphere;
}

Mesh* CreateRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float width,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(width , 0, 0), color),
        VertexFormat(corner + glm::vec3(width , length , 0), color),
        VertexFormat(corner + glm::vec3(0, length , 0), color)
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
Mesh* CreateTriangle(
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
        VertexFormat(corner + glm::vec3(0, length, 0), color),
        VertexFormat(corner + glm::vec3(-length / 2, length / 2, 0), color),
    };

    Mesh* triangle = new Mesh(name);
    std::vector<unsigned int> indices = { 2, 1, 0 };

    if (!fill) {
        triangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {

        triangle->SetDrawMode(GL_TRIANGLES);
    }

    triangle->InitFromData(vertices, indices);
    return triangle;
}

tema1::tema1()
{
}


tema1::~tema1()
{
}


void tema1::Init()
{
    tr = new gfxc::TextRenderer(window->props.selfDir, window->GetResolution().x, window->GetResolution().y);

    tr->Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Hack-Bold.ttf"), 18);
    showLevelTime = 0;
    
    srand(time(0));
    resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    realResolution.x = resolution.x;
    realResolution.y = resolution.y;

    rotateWings = true;
    gameOver = false;
    up = false;
    down = false;
    shotsDone = false;
    power = false;
    win = false;

    angularStep = 0;
    wingAngle = 0;
    score = 0;
    lastClick = 0;
    translateX = 0;
    translateY = 0;
    playTime = 0;
    ducks = 0;
    gameOverTime = 0;
    clicksForPower = 0;
    hitCounter = 0;
    winTime = 0;
    scalePower = 1;
    oxLimit = 10;
    oyLimit = 10;
    grassPart = 4;

    InitSpeed();

    center = glm::vec3(rand() % (resolution.x - oxLimit * 4), rand() % (resolution.y / grassPart), 0);
    if (center.y <= oyLimit)
        center.y += oyLimit;
    if (center.x <= oxLimit)
        center.x += oxLimit;
    centerOrigin = glm::vec3(0, 0, 0);

    centerNeck = glm::vec3(centerOrigin.x + 56, centerOrigin.y - 5, 0);
    centerTail = glm::vec3(centerOrigin.x - 54, centerOrigin.y - 22.5f, 0);
    centerHead = glm::vec3(centerOrigin.x + 74, centerOrigin.y + 27, 0);
    cornerPeak = glm::vec3(centerHead.x - 134, centerHead.y - 83, 0);
    cornerWing = glm::vec3(centerOrigin.x + 10 , centerOrigin.y + 20, 0);
    centerEye = glm::vec3(centerHead.x + 10, centerHead.y + 7, 0);
    bodyRadius = 60;
    neckLength = 25;
    tailLength = 45;
    headRadius = 30;
    peakLength = 17;
    wingLength = 60;
    scaleYBody = 0.5f;
    lengthCol = bodyRadius * 2 + headRadius * 2 + neckLength + peakLength * 2 + tailLength;
    heightCol = bodyRadius * 2 * scaleYBody + headRadius * 2 + neckLength * 2;
    shots = 3;
    lives = 3;

    Mesh* body = CreateCircle("body", bodyRadius, centerOrigin, glm::vec3(0.5f, 0.35f, 0.05f), false);
    AddMeshToList(body);
    Mesh* neck = CreateRectangle("neck", centerNeck, neckLength / 1.5F,  neckLength, glm::vec3(0.5f, 0.35f, 0.05f), true);
    AddMeshToList(neck);
    Mesh* tail1 = CreateTriangle("tail1", centerTail, tailLength, glm::vec3(0.5f, 0.35f, 0.05f), true);
    AddMeshToList(tail1);
    Mesh* head = CreateCircle("head", headRadius, centerHead, glm::vec3(0, 0.4f, 0), true);
    AddMeshToList(head);
    Mesh* peak = CreateTriangle("peak", cornerPeak, peakLength, glm::vec3(1, 0.5f, 0), true);
    AddMeshToList(peak);
    Mesh* wing = CreateTriangle("wing", cornerWing, wingLength, glm::vec3(0, 0.4f, 0), true);
    AddMeshToList(wing);
    Mesh* eye = CreateCircle("eye", 3, centerEye, glm::vec3(0, 0, 0), true);
    AddMeshToList(eye);
   
    Mesh* shot = CreateRectangle("shot",  centerOrigin, 25, 35, glm::vec3(0, 0, 0), true);
    AddMeshToList(shot);
    Mesh* shot1 = CreateTriangle("shot1", centerOrigin, 25, glm::vec3(0, 0, 0), true);
    AddMeshToList(shot1);

    Mesh* outside = CreateRectangle("outside",centerOrigin,  65, 150, glm::vec3(1, 1, 1), false);
    AddMeshToList(outside);
    Mesh* life = CreateCircle("life", 20, centerOrigin, glm::vec3(1, 0, 0), true);
    AddMeshToList(life);

    Mesh* scoreOutside = CreateRectangle("scoreOutside", centerOrigin,70, 270, glm::vec3(1, 1, 1), false);
    AddMeshToList(scoreOutside);

    Mesh* grass = CreateRectangle("grass", centerOrigin, resolution.x, (float)resolution.y / grassPart, glm::vec3(0.1f, 0.55f, 0.1f), true);
    AddMeshToList(grass);
    Mesh* grassTexture = CreateTriangle("grassTexture", centerOrigin, 30, glm::vec3(0.1f, 0.55f, 0.1f), true);
    AddMeshToList(grassTexture);
    Mesh* grassIn = CreateTriangle("grassIn", centerOrigin, 40, glm::vec3(0, 0.5f, 0), true);
    AddMeshToList(grassIn);

    cloudRadius = 100;
    Mesh* cloud = CreateCircle("cloud", cloudRadius, centerOrigin, glm::vec3(0.95f, 0.97f, 1), true);
    AddMeshToList(cloud);

    Mesh* trunk = CreateRectangle("trunk", centerOrigin, 55, 175, glm::vec3(0.3f, 0.3f, 0.1f), true);
    AddMeshToList(trunk);
    int corollaRadius = 150;
    Mesh* corolla = CreateCircle("corolla", corollaRadius, centerOrigin, glm::vec3(0, 0.3, 0.2), true);
    AddMeshToList(corolla);
}


void tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.5f, 0.8f, 0.9f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void tema1::RenderScene() {
    // interior iarba
    int k = 0;
    for (int j = 0; j <= 3; j++) {
        k++;
        for (int i = 0; i < 3; i++) {
            int q;
            if (k % 2 == 1)
                q = -k;
            else q = k;
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(0, (resolution.y / grassPart - q * 30) % (resolution.y / grassPart));
            modelMatrix *= transform2D::Translate(i * 20  + 50 + k * 200, 0);
            modelMatrix *= transform2D::Rotate(M_PI / 4);
            RenderMesh2D(meshes["grassIn"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(i * 20  + 70 +  k * 200, (resolution.y / grassPart - q * 30) % (resolution.y / grassPart));
            modelMatrix *= transform2D::Rotate(-M_PI / 2);
            RenderMesh2D(meshes["grassIn"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(i * 20 + 85 +  k * 200, (resolution.y / grassPart + 30 - q * 30)  %(resolution.y / grassPart));
            modelMatrix *= transform2D::Rotate(-M_PI - M_PI / 4);
            RenderMesh2D(meshes["grassIn"], shaders["VertexColor"], modelMatrix);
        }
    }

    // exterior iarba
    for (int i = 0; i < 65; i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(i * 20 , resolution.y / grassPart );
        modelMatrix *= transform2D::Rotate(M_PI / 4);
        RenderMesh2D(meshes["grassTexture"], shaders["VertexColor"], modelMatrix);
        
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(i * 20 + 20, resolution.y / grassPart);
            modelMatrix *= transform2D::Rotate(-M_PI / 2);
            RenderMesh2D(meshes["grassTexture"], shaders["VertexColor"], modelMatrix);
     
    }
    modelMatrix = glm::mat3(1);
    RenderMesh2D(meshes["grass"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(resolution.x - 220 , resolution.y - 30);
    modelMatrix *= transform2D::Rotate( - M_PI / 2);
    RenderMesh2D(meshes["outside"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(resolution.x - 220, resolution.y - 100);
    modelMatrix *= transform2D::Rotate(-M_PI / 2);
    RenderMesh2D(meshes["outside"], shaders["VertexColor"], modelMatrix);
    for (int i = 0; i < shots; i++)
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(resolution.x - 210 + i * 50, resolution.y - 88);
        RenderMesh2D(meshes["shot"], shaders["VertexColor"], modelMatrix);
        modelMatrix = glm::mat3(1);

        modelMatrix *= transform2D::Translate(resolution.x - 210 + i * 50, resolution.y - 47);
        modelMatrix *= transform2D::Rotate(-M_PI / 2);
        RenderMesh2D(meshes["shot1"], shaders["VertexColor"], modelMatrix);
    }

    // vietile
    for (int i = 0; i < lives; i++)
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(resolution.x - 195 + i * 50, resolution.y - 130);
        RenderMesh2D(meshes["life"], shaders["VertexColor"], modelMatrix);
    }

    // scorul
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(resolution.x - 280, resolution.y - 175);
    modelMatrix *= transform2D::Rotate(- M_PI/ 2);
    RenderMesh2D(meshes["scoreOutside"], shaders["VertexColor"], modelMatrix);


    if (score >= 270) {
        score = 0;
    }
   
    Mesh* scoreInside = CreateRectangle("scoreInside", centerOrigin, 70, (int)score, glm::vec3(0, 0, 1), true);
    AddMeshToList(scoreInside);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(resolution.x - 280, resolution.y - 175);
    modelMatrix *= transform2D::Rotate(-M_PI / 2);
    RenderMesh2D(meshes["scoreInside"], shaders["VertexColor"], modelMatrix);


}

void tema1::DrawTree() {
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate((float)resolution.x / 4 * 2.5f + 32, resolution.y / 4 + 250);
    modelMatrix *= transform2D::Scale(0.7, 1);
    RenderMesh2D(meshes["corolla"], shaders["VertexColor"], modelMatrix);

    glm::mat3 modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate((float)resolution.x / 4 * 2.5f, resolution.y / 4);
    RenderMesh2D(meshes["trunk"], shaders["VertexColor"], modelMatrix);

}

void tema1::drawCloud() {
    glm::mat3 modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate((float)resolution.x / 4, resolution.y / 1.5f);
    modelMatrix *= transform2D::Scale(1, 0.5);
    RenderMesh2D(meshes["cloud"], shaders["VertexColor"], modelMatrix);

    for (int i = 0; i <= 4; i++)
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate((float)resolution.x / 4 - 85, (float)resolution.y / 1.5f);
        modelMatrix *= transform2D::Scale(0.3, 0.3);
        modelMatrix *= transform2D::Translate((float)resolution.x / 4, resolution.y / 1.5f);
        modelMatrix *= transform2D::Rotate(i * M_PI / 12);
        modelMatrix *= transform2D::Translate(-(float)resolution.x / 4, -resolution.y / 1.5f);
        RenderMesh2D(meshes["cloud"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate((float)resolution.x / 4 - 100, resolution.y / 1.5f - cloudRadius * 2.6f);
        modelMatrix *= transform2D::Scale(0.3, 0.3);
        modelMatrix *= transform2D::Translate((float)resolution.x / 4, resolution.y / 1.5f);
        modelMatrix *= transform2D::Rotate(M_PI + i * M_PI / 12);
        modelMatrix *= transform2D::Translate(-(float)resolution.x / 4, -resolution.y / 1.5f);
        RenderMesh2D(meshes["cloud"], shaders["VertexColor"], modelMatrix);
    }
}

void tema1::drawDuck() {
    modelMatrix = glm::mat3(1);
    modelMatrix *= duck;
    modelMatrix *= transform2D::Scale(1, scaleYBody);
    if (!down) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= duck;
        RenderMesh2D(meshes["eye"], shaders["VertexColor"], modelMatrix);

        float wingRotate = 2;
        for (int i = 0; i < 3; i++) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= tema1::duck;
            modelMatrix *= transform2D::Translate(cornerWing.x, cornerWing.y);
            modelMatrix *= transform2D::Rotate(wingRotate * M_PI / 4);
            modelMatrix *= forWings;
            modelMatrix *= transform2D::Translate(-cornerWing.x, -cornerWing.y);
            RenderMesh2D(meshes["wing"], shaders["VertexColor"], modelMatrix);

            wingRotate += 0.25f;
        }
        modelMatrix = glm::mat3(1);
        modelMatrix *= duck;
        modelMatrix *= transform2D::Translate(centerHead[0], centerHead[1]);
        modelMatrix *= transform2D::Scale(1, 0.7);
        modelMatrix *= transform2D::Translate(-centerHead[0], -centerHead[1]);
        RenderMesh2D(meshes["head"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= duck;
        modelMatrix *= transform2D::Scale(1, 0.65);
        RenderMesh2D(meshes["tail1"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= duck;
        modelMatrix *= transform2D::Scale(1, scaleYBody);
        RenderMesh2D(meshes["body"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= duck;
        modelMatrix *= transform2D::Translate(centerNeck.x, centerNeck.y);
        modelMatrix *= transform2D::Rotate(3.14f / 4);
        modelMatrix *= transform2D::Translate(-centerNeck.x, -centerNeck.y);
        RenderMesh2D(meshes["neck"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= duck;
        modelMatrix *= transform2D::Scale(1.7f, 0.5);
        modelMatrix *= transform2D::Rotate(M_PI);
        RenderMesh2D(meshes["peak"], shaders["VertexColor"], modelMatrix);

        wingRotate = 1;
        for (int i = 0; i < 3; i++) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= duck;
            modelMatrix *= transform2D::Translate(cornerWing.x, cornerWing.y);
            modelMatrix *= transform2D::Rotate(wingRotate * M_PI / 4);
            modelMatrix *= forWings;
            modelMatrix *= transform2D::Translate(-cornerWing.x, -cornerWing.y);
            RenderMesh2D(meshes["wing"], shaders["VertexColor"], modelMatrix);
            wingRotate += 0.25f;
        }
    }
    else {
        // rata a fost impuscata
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(-20, -5);
        modelMatrix *= duck;
        RenderMesh2D(meshes["eye"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(-42, -5);
        modelMatrix *= duck;
        RenderMesh2D(meshes["eye"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(bodyRadius * scaleYBody - (float)headRadius/7 , -bodyRadius * 2 - (float)headRadius/2.2);
        modelMatrix *= duck;
        modelMatrix *= transform2D::Scale(1, 0.5);
        RenderMesh2D(meshes["peak"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= duck;
        modelMatrix *= transform2D::Translate(10, -25);
        modelMatrix *= transform2D::Translate(centerHead.x, centerHead.y);
        modelMatrix *= transform2D::Scale(1, 0.7);
        modelMatrix *= transform2D::Translate(-centerHead.x, -centerHead.y);
        RenderMesh2D(meshes["head"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= duck;
        modelMatrix *= transform2D::Scale(1, scaleYBody);
        RenderMesh2D(meshes["body"], shaders["VertexColor"], modelMatrix);

        float wingRotate = 1;
        for (int i = 0; i < 3; i++) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(0, -20);
            modelMatrix *= duck;
            modelMatrix *= transform2D::Translate(cornerWing.x, cornerWing.y);
            modelMatrix *= transform2D::Rotate(wingRotate * M_PI / 4);
            modelMatrix *= transform2D::Rotate( -M_PI / 6);
            modelMatrix *= transform2D::Translate(-cornerWing.x, -cornerWing.y);
            RenderMesh2D(meshes["wing"], shaders["VertexColor"], modelMatrix);
            wingRotate += 0.25f;
        }

        wingRotate = 1;
        for (int i = 0; i < 3; i++) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(-40, -20);
            modelMatrix *= duck;
            modelMatrix *= transform2D::Translate(cornerWing.x, cornerWing.y);
            modelMatrix *= transform2D::Rotate(wingRotate * M_PI / 4);
            modelMatrix *= transform2D::Rotate(-M_PI / 2);
            modelMatrix *= transform2D::Scale(1, -1);
            modelMatrix *= transform2D::Translate(-cornerWing.x, -cornerWing.y);
            RenderMesh2D(meshes["wing"], shaders["VertexColor"], modelMatrix);
            wingRotate += 0.25f;
        }  
    }
}

void tema1::InitSpeed() {
    countHitLeftRight = 0;
    turnSpeedCoef = 1;
    if (ducks == 0) {
        initialSpeed = rand() % 50 + 250;
    }
    speed = initialSpeed + (ducks/5) * initialSpeed / 5;
    speedOX = rand() % int(speed / 3) + (speed / 3);
    speedOX = rand() % 2 == 0 ? speedOX : -speedOX;
    speedOY = sqrt(speed * speed - speedOX * speedOX);
    if (speedOX < 0) {
        countHitLeftRight++;
        mirror = true;
    }
}

void tema1::Update(float deltaTimeSeconds)
{
    if (level > 5 && winTime < 2.5f) {
        tr->RenderText("YOU WON", resolution.x / 2.5f, resolution.y / 3, 3, glm::vec3(0, 0, 0));
        ducks = 0;
        winTime += deltaTimeSeconds;
        win = true;
        if (winTime >= 2.5f) {
            lives = 3;
            shots = 3;
        }
        score = 0;
        showLevelTime = 0;
    }
    else if (lives == 0 && gameOverTime < 2.5f) {
        tr->RenderText("GAME OVER", resolution.x / 2.5f, resolution.y / 3, 3, glm::vec3(0, 0, 0));
        ducks = 0;
        gameOverTime += deltaTimeSeconds;
        gameOver = true;
        if (gameOverTime >= 2.5f) {
            lives = 3;
            shots = 3;
        }
        score = 0;
        showLevelTime = 0;
    }
    else if (ducks % 5 == 0 && showLevelTime < 2.5f) {
        if (gameOver == true || win == true)
            InitSpeed();
        winTime = 0;
        gameOver = false;
        win = false;
        gameOverTime = 0;
        showLevelTime += deltaTimeSeconds;

        level = ducks / 5 + 1;
        if (level == 1)
            power = false;
        string level1 = "Level " + std::to_string(level);
        tr->RenderText(level1, resolution.x / 2.5f, resolution.y / 3, 3, glm::vec3(0, 0, 0));

        center.x = rand() % (resolution.x - oxLimit * 4);
        center.y = rand() % (resolution.y / grassPart);
        if (center.x <= oxLimit)
            center.x += oxLimit;
        if (center.y <= oyLimit)
            center.y += oyLimit;
        playTime = 0;

        RenderScene();

    }
    else {
        {
            // rotatia aripilor
            forWings = glm::mat3(1);
            duck = glm::mat3(1);
            forWings *= transform2D::Rotate(-M_PI / 8);

            if (rotateWings == true)
            {
                wingAngle += deltaTimeSeconds * M_PI / 2;
                if (wingAngle >= M_PI / 3)
                    rotateWings = false;
            }
            else {
                wingAngle -= deltaTimeSeconds * M_PI / 2;
                rotateWings = false;
                if (wingAngle <= 0)
                    rotateWings = true;
            }
            forWings *= transform2D::Rotate(wingAngle);

            // intoarcerea la marginea ecranului
            turnSpeedCoef = 1;
            angularStep = 0;
            if (center.x < 0) {
                countHitLeftRight++;
                speedOX = -speedOX;
                turnSpeedCoef = 1.1f * (ducks / 5 + 1);
            }
            else if (center.x > resolution.x) {
                countHitLeftRight++;
                speedOX = -speedOX;
                turnSpeedCoef = 1.1f * (ducks / 5 + 1);
            }
            else if (center.y > resolution.y) {
                speedOY = -speedOY;
                turnSpeedCoef = 1.1f * (ducks / 5 + 1);
            }
            else if (center.y < 0 && !down)
            {
                speedOY = -speedOY;
                turnSpeedCoef = 1.1f * (ducks / 5 + 1);
            }
            else if (center.x < 0 && center.y < 0 || center.x < 0 && center.y > resolution.y ||
                center.x > resolution.x && center.y < 0 ||
                center.x > resolution.x && center.y > resolution.y) {
                speedOX = -speedOX;
                speedOY = -speedOY;
                turnSpeedCoef = 1.1f * (ducks / 5 + 1);
            }
        }

        translateX += speedOX * deltaTimeSeconds * turnSpeedCoef;
        translateY += speedOY * deltaTimeSeconds * turnSpeedCoef;
        duck *= transform2D::Translate(center.x + translateX, center.y + translateY);
            
        if (speedOX > 0)
            tetha = atan(speedOY/ speedOX);
        else if (speedOX < 0)
            tetha = M_PI + atan(speedOY/ speedOX);
        
            if (!down && (playTime > 6 || shotsDone))
            {
                if (center.y < resolution.y / 2 && center.x + 20 <= resolution.x && center.x - 20 >= 0 || up || shotsDone) {
                    speedOY = 300;
                    speedOX = 0;
                    up = true;
                }
            }
            if (playTime >= 6 && center.y >= resolution.y  && up
                || shotsDone && center.y >= resolution.y  && up) {
                up = false;
                shots = 3;
                lives--;
                ducks++;
                showLevelTime = 0;
                center.x = rand() % resolution.x;
                center.y = rand() % (resolution.y / grassPart);
                if (center.y <= 10)
                    center.y += 20;
                playTime = 0;
                shotsDone = false;
                InitSpeed();
            }

            if (down)
            {
                speedOX = 0;
                speedOY = -400;
            }
            if (center.y <= 10 && down == true) {
                down = false;
                center.x = rand() % (resolution.x - oxLimit * 10);
                center.y = rand() % (resolution.y / grassPart);
                if (center.y <= oyLimit)
                    center.y += oyLimit;
                if (center.x <= oxLimit)
                    center.x += oyLimit;
                ducks++;
                showLevelTime = 0;
                playTime = 0;
                shots = 3;
                InitSpeed();
            }

        if (up)
        {
            duck *= transform2D::Rotate(M_PI / 2);
        }
        else if(down)
            duck *= transform2D::Rotate(-M_PI / 2);
        else {
            duck *= transform2D::Rotate(tetha);

            if (mirror)
                duck *= transform2D::Scale(1, countHitLeftRight % 2 == 1 ? -1 : 1);
        }

        if (score == 0 && level > 1)
            if (!down) {
                power = true;
                if (playTime == 0)
                    clicksForPower = 2;
            }

        lastClick += deltaTimeSeconds;
        playTime += deltaTimeSeconds;

        if (power && clicksForPower == 2) {
            scalePower = 1.8f;
        }
        else scalePower = 1;
        
        duck *= transform2D::Scale(scalePower, scalePower);

        RenderScene();
        drawDuck();
        drawCloud();
        DrawTree();
        center.y += translateY;
        center.x += translateX;
        translateX = 0;
        translateY = 0;
    }
}

void tema1::FrameEnd()
{
}


void tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move even

}


void tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (lastClick > 0.2) {
        hitCounter = 0;
        shots--;

        // resize
        mouseX = mouseX * (resolution.x / (float)realResolution.x);
        mouseY = resolution.y - mouseY * (resolution.y / (float)realResolution.y);

        cornerCollision.push_back(glm::ivec2());
        cornerCollision.push_back(glm::ivec2());
        cornerCollision.push_back(glm::ivec2());
        cornerCollision.push_back(glm::ivec2());
        cornerCollision.at(2).x = cornerCollision.at(0).x = center.x - lengthCol / 2 * scalePower;
        cornerCollision.at(0).y = cornerCollision.at(1).y = center.y - heightCol / 2 * scalePower;
        cornerCollision.at(1).x = cornerCollision.at(3).x = center.x + lengthCol / 2 * scalePower;
        cornerCollision.at(2).y = cornerCollision.at(3).y = center.y + heightCol / 2 * scalePower;
        for (int i = 0; i < cornerCollision.size(); i++)
        {
            cornerCollision.at(i).x = (cornerCollision.at(i).x - center.x) * cos(tetha) - (cornerCollision.at(i).y - center.y) * sin(tetha) + center.x;
            cornerCollision.at(i).y = (cornerCollision.at(i).x - center.x) * sin(tetha) + (cornerCollision.at(i).y - center.y) * cos(tetha) + center.y;
        }
       
        if (!up && !down)
            if (mouseX >= std::min({ cornerCollision.at(0).x, cornerCollision.at(1).x, cornerCollision.at(2).x, cornerCollision.at(3).x})
                && mouseX <= std::max({ cornerCollision.at(0).x, cornerCollision.at(1).x, cornerCollision.at(2).x, cornerCollision.at(3).x})
                && mouseY <= std::max({ cornerCollision.at(0).y, cornerCollision.at(1).y, cornerCollision.at(2).y, cornerCollision.at(3).y})
                && mouseY >= std::min({ cornerCollision.at(0).y, cornerCollision.at(1).y, cornerCollision.at(2).y, cornerCollision.at(3).y})) {
                if(power)
                   clicksForPower--;
                if (!power) {
                    down = true;
                    score += 27;
                    if (shots <= 0 && playTime < 6 && playTime > 0 && !down) {
                        lives--;
                    }
                }
                else if (power && clicksForPower == 0) {
                    scalePower = 1;
                    power = false;
                    down = true;
                    score += 27;
                    if (shots <= 0 && playTime < 6 && !down)
                        lives--;
                }
                else if(power && clicksForPower == 1) {
                    scalePower = 1;
                    if (shots <= 0 && playTime < 6 && !down)
                        lives--;
                }
                
            }
            else if (shots <= 0 && playTime > 0 && playTime < 6) {
                shotsDone = true;
            }
        lastClick = 0;
    }
}


void tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}


void tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void tema1::OnWindowResize(int width, int height)
{
    realResolution.x = width;
    realResolution.y = height;
}