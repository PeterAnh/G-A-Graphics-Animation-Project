
#include "Angel.h"

#include <stdlib.h>
#include <dirent.h>
#include <time.h>

// Open Asset Importer header files (in ../../assimp--3.0.1270/include)
// This is a standard open source library for loading meshes, see gnatidread.h
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

GLint windowHeight = 640, windowWidth = 960;

// gnatidread.cpp is the CITS3003 "Graphics n Animation Tool Interface & Data
// Reader" code.  This file contains parts of the code that you shouldn't need
// to modify (but, you can).
#include "gnatidread.h"

using namespace std; // Import the C++ standard functions (e.g., min)

// IDs for the GLSL program and GLSL variables.
GLuint shaderProgram;   // The number identifying the GLSL shader program

// IDs for vshader input vars (from glGetAttribLocation)
GLuint vPosition;
GLuint vNormal;
GLuint vTexCoord;

// IDs for uniform variables (from glGetUniformLocation)
GLuint projectionU;
GLuint modelViewU;
GLuint textureU;
GLuint texScaleU;
GLuint lightCountU;

struct LightUniform
{
    GLuint lightPositionU;
    GLuint ambientProductU;
    GLuint diffuseProductU;
    GLuint specularProductU;
    GLuint lightDirectionalU;
};

const int maxLights = 16; 
LightUniform lightsU[maxLights];
GLuint shininessU;

// Distance from the camera to the centre of the scene
static float viewDist = 1.5;

// rotates the camera sideways around the centre
static float camRotSidewaysDeg = 0;

// rotates the camera up and over the centre.
static float camRotUpAndOverDeg = 20;

mat4 projection; // Projection matrix - set in the reshape function
mat4 view;       // View matrix - set in the display function.

// These are used to set the window title
char lab[] = "Project1";
char *programName = nullptr; // Set in main
int numDisplayCalls = 0;  // Used to calculate the number of frames per second

//------Meshes----------------------------------------------------------------
// Uses the type aiMesh from ../../assimp--3.0.1270/include/assimp/mesh.h
//                           (numMeshes is defined in gnatidread.h)
aiMesh *meshes[numMeshes]; // An array of mesh pointer's to draw
GLuint vaoIDs[numMeshes];  // and a corresponding VAO ID from glGenVertexArrays

// -----Textures----------------------------------------------------------------
//                           (numTextures is defined in gnatidread.h)
texture *textures[numTextures]; // Texture pointers - see gnatidread.h
GLuint textureIDs[numTextures]; // Stores the IDs returned by glGenTextures

//------Scene Objects---------------------------------------------------------
//
// For each object in a scene we store the following
// Note: the following is exactly what the sample solution uses, you can do 
// things differently if you want.
struct SceneObject
{
    vec4    loc;
    float   scale;
    float   angles[3];                  // rotations around X, Y and Z axes.
    float   diffuse, specular, ambient; // Amount of each light component
    float   shine;
    vec3    rgb;
    float   brightness; // Multiplies all colours
    int     meshId;
    int     texId;
    float   texScale;
    int     directional;
};

const int maxObjects = 1024; // Scenes with more than 1024 objects seem unlikely

SceneObject sceneObjs[maxObjects];  // An array of objects currently in the scene.
int objectCount = 0;                 // How many objects are currenly in the scene.
SceneObject *currObject = nullptr;  // The current object
SceneObject *toolObj = nullptr;     // The object currently being modified
SceneObject *ground = nullptr;      // Ground object
SceneObject *lights[maxLights];     // Light objects
int lightCount = 0;                 // Number of lights in the scene.

//----------------------------------------------------------------------------
//
// Loads a texture by number, and binds it for later use.
void loadTextureIfNotAlreadyLoaded(int i)
{
    if (textures[i] != nullptr)
        return; // The texture is already loaded.

    textures[i] = loadTextureNum(i);
    CheckError();
    glActiveTexture(GL_TEXTURE0);
    CheckError();

    // Based on: http://www.opengl.org/wiki/Common_Mistakes
    glBindTexture(GL_TEXTURE_2D, textureIDs[i]);
    CheckError();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textures[i]->width, textures[i]->height,
                 0, GL_RGB, GL_UNSIGNED_BYTE, textures[i]->rgbData);
    CheckError();
    glGenerateMipmap(GL_TEXTURE_2D);
    CheckError();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    CheckError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    CheckError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    CheckError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    CheckError();

    glBindTexture(GL_TEXTURE_2D, 0);
    CheckError(); // Back to default texture
}

