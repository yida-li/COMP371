
#pragma once
#include <iostream>
#include <list>
#include <vector>
#include <cstdlib>
#include <time.h>
#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // cross-platform interface for creating a graphical context,
#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
using namespace std;

const char* getVertexShaderSource();

const char* getFragmentShaderSource();

int compileAndLinkShaders();
int createVertexArrayObject_yida();
int createVertexArrayObject_world();
int createVertexArrayObject_wall();
int createVertexArrayObject_ceiling();
int createVertexArrayObject_floor();
int createVertexArrayObject_NOSE();
int createVertexArrayObject_Alphabet2();
int createVertexArrayObject_Alphabet3();
int createVertexArrayObject_Alphabet4();
int createVertexArrayObject_Alphabet5();

bool initContext();

GLFWwindow* window = NULL;

int main(int argc, char* argv[])
{
    if (!initContext()) return -1;

    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Compile and link shaders here ...
    int shaderProgram = compileAndLinkShaders();

    // We can set the shader once, since we have only one
    glUseProgram(shaderProgram);


    // Camera parameters for view transform
    vec3 cameraPosition(0.0f, 10.0f, 40.0f);

    vec3 cameraArray[] = {

        vec3(0.0f, 10.0f, 40.0f),

        vec3(0.0f, 59.0f, 0.0f)
    };


    vec3 cameraLookAt(0.0f, 0.0f, -1.0f);
    vec3 cameraUp(0.0f, 1.0f, 0.0f);
    vec3 defaultPositions[] = {
                            vec3(-20.0f, 8.0f, 5.0f),  // Y
                            vec3(-10.0f, 8.0f, 5.0f),    // I
                            vec3(0.0f, 8.0f, 5.0f),    // D
                            vec3(10.0f, 8.0f, 5.0f),    // A
                            vec3(20.0f, 8.0f, 5.0f)    // L
    };
    vec3 cubePositions[] = {
                            vec3(-20.0f, 8.0f, 5.0f),  // Y
                            vec3(-10.0f, 8.0f, 5.0f),    // I
                            vec3(0.0f, 8.0f, 5.0f),    // D
                            vec3(10.0f, 8.0f, 5.0f),    // A
                            vec3(20.0f, 8.0f, 5.0f)    // L
    }; // YIDA


    vec3 normalplan(100.0f, 0.05f, 100.0f);


    GLfloat rotations_horizontal[] = {
        0.0f, // Y
        0.0f, // I 
        0.0f, // D
        0.0f, // A
        0.0f  // L
    };

    GLfloat rotations_vertical[] = {
        0.0f, // Y
        0.0f, // I
        0.0f, // D
        0.0f, // A
        0.0f  // L
    };
    GLfloat model_scale[] = {
       1.0f, // Y
       1.0f, // I
       1.0f, // D
       1.0f, // A
       1.0f  // L

    };

    GLfloat world_rotation_horizontal = 0.0f;
    GLfloat world_rotation_vertical = 0.0f;
    int currentModel = 0;
    GLenum mode = GL_TRIANGLES;
    bool caps = false;
    // Other camera parameters
    float cameraSpeed = 1.0f;
    float cameraFastSpeed = 15 * cameraSpeed;
    float cameraHorizontalAngle = 90.0f;

    float cameraVerticalAngle = -10.0f;





    bool  cameraFirstPerson = false; // press 1 or 2 to toggle this variable

    // Set projection matrix for shader, this won't change
    mat4 projectionMatrix = glm::perspective(70.0f,            // field of view in degrees
        800.0f / 600.0f,  // aspect ratio
        0.01f, 100.0f);   // near and far (near > 0)

    GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

    // Set initial view matrix
    mat4 viewMatrix = lookAt(cameraPosition,  // eye
        cameraPosition + cameraLookAt,  // center
        cameraUp); // up

    GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);


    // Define and upload geometry to the GPU here ...

    int vao_world = createVertexArrayObject_world();
    int vao_yida = createVertexArrayObject_yida();
    int vao_wall = createVertexArrayObject_wall();
    int vao_ceiling = createVertexArrayObject_ceiling();
    int vao_floor = createVertexArrayObject_floor();
    int vao_nose = createVertexArrayObject_NOSE();
    int vao_a2 = createVertexArrayObject_Alphabet2();
    int vao_a3 = createVertexArrayObject_Alphabet3();
    int vao_a4 = createVertexArrayObject_Alphabet4();
    int vao_a5 = createVertexArrayObject_Alphabet5();


    // For frame time
    float lastFrameTime = glfwGetTime();
    int lastMouseLeftState = GLFW_RELEASE;
    int lastCapsLockState = GLFW_RELEASE;
    int lastUState = GLFW_RELEASE;
    int lastJState = GLFW_RELEASE;
    int lastLeftState = GLFW_RELEASE;
    int lastRightState = GLFW_RELEASE;
    int lastUpState = GLFW_RELEASE;
    int lastDownState = GLFW_RELEASE;
    int lastRState = GLFW_RELEASE;



    double lastMousePosX, lastMousePosY;
    glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

    // Enable Backface culling
    glEnable(GL_CULL_FACE);

    //Enable Depth Test
    glEnable(GL_DEPTH_TEST);



    // Entering Game Loop
    while (!glfwWindowShouldClose(window))
    {
        // Frame time calculation
        float dt = glfwGetTime() - lastFrameTime;
        lastFrameTime += dt;
        // - Clear Depth Buffer Bit as well
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //************************************************************************************************************
        //************************************************************************************************************
        // Ground model

        glBindVertexArray(vao_floor);
       
        mat4 groundWorldMatrix = translate(mat4(1.0f), vec3(1.0f, -1.0f, 1.0f))
            * scale(mat4(1.0f), vec3(100.0f, 0.05f, 100.0f))
            * rotate(mat4(1.0f), -radians(world_rotation_horizontal), vec3(0.0f, 1.0f, 0.0f))
            * rotate(mat4(1.0f), -radians(world_rotation_vertical), vec3(0.01f, 0.0f, 0.0f));
        GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &groundWorldMatrix[0][0]);
        glDrawArrays(mode, 0, 36);
        glBindVertexArray(vao_world);

        mat4 south
            = scale(mat4(1.0f), vec3(500.01f, 500.0f, 1.0f))
            * translate(mat4(1.0f), vec3(0.0f, 0.0f, 45.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &south[0][0]);
        glDrawArrays(mode, 0, 36);
   

        mat4 Wall2 = groundWorldMatrix
            * scale(mat4(1.0f), vec3(0.01f, 20.0f, 0.01f))
            * translate(mat4(1.0f), vec3(-45.0f, 1.0f, -45.0f))
            //
            * scale(mat4(1.0f), vec3(100.0f, 100.0f, 1.0f));

        for (GLint yAxis = 0; yAxis < 25; yAxis++) { // row
            for (GLint xAxis = 0; xAxis <= 60; xAxis++) { // column

                // alphabet 1 : Y hole 

                if ((xAxis == 4 || xAxis == 5 || xAxis == 6 || xAxis == 3) && (yAxis == 2 || yAxis == 3 || yAxis == 4 || yAxis == 5 || yAxis == 6 || yAxis == 7)) continue;
                if ((xAxis == 10 || xAxis == 12 || xAxis == 11 || xAxis == 9) && (yAxis == 2 || yAxis == 3 || yAxis == 4 || yAxis == 5 || yAxis == 6 || yAxis == 7)) continue;

                if ((xAxis == 14 || xAxis == 15 ) && (yAxis == 2 || yAxis == 3 || yAxis == 4  || yAxis == 6 || yAxis == 7)) continue;


                mat4 yidaWall = Wall2
                    * scale(mat4(1.0f), vec3(0.05f, 0.05f, 0.1f))

                    * translate(mat4(1.0f), vec3(0.0f + xAxis, 0.0f + yAxis, 0.0f));
                glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &yidaWall[0][0]);
                glDrawArrays(mode, 0, 36);
            }
        }

     

        mat4 east
            = scale(mat4(1.0f), vec3(1.0f, 500.0f, 500.0f))
            * translate(mat4(1.0f), vec3(45.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &east[0][0]);
        glDrawArrays(mode, 0, 36);
        mat4 west
            = scale(mat4(1.0f), vec3(1.0f, 500.0f, 500.0f))
            * translate(mat4(1.0f), vec3(-45.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &west[0][0]);
        glDrawArrays(mode, 0, 36);

        glBindVertexArray(vao_ceiling);
        mat4 sky = groundWorldMatrix
            * scale(mat4(1.0f), vec3(0.01f, 20.0f, 0.01f))
            * translate(mat4(1.0f), vec3(0.0f, 60.0f, 0.0f))
            * scale(mat4(1.0f), vec3(1000.2f, 10.4f, 1000.2f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &sky[0][0]);
        glDrawArrays(mode, 0, 36);




        glBindVertexArray(vao_a2);

        vec3 cubePosition = cubePositions[0];
        GLfloat rotationDeg_horizontal = rotations_horizontal[0] * 0.01;
        GLfloat rotationDeg_vertical = rotations_vertical[0] * 0.01;

        mat4 alphabetOne0 = groundWorldMatrix
            * scale(mat4(1.0f), vec3(0.01f, 20.0f, 0.01f))
            * translate(mat4(1.0f), cubePosition)
            * scale(mat4(1.0f), vec3(2.0f, 2.0f, 2.0f))
            * rotate(mat4(1.0f), -radians(rotationDeg_horizontal), vec3(0.0f, 1.0f, 0.0f))
            * rotate(mat4(1.0f), -radians(rotationDeg_vertical), vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &alphabetOne0[0][0]);
        glDrawArrays(mode, 0, 36);

        glBindVertexArray(vao_a2);
        mat4 alphabetOne5 = alphabetOne0
            * translate(mat4(1.0f), vec3(1.0f, 0.0f, 0.0f))
            * scale(mat4(1.0f), vec3(1.2f, 1.2f, 1.2f))
            * rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &alphabetOne5[0][0]);
        glDrawArrays(mode, 0, 36);

        glBindVertexArray(vao_nose);
        mat4 alphabetOne7 = alphabetOne0
            * translate(mat4(1.0f), vec3(1.7f, 0.0f, 0.0f))
            * scale(mat4(1.0f), vec3(0.2f, 0.2f, 0.2f))
            * rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &alphabetOne7[0][0]);
        glDrawArrays(mode, 0, 36);

        mat4 alphabetOne8 = alphabetOne0
            * translate(mat4(1.0f), vec3(1.6f, 0.7f, 0.7f))
            * scale(mat4(1.0f), vec3(0.2f, 0.2f, 0.2f))
            * rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &alphabetOne8[0][0]);
        
        glDrawArrays(mode, 0, 36);

        mat4 alphabetOne9 = alphabetOne0
            * translate(mat4(1.0f), vec3(1.6f, 0.7f, -0.7f))
            * scale(mat4(1.0f), vec3(0.2f, 0.2f, 0.2f))
            * rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &alphabetOne9[0][0]);

        glDrawArrays(mode, 0, 36);


        glBindVertexArray(vao_a3);
        mat4 alphabetOne6 = alphabetOne0
            * translate(mat4(1.0f), vec3(-1.0f, 0.0f, 0.0f))
            * scale(mat4(1.0f), vec3(1.0f, 0.2f, 0.2f))
            * rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &alphabetOne6[0][0]);

        glDrawArrays(mode, 0, 36);

        glBindVertexArray(vao_a2);

         mat4 alphabetOne2 = alphabetOne0
            * translate(mat4(1.0f), vec3(-0.35f, -1.0f, -0.35f))
             * scale(mat4(1.0f), vec3(0.2f, 1.0f, 0.2f))
            * rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &alphabetOne2[0][0]);
        glDrawArrays(mode, 0, 36);
        mat4 alphabetOne3 = alphabetOne0
            * translate(mat4(1.0f), vec3(-0.35f, -1.0f, 0.35f))
            * scale(mat4(1.0f), vec3(0.2f, 1.0f, 0.2f))
            * rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &alphabetOne3[0][0]);
        glDrawArrays(mode, 0, 36);

        mat4 alphabetOne1 = alphabetOne0
            * translate(mat4(1.0f), vec3(0.35f, -1.0f, -0.35f))
            * scale(mat4(1.0f), vec3(0.2f, 1.0f, 0.2f))
            * rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &alphabetOne1[0][0]);
        glDrawArrays(mode, 0, 36);

        mat4 alphabetOne4 = alphabetOne0
            * translate(mat4(1.0f), vec3(0.35f, -1.0f, 0.35f))
            * scale(mat4(1.0f), vec3(0.2f, 1.0f, 0.2f))
            * rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &alphabetOne4[0][0]);
        glDrawArrays(mode, 0, 36);

        /* 
       

        mat4 alphabetOne3 = alphabetOne0
            * translate(mat4(1.0f), vec3(0.0f, 0.0f, 4.0f))
            * rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &alphabetOne3[0][0]);
        glDrawArrays(mode, 0, 36);

        mat4 alphabetOne4 = alphabetOne0
            * translate(mat4(1.0f), vec3(0.0f, 0.0f, 1.0f))
            * rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &alphabetOne4[0][0]);
        glDrawArrays(mode, 0, 36);

        mat4 alphabetOne5 = alphabetOne0
            * translate(mat4(1.0f), vec3(0.0f, 0.0f, -2.0f))
            * rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &alphabetOne5[0][0]);
        glDrawArrays(mode, 0, 36);

        mat4 alphabetOne6 = alphabetOne0
            * translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f))
            * rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &alphabetOne6[0][0]);
        glDrawArrays(mode, 0, 36);

        mat4 alphabetOne7 = alphabetOne0
            * translate(mat4(1.0f), vec3(0.0f, -2.0f, 0.0f))
            * rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &alphabetOne7[0][0]);
        glDrawArrays(mode, 0, 36);

        mat4 alphabetOne8 = alphabetOne0
            * translate(mat4(1.0f), vec3(-2.0f, 1.0f, 0.0f))
            * rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &alphabetOne8[0][0]);
        glDrawArrays(mode, 0, 36);

        mat4 alphabetOne9 = alphabetOne0
            * translate(mat4(1.0f), vec3(-2.0f, 2.0f, 0.0f))
            * rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &alphabetOne9[0][0]);
        glDrawArrays(mode, 0, 36);

        mat4 alphabetOne10 = alphabetOne0
            * translate(mat4(1.0f), vec3(-2.0f, -2.0f, 0.0f))
            * rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &alphabetOne10[0][0]);
        glDrawArrays(mode, 0, 36);

        mat4 alphabetOne11 = alphabetOne0
            * translate(mat4(1.0f), vec3(-1.0f, -2.0f, 0.0f))
            * rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &alphabetOne11[0][0]);
        glDrawArrays(mode, 0, 36);

        */
       




        glBindVertexArray(0);

        // End Frame
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Handle escape input
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        //  Moving camera exercise
        bool fastCam = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
        float currentCameraSpeed = (fastCam) ? cameraFastSpeed : cameraSpeed;

        //  - Calculate mouse motion dx and dy
        //  - Update camera horizontal and vertical angle
        double mousePosX, mousePosY;
        glfwGetCursorPos(window, &mousePosX, &mousePosY);

        // Pan -- change along x when right button pressed
        double dx = 0.0f;
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            dx = mousePosX - lastMousePosX;
        }
        // tilt -- change along y when middle button pressed
        double dy = 0.0f;
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
            dy = mousePosY - lastMousePosY;
        }
        double dz = 0.0f;
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            dz = mousePosY - lastMousePosY;
            cameraPosition += cameraLookAt * float(dz) * 0.03f;
        }
        lastMousePosX = mousePosX;
        lastMousePosY = mousePosY;

        // Convert to spherical coordinates
        const float cameraAngularSpeed = 20.0f;
        cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;
        cameraVerticalAngle -= dy * cameraAngularSpeed * dt;

        // Clamp vertical angle to [-85, 85] degrees
        cameraVerticalAngle = std::max(-85.0f, std::min(85.0f, cameraVerticalAngle));
        if (cameraHorizontalAngle > 360)
        {
            cameraHorizontalAngle -= 360;
        }
        else if (cameraHorizontalAngle < -360)
        {
            cameraHorizontalAngle += 360;
        }

        float theta = radians(cameraHorizontalAngle);
        float phi = radians(cameraVerticalAngle);

        cameraLookAt = vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));
        vec3 cameraSideVector = glm::cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f));

        glm::normalize(cameraSideVector);


        // @TODO 5 = use camera lookat and side vectors to update positions with ASDW
        // adjust code below
        // 
        // World Orientation
        if (lastLeftState == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            world_rotation_horizontal -= 90.0f;
        }
        lastLeftState = glfwGetKey(window, GLFW_KEY_LEFT);

        if (lastRightState == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            world_rotation_horizontal += 90.0f;
        }
        lastRightState = glfwGetKey(window, GLFW_KEY_RIGHT);

        if (lastUpState == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            world_rotation_vertical += 90.0f;
        }
        lastUpState = glfwGetKey(window, GLFW_KEY_UP);

        if (lastDownState == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            world_rotation_vertical -= 90.0f;
        }
        lastDownState = glfwGetKey(window, GLFW_KEY_DOWN);

        if (caps) { // WASD to translate position
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // move camera to the left
            {
                cubePositions[currentModel] += vec3(-1.0f, 0.0f, 0.0f) * currentCameraSpeed * dt;
            }

            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // move camera to the right
            {
                cubePositions[currentModel] -= vec3(-1.0f, 0.0f, 0.0f) * currentCameraSpeed * dt;
            }

            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // move camera up
            {
                cubePositions[currentModel] += vec3(0.0f, 0.0f, 1.0f) * currentCameraSpeed * dt;
            }

            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // move camera down
            {
                cubePositions[currentModel] -= vec3(0.0f, 0.0f, 1.0f) * currentCameraSpeed * dt;
            }
            if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) // move camera down
            {
                cubePositions[currentModel] -= vec3(0.0f, 1.0f, 0.0f) * currentCameraSpeed * dt;
            }
            if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) // move camera down
            {
                cubePositions[currentModel] -= vec3(0.0f, -1.0f, 0.0f) * currentCameraSpeed * dt;
            }
        }
        if (!caps) { // WASD to rotate w/ static position (Ws for vertical rotation | AD for horizontal rotation)
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            {
                rotations_horizontal[currentModel] += 5.0f; // left
            }
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            {
                rotations_horizontal[currentModel] -= 5.0f; // right
            }
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            {
                rotations_vertical[currentModel] += 5.0f; // up
            }
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            {
                rotations_vertical[currentModel] -= 5.0f; // down
            }
        }

        // Clear model changes
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            cubePositions[currentModel] = defaultPositions[currentModel];
            rotations_horizontal[currentModel] = 0.0f;
            rotations_vertical[currentModel] = 0.0f;
            model_scale[currentModel] = 1.0f;
            world_rotation_horizontal = 0.0f;
            world_rotation_vertical = 0.0f;


        }


        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        {
            mode = GL_POINTS;
        }
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        {
            mode = GL_LINES;
        }
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        {
            mode = GL_TRIANGLES;
        }
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        {
            currentModel = 0;
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        {
            currentModel = 1;
        }
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        {
            currentModel = 2;
        }


        if (lastUState == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        {
            model_scale[currentModel] += 0.1;
        }
        lastUState = glfwGetKey(window, GLFW_KEY_U);


        if (lastJState == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        {
            model_scale[currentModel] -= 0.1;
        }
        lastJState = glfwGetKey(window, GLFW_KEY_J);


        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) // move camera down
        {
            currentModel = 3;
        }

        if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) // move camera down
        {
            currentModel = 4;
        }
        if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) // move camera down
        {
            cameraVerticalAngle = -90.0f;

            cameraPosition = cameraArray[1];
        }
        if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS) // move camera down
        {
            cameraVerticalAngle = -20.0f;
            cameraPosition = cameraArray[0];
            world_rotation_horizontal = 0.0f;
        }

        if (lastCapsLockState == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS)
        {
            caps = !caps;
        }
        lastCapsLockState = glfwGetKey(window, GLFW_KEY_CAPS_LOCK);

        // TODO 6
        // Set the view matrix for first and third person cameras
        // - In first person, camera lookat is set like below
        // - In third person, camera position is on a sphere looking towards center

        mat4 viewMatrix = mat4(1.0);

        if (cameraFirstPerson) {
            viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
        }
        else {
            float radius = 5.0f;
            glm::vec3 position = cameraPosition - radius * cameraLookAt;
            viewMatrix = lookAt(position, position + cameraLookAt, cameraUp);
        }

        GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);


    }


    // Shutdown GLFW
    glfwTerminate();

    return 0;
}

