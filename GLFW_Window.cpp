#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Angel.h"
#include <iostream>
#include<glm/ext.hpp>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include"Sphere.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <vector>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void SunDrawing();
void planetsDrawing(float orbitRadius, float angle, int textureIndex, float scalefactor);
void SaturnRingDraw(float innerRadius, float outerRadius, glm::vec3 color, float orbitRadius, float angle);

const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

// Sun and planets
Sphere sun(0.3f);
Sphere planet(0.05f);
GLuint vao, vboSun, vboPlanet, iboSun, iboPlanet, vPosition,
colorAttrib, modelAttrib, projectionAttrib, viewAttrib,
vColor, ringVBO, ringVAO, texture1, texture2, vNormal, texCoord;
std::vector<GLuint> planetTextures;
std::vector<std::string> planetTexturePaths;

glm::vec3 cameraPos, cameraTarget, cameraDirection, cameraRight, cameraUp, cameraFront;
glm::mat4 view, projection, model;
GLfloat yaw = -90.0f;
GLfloat speed = 0.5f;
GLuint program;

float angle = 0.0f;
typedef Angel::vec4  color4;
typedef Angel::vec4  point4;
float planetScales[8] = { 0.3f, 0.9f, 1.0f, 0.5, 10.0f, 9.0f, 3.9f, 3.8f };


void idle() {
    angle += 0.0001f;
}



