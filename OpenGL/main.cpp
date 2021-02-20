#include<iostream>
#include<sstream>


#include<GL/glew.h> // Glew must come first, before glfw.
#include<GLFW/glfw3.h>

#include<glm/gtc/matrix_transform.hpp>


#include"ColorConsole.h"
#include"ShaderProgram.h"
//#include"Texture2D.h"
#include"Camera.h"
//#include"Mesh.h"
#include"Light.h"
#include"Model.h"


const char* APP_TITLE = "OpenGL Training";
bool FULL_SCREEN = false;
bool g_bFullLight = true;
GLFWwindow* g_pWindow = NULL;
int g_WindowHeight = 720;
int g_WindowWidth = 1024;
const std::string planeTexture = "Stone_Wall.dds";
const std::string crateTexture = "crate_diffuse.png";

/*
OrbitCamera* g_pOrbitCamera = NULL;
float g_fYaw = 0.0f;
float g_fPitch = 0.0f;
float g_fRadius = 10.0f;
const float MOUSE_SENSITIVITY = 0.25f;
*/

FPSCamera g_pFPSCamera(glm::vec3(10.0f, 20.0f, 5.0f));
const double ZOOM_SENSITIVITY = -3.0f;
const float MOVE_SPEED =20.0f;
const float MOUSE_SENSITIVITY = 0.1f;


void MouseMoveCallback(GLFWwindow* wnd, double posX, double posY);
void MouseScrollCallback(GLFWwindow* wnd, double deltaX, double deltaY);
void Update(double d);

void KeyInputCallback(GLFWwindow* wnd, int key, int scancode, int action, int mode);
void OnWindowResize(GLFWwindow* wnd, int width, int height);
void ShowFPS(GLFWwindow* wnd);
bool InitOpenGL();