const char* getVertexShaderSource()
{
    // For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files
    return
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;"
        "layout (location = 1) in vec3 aColor;"
        ""
        "uniform mat4 worldMatrix;"
        "uniform mat4 viewMatrix = mat4(1.0);"  // default value for view matrix (identity)
        "uniform mat4 projectionMatrix = mat4(1.0);"
        ""
        "out vec3 vertexColor;"
        "void main()"
        "{"
        "   vertexColor = aColor;"
        "   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;"
        "   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
        "}";
}

const char* getFragmentShaderSource()
{
    return
        "#version 330 core\n"
        "in vec3 vertexColor;"
        "out vec4 FragColor;"
        "void main()"
        "{"
        "   FragColor = vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);"
        "}";
}

int compileAndLinkShaders()
{
    // compile and link shader program
    // return shader program id
    // ------------------------------------

    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderSource = getVertexShaderSource();
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderSource = getFragmentShaderSource();
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

int createVertexArrayObject_yida()
{
    // Cube model
    vec3 vertexArray[] = {  // position,                            color
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.51f, 0.29f,		0.65f), //haze
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.51f, 0.29f,		0.65f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.51f, 0.29f,		0.65f),

        vec3(-0.5f,-0.5f,-0.5f), vec3(0.51f, 0.29f,		0.65f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.51f, 0.29f,		0.65f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.51f, 0.29f,		0.65f),

        vec3(0.5f, 0.5f,-0.5f), vec3(0.29f, 0.38f,	0.65f), // ocean
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.29f, 0.38f,	0.65f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.29f, 0.38f,	0.65f),

        vec3(0.5f, 0.5f,-0.5f), vec3(0.29f, 0.38f,	0.65f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.29f, 0.38f,	0.65f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.29f, 0.38f,	0.65f),

        vec3(0.5f,-0.5f, 0.5f), vec3(0.4f, 0.4f, 0.4f), // bottom - turquoise
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.4f, 0.4f, 0.4f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.4f, 0.4f, 0.4f),

        vec3(0.5f,-0.5f, 0.5f), vec3(0.4f, 0.4f, 0.4f),
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.4f, 0.4f, 0.4f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.4f, 0.4f, 0.4f),

        vec3(-0.5f, 0.5f, 0.5f), vec3(0.2f, 0.2f, 0.2f), // near - green
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.2f, 0.2f, 0.2f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.2f, 0.2f, 0.2f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.2f, 0.2f, 0.2f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.2f, 0.2f, 0.2f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.2f, 0.2f, 0.2f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.3f,0.65f ,0.29f), // right - teal
        vec3(0.5f,-0.5f,-0.5f), vec3(0.3f,0.65f ,0.29f),
        vec3(0.5f, 0.5f,-0.5f), vec3(0.3f,0.65f ,0.29f),

        vec3(0.5f,-0.5f,-0.5f), vec3(0.3f,0.65f ,0.29f),
        vec3(0.5f, 0.5f, 0.5f), vec3(0.3f,0.65f ,0.29f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.3f,0.65f ,0.29f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.5f, 0.5f, 0.5f), // pink overdrive
        vec3(0.5f, 0.5f,-0.5f), vec3(0.5f, 0.5f, 0.5f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.5f, 0.5f, 0.5f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.5f, 0.5f, 0.5f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.5f, 0.5f, 0.5f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.5f, 0.5f, 0.5f),

    };


    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);


    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
        3,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        2 * sizeof(vec3), // stride - each vertex contain 2 vec3 (position, color)
        (void*)0             // array buffer offset
    );
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
        3,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(vec3),
        (void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
    );
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBindVertexArray(0);

    return vertexArrayObject;
}