void initTexture()
{
    stbi_set_flip_vertically_on_load(true);
    int width, height, channels;

    // Define the file names
    planetTexturePaths = {
        "sun.bmp",
        "mercury.bmp",
        "venus.bmp",
        "earth.bmp",
         "mars.bmp",
         "jupiter.bmp",
         "saturn.bmp",
          "uranus.bmp",
         "neptune.bmp"
    };

    planetTextures.resize(9);
    for (int i = 0; i < 9; ++i)
    {
        glGenTextures(1, &planetTextures[i]);
        glBindTexture(GL_TEXTURE_2D, planetTextures[i]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        unsigned char* data = stbi_load(planetTexturePaths[i].c_str(), &width, &height, &channels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Failed to load texture: " << planetTexturePaths[i] << std::endl;
        }
    }
}

void initLight()
{
    color4 light_ambient(0.7f, 0.7f, 0.7f, 1.0f);  // Light Gray
    color4 light_diffuse(1.0f, 1.0f, 0.0f, 1.0f); // Yellow
    color4 light_specular(1.0f, 1.0f, 1.0f, 1.0f); // White
    float material_shininess = 100.0f;


    point4 light_position(0.0f, 0.0f, 0.0f, 1.0);
    glUniform4fv(glGetUniformLocation(program, "pointLight.ambient"), 1, light_ambient);
    glUniform4fv(glGetUniformLocation(program, "pointLight.diffuse"), 1, light_diffuse);
    glUniform4fv(glGetUniformLocation(program, "pointLight.specular"), 1, light_specular);
    glUniform4fv(glGetUniformLocation(program, "pointLight.position"), 1, light_position);
    glUniform1f(glGetUniformLocation(program, "pointLight.constant"), 1.0f);
    glUniform1f(glGetUniformLocation(program, "pointLight.linear"), 0.09f);
    glUniform1f(glGetUniformLocation(program, "pointLight.quadratic"), 0.032f);

    color4 material_ambient(0.5f, 0.5f, 0.5f, 1.0f);
    color4 material_diffuse(1.0f, 1.0f, 1.0f, 1.0f);
    color4 material_specular(1.0f, 1.0f, 1.0f, 1.0f);
    glUniform1f(glGetUniformLocation(program, "materialShininess"), material_shininess);
    glUniform4fv(glGetUniformLocation(program, "material.ambient"), 1, material_ambient);
    glUniform4fv(glGetUniformLocation(program, "material.diffuse"), 1, material_diffuse);
    glUniform4fv(glGetUniformLocation(program, "material.specular"), 1, material_specular);

}


void SunDrawing() {
    glBindBuffer(GL_ARRAY_BUFFER, vboSun);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboSun);
    glBindVertexArray(vao);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, sun.getInterleavedStride(), BUFFER_OFFSET(0));
    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, sun.getInterleavedStride(), BUFFER_OFFSET(sizeof(float) * 3));
    glVertexAttribPointer(texCoord, 2, GL_FLOAT, GL_FALSE, sun.getInterleavedStride(), BUFFER_OFFSET(sizeof(float) * 6));


    glUniform1i(glGetUniformLocation(program, "planetTexture"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, planetTextures[0]);

    model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(modelAttrib, 1, GL_FALSE, glm::value_ptr(model));
    glDrawElements(GL_TRIANGLES, sun.getIndexSize() / sizeof(GLuint), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}


void planetsDrawing(float orbitRadius, float angle, int textureIndex, float scalefactor) {
    glBindBuffer(GL_ARRAY_BUFFER, vboPlanet);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboPlanet);
    glBindVertexArray(vao);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, planet.getInterleavedStride(), BUFFER_OFFSET(0));
    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, planet.getInterleavedStride(), BUFFER_OFFSET(sizeof(float) * 3));
    glVertexAttribPointer(texCoord, 2, GL_FLOAT, GL_FALSE, planet.getInterleavedStride(), BUFFER_OFFSET(sizeof(float) * 6));


    glUniform1i(glGetUniformLocation(program, "planetTexture"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, planetTextures[textureIndex]);

    model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, glm::vec3(orbitRadius, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(scalefactor, scalefactor, scalefactor));
    glUniformMatrix4fv(modelAttrib, 1, GL_FALSE, glm::value_ptr(model));
    glDrawElements(GL_TRIANGLES, planet.getIndexSize() / sizeof(GLuint), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}

void SaturnRingDraw(float innerRadius, float outerRadius, glm::vec3 color, float orbitRadius, float angle) {
    const int NUM_POINTS = 1000;
    static bool initialized = false;
    static std::vector<float> ringVertices;

    if (!initialized) {

        for (int i = 0; i < NUM_POINTS; ++i) {
            float distance = innerRadius + static_cast<float>(rand()) / RAND_MAX * (outerRadius - innerRadius);
            float theta = static_cast<float>(rand()) / RAND_MAX * 2.0f * glm::pi<float>();
            float x = distance * glm::cos(theta);
            float z = distance * glm::sin(theta);
            float y = 0.0f;
            ringVertices.push_back(x);
            ringVertices.push_back(y);
            ringVertices.push_back(z);

        }

        glBindVertexArray(ringVAO);
        glBindBuffer(GL_ARRAY_BUFFER, ringVBO);
        glBufferData(GL_ARRAY_BUFFER, ringVertices.size() * sizeof(float), &ringVertices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(vPosition);
        initialized = true;
    }
    glBindVertexArray(ringVAO);
    model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, glm::vec3(orbitRadius, 0.0f, 0.0f));
    glUniformMatrix4fv(modelAttrib, 1, GL_FALSE, glm::value_ptr(model));
    glUniform4f(colorAttrib, color.r, color.g, color.b, 1.0f);
    glDrawArrays(GL_POINTS, 0, NUM_POINTS);
    glBindVertexArray(0);
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glUniformMatrix4fv(viewAttrib, 1, GL_FALSE, glm::value_ptr(view));


    SunDrawing();
    float orbitRadius = 1.5f;

    int iring = 0;


    for (int i = 1; i < 9; i++) {

        planetsDrawing(orbitRadius * i, angle * i * 40.0f, i, planetScales[i - 1]);
        if (i == 6) {
            iring = i;
        }


    }

    float innerRadius = 1.5f;
    float outerRadius = 1.7f;
    SaturnRingDraw(innerRadius, outerRadius, glm::vec3(0.8f, 0.8f, 0.8f), orbitRadius * (iring), angle * (iring) * 40.0f);




}

void init() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vboSun);
    glBindBuffer(GL_ARRAY_BUFFER, vboSun);
    glBufferData(GL_ARRAY_BUFFER, sun.getInterleavedVertexSize(), sun.getInterleavedVertices(), GL_STATIC_DRAW);

    glGenBuffers(1, &iboSun);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboSun);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sun.getIndexSize(), sun.getIndices(), GL_STATIC_DRAW);

    glGenBuffers(1, &vboPlanet);
    glBindBuffer(GL_ARRAY_BUFFER, vboPlanet);
    glBufferData(GL_ARRAY_BUFFER, planet.getInterleavedVertexSize(), planet.getInterleavedVertices(), GL_STATIC_DRAW);

    glGenBuffers(1, &iboPlanet);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboPlanet);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, planet.getIndexSize(), planet.getIndices(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &ringVAO);
    glGenBuffers(1, &ringVBO);

    program = InitShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);

    vPosition = glGetAttribLocation(program, "vertexPosition");
    vColor = glGetUniformLocation(program, "vColor");

    vNormal = glGetAttribLocation(program, "vertexNormal");
    glEnableVertexAttribArray(vNormal);

    texCoord = glGetAttribLocation(program, "texCoord");
    glEnableVertexAttribArray(texCoord);


    colorAttrib = glGetUniformLocation(program, "vColor");
    modelAttrib = glGetUniformLocation(program, "model");
    viewAttrib = glGetUniformLocation(program, "view");
    projectionAttrib = glGetUniformLocation(program, "projection");

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);

    cameraPos = glm::vec3(0.0f, 0.0f, 10.0);
    cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

    cameraDirection.x = glm::cos(glm::radians(yaw));
    cameraDirection.z = glm::sin(glm::radians(yaw));
    cameraFront = glm::normalize(cameraDirection);

    cameraRight = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), cameraDirection));
    cameraUp = glm::cross(cameraDirection, cameraRight);

    projection = glm::perspective(glm::radians(55.0f), 1.0f, 0.1f, 100.0f);
    glUniformMatrix4fv(projectionAttrib, 1, GL_FALSE, glm::value_ptr(projection));
    initLight();
    initTexture();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
}

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, true);
            break;
        case GLFW_KEY_R:
            cameraPos += speed * cameraUp;
            std::cout << "R is pressed " << std::endl;

            break;
        case GLFW_KEY_F:
            cameraPos -= speed * cameraUp;
            break;
        case GLFW_KEY_W:
            cameraPos += speed * cameraFront;
            break;
        case GLFW_KEY_S:
            cameraPos -= speed * cameraFront;
            break;
        case GLFW_KEY_A:
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
            break;
        case GLFW_KEY_D:
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
            break;
        case GLFW_KEY_Q:
            yaw -= 1.0f;
            break;
        case GLFW_KEY_E:
            yaw += 1.0f;
            break;
        }
        cameraDirection.x = glm::cos(glm::radians(yaw));
        cameraDirection.z = glm::sin(glm::radians(yaw));
        cameraFront = glm::normalize(cameraDirection);
    }
}
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL with GLFW", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }


    //glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetKeyCallback(window, keyboard_callback);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // build and compile our shader program
    // ------------------------------------
    // vertex shader


    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    init();
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // Process input of the user keyboard and mouse
        processInput(window);

        //Call function idle and display

        display();



        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }



    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}