//------Mesh loading----------------------------------------------------------
//
// The following uses the Open Asset Importer library via loadMesh in
// gnatidread.h to load models in .x format, including vertex positions,
// normals, and texture coordinates.
// You shouldn't need to modify this - it's called from drawMesh below.
void loadMeshIfNotAlreadyLoaded(int meshNumber)
{
    if (meshNumber >= numMeshes || meshNumber < 0)
    {
        printf("Error - no such model number");
        exit(1);
    }

    if (meshes[meshNumber] != nullptr)
        return; // Already loaded

    aiMesh *mesh = loadMesh(meshNumber);
    meshes[meshNumber] = mesh;

    glBindVertexArrayAPPLE(vaoIDs[meshNumber]);

    // Create and initialize a buffer object for positions and texture 
    // coordinates, initially empty. mesh->mTextureCoords[0] has space for up 
    // to 3 dimensions, but we only need 2.
    GLuint buffer[1];
    glGenBuffers(1, buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(float) * (3 + 3 + 3) * mesh->mNumVertices, nullptr,
                 GL_STATIC_DRAW);

    int nVerts = mesh->mNumVertices;
    // Next, we load the position and texCoord data in parts.
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 3 * nVerts,
                    mesh->mVertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * 3 * nVerts,
                    sizeof(float) * 3 * nVerts, mesh->mTextureCoords[0]);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * 6 * nVerts,
                    sizeof(float) * 3 * nVerts, mesh->mNormals);

    // Load the element index data
    GLuint elements[mesh->mNumFaces * 3];
    for (GLuint i = 0; i < mesh->mNumFaces; i++)
    {
        elements[i * 3] = mesh->mFaces[i].mIndices[0];
        elements[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
        elements[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
    }

    GLuint elementBufferId[1];
    glGenBuffers(1, elementBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferId[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh->mNumFaces * 3,
                 elements, GL_STATIC_DRAW);

    // vPosition is 4D - the conversion sets the fourth dimension (w) to 1.0
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 
    BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vPosition);

    // vTexCoord is 2D - the third dimension is ignored (it's always 0.0)
    glVertexAttribPointer(
        vTexCoord, 3, GL_FLOAT, GL_FALSE, 0,
        BUFFER_OFFSET(sizeof(float) * 3 * mesh->mNumVertices));
    glEnableVertexAttribArray(vTexCoord);
    glVertexAttribPointer(
        vNormal, 3, GL_FLOAT, GL_FALSE, 0,
        BUFFER_OFFSET(sizeof(float) * 6 * mesh->mNumVertices));
    glEnableVertexAttribArray(vNormal);
    CheckError();
}

//----------------------------------------------------------------------------

static void mouseClickOrScroll(int button, int state, int x, int y)
{
    const int WHEEL_UP = 3;
    const int WHEEL_DOWN = 4;

    switch (button)
    {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN)
                if (glutGetModifiers() == GLUT_ACTIVE_SHIFT)
                    activateTool(GLUT_MIDDLE_BUTTON);
                else
                    activateTool(GLUT_LEFT_BUTTON);
            else if (state == GLUT_UP)
                deactivateTool();
            break;
        case GLUT_MIDDLE_BUTTON:
            if (state == GLUT_DOWN)
                activateTool(button);
            else if (state == GLUT_UP)
                deactivateTool();
            break;
        case WHEEL_UP:
            viewDist = (viewDist < 0.0 ? viewDist : viewDist * 0.8) - 0.05;
            break;
        case WHEEL_DOWN:
            viewDist = (viewDist < 0.0 ? viewDist : viewDist * 1.25) + 0.05;
            break;
        default:
            break;
    }
}

//----------------------------------------------------------------------------

static void mousePassiveMotion(int x, int y)
{
    mouseX = x;
    mouseY = y;
}

//----------------------------------------------------------------------------

mat2 camRotZ()
{
    return rotZ(-camRotSidewaysDeg) * mat2(10.0, 0, 0, -10.0);
}

//------callback functions for doRotate below and later-----------------------

