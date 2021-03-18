#include <iostream>
#include <cstring>
//#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include "windowlib.h"
#include "vao.h"
#include "obj-loader/objloader.h"
#include "program.h"
#include "octree.h"
#include <math.h>
#include <unordered_set>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define v1 -0.5,0.5,0.5
#define v2 -0.5,0.5,-0.5
#define v3 0.5,0.5,-0.5
#define v4 0.5,0.5,0.5
#define v5 -0.5,-0.5,0.5
#define v6 -0.5,-0.5,-0.5
#define v7 0.5,-0.5,-0.5
#define v8 0.5,-0.5,0.5

using namespace std;

float vert[] = {
v1, v4, v3, v3, v2, v1,
v2, v3, v7, v7, v6, v2,
v6, v7, v8, v8, v5, v6,
v5, v8, v4, v4, v1, v5,
v8, v7, v3, v3, v4, v8,
v6, v5, v1, v1, v2, v6
};

float texCoord[] = {0,0, 1,0, 1,1, 1,1, 0,1, 0,0,
                    0,0, 1,0, 1,1, 1,1, 0,1, 0,0,
                    0,0, 1,0, 1,1, 1,1, 0,1, 0,0,
                    0,0, 1,0, 1,1, 1,1, 0,1, 0,0,
                    0,0, 1,0, 1,1, 1,1, 0,1, 0,0,
                    0,0, 1,0, 1,1, 1,1, 0,1, 0,0,
};

bool cursor = false;

bool moveForward = false, moveBack = false, moveLeft = false, moveRight = false, moveUp = false, moveDown = false;
int prev_mouse_x = 0.0;
int prev_mouse_y = 0.0;

float ySpeed = 0.0;
float G = -0.08;
float xRot = 0.0, yRot = 0.0;
//float xCam = 0.0, yCam = 3.0, zCam = 0.0;
vec3 cam_pos;
float camRotSpeed = 0.000005;
float camMoveSpeed = 0.1;

GLuint vertexVBO;

unsigned int texture;

void InitTexture()
{

    int width, height, cnt;

    /*
    width = 2;
    height = 2;
    struct {unsigned char r,g,b,a; } data[2][2];
    cnt = 4;
    memset(data, 0, sizeof(data));
    data[0][0].r = 255;
    data[1][0].g = 255;
    data[1][1].b = 255;
    data[0][1].r = 255;
    data[0][1].g = 255;
    data[0][1].b = 255;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                                    0, cnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
*/

    unsigned char *data = stbi_load("cobble.png", &width, &height, &cnt, 0);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                                    0, cnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
}

