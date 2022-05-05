#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_m.h"
#include "camera.h"
#include "model.h"
#include "filesystem.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);
unsigned int loadCubemap(vector<std::string> faces);
void createPlanet();
void createSun();

// settings
const unsigned int SCR_WIDTH = 2800;
const unsigned int SCR_HEIGHT = 1400;

// camera
Camera camera(glm::vec3(11.0f, 1.0f, 24.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// earth
unsigned int planetVAO;
unsigned int planetVBO;
unsigned int planetEBO;
int indexCount1;

//sun
unsigned int sunVAO;
unsigned int sunVBO;
unsigned int sunEBO;
int indexCount2;

//textures
unsigned int planetTexture;
unsigned int sunTexture;
float angle;

unsigned int planetTexture2;
unsigned int planetTexture3;
unsigned int planetTexture4;
unsigned int planetTexture5;
unsigned int planetTexture6;
unsigned int planetTexture7;
unsigned int planetTexture8;

int indexCount3;

//lighting
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool rotFlg2;
float lightX, lightY, lightZ;
unsigned int lightVAO;

bool mouseFlg = true;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Solar System Scene 2", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    // build and compile shaders
    // -------------------------
    Shader skyboxShader("../../src/shader/exp02/6.2.skybox.vert", "../../src/shader/exp02/6.2.skybox.frag");
    Shader planetShader("../../src/shader/exp02/6.2.planet.vert", "../../src/shader/exp02/6.2.planet.frag");
    Shader sunShader("../../src/shader/exp02/6.2.planet2.vert", "../../src/shader/exp02/6.2.planet2.frag");
    Shader planetShader2("../../src/shader/exp02/6.2.planet.vert", "../../src/shader/exp02/6.2.planet.frag");


    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    //make planet, all other planets ecxept for earth
    planetShader2.use();
    createPlanet();
    planetTexture2 = loadTexture(FileSystem::getPath("resources/textures/mercury.jpg").c_str());
    planetTexture3 = loadTexture(FileSystem::getPath("resources/textures/venus.jpg").c_str());
    planetTexture4 = loadTexture(FileSystem::getPath("resources/textures/mars.jpg").c_str());
    planetTexture5 = loadTexture(FileSystem::getPath("resources/textures/jupiter.jpg").c_str());
    planetTexture6 = loadTexture(FileSystem::getPath("resources/textures/saturn2.jpg").c_str());
    planetTexture7 = loadTexture(FileSystem::getPath("resources/textures/uranus2.jpg").c_str());
    planetTexture8 = loadTexture(FileSystem::getPath("resources/textures/neptune.jpg").c_str());
    planetShader2.setInt("aTexture", 0);


    //make planet earth
    planetShader.use();
    createPlanet();
    planetTexture = loadTexture(FileSystem::getPath("resources/textures/earth.jpg").c_str());
    planetShader.setInt("aTexture", 0);

    //make sun
    sunShader.use();
    createSun();
    sunTexture = loadTexture(FileSystem::getPath("resources/textures/sun.jpg").c_str());
    sunShader.setInt("aTexture", 0);

    // skybox VAO
    skyboxShader.use();
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glBindVertexArray(skyboxVAO);

    glGenBuffers(1, &skyboxVBO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // load textures
    // -------------
    //unsigned int cubeTexture = loadTexture(FileSystem::getPath("resources/textures/container.jpg").c_str());

    vector<std::string> faces
    {
        FileSystem::getPath("resources/textures/spacebox/right.png"),
        FileSystem::getPath("resources/textures/spacebox/left.png"),
        FileSystem::getPath("resources/textures/spacebox/top.png"),
        FileSystem::getPath("resources/textures/spacebox/bottom.png"),
        FileSystem::getPath("resources/textures/spacebox/front.png"),
        FileSystem::getPath("resources/textures/spacebox/back.png")
    };
    unsigned int cubemapTexture = loadCubemap(faces);

    // shader configuration
    // --------------------
    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        //-------------------------earth--------------------------------------
        planetShader.use();
        // draw scene as normal
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        float sphereScale = 0.5f;
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, planetTexture);
        //model = glm::mat4(1.0f);
        angle = (float)glfwGetTime() / 1.4f; // speed of rotation
        model = glm::translate(model, glm::vec3(6.9f, 0.0f, 0.0f));

        model = glm::scale(model, glm::vec3(sphereScale, sphereScale, sphereScale));

        model = glm::rotate(model, -angle, glm::vec3(0.15f, -1.0f, 0.0f));
        //model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        //model = glm::translate(model, glm::vec3(-17.0f, 0.0f, 0.0f));

        planetShader.setMat4("model", model);
        planetShader.setMat4("view", view);
        planetShader.setMat4("projection", projection);

        planetShader.setVec3("light.position", lightPos.x, lightPos.y, lightPos.z);
        planetShader.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
        planetShader.setVec3("light.ambient", 0.4f, 0.4f, 0.4f);
        planetShader.setVec3("light.diffuse", 0.6f, 0.6f, 0.6f);
        //planetShader.setVec3("light.specular", 0.5f, 0.5f, 0.5f); // sun does not need specular
        //planetShader.setVec3("material.specular", 0.6f, 0.6f, 0.6f);
        planetShader.setFloat("material.shininess", 64.0f);
        planetShader.setInt("material.diffuse", 0.5f);

        glBindVertexArray(planetVAO);
        glDrawElements(GL_TRIANGLE_STRIP, indexCount1, GL_UNSIGNED_INT, 0);

        //-------------------------------sun------------------------------------------
        sunShader.use();
        // draw scene as normal
        model = glm::mat4(1.0f);
        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        float sunScale = 4.3f;
        model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(sunScale, sunScale, sunScale));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sunTexture);
        //model = glm::mat4(1.0f);
        //angle = (float)glfwGetTime() / 3.0f;
        //model = glm::rotate(model, -angle, glm::vec3(2.0f, 1.0f, 0.0f));
        sunShader.setMat4("model", model);
        sunShader.setMat4("view", view);
        sunShader.setMat4("projection", projection);
        glBindVertexArray(sunVAO);
        glDrawElements(GL_TRIANGLE_STRIP, indexCount2, GL_UNSIGNED_INT, 0);

        //--------------------mercury-------------------------------------------------------
        planetShader2.use();
        // draw scene as normal
        model = glm::mat4(1.0f);
        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        float myScale = 0.2f;
        angle = (float)glfwGetTime() / 0.8f;
        //the x for the below dictates where planet is centered
        model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(myScale, myScale, myScale));
        model = glm::rotate(model, -angle, glm::vec3(0.15f, -1.0f, 0.0f));
        //float lightX, lightY, lightZ;
        //glm::vec3 lightPos(1.0f, 0.1f, 2.0f);
        //lightX = -1.0f * sin(glfwGetTime());
        //lightY = 0.1f;
        //lightZ = 2.0f * cos(glfwGetTime());
       
        //lightPos = glm::vec3(lightX, lightY, lightZ);
        //model = glm::translate(model, lightPos);
        //the x for this below dictates width of orbit
        //model = glm::translate(model, glm::vec3(-22.0f, 0.0f, 2.0f));


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, planetTexture2);
        //model = glm::mat4(1.0f);
        //angle = (float)glfwGetTime() / 3.0f;
        //model = glm::rotate(model, -angle, glm::vec3(2.0f, 1.0f, 0.0f));
        planetShader2.setMat4("model", model);
        planetShader2.setMat4("view", view);
        planetShader2.setMat4("projection", projection);
        // these lighting caculations here, set it for all other planets also
        planetShader2.setVec3("light.position", lightPos.x, lightPos.y, lightPos.z);
        planetShader2.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
        planetShader2.setVec3("light.ambient", 0.4f, 0.4f, 0.4f);
        planetShader2.setVec3("light.diffuse", 0.6f, 0.6f, 0.6f);
        //planetShader2.setVec3("light.specular", 0.0f, 0.0f, 0.0f); //dont need specular for a sun, sun does not have a point light, just ambient and diffuse
        //planetShader2.setVec3("material.specular", 0.3f, 0.3f, 0.3);
        planetShader2.setFloat("material.shininess", 64.0f);
        planetShader2.setInt("material.diffuse", 0.0f);
        glBindVertexArray(planetVAO);
        glDrawElements(GL_TRIANGLE_STRIP, indexCount2, GL_UNSIGNED_INT, 0);

        //-------------------------venus-----------------------------------
        planetShader2.use();
        // draw scene as normal
        model = glm::mat4(1.0f);
        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        myScale = 0.45f;
        //the x for the below dictates where planet is centered
        model = glm::translate(model, glm::vec3(5.1f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(myScale, myScale, myScale));
        angle = (float)glfwGetTime() / 1.0f;

        model = glm::rotate(model, -angle, glm::vec3(0.15f, -1.0f, 0.0f));

        //the x for this below dictates width of orbit
        //model = glm::translate(model, glm::vec3(-16.0f, 0.0f, 2.0f));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, planetTexture3);
        planetShader2.setMat4("model", model);
        planetShader2.setMat4("view", view);
        planetShader2.setMat4("projection", projection);
        glBindVertexArray(planetVAO);
        glDrawElements(GL_TRIANGLE_STRIP, indexCount2, GL_UNSIGNED_INT, 0);

        //---------------mars--------------------------------------------
        planetShader2.use();
        // draw scene as normal
        model = glm::mat4(1.0f);
        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        myScale = 0.4f;
        //the x for the below dictates where planet is centered
        model = glm::translate(model, glm::vec3(9.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(myScale, myScale, myScale));
        angle = (float)glfwGetTime() / 1.5f;

        model = glm::rotate(model, -angle, glm::vec3(0.15f, -1.0f, 0.0f));

        //the x for this below dictates width of orbit
        //model = glm::translate(model, glm::vec3(-25.0f, 0.0f, 2.0f));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, planetTexture4);
        planetShader2.setMat4("model", model);
        planetShader2.setMat4("view", view);
        planetShader2.setMat4("projection", projection);
        glBindVertexArray(planetVAO);
        glDrawElements(GL_TRIANGLE_STRIP, indexCount2, GL_UNSIGNED_INT, 0);

        //---------------jupiter--------------------------------------------
        planetShader2.use();
        // draw scene as normal
        model = glm::mat4(1.0f);
        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        myScale = 1.8f;
        //the x for the below dictates where planet is centered
        model = glm::translate(model, glm::vec3(13.0f, .0f, 0.0f));
        model = glm::scale(model, glm::vec3(myScale, myScale, myScale));
        angle = (float)glfwGetTime() / 2.0f;

        model = glm::rotate(model, -angle, glm::vec3(0.15f, -1.0f, 0.0f));

        //the x for this below dictates width of orbit
        //model = glm::translate(model, glm::vec3(-7.0f, 0.0f, 2.0f));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, planetTexture5);
        planetShader2.setMat4("model", model);
        planetShader2.setMat4("view", view);
        planetShader2.setMat4("projection", projection);
        glBindVertexArray(planetVAO);
        glDrawElements(GL_TRIANGLE_STRIP, indexCount2, GL_UNSIGNED_INT, 0);

        //---------------saturn--------------------------------------------
        planetShader2.use();
        // draw scene as normal
        model = glm::mat4(1.0f);
        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        myScale = 1.4f;
        //the x for the below dictates where planet is centered
        model = glm::translate(model, glm::vec3(16.9f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(myScale, myScale, myScale));
        angle = (float)glfwGetTime() / 2.3f;

        model = glm::rotate(model, -angle, glm::vec3(0.15f, -1.0f, 0.0f));

        //the x for this below dictates width of orbit
        //model = glm::translate(model, glm::vec3(-12.8f, 0.0f, 2.0f));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, planetTexture6);
        planetShader2.setMat4("model", model);
        planetShader2.setMat4("view", view);
        planetShader2.setMat4("projection", projection);
        glBindVertexArray(planetVAO);
        glDrawElements(GL_TRIANGLE_STRIP, indexCount2, GL_UNSIGNED_INT, 0);

        //---------------uranus--------------------------------------------
        planetShader2.use();
        // draw scene as normal
        model = glm::mat4(1.0f);
        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        myScale = 1.18f;
        //the x for the below dictates where planet is centered
        model = glm::translate(model, glm::vec3(20.3f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(myScale, myScale, myScale));
        angle = (float)glfwGetTime() / 2.5f;

        model = glm::rotate(model, -angle, glm::vec3(0.15f, -1.0f, 0.0f));

        //the x for this below dictates width of orbit
        //model = glm::translate(model, glm::vec3(-17.9f, 0.0f, 2.0f));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, planetTexture7);
        planetShader2.setMat4("model", model);
        planetShader2.setMat4("view", view);
        planetShader2.setMat4("projection", projection);
        glBindVertexArray(planetVAO);
        glDrawElements(GL_TRIANGLE_STRIP, indexCount2, GL_UNSIGNED_INT, 0);

        //---------------neptune--------------------------------------------
        planetShader2.use();
        // draw scene as normal
        model = glm::mat4(1.0f);
        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        myScale = 1.1f;
        //the x for the below dictates where planet is centered
        model = glm::translate(model, glm::vec3(24.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(myScale, myScale, myScale));
        angle = (float)glfwGetTime() / 2.9f;

        model = glm::rotate(model, -angle, glm::vec3(0.15f, -1.0f, 0.0f));

        //the x for this below dictates width of orbit
        //model = glm::translate(model, glm::vec3(-21.4f, 0.0f, 2.0f));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, planetTexture8);
        planetShader2.setMat4("model", model);
        planetShader2.setMat4("view", view);
        planetShader2.setMat4("projection", projection);
        glBindVertexArray(planetVAO);
        glDrawElements(GL_TRIANGLE_STRIP, indexCount2, GL_UNSIGNED_INT, 0);
        //----------------------------------------------------------------------
        // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVAO);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime * 4); // deltaTime * 4 to increase movement speed
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime * 4);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime * 4);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime * 4);
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        mouseFlg = true;
    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        mouseFlg = false;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (mouseFlg) {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        else
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

//create a sphere 
void createPlanet()
{
    glGenVertexArrays(1, &planetVAO);

    glGenBuffers(1, &planetVBO);
    glGenBuffers(1, &planetEBO);

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> uv;
    std::vector<unsigned int> indices;

    const unsigned int X_SEGMENTS = 64;
    const unsigned int Y_SEGMENTS = 64;
    const float PI = 3.14159265359;
    for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
    {
        for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
        {
            float xSegment = (float)x / (float)X_SEGMENTS;
            float ySegment = (float)y / (float)Y_SEGMENTS;
            float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            float yPos = std::cos(ySegment * PI);
            float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

            positions.push_back(glm::vec3(xPos, yPos, zPos));
            uv.push_back(glm::vec2(xSegment, ySegment));
        }
    }

    bool oddRow = false;
    for (int y = 0; y < Y_SEGMENTS; ++y)
    {
        if (!oddRow) //even rows: y == 0, y == 2; and so on
        {
            for (int x = 0; x <= X_SEGMENTS; ++x)
            {
                indices.push_back(y * (X_SEGMENTS + 1) + x);
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
            }
        }
        else
        {
            for (int x = X_SEGMENTS; x >= 0; --x)
            {
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                indices.push_back(y * (X_SEGMENTS + 1) + x);
            }
        }
        oddRow = !oddRow;
    }
    indexCount1 = indices.size();

    std::vector<float> data;
    for (int i = 0; i < positions.size(); ++i)
    {
        data.push_back(positions[i].x);
        data.push_back(positions[i].y);
        data.push_back(positions[i].z);
        if (uv.size() > 0)
        {
            data.push_back(uv[i].x);
            data.push_back(uv[i].y);
        }
    }

    glBindVertexArray(planetVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planetVBO);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planetEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    float stride = (3 + 2) * sizeof(float);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
}

//create a sphere 
void createSun()
{
    glGenVertexArrays(1, &sunVAO);

    glGenBuffers(1, &sunVBO);
    glGenBuffers(1, &sunEBO);

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> uv;
    std::vector<unsigned int> indices;

    const unsigned int X_SEGMENTS = 64;
    const unsigned int Y_SEGMENTS = 64;
    const float PI = 3.14159265359;
    for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
    {
        for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
        {
            float xSegment = (float)x / (float)X_SEGMENTS;
            float ySegment = (float)y / (float)Y_SEGMENTS;
            float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            float yPos = std::cos(ySegment * PI);
            float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

            positions.push_back(glm::vec3(xPos, yPos, zPos));
            uv.push_back(glm::vec2(xSegment, ySegment));
        }
    }

    bool oddRow = false;
    for (int y = 0; y < Y_SEGMENTS; ++y)
    {
        if (!oddRow) //even rows: y == 0, y == 2; and so on
        {
            for (int x = 0; x <= X_SEGMENTS; ++x)
            {
                indices.push_back(y * (X_SEGMENTS + 1) + x);
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
            }
        }
        else
        {
            for (int x = X_SEGMENTS; x >= 0; --x)
            {
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                indices.push_back(y * (X_SEGMENTS + 1) + x);
            }
        }
        oddRow = !oddRow;
    }
    indexCount2 = indices.size();

    std::vector<float> data;
    for (int i = 0; i < positions.size(); ++i)
    {
        data.push_back(positions[i].x);
        data.push_back(positions[i].y);
        data.push_back(positions[i].z);
        if (uv.size() > 0)
        {
            data.push_back(uv[i].x);
            data.push_back(uv[i].y);
        }
    }

    glBindVertexArray(sunVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sunVBO);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sunEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    float stride = (3 + 2) * sizeof(float);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
}