static void adjustCamrotsideViewdist(vec2 cv)
{
    camRotSidewaysDeg += cv[0];
    viewDist += cv[1];
}

static void adjustcamSideUp(vec2 su)
{
    camRotSidewaysDeg += su[0];
    camRotUpAndOverDeg += su[1];
}

static void adjustLocXZ(vec2 xz)
{
    toolObj->loc[0] += xz[0];
    toolObj->loc[2] += xz[1];
}

static void adjustScaleY(vec2 sy)
{
    toolObj->scale += sy[0];
    toolObj->loc[1] += sy[1];
}

//----------------------------------------------------------------------------
//------Set the mouse buttons to rotate the camera----------------------------
//------around the centre of the scene.---------------------------------------
//----------------------------------------------------------------------------

static void doRotate()
{
    setToolCallbacks(adjustCamrotsideViewdist, mat2(400, 0, 0, -2),
                     adjustcamSideUp, mat2(400, 0, 0, -90));
}

//------Add an object to the scene--------------------------------------------

static SceneObject* addObject(int meshId)
{
    currObject = toolObj = &sceneObjs[objectCount++];

    vec2 currPos = currMouseXYworld(camRotSidewaysDeg);
    currObject->loc[0] = currPos[0];
    currObject->loc[1] = 0.0;
    currObject->loc[2] = currPos[1];
    currObject->loc[3] = 1.0;

    if (meshId != MeshId::Ground && meshId != MeshId::Sphere)
        currObject->scale = 0.005;

    currObject->rgb = 0.7;
    currObject->rgb[1] = 0.7;
    currObject->rgb[2] = 0.7;
    currObject->brightness = 1.0;

    currObject->diffuse = 1.0;
    currObject->specular = 0.5;
    currObject->ambient = 0.7;
    currObject->shine = 10.0;

    currObject->angles[0] = 0.0;
    currObject->angles[1] = 180.0;
    currObject->angles[2] = 0.0;

    currObject->meshId = meshId;
    currObject->texId = rand() % numTextures;
    currObject->texScale = 2.0;
    currObject->directional = GL_FALSE;


    setToolCallbacks(adjustLocXZ, camRotZ(),
                     adjustScaleY, mat2(0.05, 0, 0, 10.0));
    glutPostRedisplay();
    return currObject;
}

static void removeObject()
{
    if (objectCount > 0)
    {
        if (currObject == &sceneObjs[objectCount - 1])
        {
            currObject = (currObject == sceneObjs) ? nullptr : currObject - 1;
        }

        if (toolObj == &sceneObjs[objectCount])
        {
            toolObj = nullptr;
            deactivateTool();
        }

        objectCount--;
    }
}

//------The init function-----------------------------------------------------