int createVertexArrayObject_wall()
{
    // wall color Darknesssun   0.160f, 0.2f, 0.011f
    vec3 vertexArray[] = {  // position,                            color
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.160f, 0.2f, 0.011f), //left 
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.160f, 0.2f, 0.011f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.160f, 0.2f, 0.011f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.160f, 0.2f, 0.011f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.160f, 0.2f, 0.011f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.160f, 0.2f, 0.011f),

        vec3(0.5f, 0.5f,-0.5f), vec3(0.160f, 0.2f, 0.011f), // far 
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.160f, 0.2f, 0.011f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.160f, 0.2f, 0.011f),
        vec3(0.5f, 0.5f,-0.5f), vec3(0.160f, 0.2f, 0.011f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.160f, 0.2f, 0.011f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.160f, 0.2f, 0.011f),

        vec3(0.5f,-0.5f, 0.5f), vec3(0.160f, 0.2f, 0.011f), // bottom 
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.160f, 0.2f, 0.011f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.160f, 0.2f, 0.011f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.160f, 0.2f, 0.011f),
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.160f, 0.2f, 0.011f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.160f, 0.2f, 0.011f),

        vec3(-0.5f, 0.5f, 0.5f), vec3(0.160f, 0.2f, 0.011f), // near 
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.160f, 0.2f, 0.011f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.160f, 0.2f, 0.011f),
        vec3(0.5f, 0.5f, 0.5f), vec3(0.160f, 0.2f, 0.011f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.160f, 0.2f, 0.011f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.160f, 0.2f, 0.011f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.160f, 0.2f, 0.011f), // right 
        vec3(0.5f,-0.5f,-0.5f), vec3(0.160f, 0.2f, 0.011f),
        vec3(0.5f, 0.5f,-0.5f), vec3(0.160f, 0.2f, 0.011f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.160f, 0.2f, 0.011f),
        vec3(0.5f, 0.5f, 0.5f), vec3(0.160f, 0.2f, 0.011f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.160f, 0.2f, 0.011f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.160f, 0.2f, 0.011f), // top     green 
        vec3(0.5f, 0.5f,-0.5f), vec3(0.160f, 0.2f, 0.011f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.160f, 0.2f, 0.011f),
        vec3(0.5f, 0.5f, 0.5f), vec3(0.160f, 0.2f, 0.011f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.160f, 0.2f, 0.011f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.160f, 0.2f, 0.011f),

    };


    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);


    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
        3,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        2 * sizeof(vec3), // stride - each vertex contain 2 vec3 (position, color)
        (void*)0             // array buffer offset
    );
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
        3,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(vec3),
        (void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
    );
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBindVertexArray(0);

    return vertexArrayObject;
}
int createVertexArrayObject_world()
{
    // world color 
    vec3 vertexArray[] = {  // position,                            color
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.878f, 0.8156f, 0.807f), //left - red
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.878f, 0.8156f, 0.807f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.878f, 0.8156f, 0.807f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.878f, 0.8156f, 0.807f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.878f, 0.8156f, 0.807f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.878f, 0.8156f, 0.807f),

        vec3(0.5f, 0.5f,-0.5f), vec3(0.878f, 0.8156f, 0.807f), // far - blue
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.878f, 0.8156f, 0.807f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.878f, 0.8156f, 0.807f),
        vec3(0.5f, 0.5f,-0.5f), vec3(0.878f, 0.8156f, 0.807f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.878f, 0.8156f, 0.807f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.878f, 0.8156f, 0.807f),

        vec3(0.5f,-0.5f, 0.5f), vec3(0.878f, 0.8156f, 0.807f), // bottom - turquoise
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.878f, 0.8156f, 0.807f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.878f, 0.8156f, 0.807f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.878f, 0.8156f, 0.807f),
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.878f, 0.8156f, 0.807f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.878f, 0.8156f, 0.807f),

        vec3(-0.5f, 0.5f, 0.5f), vec3(0.878f, 0.8156f, 0.807f), // near - green
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.878f, 0.8156f, 0.807f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.878f, 0.8156f, 0.807f),
        vec3(0.5f, 0.5f, 0.5f), vec3(0.878f, 0.8156f, 0.807f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.878f, 0.8156f, 0.807f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.878f, 0.8156f, 0.807f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.878f, 0.8156f, 0.807f), // right - purple
        vec3(0.5f,-0.5f,-0.5f), vec3(0.878f, 0.8156f, 0.807f),
        vec3(0.5f, 0.5f,-0.5f), vec3(0.878f, 0.8156f, 0.807f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.878f, 0.8156f, 0.807f),
        vec3(0.5f, 0.5f, 0.5f), vec3(0.878f, 0.8156f, 0.807f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.878f, 0.8156f, 0.807f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.878f, 0.8156f, 0.807f), // top    green
        vec3(0.5f, 0.5f,-0.5f), vec3(0.878f, 0.8156f, 0.807f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.878f, 0.8156f, 0.807f),
        vec3(0.5f, 0.5f, 0.5f), vec3(0.878f, 0.8156f, 0.807f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.878f, 0.8156f, 0.807f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.878f, 0.8156f, 0.807f),

        // AXIS INDICATORS

          vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), // x axis - red
          vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), //

          vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), // y axis - green
          vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), //

          vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), // z axis - blue
          vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f) //

    };


    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);


    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
        3,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        2 * sizeof(vec3), // stride - each vertex contain 2 vec3 (position, color)
        (void*)0             // array buffer offset
    );
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
        3,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(vec3),
        (void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
    );
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBindVertexArray(0);

    return vertexArrayObject;
}
int createVertexArrayObject_ceiling()
{
    //   Sky model 99, 81, 80      0.388f , 0.317f, 0.313f
    vec3 vertexArray[] = {  // position,                            color
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f ,0.258f, 0.709f), //left
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f ,0.258f, 0.709f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f ,0.258f, 0.709f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f ,0.258f, 0.709f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f ,0.258f, 0.709f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f ,0.258f, 0.709f),

        vec3(0.5f, 0.5f,-0.5f), vec3(0.0f ,0.258f, 0.709f), // far
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f ,0.258f, 0.709f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f ,0.258f, 0.709f),
        vec3(0.5f, 0.5f,-0.5f), vec3(0.0f ,0.258f, 0.709f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.0f ,0.258f, 0.709f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f ,0.258f, 0.709f),

        vec3(0.5f,-0.5f, 0.5f), vec3(0.0f ,0.258f, 0.709f), // bottom 
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f ,0.258f, 0.709f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.0f ,0.258f, 0.709f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.0f ,0.258f, 0.709f),
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f ,0.258f, 0.709f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f ,0.258f, 0.709f),

        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f ,0.258f, 0.709f), // near 
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f ,0.258f, 0.709f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.0f ,0.258f, 0.709f),
        vec3(0.5f, 0.5f, 0.5f), vec3(0.0f ,0.258f, 0.709f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f ,0.258f, 0.709f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.0f ,0.258f, 0.709f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.0f ,0.258f, 0.709f), // right 
        vec3(0.5f,-0.5f,-0.5f), vec3(0.0f ,0.258f, 0.709f),
        vec3(0.5f, 0.5f,-0.5f), vec3(0.0f ,0.258f, 0.709f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.0f ,0.258f, 0.709f),
        vec3(0.5f, 0.5f, 0.5f), vec3(0.0f ,0.258f, 0.709f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.0f ,0.258f, 0.709f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.0f ,0.258f, 0.709f), // top   
        vec3(0.5f, 0.5f,-0.5f), vec3(0.0f ,0.258f, 0.709f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f ,0.258f, 0.709f),
        vec3(0.5f, 0.5f, 0.5f), vec3(0.0f ,0.258f, 0.709f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f ,0.258f, 0.709f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f ,0.258f, 0.709f),


    };


    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);


    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
        3,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        2 * sizeof(vec3), // stride - each vertex contain 2 vec3 (position, color)
        (void*)0             // array buffer offset
    );
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
        3,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(vec3),
        (void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
    );
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBindVertexArray(0);

    return vertexArrayObject;
}
int createVertexArrayObject_floor()
{
    //   ground 94, 47, 43  0.368f,0.184f,0.1686f
    vec3 vertexArray[] = {  // position,                            color
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.368f,0.184f,0.1686f), //left
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.368f,0.184f,0.1686f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.368f,0.184f,0.1686f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.368f,0.184f,0.1686f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.368f,0.184f,0.1686f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.368f,0.184f,0.1686f),

        vec3(0.5f, 0.5f,-0.5f), vec3(0.368f,0.184f,0.1686f), // far
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.368f,0.184f,0.1686f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.368f,0.184f,0.1686f),
        vec3(0.5f, 0.5f,-0.5f), vec3(0.368f,0.184f,0.1686f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.368f,0.184f,0.1686f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.368f,0.184f,0.1686f),

        vec3(0.5f,-0.5f, 0.5f), vec3(0.368f,0.184f,0.1686f), // bottom 
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.368f,0.184f,0.1686f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.368f,0.184f,0.1686f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.368f,0.184f,0.1686f),
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.368f,0.184f,0.1686f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.368f,0.184f,0.1686f),

        vec3(-0.5f, 0.5f, 0.5f), vec3(0.368f,0.184f,0.1686f), // near 
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.368f,0.184f,0.1686f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.368f,0.184f,0.1686f),
        vec3(0.5f, 0.5f, 0.5f), vec3(0.368f,0.184f,0.1686f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.368f,0.184f,0.1686f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.368f,0.184f,0.1686f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.368f,0.184f,0.1686f), // right 
        vec3(0.5f,-0.5f,-0.5f), vec3(0.368f,0.184f,0.1686f),
        vec3(0.5f, 0.5f,-0.5f), vec3(0.368f,0.184f,0.1686f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.368f,0.184f,0.1686f),
        vec3(0.5f, 0.5f, 0.5f), vec3(0.368f,0.184f,0.1686f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.368f,0.184f,0.1686f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.368f,0.184f,0.1686f), // top   
        vec3(0.5f, 0.5f,-0.5f), vec3(0.368f,0.184f,0.1686f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.368f,0.184f,0.1686f),
        vec3(0.5f, 0.5f, 0.5f), vec3(0.368f,0.184f,0.1686f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.368f,0.184f,0.1686f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.368f,0.184f,0.1686f),


    };


    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);


    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
        3,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        2 * sizeof(vec3), // stride - each vertex contain 2 vec3 (position, color)
        (void*)0             // array buffer offset
    );
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
        3,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(vec3),
        (void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
    );
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBindVertexArray(0);

    return vertexArrayObject;
}