int main(){

    using namespace std;
    cout << color(colors::DARKBLUE);
    cout << "Program running...\n\n" << white;

    // Initialize opengl, window and glfw.
    if (!InitOpenGL()) {
        return -1;
    }


    // Testing Assimp.
    AssimpModel* assModel = new AssimpModel("ships/fighter_1/fighter_1.obj");
    AssimpModel* assModel2 = new AssimpModel("ships/fighter_2/fighter_2.obj");
    AssimpModel* assModel3 = new AssimpModel("ships/fighter_3/fighter_3.obj");


    ShaderProgram basicShader;
    basicShader.LoadShaders("basic.vert", "basic.frag");
    

    // Cretae mesh models.
    /*
    const int numModels = 4;
    Mesh mesh[numModels];
    */
    //Texture2D texture[numModels];

    /*
    mesh[0].LoadOBJ("Sphere.obj"); // Load differnt models.
    mesh[1].LoadOBJ("plane.obj");
    mesh[2].LoadOBJ("Sphere.obj");
    mesh[3].LoadOBJ("backpack.obj");
    */


    /*
    mesh[0].SetTexture("Sandstone.dds"); // Load differnt models.
    mesh[1].SetTexture("rocks.jpg");
    mesh[2].SetTexture("mountain.dds");
    mesh[3].SetTexture("diffuse.jpg");
    */

    // Cretae mesh models.
    const int numModels = 4;
    Mesh mesh[numModels];

    /*
    Texture2D texture[numModels];

    mesh[0].LoadOBJ("Sphere.obj"); // Load differnt models.
    mesh[1].LoadOBJ("plane.obj");
    mesh[2].LoadOBJ("Sphere.obj");
    mesh[3].LoadOBJ("Sphere.obj");
    


    texture[0].LoadTexture("Sandstone.dds", true); // Load diff. textures.
    texture[1].LoadTexture("rocks.jpg", true);
    texture[2].LoadTexture("mountain.dds", true);
    texture[3].LoadTexture("crate_diffuse.png", true);
    */

    /*
    Mesh lightMesh;
    lightMesh.LoadOBJ("Sphere.obj");
    glm::vec3 lightScale = { 0.2f, 0.2f, 0.2f };
    */

    PointLight lightMesh;
    lightMesh.SetShader("lighting_all.vert", "lighting_all.frag");
    lightMesh.SetPosition(glm::vec3(0.0f, 10.0f, 0.0f));
    lightMesh.SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
    lightMesh.SetColor(glm::vec3(1.0f, 1.0f, 1.0f));

    PointLight lightRed;
    lightRed.SetPosition(glm::vec3(10.0f, 10.0f, 0.0f));
    lightRed.SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
    lightRed.SetColor(glm::vec3(1.0f, 0.0f, 0.0f));

    PointLight lightGreen;
    lightGreen.SetPosition(glm::vec3(0.0f, 10.0f, 10.0f));
    lightGreen.SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
    lightGreen.SetColor(glm::vec3(0.0f, 1.0f, 0.0f));

    PointLight lightWhite;
    lightWhite.SetPosition(glm::vec3(0.0f, 50.0f, 0.0f));
    lightWhite.SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
    lightWhite.SetColor(glm::vec3(1.0f, 1.0f, 1.0f));

    /*
    ShaderProgram lightShader; // For drawing light.
    lightShader.LoadShaders("basic.vert", "basic.frag");
    */
    /*
    ShaderProgram lightingShader;
    lightingShader.LoadShaders("lighting_point.vert", "lighting_point.frag");
    */


    // Model positions.
    /*
    glm::vec3 modelPos[] = {
        glm::vec3(-20.0f, 1.0f, 0.0f), // Sphere 1

        glm::vec3(-10.0f, -10.0f, 0.0f), // Plane

        glm::vec3(0.0f, 10.0f, -20.0f), // Sphere 2

        glm::vec3(0.0f, 2.0f, 0.0f) // Backpack
        glm::vec3(5.0f, 1.0f, 10.0f) // Sphere 3
    };
    */

    /*
    // Model scales.
    glm::vec3 modelScale[] = {
    glm::vec3(1.0f, 1.0f, 1.0f), // Sphere 1

    glm::vec3(0.3f, 0.3f, 0.3f), // Plane

    glm::vec3(1.0f, 1.0f, 1.0f), // Sphere 2

    glm::vec3(2.0f, 2.0f, 2.0f) // Backpack
    glm::vec3(2.0f, 2.0f, 2.0f) // Sphere 3
    };
    */

    // Turn on wireframe mode.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Currently, vertices are send clockwise to shader,
                                                // thus, wiring is clockwise. Here it doesnt matter -> GL_FRONT_AND_BACK .






    // Light rotation
    float angle = 0.0f;
    double lastTime = glfwGetTime();

    // Enter mainloop.
    while (!glfwWindowShouldClose(g_pWindow)) {

        double currTime = glfwGetTime();
        double deltaTime = currTime - lastTime;


        // Poll events.
        glfwPollEvents();
        Update(deltaTime); // Get input first then update.

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear screen and depth buffer.



        // Rotate cube.
       // Create matrices for shader.
        glm::mat4 model, view, projection;


        //view = glm::lookAt(camPos, targetPos, upPos);
        view = g_pFPSCamera.GetViewMatrix();

        // Projection matrix.
        projection = glm::perspective(glm::radians(g_pFPSCamera.GetFieldOfView()), (float)g_WindowWidth/g_WindowHeight, 0.1f, 1000.0f);


        glm::vec3 viewPos;
        viewPos.x = g_pFPSCamera.GetPos().x;
        viewPos.y = g_pFPSCamera.GetPos().y;
        viewPos.z = g_pFPSCamera.GetPos().z;


        /*
        // Light.
        glm::vec3 lightPos(-5.0f, 10.0f, 10.0f);
        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
        //glm::vec3 lightDirection(0.0f, -0.88f, -0.17f);
        */

        
        // Move Light...
        angle += (float)deltaTime * 50.0f;
        
        float x = 15.0f * sinf(glm::radians(angle));
        float y = 10.0f + 20.0f * cosf(glm::radians(angle));
        float z = 10.0f + 20.0f * cosf(glm::radians(angle));
        lightMesh.SetPosition(glm::vec3(x, y, z));
        lightMesh.SetColor(glm::vec3(glm::clamp(x/2, 0.1f, 1.0f), glm::clamp(y/y - x, 0.1f, 1.0f), glm::clamp(x - y + z, 0.1f, 1.0f)));
        if (g_bFullLight) {
            lightMesh.SetBrightness(1.0f);
        }
        else {
            lightMesh.SetBrightness(0.1f);
        }

        // Move white light...
        lightWhite.SetPosition(glm::vec3(0.0f, y, 0.0f));

        /*
        lightingShader.Use();

        // Pass matrix values to shaders..
        lightingShader.SetUniform("view", view);
        lightingShader.SetUniform("projection", projection);
        //lightingShader.SetUniform("lightColor", lightColor); // Send color defs. to shader.
        //lightingShader.SetUniform("lightPos", lightPos); // Send color defs. to shader.
        lightingShader.SetUniform("viewPos", viewPos); // Send color defs. to shader.

        // Light sctruct for shader...
        lightingShader.SetUniform("light.ambient", glm::vec3(0.3, 0.3, 0.3)); // Send color defs. to shader.
        lightingShader.SetUniform("light.diffuse", lightColor); // Send color defs. to shader.
        lightingShader.SetUniform("light.specular", glm::vec3(1.0, 1.0, 1.0)); // Send color defs. to shader
        lightingShader.SetUniform("light.position", lightPos); // Send color defs. to shader.
        //lightingShader.SetUniform("light.direction", g_pFPSCamera.GetLookTo()); // Send color defs. to shader.

        // Point...
        // Uniforms for point light - attenuaton equation
        lightingShader.SetUniform("light.constant", 1.0f); // Send color defs. to shader.
        lightingShader.SetUniform("light.linear", 0.07f); // Send color defs. to shader.
        lightingShader.SetUniform("light.exponent", 0.017f); // Send color defs. to shader.

        lightingShader.SetUniform("light.intensity", 1.0f); // Send color defs. to shader.
        */

        // Sppot light....
        //lightingShader.SetUniform("light.cosInnerCone", glm::cos(glm::radians(15.0f))); // Send color defs. to shader.
        //lightingShader.SetUniform("light.cosOuterCone", glm::cos(glm::radians(20.0f))); // Send color defs. to shader.


        // Adding 3 more lights for testing.
        glm::vec3 lightsPos[] = {

            glm::vec3(-25.0f, 10.0f, 10.0f),

             glm::vec3(25.0f, 10.0f, 10.0f),

             glm::vec3(0.0f, 25.0f, 0.0f)
        };

        glm::vec3 lightsColor[] = {

            glm::vec3(0.0f, 0.5f, 0.5f),

             glm::vec3(1.0f, 1.0f, 1.0f),

             glm::vec3(0.7f, 0.7f, 0.0f)
        };


        lightMesh.Use();
        lightMesh.GetLightShader()->SetUniform("view", view);
        lightMesh.GetLightShader()->SetUniform("projection", projection);
        lightMesh.GetLightShader()->SetUniform("viewPos", viewPos);
        lightMesh.GetLightShader()->SetUniform("lightsInScene", 4);
        lightMesh.GetLightShader()->SetUniform("CameraPosition", g_pFPSCamera.GetPos());
        lightMesh.GetLightShader()->SetUniform("FogColor", glm::vec3(0.0f, 0.25f, 0.38f));
        lightMesh.GetLightShader()->SetUniform("pointLight[0].ambient", glm::vec3(0.1, 0.1, 0.1)); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[0].diffuse", lightMesh.GetColor()); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[0].specular", glm::vec3(1.0, 1.0, 1.0)); // Send color defs. to shader
        lightMesh.GetLightShader()->SetUniform("pointLight[0].position", lightMesh.GetPosition()); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[0].constant", 1.0f); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[0].linear", 0.09f); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[0].quadratic", 0.032f); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[0].brightness", lightMesh.GetBrightness()); // Send color defs. to shader.


        lightMesh.GetLightShader()->SetUniform("pointLight[1].ambient", glm::vec3(0.1, 0.1, 0.1)); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[1].diffuse", lightRed.GetColor()); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[1].specular", glm::vec3(1.0, 1.0, 1.0)); // Send color defs. to shader
        lightMesh.GetLightShader()->SetUniform("pointLight[1].position", lightRed.GetPosition()); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[1].constant", 1.0f); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[1].linear", 0.09f); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[1].quadratic", 0.032f); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[1].brightness", lightRed.GetBrightness()); // Send color defs. to shader.

        lightMesh.GetLightShader()->SetUniform("pointLight[2].ambient", glm::vec3(0.1, 0.1, 0.1)); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[2].diffuse", lightGreen.GetColor()); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[2].specular", glm::vec3(1.0, 1.0, 1.0)); // Send color defs. to shader
        lightMesh.GetLightShader()->SetUniform("pointLight[2].position", lightGreen.GetPosition()); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[2].constant", 1.0f); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[2].linear", 0.09f); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[2].quadratic", 0.032f); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[2].brightness", lightGreen.GetBrightness()); // Send color defs. to shader.

        lightMesh.GetLightShader()->SetUniform("pointLight[3].ambient", glm::vec3(0.1, 0.1, 0.1)); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[3].diffuse", lightWhite.GetColor()); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[3].specular", glm::vec3(1.0, 1.0, 1.0)); // Send color defs. to shader
        lightMesh.GetLightShader()->SetUniform("pointLight[3].position", lightWhite.GetPosition()); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[3].constant", 1.0f); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[3].linear", 0.007f); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[3].quadratic", 0.0002f); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[3].linear", 0.014f); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[3].quadratic", 0.0007f); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[3].brightness", lightWhite.GetBrightness()); // Send color defs. to shader.


        /*
        lightMesh.GetLightShader()->SetUniform("pointLight[1].ambient", glm::vec3(0.1, 0.1, 0.1)); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[1].diffuse", lightsColor[0]); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[1].specular", glm::vec3(1.0, 1.0, 1.0)); // Send color defs. to shader
        lightMesh.GetLightShader()->SetUniform("pointLight[1].position", lightsPos[0]); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[1].constant", 1.0f); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[1].linear", 0.07f); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[1].quadratic", 0.017f); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[1].brightness", 1); // Send color defs. to shader.

        lightMesh.GetLightShader()->SetUniform("pointLight[2].ambient", glm::vec3(0.1, 0.1, 0.1)); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[2].diffuse", lightsColor[1]); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[2].specular", glm::vec3(1.0, 1.0, 1.0)); // Send color defs. to shader
        lightMesh.GetLightShader()->SetUniform("pointLight[2].position", lightsPos[1]); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[2].constant", 1.0f); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[2].linear", 0.027f); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[2].quadratic", 0.0028f); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[2].brightness", 1); // Send color defs. to shader.

        lightMesh.GetLightShader()->SetUniform("pointLight[3].ambient", glm::vec3(0.1, 0.1, 0.1)); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[3].diffuse", lightsColor[2]); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[3].specular", glm::vec3(1.0, 1.0, 1.0)); // Send color defs. to shader
        lightMesh.GetLightShader()->SetUniform("pointLight[3].position", lightsPos[2]); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[3].constant", 1.0f); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[3].linear", 0.07f); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[3].quadratic", 0.017f); // Send color defs. to shader.
        lightMesh.GetLightShader()->SetUniform("pointLight[3].brightness", 1); // Send color defs. to shader.
        */


        // Draw models.
        /*
        for (int i = 0; i < numModels; i++) {

            model = glm::translate(glm::mat4(), modelPos[i]) * glm::scale(glm::mat4(), modelScale[i]); // First matrix is identity.
            lightMesh.GetLightShader()->SetUniform("model", model);



            switch (i) {
            case 0:
                // Material struct for shader...
                lightMesh.GetLightShader()->SetUniform("material.ambient", glm::vec3(0.1, 0.1, 0.1));
                lightMesh.GetLightShader()->SetUniformSampler("material.diffuseMap", 0); // Diffusemap to slot 0. Setting slot needed for setting multuple textures...
                lightMesh.GetLightShader()->SetUniform("material.specular", glm::vec3(1.0f, 1.0f, 1.0f));
                lightMesh.GetLightShader()->SetUniform("material.shininess", 128.0f);
                break;

            case 1:
                // Material struct for shader...
                lightMesh.GetLightShader()->SetUniform("material.ambient", glm::vec3(0.1, 0.1, 0.1));
                lightMesh.GetLightShader()->SetUniformSampler("material.diffuseMap", 0); // Diffusemap to slot 0. Setting slot needed for setting multuple textures...
                lightMesh.GetLightShader()->SetUniform("material.specular", glm::vec3(0.2f, 0.2f, 0.2f));
                lightMesh.GetLightShader()->SetUniform("material.shininess", 16.0f);
                break;

            case 2:
                // Material struct for shader...
                lightMesh.GetLightShader()->SetUniform("material.ambient", glm::vec3(0.1, 0.1, 0.1));
                lightMesh.GetLightShader()->SetUniformSampler("material.diffuseMap", 0); // Diffusemap to slot 0. Setting slot needed for setting multuple textures...
                lightMesh.GetLightShader()->SetUniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
                lightMesh.GetLightShader()->SetUniform("material.shininess", 32.0f);
                break;

            case 3:
                // Material struct for shader...
                lightMesh.GetLightShader()->SetUniform("material.ambient", glm::vec3(0.1, 0.1, 0.1));
                lightMesh.GetLightShader()->SetUniformSampler("material.diffuseMap", 0); // Diffusemap to slot 0. Setting slot needed for setting multuple textures...
                lightMesh.GetLightShader()->SetUniform("material.specular", glm::vec3(1.0f, 1.0f, 1.0f));
                lightMesh.GetLightShader()->SetUniform("material.shininess", 16.0f);
                break;

            default:
            if (i < 2) {

                // Material struct for shader...
                lightMesh.GetLightShader()->SetUniform("material.ambient", glm::vec3(0.5, 0.5, 0.5));
                lightMesh.GetLightShader()->SetUniformSampler("material.diffuseMap", 0); // Diffusemap to slot 0. Setting slot needed for setting multuple textures...
                lightMesh.GetLightShader()->SetUniform("material.specular", glm::vec3(0.2f, 0.2f, 0.2f));
                lightMesh.GetLightShader()->SetUniform("material.shininess", 1.0f);
            }
            else {

                // Material struct for shader...
                lightMesh.GetLightShader()->SetUniform("material.ambient", glm::vec3(0.2, 0.2, 0.2));
                lightMesh.GetLightShader()->SetUniformSampler("material.diffuseMap", 0); // Diffusemap to slot 0. Setting slot needed for setting multuple textures...
                lightMesh.GetLightShader()->SetUniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
                lightMesh.GetLightShader()->SetUniform("material.shininess", 64.0f);
                break;
            }


            // Draw.
            mesh[i].DrawMesh();
        }
        */


        model = glm::translate(glm::mat4(), glm::vec3(-2.0f, 3.0f, -2.0f)) * glm::scale(glm::mat4(), glm::vec3(0.5f, 0.5f, 0.5f));
        lightMesh.GetLightShader()->SetUniform("model", model);
        lightMesh.GetLightShader()->SetUniform("mainLightPos", lightMesh.GetPosition());

        assModel->Draw(*lightMesh.GetLightShader());

        model = glm::translate(glm::mat4(), glm::vec3(-10.0f, 3.0f, -2.0f)) * glm::scale(glm::mat4(), glm::vec3(0.5f, 0.5f, 0.5f));
        lightMesh.GetLightShader()->SetUniform("model", model);

        assModel2->Draw(*lightMesh.GetLightShader());
        
        model = glm::translate(glm::mat4(), glm::vec3(20.0f, 3.0f, -2.0f)) * glm::scale(glm::mat4(), glm::vec3(0.5f, 0.5f, 0.5f));
        lightMesh.GetLightShader()->SetUniform("model", model);
        assModel3->Draw(*lightMesh.GetLightShader());

       
       
        /*
            // Draw.
            texture[i].Bind(0);
            mesh[i].DrawMesh();
            texture[i].Unbind();
        */


        /*
        // Draw light.
        model = glm::translate(glm::mat4(), lightPos) * glm::scale(glm::mat4(), lightScale);
        lightShader.Use();

        // Set uniforms.
        lightShader.SetUniform("lightColor", lightColor);

        lightShader.SetUniform("model", model);
        lightShader.SetUniform("view", view);
        lightShader.SetUniform("projection", projection);

        lightMesh.DrawMesh();
        */

        lightMesh.Draw(projection, view, viewPos);
        lightRed.Draw(projection, view, viewPos);
        lightGreen.Draw(projection, view, viewPos);
        lightWhite.Draw(projection, view, viewPos);



        glfwSwapBuffers(g_pWindow); // Double buffered application.
        

        // Update window FPS title.
        ShowFPS(g_pWindow);

        lastTime = currTime;
    }




    glfwTerminate();
    return 0;
}