void init(void)
{
    // Initialize random seed - so the starting scene varies
    srand(time(nullptr)); 
    aiInit();

    // Allocate vertex array objects for meshes
    glGenVertexArraysAPPLE(numMeshes, vaoIDs);
    CheckError();

    // Allocate texture objects
    glGenTextures(numTextures, textureIDs);
    CheckError(); 

    // Load shaders and use the resulting shader program
    shaderProgram = InitShader("vStart.glsl", "fStart.glsl");

    glUseProgram(shaderProgram);
    CheckError();

    // Initialize the attributes from the vertex shader
    vPosition = glGetAttribLocation(shaderProgram, "vPosition");
    CheckError();
    vNormal = glGetAttribLocation(shaderProgram, "vNormal");
    CheckError();
    vTexCoord = glGetAttribLocation(shaderProgram, "vTexCoord");
    CheckError();

    // Initialize the uniform locations from the shaders
    projectionU = glGetUniformLocation(shaderProgram, "Projection");
    CheckError();
    modelViewU = glGetUniformLocation(shaderProgram, "ModelView");
    CheckError();
    shininessU = glGetUniformLocation(shaderProgram, "Shininess");
    CheckError();
    textureU = glGetUniformLocation(shaderProgram, "texture");
    CheckError();
    texScaleU = glGetUniformLocation(shaderProgram, "texScale");
    CheckError();
    lightCountU = glGetUniformLocation(shaderProgram, "LightCount");
    CheckError();

    for (int i = 0; i < maxLights; i++)
    {
        const char *l = ("LightPosition[" + std::to_string(i) + "]").c_str();
        lightsU[i].lightPositionU = glGetUniformLocation(shaderProgram, l);
        CheckError();

        const char *a = ("AmbientProduct[" + std::to_string(i) + "]").c_str();
        lightsU[i].ambientProductU = glGetUniformLocation(shaderProgram, a);
        CheckError();

        const char *d = ("DiffuseProduct[" + std::to_string(i) + "]").c_str();
        lightsU[i].diffuseProductU = glGetUniformLocation(shaderProgram, d);
        CheckError();

        const char *s = ("SpecularProduct[" + std::to_string(i) + "]").c_str();
        lightsU[i].specularProductU = glGetUniformLocation(shaderProgram, s);
        CheckError();

        const char *r = ("UseDirectional[" + std::to_string(i) + "]").c_str();
        lightsU[i].lightDirectionalU = glGetUniformLocation(shaderProgram, r);
        CheckError();
    }

    // Objects 0, and 1 are the ground and the first light.
    ground = addObject(0);                  // Square for the ground
    ground->loc = vec4(0.0, 0.0, 0.0, 1.0);
    ground->scale = 10.0;
    ground->angles[0] = 90.0;               // Rotate it.
    ground->texScale = 5.0;                 // Repeat the texture.

    lights[0] = addObject(55);              // Sphere for the first light
    lights[0]->loc = vec4(2.0, 1.0, 1.0, 1.0);
    lights[0]->scale = 0.1;
    lights[0]->texId = 0;                   // Plain texture
    lights[0]->brightness = 1.0;            // The light's brightness.
    lightCount++;

    lights[1] = addObject(55);              // Sphere for the first light
    lights[1]->loc = vec4(-2.0, 1.0, 1.0, 1.0);
    lights[1]->scale = 0.4;
    lights[1]->texId = 0;                   // Plain texture
    lights[1]->brightness = 1.0;            // The light's brightness.
    lights[1]->directional = GL_TRUE;       // Directional 
    lightCount++;

    addObject(rand() % numMeshes); // A test mesh

    SceneObject* originObj = addObject(55);
    originObj->loc = vec4(0.0);
    originObj->rgb = vec3(1.0, 0.0, 0.0);
    originObj->scale = 0.3;
    originObj->texId = 0;
    originObj->brightness = 1.0;

    // We need to enable the depth test to discard fragments that
    // are behind previously drawn fragments for the same pixel.
    glEnable(GL_DEPTH_TEST);
    doRotate();                       // Start in camera rotate mode.
    glClearColor(0.0, 0.0, 0.0, 1.0); /* black background */
}

//----------------------------------------------------------------------------

void drawMesh(SceneObject& sceneObj)
{

    // Activate a texture, loading if needed.
    loadTextureIfNotAlreadyLoaded(sceneObj.texId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureIDs[sceneObj.texId]);

    // Texture 0 is the only texture type in this program, and is for the rgb
    // colour of the surface but there could be separate types for, e.g.,
    // specularity and normals.
    glUniform1i(textureU, 0);

    // Set the texture scale for the shaders
    glUniform1f(texScaleU, sceneObj.texScale);

    // Set the projection matrix for the shaders
    glUniformMatrix4fv(projectionU, 1, GL_TRUE, projection);

    // Set the model matrix - this should combine translation, rotation and
    // scaling based on what's in the sceneObj structure (see near the top of
    // the program).

    mat4 model = Translate(sceneObj.loc) 
        * RotateX(sceneObj.angles[0]) 
        * RotateY(sceneObj.angles[1]) 
        * RotateZ(sceneObj.angles[2])  
        * Scale(sceneObj.scale);

    // Set the model-view matrix for the shaders
    glUniformMatrix4fv(modelViewU, 1, GL_TRUE, view * model);

    // Activate the VAO for a mesh, loading if needed.
    loadMeshIfNotAlreadyLoaded(sceneObj.meshId);
    CheckError();
    glBindVertexArrayAPPLE(vaoIDs[sceneObj.meshId]);
    CheckError();

    glDrawElements(GL_TRIANGLES, meshes[sceneObj.meshId]->mNumFaces * 3,
                   GL_UNSIGNED_INT, NULL);
    CheckError();
}

//----------------------------------------------------------------------------