int  createVertexArrayObject_NOSE() {

    // DARKISH BROWN       66, 44, 43          0.258f, 0.1725f,0.1725f,
    vec3 vertexArray[] = {  // position,                            color
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.258f, 0.1725f,0.1725f), //left 
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.258f, 0.1725f,0.1725f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.258f, 0.1725f,0.1725f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.258f, 0.1725f,0.1725f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.258f, 0.1725f,0.1725f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.258f, 0.1725f,0.1725f),

        vec3(0.5f, 0.5f,-0.5f), vec3(0.258f, 0.1725f,0.1725f), // far 
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.258f, 0.1725f,0.1725f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.258f, 0.1725f,0.1725f),
        vec3(0.5f, 0.5f,-0.5f), vec3(0.258f, 0.1725f,0.1725f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.258f, 0.1725f,0.1725f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.258f, 0.1725f,0.1725f),

        vec3(0.5f,-0.5f, 0.5f), vec3(0.258f, 0.1725f,0.1725f), // bottom 
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.258f, 0.1725f,0.1725f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.258f, 0.1725f,0.1725f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.258f, 0.1725f,0.1725f),
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.258f, 0.1725f,0.1725f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.258f, 0.1725f,0.1725f),

        vec3(-0.5f, 0.5f, 0.5f), vec3(0.258f, 0.1725f,0.1725f), // near 
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.258f, 0.1725f,0.1725f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.258f, 0.1725f,0.1725f),
        vec3(0.5f, 0.5f, 0.5f), vec3(0.258f, 0.1725f,0.1725f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.258f, 0.1725f,0.1725f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.258f, 0.1725f,0.1725f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.258f, 0.1725f,0.1725f), // right 
        vec3(0.5f,-0.5f,-0.5f), vec3(0.258f, 0.1725f,0.1725f),
        vec3(0.5f, 0.5f,-0.5f), vec3(0.258f, 0.1725f,0.1725f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.258f, 0.1725f,0.1725f),
        vec3(0.5f, 0.5f, 0.5f), vec3(0.258f, 0.1725f,0.1725f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.258f, 0.1725f,0.1725f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.258f, 0.1725f,0.1725f), // top     green 
        vec3(0.5f, 0.5f,-0.5f), vec3(0.258f, 0.1725f,0.1725f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.258f, 0.1725f,0.1725f),
        vec3(0.5f, 0.5f, 0.5f), vec3(0.258f, 0.1725f,0.1725f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.258f, 0.1725f,0.1725f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.258f, 0.1725f,0.1725f),

    };


    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);


    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
        3,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        2 * sizeof(vec3), // stride - each vertex contain 2 vec3 (position, color)
        (void*)0             // array buffer offset
    );
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
        3,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(vec3),
        (void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
    );
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBindVertexArray(0);

    return vertexArrayObject;
}
int  createVertexArrayObject_Alphabet2() {
    //    violet       0.858f,0.203f,0.92f
    vec3 vertexArray[] = {  // 
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.858f,0.203f,0.92f), //left 
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.858f,0.203f,0.92f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.858f,0.203f,0.92f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.858f,0.203f,0.92f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.858f,0.203f,0.92f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.858f,0.203f,0.92f),

        vec3(0.5f, 0.5f,-0.5f), vec3(0.858f,0.203f,0.92f), // far 
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.858f,0.203f,0.92f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.858f,0.203f,0.92f),
        vec3(0.5f, 0.5f,-0.5f), vec3(0.858f,0.203f,0.92f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.858f,0.203f,0.92f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.858f,0.203f,0.92f),

        vec3(0.5f,-0.5f, 0.5f), vec3(0.858f,0.203f,0.92f), // bottom 
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.858f,0.203f,0.92f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.858f,0.203f,0.92f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.858f,0.203f,0.92f),
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.858f,0.203f,0.92f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.858f,0.203f,0.92f),

        vec3(-0.5f, 0.5f, 0.5f), vec3(0.858f,0.203f,0.92f), // near 
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.858f,0.203f,0.92f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.858f,0.203f,0.92f),
        vec3(0.5f, 0.5f, 0.5f), vec3(0.858f,0.203f,0.92f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.858f,0.203f,0.92f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.858f,0.203f,0.92f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.858f,0.203f,0.92f), // right 
        vec3(0.5f,-0.5f,-0.5f), vec3(0.858f,0.203f,0.92f),
        vec3(0.5f, 0.5f,-0.5f), vec3(0.858f,0.203f,0.92f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.858f,0.203f,0.92f),
        vec3(0.5f, 0.5f, 0.5f), vec3(0.858f,0.203f,0.92f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.858f,0.203f,0.92f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.858f,0.203f,0.92f), // top     green 
        vec3(0.5f, 0.5f,-0.5f), vec3(0.858f,0.203f,0.92f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.858f,0.203f,0.92f),
        vec3(0.5f, 0.5f, 0.5f), vec3(0.858f,0.203f,0.92f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.858f,0.203f,0.92f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.858f,0.203f,0.92f),

    };


    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);


    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
        3,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        2 * sizeof(vec3), // stride - each vertex contain 2 vec3 (position, color)
        (void*)0             // array buffer offset
    );
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
        3,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(vec3),
        (void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
    );
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBindVertexArray(0);

    return vertexArrayObject;
}
int  createVertexArrayObject_Alphabet3() {
    // the color for alphabet3 is burgundy 0.709f,0.0f,0.082f
    vec3 vertexArray[] = {
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.709f,0.0f,0.082f), //left 
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.709f,0.0f,0.082f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.709f,0.0f,0.082f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.709f,0.0f,0.082f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.709f,0.0f,0.082f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.709f,0.0f,0.082f),

        vec3(0.5f, 0.5f,-0.5f), vec3(0.709f,0.0f,0.082f), // far 
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.709f,0.0f,0.082f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.709f,0.0f,0.082f),
        vec3(0.5f, 0.5f,-0.5f), vec3(0.709f,0.0f,0.082f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.709f,0.0f,0.082f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.709f,0.0f,0.082f),

        vec3(0.5f,-0.5f, 0.5f), vec3(0.709f,0.0f,0.082f), // bottom 
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.709f,0.0f,0.082f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.709f,0.0f,0.082f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.709f,0.0f,0.082f),
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.709f,0.0f,0.082f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.709f,0.0f,0.082f),

        vec3(-0.5f, 0.5f, 0.5f), vec3(0.709f,0.0f,0.082f), // near 
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.709f,0.0f,0.082f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.709f,0.0f,0.082f),
        vec3(0.5f, 0.5f, 0.5f), vec3(0.709f,0.0f,0.082f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.709f,0.0f,0.082f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.709f,0.0f,0.082f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.709f,0.0f,0.082f), // right 
        vec3(0.5f,-0.5f,-0.5f), vec3(0.709f,0.0f,0.082f),
        vec3(0.5f, 0.5f,-0.5f), vec3(0.709f,0.0f,0.082f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.709f,0.0f,0.082f),
        vec3(0.5f, 0.5f, 0.5f), vec3(0.709f,0.0f,0.082f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.709f,0.0f,0.082f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.709f,0.0f,0.082f), // top     green 
        vec3(0.5f, 0.5f,-0.5f), vec3(0.709f,0.0f,0.082f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.709f,0.0f,0.082f),
        vec3(0.5f, 0.5f, 0.5f), vec3(0.709f,0.0f,0.082f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.709f,0.0f,0.082f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.709f,0.0f,0.082f),

    };


    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);


    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
        3,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        2 * sizeof(vec3), // stride - each vertex contain 2 vec3 (position, color)
        (void*)0             // array buffer offset
    );
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
        3,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(vec3),
        (void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
    );
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBindVertexArray(0);

    return vertexArrayObject;
}
int  createVertexArrayObject_Alphabet4() {
    // the color for alphabet4 is ocean 0.082f,0.0f,0.709f
    vec3 vertexArray[] = {
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.082f,0.0f,0.709f), //left 
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.082f,0.0f,0.709f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.082f,0.0f,0.709f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.082f,0.0f,0.709f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.082f,0.0f,0.709f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.082f,0.0f,0.709f),

        vec3(0.5f, 0.5f,-0.5f), vec3(0.082f,0.0f,0.709f), // far 
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.082f,0.0f,0.709f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.082f,0.0f,0.709f),
        vec3(0.5f, 0.5f,-0.5f), vec3(0.082f,0.0f,0.709f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.082f,0.0f,0.709f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.082f,0.0f,0.709f),

        vec3(0.5f,-0.5f, 0.5f), vec3(0.082f,0.0f,0.709f), // bottom 
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.082f,0.0f,0.709f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.082f,0.0f,0.709f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.082f,0.0f,0.709f),
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.082f,0.0f,0.709f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.082f,0.0f,0.709f),

        vec3(-0.5f, 0.5f, 0.5f), vec3(0.082f,0.0f,0.709f), // near 
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.082f,0.0f,0.709f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.082f,0.0f,0.709f),
        vec3(0.5f, 0.5f, 0.5f), vec3(0.082f,0.0f,0.709f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.082f,0.0f,0.709f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.082f,0.0f,0.709f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.082f,0.0f,0.709f), // right 
        vec3(0.5f,-0.5f,-0.5f), vec3(0.082f,0.0f,0.709f),
        vec3(0.5f, 0.5f,-0.5f), vec3(0.082f,0.0f,0.709f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.082f,0.0f,0.709f),
        vec3(0.5f, 0.5f, 0.5f), vec3(0.082f,0.0f,0.709f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.082f,0.0f,0.709f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.082f,0.0f,0.709f), // top     green 
        vec3(0.5f, 0.5f,-0.5f), vec3(0.082f,0.0f,0.709f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.082f,0.0f,0.709f),
        vec3(0.5f, 0.5f, 0.5f), vec3(0.082f,0.0f,0.709f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.082f,0.0f,0.709f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.082f,0.0f,0.709f),

    };


    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);


    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
        3,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        2 * sizeof(vec3), // stride - each vertex contain 2 vec3 (position, color)
        (void*)0             // array buffer offset
    );
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
        3,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(vec3),
        (void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
    );
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBindVertexArray(0);

    return vertexArrayObject;
}
int  createVertexArrayObject_Alphabet5() {
    // the color for alphabet5 is teal 0.0f,0.462f,0.709f
    vec3 vertexArray[] = {
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f,0.462f,0.709f), //left 
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f,0.462f,0.709f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f,0.462f,0.709f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f,0.462f,0.709f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f,0.462f,0.709f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f,0.462f,0.709f),

        vec3(0.5f, 0.5f,-0.5f), vec3(0.0f,0.462f,0.709f), // far 
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f,0.462f,0.709f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f,0.462f,0.709f),
        vec3(0.5f, 0.5f,-0.5f), vec3(0.0f,0.462f,0.709f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.0f,0.462f,0.709f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f,0.462f,0.709f),

        vec3(0.5f,-0.5f, 0.5f), vec3(0.0f,0.462f,0.709f), // bottom 
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f,0.462f,0.709f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.0f,0.462f,0.709f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.0f,0.462f,0.709f),
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f,0.462f,0.709f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f,0.462f,0.709f),

        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f,0.462f,0.709f), // near 
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f,0.462f,0.709f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.0f,0.462f,0.709f),
        vec3(0.5f, 0.5f, 0.5f), vec3(0.0f,0.462f,0.709f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f,0.462f,0.709f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.0f,0.462f,0.709f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.0f,0.462f,0.709f), // right 
        vec3(0.5f,-0.5f,-0.5f), vec3(0.0f,0.462f,0.709f),
        vec3(0.5f, 0.5f,-0.5f), vec3(0.0f,0.462f,0.709f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.0f,0.462f,0.709f),
        vec3(0.5f, 0.5f, 0.5f), vec3(0.0f,0.462f,0.709f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.0f,0.462f,0.709f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.0f,0.462f,0.709f), // top     green 
        vec3(0.5f, 0.5f,-0.5f), vec3(0.0f,0.462f,0.709f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f,0.462f,0.709f),
        vec3(0.5f, 0.5f, 0.5f), vec3(0.0f,0.462f,0.709f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f,0.462f,0.709f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f,0.462f,0.709f),

    };


    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);


    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
        3,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        2 * sizeof(vec3), // stride - each vertex contain 2 vec3 (position, color)
        (void*)0             // array buffer offset
    );
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
        3,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(vec3),
        (void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
    );
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBindVertexArray(0);

    return vertexArrayObject;
}
bool initContext() {     // Initialize GLFW and OpenGL version
    glfwInit();

#if defined(PLATFORM_OSX)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
    // On windows, we set OpenGL version to 2.1, to support more hardware
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif

    // Create Window and rendering context using GLFW, resolution is 800x600
    window = glfwCreateWindow(1024, 768, "Eden's Infinity", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);

    // @TODO 3 - Disable mouse cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glViewport(0, 0, 1024, 768);
    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return false;
    }
    return true;
}
