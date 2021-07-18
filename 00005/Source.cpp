//
// COMP 371 Labs Framework
//
// Created by Nicolas Bergeron on 20/06/2019.
//
// Inspired by the following tutorials:
// - https://learnopengl.com/Getting-started/Hello-Window
// - https://learnopengl.com/Getting-started/Hello-Triangle

#include <iostream>


#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
                        // initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices

const char* getVertexShaderSource()
{
    // For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files
    return
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;"
        "layout (location = 1) in vec3 aColor;"
        "uniform mat4 worldMatrix;"
        "uniform mat4 viewMatrix = mat4(1.0);"
        "uniform mat4 projectionMatrix = mat4(1.0);"
        "out vec3 vertexColor;"
        "void main()"
        "{"
        "   vertexColor = aColor;"
        " mat4 trollz = projectionMatrix* viewMatrix* worldMatrix;"
        "   gl_Position = trollz * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
        "}";
}

//            basically everything from lab2 is condensed and can be interpolated into this
//         


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

glm::vec3 triangleArray[] = {
    glm::vec3(0.0f,  0.5f, 0.03f),  // top center position
    glm::vec3(1.0f,  0.0f, 0.0f),  // top center color (red)
    glm::vec3(0.5f, -0.5f, 0.03f),  // bottom right
    glm::vec3(0.0f,  1.0f, 0.0f),  // bottom right color (green)
    glm::vec3(-0.5f, -0.5f, 0.03f),  // bottom left
    glm::vec3(0.0f,  0.0f, 1.0f),  // bottom left color (blue)
};

glm::vec3 squareArray[] = {
    // First Triangle
    glm::vec3(-0.5f, -0.5f, 0.0f),
    glm::vec3(1.0f,  0.0f, 0.0f),
    glm::vec3(0.5f,  0.5f, 0.0f),
    glm::vec3(0.0f,  1.0f, 0.0f),
    glm::vec3(-0.5f,  0.5f, 0.0f),
    glm::vec3(0.0f,  0.0f, 1.0f),

    // Second Triangle
    glm::vec3(-0.5f, -0.5f, 0.0f),
    glm::vec3(1.0f,  0.0f, 0.0f),
        glm::vec3(0.5f, -0.5f, 0.0f),
   
    glm::vec3(0.0f,  1.0f, 0.0f),
     glm::vec3(0.5f,  0.5f, 0.0f),
    glm::vec3(1.0f,  1.0f, 0.0f)
};


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

int createVertexArrayObject(const glm::vec3* vertexArray, int arraySize)
{
    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, arraySize, vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
        3,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        2 * sizeof(glm::vec3), // stride - each vertex contain 2 vec3 (position, color)
        (void*)0             // array buffer offset
    );
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
        3,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(glm::vec3),
        (void*)sizeof(glm::vec3)      // color is offseted a vec3 (comes after position)
    );
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return vertexArrayObject;
}


int main(int argc, char* argv[])
{
    // Initialize GLFW and OpenGL version
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create Window and rendering context using GLFW, resolution is 800x600
    GLFWwindow* window = glfwCreateWindow(800, 600, "Comp371 - Lab 02", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);


    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // backface culling
    // Enable Backface culling
    




    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Compile and link shaders here ...
    int shaderProgram = compileAndLinkShaders();

    // Define and upload geometry to the GPU here ...
    int triangleAO = createVertexArrayObject(triangleArray, sizeof(triangleArray));
    int squareAO = createVertexArrayObject(squareArray, sizeof(squareArray));

    // Variables to be used later in tutorial
    float angle = 0;
    float rotationSpeed = 180.0f;  // 180 degrees per second
    float lastFrameTime = glfwGetTime();

    // Entering Main Loop
    while (!glfwWindowShouldClose(window))
    {
        // Each frame, reset color of each pixel to glClearColor
        glClear(GL_COLOR_BUFFER_BIT);


        // Draw geometry
        glUseProgram(shaderProgram);



        // the id and superid
           glBindVertexArray(triangleAO);



        float dt = glfwGetTime() - lastFrameTime;
        lastFrameTime += dt;

        angle = (angle + rotationSpeed * dt); // angles in degrees, but glm expects radians (conversion below)
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

        GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &rotationMatrix[0][0]);
     
        glDrawArrays(GL_TRIANGLES, 0, 3); // 3 vertices, starting at index 0

















        // the ego
        angle = (angle + rotationSpeed * dt); // angles in degrees, but glm expects radians (conversion below)
        rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &rotationMatrix[0][0]);
       
        
        
        // unbind for double triangle
        glBindVertexArray(squareAO);
       
        
        
        
        glDrawArrays(GL_TRIANGLES, 0, 6); // 6 vertices, starting at index 0

        
                                          
                                          
                                          // add a triangle
        // Top right triangle - translate by (0.5, 0.5, 0.5)
        // Scaling model by 0.25, notice negative value to flip Y axis
        glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.250f, 0.25f));
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.0f));
        
        glm::mat4 worldMatrix = translationMatrix * scalingMatrix;
        
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);




        // add a square
        // Top left square - translate by (-0.5, 0.5, 0.5)
        translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.5f, 0.0f));
        worldMatrix = translationMatrix * scalingMatrix;

        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 6);








        glBindVertexArray(0);
        // End Frame

        glfwSwapBuffers(window);
        glfwPollEvents();





        // Handle inputs
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);


        // View Transform

        // by default, camera is centered at the origin and look towards negative z-axis
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        {
            glm::mat4 viewMatrix = glm::mat4(1.0f);

            GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
            glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
        }

        // shift camera to the left
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) // shift camera to the left
        {
            glm::mat4 viewMatrix = glm::lookAt(glm::vec3(-0.5f, 0.0f, 0.0f),  // eye
                glm::vec3(-0.5f, 0.0f, -1.0f),  // center
                glm::vec3(0.0f, 1.0f, 0.0f));// up

            GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
            glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
        }

        // Projection Transform
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        {
            glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f),  // field of view in degrees
               1.5f / 1.0f,      // aspect ratio
                0.01f, 2.0f);       // near and far (near > 0)

            GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
            glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
        }

        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        {
            glm::mat4 projectionMatrix = glm::ortho(-4.0f, 4.0f,    // left/right
                -3.0f, 3.0f,    // bottom/top
                -100.0f, 100.0f);  // near/far (near == 0 is ok for ortho)

            GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
            glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
        }

    }

    // Shutdown GLFW
    glfwTerminate();

    return 0;
}