void display(void)
{
    numDisplayCalls++;

    // May report a harmless GL_INVALID_OPERATION with GLEW on the first frame
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    CheckError(); 

    // Set the view matrix.  To start with this just moves the camera
    // backwards.  You'll need to add appropriate rotations.
    view = Translate(0.0, 0.0, -viewDist) *
           RotateX(camRotUpAndOverDeg) *
           RotateY(camRotSidewaysDeg);

    for (int i = 0; i < lightCount; i++)
    {
        vec4 lightPosition = lights[i]->directional
            ? (view * lights[i]->loc) - (view * vec4(0.0, 0.0, 0.0, 1.0))
            : view * lights[i]->loc;

        glUniform4fv(lightsU[i].lightPositionU, 1, lightPosition);
        CheckError();

        glUniform1i(lightsU[i].lightDirectionalU, lights[i]->directional);
        CheckError();
    }

    for (int i = 0; i < objectCount; i++)
    {
        SceneObject& so = sceneObjs[i];

        glUniform1i(lightCountU, lightCount);

        for (int j = 0; j < lightCount; j++)
        {
            SceneObject& light = *lights[j];
            LightUniform& lightU = lightsU[j];

            vec3 rgb = so.rgb * light.rgb * so.brightness * light.brightness;
            glUniform3fv(lightU.ambientProductU, 1, so.ambient * rgb);
            CheckError();
            glUniform3fv(lightU.diffuseProductU, 1, so.diffuse * rgb);
            CheckError();
            glUniform3fv(lightU.specularProductU, 1, so.specular * rgb);
            CheckError();
        }

        glUniform1f(shininessU, so.shine);
        CheckError();

        drawMesh(so);
    }

    glutSwapBuffers();
}

//----------------------------------------------------------------------------
//------Menus-----------------------------------------------------------------
//----------------------------------------------------------------------------

struct MenuId
{
    enum Id
    {
        RgbBrightness = 10,
        BasicLighting = 20,
        PositionScale = 41,
        RotateMoveCamera = 50,
        RotationTextureScale = 55,
        RemoveObject = 60,
        MoveLight1 = 70,
        RgbLight1 = 71,
        MoveLight2 = 80,
        RgbLight2 = 81,
        Exit = 99
    };
};

static void objectMenu(int id)
{
    deactivateTool();
    addObject(id);
}

static void texMenu(int id)
{
    deactivateTool();
    if (currObject != nullptr)
    {
        currObject->texId = id;
        glutPostRedisplay();
    }
}

static void groundMenu(int id)
{
    deactivateTool();
    ground->texId = id;
    glutPostRedisplay();
}

static void adjustBrightnessY(vec2 by)
{
    toolObj->brightness += by[0];
    toolObj->loc[1] += by[1];
}

static void adjustRedGreen(vec2 rg)
{
    toolObj->rgb[0] += rg[0];
    toolObj->rgb[1] += rg[1];
}

static void adjustBlueBrightness(vec2 bl_br)
{
    toolObj->rgb[2] += bl_br[0];
    toolObj->brightness += bl_br[1];
}

static void adjustAmbientDiffuse(vec2 am_dif)
{
    toolObj->ambient += am_dif[0];
    toolObj->diffuse += am_dif[1];
}

static void adjustSpecularShine(vec2 spec_shine)
{
    toolObj->specular += spec_shine[0];
    toolObj->shine += spec_shine[1];
}

static void lightMenu(int id)
{
    deactivateTool();
    switch (id)
    {
    case MenuId::MoveLight1:
        toolObj = lights[0];
        setToolCallbacks(adjustLocXZ, camRotZ(),
                         adjustBrightnessY, mat2(1.0, 0.0, 0.0, 10.0));
        break;
    case MenuId::RgbLight1:
        toolObj = lights[0];
        setToolCallbacks(adjustRedGreen, mat2(1.0, 0, 0, 1.0),
                         adjustBlueBrightness, mat2(1.0, 0, 0, 1.0));
        break;
    case MenuId::MoveLight2:
        toolObj = lights[1];
        setToolCallbacks(adjustLocXZ, camRotZ(),
                         adjustBrightnessY, mat2(1.0, 0.0, 0.0, 10.0));
        break;
    case MenuId::RgbLight2:
        toolObj = lights[1];
        setToolCallbacks(adjustRedGreen, mat2(1.0, 0, 0, 1.0),
                         adjustBlueBrightness, mat2(1.0, 0, 0, 1.0));
        break;
    default:
        printf("Error in lightMenu\n");
        exit(1);
        break;
    }
}