void KeyInputCallback(GLFWwindow* wnd, int key, int scancode, int action, int mode){

    if (action == GLFW_PRESS) { // On key press.

        switch (key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(wnd, GL_TRUE); // Close window on Esc key press.
            break;


        case GLFW_KEY_SPACE:
            g_bFullLight = (g_bFullLight == true) ? false : true;
            break;

        default:
            break;
        }
    }
}

void ShowFPS(GLFWwindow* wnd) {

    static double prev = 0.0;
    static int frameCount = 0;
    double elapsed;
    double currTime = glfwGetTime(); // Get number of seconds since GLFW start.

    elapsed = currTime - prev;

    // Limit update of FPS.
    if (elapsed > 0.25) {

        prev = currTime;

        double fps = (double)frameCount / elapsed; // Determine number of frames per second.
        double msPerFrame = 1000.0 / fps;

        // Update window title bar.
        std::ostringstream sout;
        sout.precision(3);
        sout << std::fixed << APP_TITLE << "    " << "FPS:" << fps << "  FrameTime:" << msPerFrame << "(ms)";
    
    
        // Set window title.
        glfwSetWindowTitle(wnd, sout.str().c_str());


        // Reset values.
        frameCount = 0;
    }

    frameCount++;
}


bool InitOpenGL() {

    // Init GLFW.
    if (!glfwInit()) {

        return false;
    }

    // Config window and create.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Setting the opengl version to 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Usage of modern opengl.


    if (FULL_SCREEN) {

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* vidMode = glfwGetVideoMode(monitor);
        if (vidMode) {

            g_WindowHeight = vidMode->height;
            g_WindowWidth = vidMode->width;

            g_pWindow = glfwCreateWindow(vidMode->width, vidMode->height, APP_TITLE, monitor, NULL);
            glfwWindowHint(GLFW_DECORATED, GL_FALSE);
        }
    }
    else {

        g_pWindow = glfwCreateWindow(g_WindowWidth, g_WindowHeight, APP_TITLE, 0, 0);
    }

    if (!g_pWindow) {

        glfwTerminate();
        return false;
    }
    else {
        glfwMakeContextCurrent(g_pWindow);

    }



    // Initialize GLEW.
    glewExperimental = GL_TRUE; // Set up modern opengl properly.
    GLenum result = glewInit();
    if (result != GLEW_OK) {

        glfwTerminate();
        return false;
    }



    // Specify input callback for input.
    glfwSetKeyCallback(g_pWindow, KeyInputCallback);

    // Specify mouse input callback...
    glfwSetCursorPosCallback(g_pWindow, MouseMoveCallback);

    // Specify wheel scroll callback
    glfwSetScrollCallback(g_pWindow, MouseScrollCallback);

    // For FPS Camera. Set Cursor disabled and free movement.
    glfwSetInputMode(g_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(g_pWindow, g_WindowWidth / 2.0, g_WindowHeight / 2.0); // So camera pos is always in middle of screen.


    // Set clear color.
    glClearColor(0.0f, 0.25f, 0.38f, 1.0f);

    // Init viewport.
    glViewport(0, 0, g_WindowWidth, g_WindowHeight); // Defines where to draw to..

    glEnable(GL_DEPTH_TEST); // Enable depth testing for 3D.

    return true;
}


void OnWindowResize(GLFWwindow* wnd, int width, int height){

    g_WindowWidth = width;
    g_WindowHeight = height;

    glViewport(0, 0, g_WindowWidth, g_WindowHeight); // Defines where to draw to..

}


void MouseMoveCallback(GLFWwindow* wnd, double posX, double posY) {

    // Only for orbit camera.

    /*
    static glm::vec2 lastMousePos = glm::vec2(0, 0);

    if (glfwGetMouseButton(g_pWindow, GLFW_MOUSE_BUTTON_LEFT) == 1) {

        g_fYaw -= ((float)posX - lastMousePos.x) * MOUSE_SENSITIVITY;

        g_fPitch += ((float)posY - lastMousePos.y) * MOUSE_SENSITIVITY;
    }

    if (glfwGetMouseButton(g_pWindow, GLFW_MOUSE_BUTTON_RIGHT) == 1) {

        float dx = 0.01f * ((float)posX - lastMousePos.x);
        float dy = 0.01f * ((float)posY - lastMousePos.y);

        g_fRadius += dx - dy;
    }

    lastMousePos.x = (posX);
    lastMousePos.y = (posY);
    */
}

void MouseScrollCallback(GLFWwindow* wnd, double deltaX, double deltaY){

    double fov = g_pFPSCamera.GetFieldOfView() + deltaY * ZOOM_SENSITIVITY;

    fov = glm::clamp(fov, 1.0, 120.0);

    g_pFPSCamera.SetFieldOfView((float)fov);
}

void Update(double d){

    double mousex, mousey;

    glfwGetCursorPos(g_pWindow, &mousex, &mousey);

    g_pFPSCamera.Rotate((float)(g_WindowWidth / 2.0 - mousex) * MOUSE_SENSITIVITY, (float)(g_WindowHeight / 2.0 - mousey) * MOUSE_SENSITIVITY);


    glfwSetCursorPos(g_pWindow, g_WindowWidth / 2.0, g_WindowHeight / 2.0);


    // Camera movement.
    if (glfwGetKey(g_pWindow, GLFW_KEY_W) == GLFW_PRESS) {

        g_pFPSCamera.Move(MOVE_SPEED * (float)(d) * g_pFPSCamera.GetLookTo());
    }
    else if(glfwGetKey(g_pWindow, GLFW_KEY_S) == GLFW_PRESS) {

        g_pFPSCamera.Move(MOVE_SPEED * (float)(d) * -g_pFPSCamera.GetLookTo());
    }



    if (glfwGetKey(g_pWindow, GLFW_KEY_A) == GLFW_PRESS) {

        g_pFPSCamera.Move(MOVE_SPEED * (float)(d) * -g_pFPSCamera.GetRight());
    }
    else if (glfwGetKey(g_pWindow, GLFW_KEY_D) == GLFW_PRESS) {

        g_pFPSCamera.Move(MOVE_SPEED * (float)(d) * g_pFPSCamera.GetRight());
    }




    if (glfwGetKey(g_pWindow, GLFW_KEY_Z) == GLFW_PRESS) {

        g_pFPSCamera.Move(MOVE_SPEED * (float)(d)*g_pFPSCamera.GetUp());
    }
    else if (glfwGetKey(g_pWindow, GLFW_KEY_Y) == GLFW_PRESS) {

        g_pFPSCamera.Move(MOVE_SPEED * (float)(d) * -g_pFPSCamera.GetUp());
    }

}
