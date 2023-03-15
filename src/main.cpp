#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader.h"
#include "camera.h"
#include "mesh.h"
#include "bone.h"

#include <iostream>

const float PI = 3.14159265359;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
// void mouseMoveEvent(GLFWwindow* window);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void cursorPosCallback(GLFWwindow* window, double xPos, double yPos);
void scrollCallback(GLFWwindow* window, double x, double y);
unsigned int loadTexture(const char *path);
vector<Vertex> createBoudinMesh();
void drawBoudinMesh();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera *camera;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(-3.0f, -3.0f, -1.0f);
glm::vec3 lightPos2(-5.0f, -5.0f, -5.0f);


// matrices
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

glm::vec3 floorPos(0.0f, 8.0f, 0.0f);
glm::vec3 boudinPos(0.0f, 2.0f, 0.0f);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    Shader* shaderCube, *shaderLight, *shaderDepth;
	shaderCube = new Shader("shaders/cube_vs.glsl", "shaders/cube_fs.glsl");
    shaderLight = new Shader("shaders/light1_vs.glsl", "shaders/light1_fs.glsl");
    shaderDepth = new Shader("shaders/shadow_map_depth_vs.glsl", "shaders/shadow_map_depth_fs.glsl");

    // set up the camera
    // -----------------
    camera = new Camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 0.0f, 0.0f));

    // register the callback functions
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetScrollCallback(window, scrollCallback);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float cubeVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  3.0f,  0.0f),
        glm::vec3(-2.0f,  0.0f,  2.0f),
        glm::vec3(-3.5f,  2.5f,  -5.0f),
        glm::vec3(0.0f,  -2.0f,  -2.0f)
    };

    float floorVertices[] = {
        50.0f, -5.0f,  50.0f,  0.0f,  1.0f,  0.0f, 50.0f, 0.0f,
        -50.0f, -5.0f,  50.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
        -50.0f, -5.0f, -50.0f,  0.0f,  1.0f,  0.0f, 0.0f, 50.0f,

        50.0f, -5.0f,  50.0f,  0.0f,  1.0f,  0.0f, 50.0f, 0.0f,
        -50.0f, -5.0f, -50.0f,  0.0f,  1.0f,  0.0f, 0.0f, 50.0f,
        50.0f, -5.0f, -50.0f,  0.0f,  1.0f,  0.0f, 50.0f, 50.0f,
    };


    // cubes VAO & VBO
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    // pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // light cube VAO (light cube VBO = cube VBO, as they aer the same objects)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // floor VAO & VBO
    unsigned int VBO_floor, VAO_floor;
    glGenVertexArrays(1, &VAO_floor);
    glGenBuffers(1, &VBO_floor);
    glBindVertexArray(VAO_floor);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_floor);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);
    // pos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    // normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    // texture
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);
    
    unsigned int woodTexture = loadTexture("textures/wood.png");

    // uncomment this call to draw in wireframe polygons
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // configure depth map FBO
    // -----------------------
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    shaderCube->use();
    shaderCube->setInt("diffuseTexture", 0);
    shaderCube->setInt("shadowMap", 1);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);
        // mouseMoveEvent(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float lightX = -3.0f * sin(glfwGetTime());
        float lightY = sin(glfwGetTime() / 2.0f) * 1.0f;
        float lightZ = 5.0f * cos(glfwGetTime());
        lightPos = glm::vec3(lightX, lightY, lightZ);

        // 1. render depth of scene to texture (from light's perspective)
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 1.0f, far_plane = 7.5f;
        //lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); 
        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        // render scene from light's point of view
        shaderDepth->use();
        shaderDepth->setMat4("lightSpaceMatrix", lightSpaceMatrix);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glClear(GL_DEPTH_BUFFER_BIT);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, woodTexture);
            // render floor
            glBindVertexArray(VAO_floor);
            model = glm::mat4(1.0f);
            model = glm::translate(model, floorPos);
            shaderDepth->setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // render cubes
            glBindVertexArray(VAO);
            for (unsigned int i = 0; i < 5; i++)
            {
                // shaderCube->setFloat("roughness", glm::clamp((float)i/(float)5.0, 0.05f, 1.0f));
                // shaderCube->setFloat("roughness", 0.0f);
                model = glm::mat4(1.0f);
                model = glm::translate(model, cubePositions[i]);
                float angle = 35.0f * i;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                shaderDepth->setMat4("model", model);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // reset viewport as we used it with shadow size
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 2. render scene as normal using the generated depth/shadow map 
        // shader
        shaderCube->use();
        // shaderCube->setInt("diffuseTexture", 0);
        // shaderCube->setInt("shadowMap", 1);
        shaderCube->setVec3("objectColor", 0.1f, 0.5f, 1.0f);
        shaderCube->setVec3("lightPos", lightPos);
        shaderCube->setVec3("viewPos", camera->_position);
        shaderCube->setMat4("lightSpaceMatrix", lightSpaceMatrix);

        // texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, woodTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);

        // camera
        projection = glm::perspective(glm::radians(camera->_zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shaderCube->setMat4("projection", projection);
        view = camera->getView();
        shaderCube->setMat4("view", view);

        // render floor
        glBindVertexArray(VAO_floor);
        model = glm::mat4(1.0f);
        model = glm::translate(model, floorPos);
        shaderCube->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // render cubes
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 5; i++)
        {
            // shaderCube->setFloat("roughness", glm::clamp((float)i/(float)5.0, 0.05f, 1.0f));
            // shaderCube->setFloat("roughness", 0.0f);
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 35.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shaderCube->setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // render le boudin
        model = glm::mat4(1.0f);
        model = glm::translate(model, boudinPos);
        shaderCube->setMat4("model", model);
        drawBoudinMesh();

        // render light (a small cube light)
        shaderLight->use();
        shaderLight->setMat4("projection", projection);
        shaderLight->setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        shaderLight->setMat4("model", model);
        // model = glm::mat4(1.0f);
        // model = glm::translate(model, lightPos2);
        // model = glm::scale(model, glm::vec3(0.2f));
        // shaderLight->setMat4("model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &VAO_floor);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}


// create vertices of a BOUDIN to animate it
vector<Vertex> createBoudinMesh() {
    vector<Vertex> vertices;

    const int sides = 20; // number of sides
    const float radius = 0.2f; // radius of the mesh
    const float height = 2.0f; // height of the mesh

    for (int i = 0; i < sides; i++) {
        float theta1 = (i / (float)sides) * 2.0f * PI; // start angle of the side
        float theta2 = ((i + 1) / (float)sides) * 2.0f * PI; // end angle of the side

        // vertices of the side
        Vertex v1 = {
            {cos(theta1) * radius, height / 2, sin(theta1) * radius},
            {cos(theta1), 0.0f, sin(theta1)},
            {cos(theta1), 0.0f},
            {cos(theta1), 0.0f, sin(theta1)},
            {cos(theta1), 0.0f, sin(theta1)}
        };
        Vertex v2 = {
            {cos(theta2) * radius, height / 2, sin(theta2) * radius},
            {cos(theta2), 0.0f, sin(theta2)},
            {cos(theta2), 0.0f},
            {cos(theta2), 0.0f, sin(theta2)},
            {cos(theta2), 0.0f, sin(theta2)}
        };
        Vertex v3 = {
            {cos(theta2) * radius, -height / 2, sin(theta2) * radius},
            {cos(theta2), 0.0f, sin(theta2)},
            {cos(theta2), 1.0f},
            {cos(theta2), 0.0f, sin(theta2)},
            {cos(theta2), 0.0f, sin(theta2)}
        };
        Vertex v4 = {
            {cos(theta1) * radius, -height / 2, sin(theta1) * radius},
            {cos(theta1), 0.0f, sin(theta1)},
            {cos(theta1), 1.0f},
            {cos(theta1), 0.0f, sin(theta1)},
            {cos(theta1), 0.0f, sin(theta1)}
        };

        vertices.push_back(v1);
        vertices.push_back(v2);
        vertices.push_back(v3);
        vertices.push_back(v1);
        vertices.push_back(v3);
        vertices.push_back(v4);
    }

    return vertices;
}

// draw the mesh of a BOUDIN
void drawBoudinMesh() {
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // fill the VBO with the vertices
    vector<Vertex> vertices = createBoudinMesh();
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    // keys are reverted..
    float cameraSpeed = static_cast<float>(30 * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->moveUp(deltaTime, cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->moveDown(deltaTime, cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->moveLeft(deltaTime, cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->moveRight(deltaTime, cameraSpeed);
}

bool mousePressed = false;

// callback function for mouse button press events
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        camera->mousePressEvent(MouseButton::LeftClick, xPos, yPos);
        mousePressed = true;
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        mousePressed = false;
    }
}

// callback function for mouse movement events
void cursorPosCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (mousePressed)
    {
    camera->mouseMoveEvent(MouseButton::LeftClick, xPos, yPos);
    }
}

// callback function for mouse scroll events
void scrollCallback(GLFWwindow* window, double x, double y)
{
    camera->wheelEvent((float)y,(float)x);
}

// utils function to load a texture from a path
unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}



// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