static int createArrayMenu
(
    int size, 
    const char menuEntries[][128], 
    void (*menuFn)(int)
)
{
    int nSubMenus = (size - 1) / 10 + 1;
    int subMenus[nSubMenus];

    for (int i = 0; i < nSubMenus; i++)
    {
        subMenus[i] = glutCreateMenu(menuFn);
        CheckError();

        for (int j = i * 10 + 1; j <= min(i * 10 + 10, size); j++)
        {
            glutAddMenuEntry(menuEntries[j - 1], j);
            CheckError();
        }
    }

    int menuId = glutCreateMenu(menuFn);
    CheckError();

    for (int i = 0; i < nSubMenus; i++)
    {
        char num[6];
        sprintf(num, "%d-%d", i * 10 + 1, min(i * 10 + 10, size));
        glutAddSubMenu(num, subMenus[i]);
        CheckError();
    }

    return menuId;
}

static void materialMenu(int id)
{
    deactivateTool();

    if (currObject == nullptr)
        return;

    switch (id)
    {
    case MenuId::RgbBrightness:
        toolObj = currObject;
        setToolCallbacks(
            adjustRedGreen, mat2(1, 0, 0, 1),
            adjustBlueBrightness, mat2(1, 0, 0, 1));
        break;
    // You'll need to fill in the remaining menu items here.
    case MenuId::BasicLighting:
        toolObj = currObject;
        setToolCallbacks(
            adjustAmbientDiffuse, mat2(1, 0, 0, 1),
            adjustSpecularShine, mat2(1, 0, 0, 1));
        break;
    default:
        printf("Error in materialMenu\n");
    }
}

static void adjustAngleYX(vec2 angle_yx)
{
    currObject->angles[1] += angle_yx[0];
    currObject->angles[0] += angle_yx[1];
}

static void adjustAngleZTexscale(vec2 az_ts)
{
    currObject->angles[2] += az_ts[0];
    currObject->texScale += az_ts[1];
}

static void mainmenu(int id)
{
    deactivateTool();

    switch (id)
    {
    case MenuId::PositionScale:
        if (currObject != nullptr)
        {
            toolObj = currObject;
            setToolCallbacks(adjustLocXZ, camRotZ(),
                             adjustScaleY, mat2(0.05, 0, 0, 10));
        }
        break;
    case MenuId::RotateMoveCamera:
        doRotate();
        break;
    case MenuId::RotationTextureScale:
        if (currObject != nullptr)
            setToolCallbacks(adjustAngleYX, mat2(400, 0, 0, -400),
                             adjustAngleZTexscale, mat2(400, 0, 0, 15));
        break;
    case MenuId::RemoveObject:
        removeObject();
        break;
    case MenuId::Exit:
        exit(0);
        break;
    default:
        break;
    }
}