void InitVBO()
{
    glGenBuffers(1, &vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

int GetHeight(int x, int y)
{
    return -abs(x) - abs(y);
}

void DrawBlock(vec3 pos)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &vert);
        glTexCoordPointer(2, GL_FLOAT, 0, texCoord);
                glPushMatrix();
                glColor3f(1,1,1);
                glTranslatef(pos.x, pos.y, pos.z);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                glPopMatrix();
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void DrawTriangle()
{
    glBegin(GL_TRIANGLES);
        glColor3f(1,0,0); glVertex3f(0, 0, 0);
        glColor3f(0,0,1); glVertex3f(10, 0, 0);
        glColor3f(0,1,0); glVertex3f(0, 0, 10);
    glEnd();
}

void MoveCamera()
{
    if (moveForward)
    {
        cam_pos.x += sin(yRot / 180 * M_PI) * camMoveSpeed;
        cam_pos.z += -cos(yRot / 180 * M_PI) * camMoveSpeed;
    }
    if (moveBack)
    {
        cam_pos.x += -sin(yRot / 180 * M_PI) * camMoveSpeed;
        cam_pos.z += cos(yRot / 180 * M_PI) * camMoveSpeed;
    }
    if (moveLeft)
    {
        cam_pos.x += -cos(yRot / 180 * M_PI) * camMoveSpeed;
        cam_pos.z += -sin(yRot / 180 * M_PI) * camMoveSpeed;
    }
    if (moveRight)
    {
        cam_pos.x += cos(yRot / 180 * M_PI) * camMoveSpeed;
        cam_pos.z += sin(yRot / 180 * M_PI) * camMoveSpeed;
    }
    if (moveUp)
    {
        cam_pos.y += camMoveSpeed;
    }
    if (moveDown)
    {
        cam_pos.y -= camMoveSpeed;
    }

    /*ySpeed += G;
    yCam += ySpeed;
    if (yCam < xCam + zCam + 3)
    {
        yCam = xCam + zCam + 3;
        ySpeed = 0.0;
    }*/

    glRotatef(xRot, camRotSpeed, 0, 0);
    glRotatef(yRot, 0, camRotSpeed, 0);
    glTranslatef(-cam_pos.x, -cam_pos.y, -cam_pos.z);
}

Octree root;
Octree* me;
unordered_set<Octree*> render;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_F && action == GLFW_PRESS){
        me = me->GetNext(UP);
        render.insert(me);
    }
    if (key == GLFW_KEY_W)
    {
        if (action == GLFW_PRESS)
            moveForward = true;
        else if (action == GLFW_RELEASE)
            moveForward = false;
    }
    else if (key == GLFW_KEY_S)
    {
        if (action == GLFW_PRESS)
            moveBack = true;
        else if (action == GLFW_RELEASE)
            moveBack = false;
    }
    else if (key == GLFW_KEY_A)
    {
        if (action == GLFW_PRESS)
            moveLeft = true;
        else if (action == GLFW_RELEASE)
            moveLeft = false;
    }
    else if (key == GLFW_KEY_D)
    {
        if (action == GLFW_PRESS)
            moveRight = true;
        else if (action == GLFW_RELEASE)
            moveRight = false;
    }
    else if (key == GLFW_KEY_SPACE)
    {
        if (action == GLFW_PRESS)
            moveUp = true;
        else if (action == GLFW_RELEASE)
            moveUp = false;
    }
    else if (key == GLFW_KEY_LEFT_SHIFT)
    {
        if (action == GLFW_PRESS)
            moveDown = true;
        else if (action == GLFW_RELEASE)
            moveDown = false;
    }
    else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        if (cursor)
        {
            cursor = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else
        {
            cursor = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (!cursor)
    {
        yRot += xpos - prev_mouse_x;
        xRot += ypos - prev_mouse_y;
        if (xRot > 90.0)
            xRot = 90.0;
        if (xRot < -90.0)
            xRot = -90.0;
        prev_mouse_x = xpos;
        prev_mouse_y = ypos;
    }
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    float k = float(height) / width;
    float fov = 0.1;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-fov, fov, -k * fov, k * fov, 0.1, 300);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


int main()
{
    /* Initialize the library */
    WindowLib wl;

    GLFWwindow* window;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1200, 1000, "Voxels", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    gladLoadGL();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.1,0.1,-0.1 / 12,0.1/12,0.1,300);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);

    InitTexture();

    Program prog;
    //prog.AddShader("test.frag", GL_FRAGMENT_SHADER);
    //prog.AddShader("test.vert", GL_VERTEX_SHADER);
    //prog.Use();

    VAO vao;
    vao.AddVBO({
    -2, -2+5, 0,
    2, -2+5, 0,
    2, 2+5, 0,
    -2, 2+5, 0
    });
    vao.AddVBO({
    1,0,0,
    0,1,0,
    0,1,0,
    1,1,1
    });

    vao.AddEBO({
    0, 1, 2,
    0, 2, 3
    });


    //Octree root;
    //Octree* me
    //unordered_set<Octree*> render;

    me = root.Find(vec3(0,3,0));
    me = root.Find(vec3(0,2,0));
    me = root.Find(vec3(0,1,0));
    me = root.Find(vec3(0,0,0));

    render.insert(&root);
    render.insert(me);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(0.6,0.8,1,0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glPushMatrix();
            MoveCamera();
            //render.insert(root.Find(vec3(cam_pos.x, 0., cam_pos.z)));
            DrawTriangle();
            //prog.Use();
            //glUseProgram(0);
            DrawBlock(me->pos);
            for (auto& it: render){
                it->Draw();
            }
        glPopMatrix();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    return 0;
}