static void makeMenu()
{
    int objectId = createArrayMenu(numMeshes, objectMenuEntries, objectMenu);

    int materialMenuId = glutCreateMenu(materialMenu);
    glutAddMenuEntry("R/G/B/All", MenuId::RgbBrightness);
    glutAddMenuEntry("Ambient/Diffuse/Specular/Shine", MenuId::BasicLighting);

    int texMenuId = createArrayMenu(numTextures, textureMenuEntries, texMenu);
    int groundMenuId = createArrayMenu(numTextures, textureMenuEntries,
                                       groundMenu);

    int lightMenuId = glutCreateMenu(lightMenu);
    glutAddMenuEntry("Move Light 1",        MenuId::MoveLight1);
    glutAddMenuEntry("R/G/B/All Light 1",   MenuId::RgbLight1);
    glutAddMenuEntry("Move Light 2",        MenuId::MoveLight2);
    glutAddMenuEntry("R/G/B/All Light 2",   MenuId::RgbLight2);

    glutCreateMenu  (mainmenu);
    glutAddMenuEntry("Rotate/Move Camera",      MenuId::RotateMoveCamera);
    glutAddSubMenu  ("Add object",              objectId);
    glutAddMenuEntry("Remove object",           MenuId::RemoveObject);
    glutAddMenuEntry("Position/Scale",          MenuId::PositionScale);
    glutAddMenuEntry("Rotation/Texture Scale",  MenuId::RotationTextureScale);
    glutAddSubMenu  ("Material",                materialMenuId);
    glutAddSubMenu  ("Texture",                 texMenuId);
    glutAddSubMenu  ("Ground Texture",          groundMenuId);
    glutAddSubMenu  ("Lights",                  lightMenuId);
    glutAddMenuEntry("EXIT",                    MenuId::Exit);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

//----------------------------------------------------------------------------

void keyboard(unsigned char key, int x, int y)
{
    const uint8_t KEY_ESC = 033;

    switch (key)
    {
    case KEY_ESC:
        exit(EXIT_SUCCESS);
        break;
    }
}

//----------------------------------------------------------------------------

void idle(void)
{
    glutPostRedisplay();
}

//----------------------------------------------------------------------------

void reshape(int width, int height)
{
    windowWidth = width;
    windowHeight = height;

    glViewport(0, 0, width, height);

    // You'll need to modify this so that the view is similar to that in the
    // sample solution.
    // In particular:
    //     - the view should include "closer" visible objects (slightly tricky)
    //     - when the width is less than the height, the view should adjust so
    //         that the same part of the scene is visible across the width of
    //         the window.

    GLfloat near = 0.01;
    GLfloat far = 100.0;
    GLfloat right = near;
    GLfloat top = near * height / width;

    if (width > height)
    {
        top = near;
        right = near * width / height;
    }

    projection = Frustum(-right, right, -top, top, near, far);
}

//----------------------------------------------------------------------------

void timer(int unused)
{
    char title[256];
    sprintf(title, "%s %s: %d Frames Per Second @ %d x %d",
            lab, programName, numDisplayCalls, windowWidth, windowHeight);

    glutSetWindowTitle(title);

    numDisplayCalls = 0;
    glutTimerFunc(1000, timer, 1);
}

//----------------------------------------------------------------------------

char dirDefault1[] = "../../models-textures";
char dirDefault3[] = "/tmp/models-textures";
char dirDefault4[] = "/d/models-textures";
char dirDefault2[] = "/cslinux/examples/CITS3003/project-files/models-textures";

void fileErr(char *fileName)
{
    printf("Error reading file: %s\n", fileName);
    printf("When not in the CSSE labs, you will need to include the directory "
           "containing\n");
    printf("the models on the command line, or put it in the same folder as the"
           " exectutable.");
    exit(1);
}

//----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    // Get the program name, excluding the directory, for the window title
    programName = argv[0];
    for (char *cpointer = argv[0]; *cpointer != 0; cpointer++)
        if (*cpointer == '/' || *cpointer == '\\')
            programName = cpointer + 1;

    // Set the models-textures directory, via the first argument or some handy
    // defaults.
    if (argc > 1)
        strcpy(dataDir, argv[1]);
    else if (opendir(dirDefault1))
        strcpy(dataDir, dirDefault1);
    else if (opendir(dirDefault2))
        strcpy(dataDir, dirDefault2);
    else if (opendir(dirDefault3))
        strcpy(dataDir, dirDefault3);
    else if (opendir(dirDefault4))
        strcpy(dataDir, dirDefault4);
    else
        fileErr(dirDefault1);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);

    // glutInitContextVersion( 3, 2);
    // // May cause issues, sigh, but you should still use only OpenGL 3.2 Core
    // glutInitContextProfile( GLUT_CORE_PROFILE );            
    // glutInitContextProfile( GLUT_COMPATIBILITY_PROFILE );
    // features.
    glutCreateWindow("Initialising...");
    CheckError();

    // With some old hardware yields GL_INVALID_ENUM, if so use 
    // glewExperimental. This bug is explained at: 
    // http://www.opengl.org/wiki/OpenGL_Loading_Library
    // glewInit();

    init();
    CheckError();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);

    glutMouseFunc(mouseClickOrScroll);
    glutPassiveMotionFunc(mousePassiveMotion);
    glutMotionFunc(doToolUpdateXY);

    glutReshapeFunc(reshape);
    glutTimerFunc(1000, timer, 1);
    CheckError();

    makeMenu();
    CheckError();

    glutMainLoop();
    return 0;
}
