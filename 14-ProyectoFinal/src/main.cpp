#define _USE_MATH_DEFINES
#include <cmath>
//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>
#include <limits>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

// Shader include
#include "Headers/Shader.h"

// Model geometric includes
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/FirstPersonCamera.h"
#include "Headers/ThirdPersonCamera.h"
#include "Headers/AerialCamera.h"

// Font rendering include
#include "Headers/FontTypeRendering.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Texture.h"

// Include loader Model class
#include "Headers/Model.h"

// Include Terrain
#include "Headers/Terrain.h"

// Include Colision headers functions
#include "Headers/Colisiones.h"

// ShadowBox include
#include "Headers/ShadowBox.h"

// OpenAL include
#include <AL/alut.h>

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int screenWidth;
int screenHeight;

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

GLFWwindow *window;

Shader shader;
//Shader con skybox
Shader shaderSkybox;
//Shader con multiples luces
Shader shaderMulLighting;
//Shader para el terreno
Shader shaderTerrain;
// Shader para dibujar un objeto con solo textura
Shader shaderTexture;
//Shader para dibujar el buffer de profunidad
Shader shaderDepth;
// Shader para visualizar el buffer de profundidad
Shader shaderViewDepth;
//Shader para las particulas de fountain
Shader shaderParticlesFountain;
//Shader para las particulas de fuego
Shader shaderParticlesFire;

std::shared_ptr<Camera> camera(new ThirdPersonCamera());
float distanceFromTarget = 7.0;
const float THIRD_PERSON_TARGET_HEIGHT = 1.25f;

Sphere skyboxSphere(20, 20);
Box boxCesped;
Box boxWalls;
Box boxHighway;
Box boxLandingPad;
Sphere esfera1(10, 10);
Box boxCollider;
Sphere sphereCollider(10, 10);
Cylinder rayModel(10, 10, 1.0, 1.0, 1.0);
Box boxIntro;
Box boxHeart;
Box boxViewDepth;
// Models complex instances
// Laberinto models
Model modelEntrada;
Model modelSalida;
Model modelPuertaDer;
Model modelPuertaIzq;
Model modelMuroDer;
Model modelMuroIzq;
// Laberinto additional walls
Model modelMuro1;
Model modelMuro2;
Model modelMuro3;
Model modelMuro4;
Model modelMuro5;
Model modelMuro6;
Model modelMuro7;
Model modelMuro8;
Model modelMuro9;
Model modelMuro10;
Model modelMuro11;
Model modelMuro12;
Model modelMuro13;
Model modelMuro14;
Model modelMuro15;
// Monedas del laberinto
Model modelMoneda1;
Model modelMoneda2;
Model modelMoneda3;
Model modelMoneda4;
Model modelMoneda5;
// Enemigos
Model modelEnemy1;
Model modelEnemy2;
Model modelEnemy3;
Model modelEnemy4;
Model modelEnemy5;
// Modelos animados
// Player (replaces Mayow)
Model playerModelAnimate;
// Fountain
Model modelFountain;
// Antorcha
Model modelAntocha;
// Terrain model instance
Terrain terrain(-1, -1, 200, 8, "../Textures/heightmap.png");

ShadowBox * shadowBox;

GLuint textureCespedID, textureWallID, textureWindowID, textureHighwayID, textureLandingPadID;
GLuint textureTerrainRID, textureTerrainGID, textureTerrainBID, textureTerrainBlendMapID;
GLuint skyboxTextureID;
GLuint textureInit1ID, textureInit2ID, textureActivaID;
GLuint textureScreenID, textureScreen1LifeID, textureScreen0LivesID;
GLuint textureHeartID;
GLuint textureParticleFountainID;
GLuint textureParticleFireID;

bool iniciaPartida = false;
bool presionarOpcion = false;
bool presionarSeleccionMenu = false;

// Modelo para el render del texto
FontTypeRendering::FontTypeRendering *modelText;

GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

std::string fileNames[6] = { "../Textures/SkyboxProy/redeclipse_ft.png",
	"../Textures/SkyboxProy/redeclipse_bk.png",
	"../Textures/SkyboxProy/redeclipse_up.png",
	"../Textures/SkyboxProy/redeclipse_dn.png",
	"../Textures/SkyboxProy/redeclipse_rt.png",
	"../Textures/SkyboxProy/redeclipse_lf.png" };

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

// Model matrix definitions
glm::mat4 modelMatrixPlayer = glm::mat4(1.0f);
glm::mat4 modelMatrixFountain = glm::mat4(1.0f);
glm::mat4 modelMatrixEntrada = glm::mat4(1.0f);
glm::mat4 modelMatrixSalida = glm::mat4(1.0f);
glm::mat4 modelMatrixPuertaDer = glm::mat4(1.0f);
glm::mat4 modelMatrixPuertaIzq = glm::mat4(1.0f);
glm::mat4 modelMatrixMuroDer = glm::mat4(1.0f);
glm::mat4 modelMatrixMuroIzq = glm::mat4(1.0f);
// matrices for laberinto walls
glm::mat4 modelMatrixMuro1 = glm::mat4(1.0f);
glm::mat4 modelMatrixMuro2 = glm::mat4(1.0f);
glm::mat4 modelMatrixMuro3 = glm::mat4(1.0f);
glm::mat4 modelMatrixMuro4 = glm::mat4(1.0f);
glm::mat4 modelMatrixMuro5 = glm::mat4(1.0f);
glm::mat4 modelMatrixMuro6 = glm::mat4(1.0f);
glm::mat4 modelMatrixMuro7 = glm::mat4(1.0f);
glm::mat4 modelMatrixMuro8 = glm::mat4(1.0f);
glm::mat4 modelMatrixMuro9 = glm::mat4(1.0f);
glm::mat4 modelMatrixMuro10 = glm::mat4(1.0f);
glm::mat4 modelMatrixMuro11 = glm::mat4(1.0f);
glm::mat4 modelMatrixMuro12 = glm::mat4(1.0f);
glm::mat4 modelMatrixMuro13 = glm::mat4(1.0f);
glm::mat4 modelMatrixMuro14 = glm::mat4(1.0f);
glm::mat4 modelMatrixMuro15 = glm::mat4(1.0f);
glm::mat4 modelMatrixMoneda1 = glm::mat4(1.0f);
glm::mat4 modelMatrixMoneda2 = glm::mat4(1.0f);
glm::mat4 modelMatrixMoneda3 = glm::mat4(1.0f);
glm::mat4 modelMatrixMoneda4 = glm::mat4(1.0f);
glm::mat4 modelMatrixMoneda5 = glm::mat4(1.0f);
glm::mat4 modelMatrixEnemy1 = glm::mat4(1.0f);
glm::mat4 modelMatrixEnemy2 = glm::mat4(1.0f);
glm::mat4 modelMatrixEnemy3 = glm::mat4(1.0f);
glm::mat4 modelMatrixEnemy4 = glm::mat4(1.0f);
glm::mat4 modelMatrixEnemy5 = glm::mat4(1.0f);
glm::mat4 baseMatrixEnemy1 = glm::mat4(1.0f);
glm::mat4 baseMatrixEnemy2 = glm::mat4(1.0f);
glm::mat4 baseMatrixEnemy3 = glm::mat4(1.0f);
glm::mat4 baseMatrixEnemy4 = glm::mat4(1.0f);
glm::mat4 baseMatrixEnemy5 = glm::mat4(1.0f);

struct EnemyPatrolState {
	float maxDistance;
	float traveledDistance;
	float turnAngle;
	bool turning;
	int leg;
};

EnemyPatrolState enemyPatrol1 = { 30.0f, 0.0f, 0.0f, false, 0 };
EnemyPatrolState enemyPatrol2 = { 37.5f, 0.0f, 0.0f, false, 0 };
EnemyPatrolState enemyPatrol3 = { 21.0f, 0.0f, 0.0f, false, 0 };
EnemyPatrolState enemyPatrol4 = { 35.0f, 0.0f, 0.0f, false, 0 };
EnemyPatrolState enemyPatrol5 = { 55.0f, 0.0f, 0.0f, false, 0 };

const int ENEMY_ANIMATION_TURN = 19;
const int ENEMY_ANIMATION_WALK = 24;
const float ENEMY_PATROL_SPEED = 2.25f;
const float ENEMY_TURN_SPEED = 180.0f;

std::vector<bool> monedasActivas = { true, true, true, true, true };
int contadorMonedas = 0;
int vidasJugador = 3;
double ultimoDanioJugador = -10.0;
const double TIEMPO_INVULNERABILIDAD = 1.0;
bool puertasAbiertas = false;
bool juegoGanado = false;
AbstractModel::AABB triggerPuertaIzq;
AbstractModel::AABB triggerPuertaDer;
// Antocha positions
std::vector<glm::vec3> antochaPositions = {
	glm::vec3(40.5f, 0.0f, -45.0f),
	glm::vec3(60.5f, 0.0f, 20.0f),
	glm::vec3(-19.5f, 0.0f, -35.0f),
	glm::vec3(-44.5f, 0.0f, 65.0f),
	glm::vec3(-74.0f, 0.0f, -24.0f)
};

int animationPlayerIndex = 0; // 0 = Idle by default for Player
int modelSelected = 0;
bool enableCountSelected = true;
const float GAMEPAD_DEAD_ZONE = 0.20f;
const float GAMEPAD_PLAYER_SPEED = 12.0f;
const float GAMEPAD_PLAYER_TURN_SPEED = 120.0f;
const float GAMEPAD_CAMERA_SPEED = 3.0f;

// Blending model unsorted
std::map<std::string, glm::vec3> blendingUnsorted = {
		{"fountain", glm::vec3(0.0)},
		{"fire0", antochaPositions[0]},
		{"fire1", antochaPositions[1]},
		{"fire2", antochaPositions[2]},
		{"fire3", antochaPositions[3]},
		{"fire4", antochaPositions[4]}
};

double deltaTime;
double currTime, lastTime;


// Jump variables
bool isJump = false;
float GRAVITY = 5.0;
double tmv = 0;
double startTimeJump = 0;

// Colliders
std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> > collidersOBB;
std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> > collidersSBB;
std::map<std::string, AbstractModel::AABB> collidersMonedasAABB;
std::map<std::string, AbstractModel::AABB> collidersMurosAABB;
std::map<std::string, AbstractModel::OBB> collidersEnemigosOBB;

// OpenAL Defines
#define NUM_BUFFERS 3
#define NUM_SOURCES 3
#define NUM_ENVIRONMENTS 1
// Listener
ALfloat listenerPos[] = { 0.0, 0.0, 4.0 };
ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };
ALfloat listenerOri[] = { 0.0, 0.0, 1.0, 0.0, 1.0, 0.0 };
// Source 0
ALfloat source0Pos[] = { -2.0, 0.0, 0.0 };
ALfloat source0Vel[] = { 0.0, 0.0, 0.0 };
/*
 * Audio de Darth reservado para una futura integracion con OpenAL.
 * ALfloat source2Pos[] = { 2.0, 0.0, 0.0 };
 * ALfloat source2Vel[] = { 0.0, 0.0, 0.0 };
 */
// Buffers
ALuint buffer[NUM_BUFFERS];
ALuint source[NUM_SOURCES];
ALuint environment[NUM_ENVIRONMENTS];
// Configs
ALsizei size, freq;
ALenum format;
ALvoid *data;
int ch;
ALboolean loop;
std::vector<bool> sourcesPlay = {true};

// Framesbuffers
GLuint depthMap, depthMapFBO;
// variables  para  el sistema de particulas de la fuente
GLuint initVel, startTime;
GLuint VAOParticles;
GLuint nParticles = 200;
double currentTimeParticles, lastTimeParticles;

// Variables para partículas de fuego, estilo fuente
GLuint fireInitVel;
GLuint fireStartTime;
GLuint VAOParticlesFire;

GLuint nParticlesFire = 300;
float fireParticleLifetime = 3.0f;

// OJO: usando GL_POINTS, este tamaño está en píxeles
float fireParticleSize = 35.0f;

double currentTimeFire, lastTimeFire;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void initParticleBuffers();
void initFireParticleBuffers();
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);
AbstractModel::AABB transformAABB(const AbstractModel::AABB &aabb,
		const glm::mat4 &transform);
AbstractModel::AABB obbToAABB(const AbstractModel::OBB &obb);
bool testAABBAABB(const AbstractModel::AABB &a,
		const AbstractModel::AABB &b);
AbstractModel::OBB createModelOBB(
		Model &model, const glm::mat4 &modelMatrix, float scale);
void updateEnemyPatrol(
		Model &model, glm::mat4 &modelMatrix,
		const glm::mat4 &baseMatrix,
		EnemyPatrolState &patrol, float deltaSeconds);

AbstractModel::AABB transformAABB(const AbstractModel::AABB &aabb,
		const glm::mat4 &transform) {
	glm::vec3 transformedMin(std::numeric_limits<float>::max());
	glm::vec3 transformedMax(std::numeric_limits<float>::lowest());

	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < 2; y++) {
			for (int z = 0; z < 2; z++) {
				glm::vec3 corner(
					x == 0 ? aabb.mins.x : aabb.maxs.x,
					y == 0 ? aabb.mins.y : aabb.maxs.y,
					z == 0 ? aabb.mins.z : aabb.maxs.z);
				glm::vec3 transformedCorner = glm::vec3(
					transform * glm::vec4(corner, 1.0f));
				transformedMin = glm::min(transformedMin, transformedCorner);
				transformedMax = glm::max(transformedMax, transformedCorner);
			}
		}
	}

	return AbstractModel::AABB(transformedMin, transformedMax);
}

AbstractModel::AABB obbToAABB(const AbstractModel::OBB &obb) {
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), obb.c)
			* glm::mat4(obb.u);
	AbstractModel::AABB localAABB(-obb.e, obb.e);
	return transformAABB(localAABB, transform);
}

bool testAABBAABB(const AbstractModel::AABB &a,
		const AbstractModel::AABB &b) {
	return a.mins.x <= b.maxs.x && a.maxs.x >= b.mins.x
			&& a.mins.y <= b.maxs.y && a.maxs.y >= b.mins.y
			&& a.mins.z <= b.maxs.z && a.maxs.z >= b.mins.z;
}

AbstractModel::OBB createModelOBB(
		Model &model, const glm::mat4 &modelMatrix, float scale) {
	AbstractModel::OBB collider;
	const AbstractModel::OBB &localCollider = model.getObb();
	bool validCollider =
		std::isfinite(localCollider.c.x)
		&& std::isfinite(localCollider.c.y)
		&& std::isfinite(localCollider.c.z)
		&& std::isfinite(localCollider.e.x)
		&& std::isfinite(localCollider.e.y)
		&& std::isfinite(localCollider.e.z)
		&& localCollider.e.x > 0.0f
		&& localCollider.e.y > 0.0f
		&& localCollider.e.z > 0.0f
		&& localCollider.e.x < 10000.0f
		&& localCollider.e.y < 10000.0f
		&& localCollider.e.z < 10000.0f;

	collider.u = glm::quat_cast(modelMatrix);
	if (validCollider) {
		glm::mat4 colliderMatrix = glm::scale(
			modelMatrix, glm::vec3(scale));
		colliderMatrix = glm::translate(
			colliderMatrix, localCollider.c);
		collider.c = glm::vec3(colliderMatrix[3]);
		collider.e = localCollider.e * glm::vec3(scale);
	} else {
		collider.c = glm::vec3(modelMatrix[3])
				+ glm::vec3(0.0f, 1.0f, 0.0f);
		collider.e = glm::vec3(0.65f, 1.0f, 0.5f);
	}
	return collider;
}

void updateEnemyPatrol(
		Model &model, glm::mat4 &modelMatrix,
		const glm::mat4 &baseMatrix,
		EnemyPatrolState &patrol, float deltaSeconds) {
	if (!patrol.turning) {
		model.setAnimationIndex(ENEMY_ANIMATION_WALK);
		float remainingDistance =
			patrol.maxDistance - patrol.traveledDistance;
		float movement =
			glm::min(ENEMY_PATROL_SPEED * deltaSeconds, remainingDistance);
		patrol.traveledDistance += movement;

		if (patrol.traveledDistance >= patrol.maxDistance) {
			patrol.traveledDistance = patrol.maxDistance;
			patrol.turning = true;
			patrol.turnAngle = 0.0f;
		}
	} else {
		model.setAnimationIndex(ENEMY_ANIMATION_TURN);
		float remainingAngle = 180.0f - patrol.turnAngle;
		float rotation =
			glm::min(ENEMY_TURN_SPEED * deltaSeconds, remainingAngle);
		patrol.turnAngle += rotation;

		if (patrol.turnAngle >= 180.0f) {
			patrol.turning = false;
			patrol.turnAngle = 0.0f;
			patrol.traveledDistance = 0.0f;
			patrol.leg = (patrol.leg + 1) % 2;
		}
	}

	float distance = patrol.traveledDistance;
	if (patrol.turning)
		distance = patrol.maxDistance;

	glm::vec3 offset(0.0f);
	if (patrol.leg == 0)
		offset.z = -distance;
	else
		offset.z = -patrol.maxDistance + distance;

	float orientation =
		patrol.leg * 180.0f
		+ (patrol.turning ? patrol.turnAngle : 0.0f);
	modelMatrix = glm::translate(baseMatrix, offset);
	modelMatrix = glm::rotate(
		modelMatrix, glm::radians(orientation),
		glm::vec3(0.0f, 1.0f, 0.0f));
}

void initParticleBuffers(){
	//Generar los buffers
	glGenBuffers(1, &initVel);
	glGenBuffers(1, &startTime);

	//reserva de memoria para los buffers
	int sizeInitVel = nParticles * 3 * sizeof(GLfloat);
	int sizeStartTime = nParticles * sizeof(GLfloat);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferData(GL_ARRAY_BUFFER, sizeInitVel, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferData(GL_ARRAY_BUFFER, sizeStartTime, NULL, GL_STATIC_DRAW);

	// Generar la velocidad inicial con velocidades aleatorias
	glm::vec3 v(0.0f);
	float velocity, theta, phi;
	GLfloat *data = new GLfloat[nParticles * 3];
	for(unsigned int i = 0; i < nParticles; i++){
		theta = glm::mix(0.0f, glm::pi<float>() / 6.0f, ((float) rand() / RAND_MAX));
		phi = glm::mix(0.0f, glm::two_pi<float>(), ((float) rand() / RAND_MAX));

		v.x = sinf(theta) * cosf(phi);
		v.y = cosf(theta);
		v.z = sinf(theta) * sinf(phi);

		velocity = glm::mix(0.6f, 0.8f, ((float) rand() / RAND_MAX));
		v = glm::normalize(v) * velocity;

		data[3 * i] = v.x;
		data[3 * i + 1] = v.y;
		data[3 * i + 2] = v.z;
	}
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeInitVel, data);
	delete[] data;

	// Tiempo inicial de la particula (Cuando nace)
	data = new GLfloat[nParticles];
	float time = 0.0f;
	float rate = 0.00075f;
	for (unsigned int i = 0; i < nParticles; i++) {
		data[i] = time;
		time += rate;
	}
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeStartTime, data);
	delete[] data;
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &VAOParticles);
	glBindVertexArray(VAOParticles);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

}

void initFireParticleBuffers() {
    glGenBuffers(1, &fireInitVel);
    glGenBuffers(1, &fireStartTime);

    int sizeInitVel = nParticlesFire * 3 * sizeof(GLfloat);
    int sizeStartTime = nParticlesFire * sizeof(GLfloat);

    glBindBuffer(GL_ARRAY_BUFFER, fireInitVel);
    glBufferData(GL_ARRAY_BUFFER, sizeInitVel, NULL, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, fireStartTime);
    glBufferData(GL_ARRAY_BUFFER, sizeStartTime, NULL, GL_STATIC_DRAW);

    // Velocidades iniciales, parecido a la fuente,
    // pero con movimiento más vertical y con poca apertura lateral
    GLfloat *data = new GLfloat[nParticlesFire * 3];

    glm::vec3 v(0.0f);
    float velocity;
    float theta;
    float phi;

    for (unsigned int i = 0; i < nParticlesFire; i++) {
        theta = glm::mix(0.0f, glm::pi<float>() / 10.0f, ((float)rand() / RAND_MAX));
        phi = glm::mix(0.0f, glm::two_pi<float>(), ((float)rand() / RAND_MAX));

        v.x = sinf(theta) * cosf(phi);
        v.y = cosf(theta);
        v.z = sinf(theta) * sinf(phi);

        velocity = glm::mix(0.4f, 0.9f, ((float)rand() / RAND_MAX));
        v = glm::normalize(v) * velocity;

        data[3 * i] = v.x;
        data[3 * i + 1] = v.y;
        data[3 * i + 2] = v.z;
    }

    glBindBuffer(GL_ARRAY_BUFFER, fireInitVel);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeInitVel, data);
    delete[] data;

    // Tiempo inicial de cada partícula
    data = new GLfloat[nParticlesFire];

    float time = 0.0f;
    float rate = fireParticleLifetime / nParticlesFire;

    for (unsigned int i = 0; i < nParticlesFire; i++) {
        data[i] = time;
        time += rate;
    }

    glBindBuffer(GL_ARRAY_BUFFER, fireStartTime);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeStartTime, data);
    delete[] data;

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &VAOParticlesFire);
    glBindVertexArray(VAOParticlesFire);

    glBindBuffer(GL_ARRAY_BUFFER, fireInitVel);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, fireStartTime);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
				glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
				nullptr);

	if (window == nullptr) {
		std::cerr
				<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
				<< std::endl;
		destroy();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_PROGRAM_POINT_SIZE);

	// Inicialización de los shaders
	shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
	shaderSkybox.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox_fog.fs");
	shaderMulLighting.initialize("../Shaders/iluminacion_textura_animation_shadow.vs", "../Shaders/multipleLights_shadow.fs");
	shaderTerrain.initialize("../Shaders/terrain_shadow.vs", "../Shaders/terrain_shadow.fs");
	shaderTexture.initialize("../Shaders/texturizado.vs", "../Shaders/texturizado.fs");
	shaderViewDepth.initialize("../Shaders/texturizado.vs", "../Shaders/texturizado_depth_view.fs");
	shaderDepth.initialize("../Shaders/shadow_mapping_depth.vs", "../Shaders/shadow_mapping_depth.fs");
	shaderParticlesFountain.initialize("../Shaders/particlesFountain.vs", "../Shaders/particlesFountain.fs");
	shaderParticlesFire.initialize("../Shaders/particlesFire.vs", "../Shaders/particlesFire.fs");

	// Inicializacion de los objetos.
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(20.0f, 20.0f, 20.0f));

	boxCollider.init();
	boxCollider.setShader(&shader);
	boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	sphereCollider.init();
	sphereCollider.setShader(&shader);
	sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	rayModel.init();
	rayModel.setShader(&shader);
	rayModel.setColor(glm::vec4(1.0));

	boxCesped.init();
	boxCesped.setShader(&shaderMulLighting);

	boxWalls.init();
	boxWalls.setShader(&shaderMulLighting);

	boxHighway.init();
	boxHighway.setShader(&shaderMulLighting);

	boxLandingPad.init();
	boxLandingPad.setShader(&shaderMulLighting);

	esfera1.init();
	esfera1.setShader(&shaderMulLighting);

	boxIntro.init();
	boxIntro.setShader(&shaderTexture);
	boxIntro.setScale(glm::vec3(2.0, 2.0, 1.0));

	boxHeart.init();
	boxHeart.setShader(&shaderTexture);

	boxViewDepth.init();
	boxViewDepth.setShader(&shaderViewDepth);

	// Laberinto models
	modelEntrada.loadModel("../models/Laberinto/Entrada.obj");
	modelEntrada.setShader(&shaderMulLighting);
	modelSalida.loadModel("../models/Laberinto/Salida.obj");
	modelSalida.setShader(&shaderMulLighting);
    // Puertas del laberinto
    modelPuertaDer.loadModel("../models/Laberinto/PuertaDer.obj"); modelPuertaDer.setShader(&shaderMulLighting);
    modelPuertaIzq.loadModel("../models/Laberinto/PuertaIzq.obj"); modelPuertaIzq.setShader(&shaderMulLighting);
	modelMuroDer.loadModel("../models/Laberinto/MuroDer.obj");
	modelMuroDer.setShader(&shaderMulLighting);
	modelMuroIzq.loadModel("../models/Laberinto/MuroIzq.obj");
	modelMuroIzq.setShader(&shaderMulLighting);

	// Load additional muro models
	modelMuro1.loadModel("../models/Laberinto/Muro1.obj"); modelMuro1.setShader(&shaderMulLighting);
	modelMuro2.loadModel("../models/Laberinto/Muro2.obj"); modelMuro2.setShader(&shaderMulLighting);
	modelMuro3.loadModel("../models/Laberinto/Muro3.obj"); modelMuro3.setShader(&shaderMulLighting);
	modelMuro4.loadModel("../models/Laberinto/Muro4.obj"); modelMuro4.setShader(&shaderMulLighting);
	modelMuro5.loadModel("../models/Laberinto/Muro5.obj"); modelMuro5.setShader(&shaderMulLighting);
	modelMuro6.loadModel("../models/Laberinto/Muro6.obj"); modelMuro6.setShader(&shaderMulLighting);
	modelMuro7.loadModel("../models/Laberinto/Muro7.obj"); modelMuro7.setShader(&shaderMulLighting);
	modelMuro8.loadModel("../models/Laberinto/Muro8.obj"); modelMuro8.setShader(&shaderMulLighting);
	modelMuro9.loadModel("../models/Laberinto/Muro9.obj"); modelMuro9.setShader(&shaderMulLighting);
	modelMuro10.loadModel("../models/Laberinto/Muro10.obj"); modelMuro10.setShader(&shaderMulLighting);
	modelMuro11.loadModel("../models/Laberinto/Muro11.obj"); modelMuro11.setShader(&shaderMulLighting);
	modelMuro12.loadModel("../models/Laberinto/Muro12.obj"); modelMuro12.setShader(&shaderMulLighting);
	modelMuro13.loadModel("../models/Laberinto/Muro13.obj"); modelMuro13.setShader(&shaderMulLighting);
	modelMuro14.loadModel("../models/Laberinto/Muro14.obj"); modelMuro14.setShader(&shaderMulLighting);
	modelMuro15.loadModel("../models/Laberinto/Muro15.obj"); modelMuro15.setShader(&shaderMulLighting);

	// Monedas del laberinto
	modelMoneda1.loadModel("../models/Laberinto/Moneda1.obj"); modelMoneda1.setShader(&shaderMulLighting);
	modelMoneda2.loadModel("../models/Laberinto/Moneda2.obj"); modelMoneda2.setShader(&shaderMulLighting);
	modelMoneda3.loadModel("../models/Laberinto/Moneda3.obj"); modelMoneda3.setShader(&shaderMulLighting);
	modelMoneda4.loadModel("../models/Laberinto/Moneda4.obj"); modelMoneda4.setShader(&shaderMulLighting);
	modelMoneda5.loadModel("../models/Laberinto/Moneda5.obj"); modelMoneda5.setShader(&shaderMulLighting);

	// Enemigos
	modelEnemy1.loadModel("../models/Enemy/Enemy1.fbx"); modelEnemy1.setShader(&shaderMulLighting);
	modelEnemy2.loadModel("../models/Enemy/Enemy2.fbx"); modelEnemy2.setShader(&shaderMulLighting);
	modelEnemy3.loadModel("../models/Enemy/Enemy3.fbx"); modelEnemy3.setShader(&shaderMulLighting);
	modelEnemy4.loadModel("../models/Enemy/Enemy4.fbx"); modelEnemy4.setShader(&shaderMulLighting);
	modelEnemy5.loadModel("../models/Enemy/Enemy5.fbx"); modelEnemy5.setShader(&shaderMulLighting);

	// Player
	playerModelAnimate.loadModel("../models/Player/Player.fbx");
	playerModelAnimate.setShader(&shaderMulLighting);
	
	//Fountain
	modelFountain.loadModel("../models/fountain/fountain.obj");
	modelFountain.setShader(&shaderMulLighting);

	// Antorcha
	modelAntocha.loadModel("../models/Antocha/AntochaZero.obj");
	modelAntocha.setShader(&shaderMulLighting);

	// Terreno
	terrain.init();
	terrain.setShader(&shaderTerrain);

	// Se inicializa el model de render text
	modelText = new FontTypeRendering::FontTypeRendering(screenWidth, screenHeight);
	modelText->Initialize();

	camera->setPosition(glm::vec3(0.0, 3.0, 4.0));
	camera->setDistanceFromTarget(distanceFromTarget);
	camera->setSensitivity(1.0);
	
	// Carga de texturas para el skybox
	Texture skyboxTexture = Texture("");
	glGenTextures(1, &skyboxTextureID);
	// Tipo de textura CUBE MAP
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++) {
		skyboxTexture = Texture(fileNames[i]);
		skyboxTexture.loadImage(true);
		if (skyboxTexture.getData()) {
			glTexImage2D(types[i], 0, skyboxTexture.getChannels() == 3 ? GL_RGB : GL_RGBA, skyboxTexture.getWidth(), skyboxTexture.getHeight(), 0,
			skyboxTexture.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, skyboxTexture.getData());
		} else
			std::cout << "Failed to load texture" << std::endl;
		skyboxTexture.freeImage();
	}

	// Definiendo la textura a utilizar
	Texture textureCesped("../Textures/grassy2.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	textureCesped.loadImage();
	// Creando la textura con id 1
	glGenTextures(1, &textureCespedID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureCespedID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (textureCesped.getData()) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		std::cout << "Numero de canales :=> " << textureCesped.getChannels() << std::endl;
		glTexImage2D(GL_TEXTURE_2D, 0, textureCesped.getChannels() == 3 ? GL_RGB : GL_RGBA, textureCesped.getWidth(), textureCesped.getHeight(), 0,
		textureCesped.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureCesped.getData());
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureCesped.freeImage();

	// Definiendo la textura a utilizar
	Texture textureWall("../Textures/whiteWall.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	textureWall.loadImage();
	// Creando la textura con id 1
	glGenTextures(1, &textureWallID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureWallID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (textureWall.getData()) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, textureWall.getChannels() == 3 ? GL_RGB : GL_RGBA, textureWall.getWidth(), textureWall.getHeight(), 0,
		textureWall.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureWall.getData());
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureWall.freeImage();

	// Definiendo la textura a utilizar
	Texture textureWindow("../Textures/ventana.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	textureWindow.loadImage();
	// Creando la textura con id 1
	glGenTextures(1, &textureWindowID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureWindowID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (textureWindow.getData()) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, textureWindow.getChannels() == 3 ? GL_RGB : GL_RGBA, textureWindow.getWidth(), textureWindow.getHeight(), 0,
		textureWindow.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureWindow.getData());
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureWindow.freeImage();

	// Definiendo la textura a utilizar
	Texture textureHighway("../Textures/highway.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	textureHighway.loadImage();
	// Creando la textura con id 1
	glGenTextures(1, &textureHighwayID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureHighwayID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (textureHighway.getData()) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, textureHighway.getChannels() == 3 ? GL_RGB : GL_RGBA, textureHighway.getWidth(), textureHighway.getHeight(), 0,
		textureHighway.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureHighway.getData());
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureHighway.freeImage();

	// Definiendo la textura
	Texture textureLandingPad("../Textures/landingPad.jpg");
	textureLandingPad.loadImage(); // Cargar la textura
	glGenTextures(1, &textureLandingPadID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureLandingPadID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureLandingPad.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureLandingPad.getChannels() == 3 ? GL_RGB : GL_RGBA, textureLandingPad.getWidth(), textureLandingPad.getHeight(), 0,
		textureLandingPad.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureLandingPad.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureLandingPad.freeImage(); // Liberamos memoria

	// Defininiendo texturas del mapa de mezclas
	// Definiendo la textura
	Texture textureR("../Textures/mud.png");
	textureR.loadImage(); // Cargar la textura
	glGenTextures(1, &textureTerrainRID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureR.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureR.getChannels() == 3 ? GL_RGB : GL_RGBA, textureR.getWidth(), textureR.getHeight(), 0,
		textureR.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureR.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureR.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureG("../Textures/grassFlowers.png");
	textureG.loadImage(); // Cargar la textura
	glGenTextures(1, &textureTerrainGID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureG.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureG.getChannels() == 3 ? GL_RGB : GL_RGBA, textureG.getWidth(), textureG.getHeight(), 0,
		textureG.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureG.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureG.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureB("../Textures/path.png");
	textureB.loadImage(); // Cargar la textura
	glGenTextures(1, &textureTerrainBID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureB.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureB.getChannels() == 3 ? GL_RGB : GL_RGBA, textureB.getWidth(), textureB.getHeight(), 0,
		textureB.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureB.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureB.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureBlendMap("../Textures/blendMap.png");
	textureBlendMap.loadImage(); // Cargar la textura
	glGenTextures(1, &textureTerrainBlendMapID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureBlendMap.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureBlendMap.getChannels() == 3 ? GL_RGB : GL_RGBA, textureBlendMap.getWidth(), textureBlendMap.getHeight(), 0,
		textureBlendMap.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureBlendMap.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureBlendMap.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureIntro1("../Textures/MenuJugar.png");
	textureIntro1.loadImage(); // Cargar la textura
	glGenTextures(1, &textureInit1ID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureInit1ID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureIntro1.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureIntro1.getChannels() == 3 ? GL_RGB : GL_RGBA, textureIntro1.getWidth(), textureIntro1.getHeight(), 0,
		textureIntro1.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureIntro1.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureIntro1.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureIntro2("../Textures/MenuSalir.png");
	textureIntro2.loadImage(); // Cargar la textura
	glGenTextures(1, &textureInit2ID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureInit2ID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureIntro2.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureIntro2.getChannels() == 3 ? GL_RGB : GL_RGBA, textureIntro2.getWidth(), textureIntro2.getHeight(), 0,
		textureIntro2.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureIntro2.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureIntro2.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureScreen("../Textures/Screen.png");
	textureScreen.loadImage(); // Cargar la textura
	glGenTextures(1, &textureScreenID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureScreenID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureScreen.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureScreen.getChannels() == 3 ? GL_RGB : GL_RGBA, textureScreen.getWidth(), textureScreen.getHeight(), 0,
		textureScreen.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureScreen.getData());
		glGenerateMipmap(GL_TEXTURE_2D);

		int width = textureScreen.getWidth();
		int height = textureScreen.getHeight();
		int channels = textureScreen.getChannels();
		int dataSize = width * height * channels;
		std::vector<unsigned char> screen1Life(
			textureScreen.getData(), textureScreen.getData() + dataSize);
		std::vector<unsigned char> screen0Lives = screen1Life;

		auto clearHudRegion = [width, height, channels](
				std::vector<unsigned char> &pixels,
				int minX, int maxX, int minY, int maxY) {
			minX = glm::clamp(minX, 0, width);
			maxX = glm::clamp(maxX, 0, width);
			minY = glm::clamp(minY, 0, height);
			maxY = glm::clamp(maxY, 0, height);
			for (int y = minY; y < maxY; y++) {
				for (int x = minX; x < maxX; x++) {
					int pixelIndex = (y * width + x) * channels;
					for (int channel = 0; channel < channels; channel++)
						pixels[pixelIndex + channel] = 0;
				}
			}
		};

		clearHudRegion(screen1Life,
			static_cast<int>(width * 0.88f), width,
			0, static_cast<int>(height * 0.13f));
		clearHudRegion(screen0Lives,
			static_cast<int>(width * 0.76f), width,
			0, static_cast<int>(height * 0.13f));

		auto uploadHudTexture = [width, height, channels](
				GLuint &textureID,
				const std::vector<unsigned char> &pixels) {
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexParameteri(
				GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(
				GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(
				GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(
				GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			GLenum format = channels == 3 ? GL_RGB : GL_RGBA;
			glTexImage2D(
				GL_TEXTURE_2D, 0, format, width, height, 0,
				format, GL_UNSIGNED_BYTE, pixels.data());
			glGenerateMipmap(GL_TEXTURE_2D);
		};

		uploadHudTexture(textureScreen1LifeID, screen1Life);
		uploadHudTexture(textureScreen0LivesID, screen0Lives);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureScreen.freeImage(); // Liberamos memoria

	Texture textureHeart("../Textures/Corazon.png");
	textureHeart.loadImage();
	glGenTextures(1, &textureHeartID);
	glBindTexture(GL_TEXTURE_2D, textureHeartID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	if (textureHeart.getData()) {
		GLenum heartFormat =
			textureHeart.getChannels() == 3 ? GL_RGB : GL_RGBA;
		glTexImage2D(
			GL_TEXTURE_2D, 0, heartFormat,
			textureHeart.getWidth(), textureHeart.getHeight(), 0,
			heartFormat, GL_UNSIGNED_BYTE, textureHeart.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cout << "Fallo la carga de Corazon.png" << std::endl;
	}
	textureHeart.freeImage();

	// Definiendo la textura
	Texture textureParticlesFountain("../Textures/bluewater.png");
	textureParticlesFountain.loadImage(); // Cargar la textura
	glGenTextures(1, &textureParticleFountainID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureParticleFountainID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureParticlesFountain.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureParticlesFountain.getChannels() == 3 ? GL_RGB : GL_RGBA, textureParticlesFountain.getWidth(), textureParticlesFountain.getHeight(), 0,
		textureParticlesFountain.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureParticlesFountain.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureParticlesFountain.freeImage(); // Liberamos memoria

	// Definiendo la textura de fuego
	Texture textureParticlesFire("../Textures/fire.png"); 
	textureParticlesFire.loadImage();
	glGenTextures(1, &textureParticleFireID);
	glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (textureParticlesFire.getData()) {
		glTexImage2D(GL_TEXTURE_2D, 0,
			textureParticlesFire.getChannels() == 3 ? GL_RGB : GL_RGBA,
			textureParticlesFire.getWidth(), textureParticlesFire.getHeight(), 0,
			textureParticlesFire.getChannels() == 3 ? GL_RGB : GL_RGBA,
			GL_UNSIGNED_BYTE, textureParticlesFire.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Fallo la carga de textura de fuego" << std::endl;
	textureParticlesFire.freeImage();

	/*******************************************
	 * OpenAL init
	 *******************************************/
	alutInit(0, nullptr);
	alListenerfv(AL_POSITION, listenerPos);
	alListenerfv(AL_VELOCITY, listenerVel);
	alListenerfv(AL_ORIENTATION, listenerOri);
	alGetError(); // clear any error messages
	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating buffers !!\n");
		exit(1);
	}
	else {
		printf("init() - No errors yet.");
	}
	// Generate buffers, or else no sound will happen!
	alGenBuffers(NUM_BUFFERS, buffer);
	buffer[0] = alutCreateBufferFromFile("../sounds/fountain.wav");
	// Audio de Darth reservado:
	// buffer[2] = alutCreateBufferFromFile("../sounds/darth_vader.wav");
	int errorAlut = alutGetError();
	if (errorAlut != ALUT_ERROR_NO_ERROR){
		printf("- Error open files with alut %d !!\n", errorAlut);
		exit(2);
	}

	alGetError(); /* clear error */
	alGenSources(NUM_SOURCES, source);

	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating sources !!\n");
		exit(2);
	}
	else {
		printf("init - no errors after alGenSources\n");
	}
	alSourcef(source[0], AL_PITCH, 1.0f);
	alSourcef(source[0], AL_GAIN, 3.0f);
	alSourcefv(source[0], AL_POSITION, source0Pos);
	alSourcefv(source[0], AL_VELOCITY, source0Vel);
	alSourcei(source[0], AL_BUFFER, buffer[0]);
	alSourcei(source[0], AL_LOOPING, AL_TRUE);
	alSourcef(source[0], AL_MAX_DISTANCE, 2000);

	/*
	 * Configuracion de audio de Darth reservada:
	 * alSourcef(source[2], AL_PITCH, 1.0f);
	 * alSourcef(source[2], AL_GAIN, 0.3f);
	 * alSourcefv(source[2], AL_POSITION, source2Pos);
	 * alSourcefv(source[2], AL_VELOCITY, source2Vel);
	 * alSourcei(source[2], AL_BUFFER, buffer[2]);
	 * alSourcei(source[2], AL_LOOPING, AL_TRUE);
	 * alSourcef(source[2], AL_MAX_DISTANCE, 2000);
	 */

	/*******************************************
	 * Inicializacion del framebuffer para
	 * almacenar el buffer de profunidadad
	 *******************************************/
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
				 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//inicializacion de los datos de la particula de agua
	initParticleBuffers();
	//inicializacion de los datos de la particula de fuego
	initFireParticleBuffers();
	lastTimeFire = TimeManager::Instance().GetTime();
}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	// Shaders Delete
	shader.destroy();
	shaderMulLighting.destroy();
	shaderSkybox.destroy();
	shaderTerrain.destroy();
	shaderParticlesFountain.destroy();
	shaderParticlesFire.destroy();

	// Basic objects Delete
	skyboxSphere.destroy();
	boxCesped.destroy();
	boxWalls.destroy();
	boxHighway.destroy();
	boxLandingPad.destroy();
	esfera1.destroy();
	boxCollider.destroy();
	sphereCollider.destroy();
	rayModel.destroy();
	boxIntro.destroy();
	boxHeart.destroy();
	boxViewDepth.destroy();

	// Custom objects Delete
	modelMoneda1.destroy();
	modelMoneda2.destroy();
	modelMoneda3.destroy();
	modelMoneda4.destroy();
	modelMoneda5.destroy();
	modelEnemy1.destroy();
	modelEnemy2.destroy();
	modelEnemy3.destroy();
	modelEnemy4.destroy();
	modelEnemy5.destroy();
	modelAntocha.destroy();
	playerModelAnimate.destroy();
	modelFountain.destroy();

	// Terrains objects Delete
	terrain.destroy();

	// Textures Delete
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &textureCespedID);
	glDeleteTextures(1, &textureWallID);
	glDeleteTextures(1, &textureWindowID);
	glDeleteTextures(1, &textureHighwayID);
	glDeleteTextures(1, &textureLandingPadID);
	glDeleteTextures(1, &textureTerrainBID);
	glDeleteTextures(1, &textureTerrainGID);
	glDeleteTextures(1, &textureTerrainRID);
	glDeleteTextures(1, &textureTerrainBlendMapID);
	glDeleteTextures(1, &textureInit1ID);
	glDeleteTextures(1, &textureInit2ID);
	glDeleteTextures(1, &textureScreenID);
	glDeleteTextures(1, &textureScreen1LifeID);
	glDeleteTextures(1, &textureScreen0LivesID);
	glDeleteTextures(1, &textureHeartID);
	glDeleteTextures(1, &textureParticleFountainID);
	glDeleteTextures(1, &textureParticleFireID);

	// Cube Maps Delete
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDeleteTextures(1, &skyboxTextureID);

	// Liberar los datos del buffer de las particulas de la fuente de agua
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &initVel);
	glDeleteBuffers(1, &startTime);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAOParticles);
	// Liberar los datos del buffer de las particulas de fuego
	glDeleteBuffers(1, &fireInitVel);
	glDeleteBuffers(1, &fireStartTime);
	glDeleteVertexArrays(1, &VAOParticlesFire);

}

void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset){
	distanceFromTarget -= yoffset;
	camera->setDistanceFromTarget(distanceFromTarget);
}

void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}

	GLFWgamepadstate gamepadState;
	const bool gamepadConnected =
		glfwJoystickIsGamepad(GLFW_JOYSTICK_1) == GLFW_TRUE
		&& glfwGetGamepadState(GLFW_JOYSTICK_1, &gamepadState)
			== GLFW_TRUE;

	if(!iniciaPartida){
		const bool seleccionarOpcion =
			glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS
			|| (gamepadConnected
				&& gamepadState.buttons[GLFW_GAMEPAD_BUTTON_START]
					== GLFW_PRESS);
		const bool cambiarOpcion =
			glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS
			|| (gamepadConnected
				&& gamepadState.buttons[GLFW_GAMEPAD_BUTTON_BACK]
					== GLFW_PRESS);

		if (!presionarSeleccionMenu && seleccionarOpcion) {
			presionarSeleccionMenu = true;
			if (textureActivaID == textureInit1ID) {
				iniciaPartida = true;
				textureActivaID = textureScreenID;
			}
			else if (textureActivaID == textureInit2ID) {
				exitApp = true;
				return false;
			}
		}
		else if (!seleccionarOpcion) {
			presionarSeleccionMenu = false;
		}

		if (!presionarOpcion && cambiarOpcion) {
			presionarOpcion = true;
			if(textureActivaID == textureInit1ID)
				textureActivaID = textureInit2ID;
			else if(textureActivaID == textureInit2ID)
				textureActivaID = textureInit1ID;
		}
		else if (!cambiarOpcion) {
			presionarOpcion = false;
		}
	}

	if (iniciaPartida && gamepadConnected) {
			const float leftX =
				gamepadState.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
			const float leftY =
				gamepadState.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];
			const float rightX =
				gamepadState.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
			const float rightY =
				gamepadState.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];

			if (modelSelected == 0) {
				const bool moving =
					fabs(leftY) > GAMEPAD_DEAD_ZONE;
				const bool turning =
					fabs(leftX) > GAMEPAD_DEAD_ZONE;

				if (moving) {
					const float displacement =
						-leftY * GAMEPAD_PLAYER_SPEED
						* static_cast<float>(deltaTime);
					modelMatrixPlayer = glm::translate(
						modelMatrixPlayer,
						glm::vec3(0.0f, 0.0f, displacement));
					animationPlayerIndex =
						displacement > 0.0f ? 1 : 2;
				}

				if (turning) {
					const float rotation = glm::radians(
						-leftX * GAMEPAD_PLAYER_TURN_SPEED
						* static_cast<float>(deltaTime));
					modelMatrixPlayer = glm::rotate(
						modelMatrixPlayer, rotation,
						glm::vec3(0.0f, 1.0f, 0.0f));
					if (!moving)
						animationPlayerIndex = 0;
				}
			}

			// Solo el stick derecho controla la camara. Los gatillos
			// GLFW_GAMEPAD_AXIS_LEFT_TRIGGER y RIGHT_TRIGGER se ignoran.
			const float cameraX =
				fabs(rightX) > GAMEPAD_DEAD_ZONE
					? rightX * GAMEPAD_CAMERA_SPEED : 0.0f;
			const float cameraY =
				fabs(rightY) > GAMEPAD_DEAD_ZONE
					? rightY * GAMEPAD_CAMERA_SPEED : 0.0f;
			if (cameraX != 0.0f || cameraY != 0.0f)
				camera->mouseMoveCamera(
					cameraX, cameraY, deltaTime);

			if (!isJump
					&& gamepadState.buttons[GLFW_GAMEPAD_BUTTON_A]
						== GLFW_PRESS) {
				isJump = true;
				startTimeJump = currTime;
				tmv = 0;
			}
	}

	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, 0.0, deltaTime);
	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		camera->mouseMoveCamera(0.0, offsetY, deltaTime);

	offsetX = 0;
	offsetY = 0;

	// Seleccionar modelo
	if (enableCountSelected && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS){
		enableCountSelected = false;
		modelSelected++;
		if(modelSelected > 2)
			modelSelected = 0;
		if (modelSelected == 1) {
			camera = std::shared_ptr<Camera>(new FirstPersonCamera(glm::vec3(0.0, 3.0, 4.0)));
			camera->setSensitivity(5.0f); // Mayor sensibilidad para giro con mouse
			std::cout << "Free camera mode active" << std::endl;
		} else if (modelSelected == 2) {
			camera = std::shared_ptr<Camera>(new AerialCamera(glm::vec3(0.0, 30.0, 0.0)));
			camera->setSensitivity(1.0f);
			std::cout << "Aerial camera mode active" << std::endl;
		} else {
			camera = std::shared_ptr<Camera>(new ThirdPersonCamera());
			camera->setDistanceFromTarget(distanceFromTarget);
			camera->setSensitivity(1.0f);
		}
		std::cout << "modelSelected:" << modelSelected << std::endl;
	}
	else if(glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
		enableCountSelected = true;

	// Controles del player
	if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
		modelMatrixPlayer = glm::rotate(modelMatrixPlayer, 0.05f, glm::vec3(0, 1, 0));
		animationPlayerIndex = 0;
	} else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
		modelMatrixPlayer = glm::rotate(modelMatrixPlayer, -0.05f, glm::vec3(0, 1, 0));
		animationPlayerIndex = 0;
	}
	if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
		modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0.0, 0.0, 0.5));
		animationPlayerIndex = 1; // run when moving forward
	}
	else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
		modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0.0, 0.0, -0.5));
		animationPlayerIndex = 2; // run backward
	}

	// Animation placeholders / manual overrides for testing
	if (modelSelected == 0) {
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) animationPlayerIndex = 0; // idle
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) animationPlayerIndex = 2; // Runback
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) animationPlayerIndex = 3; // Block
		if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) animationPlayerIndex = 4; // death
		if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) animationPlayerIndex = 5; // idle
		if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) animationPlayerIndex = 6; // run back
		// keys 7/8 left as optional extra clips if present
		if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) animationPlayerIndex = 7; //death
		if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) animationPlayerIndex = 8;
		if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) animationPlayerIndex = 9;
		if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) animationPlayerIndex = 1; //Run
	}

	if (modelSelected == 1) {
		FirstPersonCamera *fps = dynamic_cast<FirstPersonCamera*>(camera.get());
		if (fps) {
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
				fps->moveFrontCamera(true, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
				fps->moveFrontCamera(false, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
				fps->moveRightCamera(true, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
				fps->moveRightCamera(false, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
				fps->setPosition(fps->getPosition() + glm::vec3(0.0f, 0.02f, 0.0f));
			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
				fps->setPosition(fps->getPosition() - glm::vec3(0.0f, 0.02f, 0.0f));
		}
	}
	if (modelSelected == 2) {
		AerialCamera *air = dynamic_cast<AerialCamera*>(camera.get());
		if (air) {
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
				air->moveFrontCamera(true, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
				air->moveFrontCamera(false, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
				air->moveRightCamera(true, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
				air->moveRightCamera(false, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
				air->setPosition(air->getPosition() + glm::vec3(0.0f, 0.25f, 0.0f));
			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
				air->setPosition(air->getPosition() - glm::vec3(0.0f, 0.25f, 0.0f));
		}
	}

	bool keySpaceStatus = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
	if(!isJump && keySpaceStatus){
		isJump = true;
		startTimeJump = currTime;
		tmv = 0;
	}

	glfwPollEvents();
	return continueApplication;
}

void prepareScene(){

	terrain.setShader(&shaderTerrain);
	
	//Grass
	//modelGrass.setShader(&shaderMulLighting);

	//Player
	playerModelAnimate.setShader(&shaderMulLighting);

	// Fountain
	modelFountain.setShader(&shaderMulLighting);

	// Monedas del laberinto
	modelMoneda1.setShader(&shaderMulLighting);
	modelMoneda2.setShader(&shaderMulLighting);
	modelMoneda3.setShader(&shaderMulLighting);
	modelMoneda4.setShader(&shaderMulLighting);
	modelMoneda5.setShader(&shaderMulLighting);

	modelEnemy1.setShader(&shaderMulLighting);
	modelEnemy2.setShader(&shaderMulLighting);
	modelEnemy3.setShader(&shaderMulLighting);
	modelEnemy4.setShader(&shaderMulLighting);
	modelEnemy5.setShader(&shaderMulLighting);

	modelAntocha.setShader(&shaderMulLighting);
}

void prepareDepthScene(){

	terrain.setShader(&shaderDepth);
	
	//Grass
	//modelGrass.setShader(&shaderDepth);

	//Player
	playerModelAnimate.setShader(&shaderDepth);

	// Fountain
	modelFountain.setShader(&shaderDepth);

	// Monedas del laberinto
	modelMoneda1.setShader(&shaderDepth);
	modelMoneda2.setShader(&shaderDepth);
	modelMoneda3.setShader(&shaderDepth);
	modelMoneda4.setShader(&shaderDepth);
	modelMoneda5.setShader(&shaderDepth);

	modelEnemy1.setShader(&shaderDepth);
	modelEnemy2.setShader(&shaderDepth);
	modelEnemy3.setShader(&shaderDepth);
	modelEnemy4.setShader(&shaderDepth);
	modelEnemy5.setShader(&shaderDepth);

	modelAntocha.setShader(&shaderDepth);
}

void renderSolidScene(){
	/*******************************************
	 * Terrain Cesped
	 *******************************************/
	// Se activa la textura del agua
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureCespedID);
	shaderTerrain.setInt("backgroundTexture", 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
	shaderTerrain.setInt("rTexture", 1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
	shaderTerrain.setInt("gTexture", 2);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
	shaderTerrain.setInt("bTexture", 3);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
	shaderTerrain.setInt("blendMapTexture", 4);
	shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(80, 80)));
	terrain.setPosition(glm::vec3(100, 0, 100));
	terrain.render();
	shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
	glBindTexture(GL_TEXTURE_2D, 0);

	/*******************************************
	 * Custom objects obj
	 *******************************************/
	// Laberinto models at origin
	modelEntrada.render(modelMatrixEntrada);
	modelSalida.render(modelMatrixSalida);
	modelPuertaDer.render(modelMatrixPuertaDer);
	modelPuertaIzq.render(modelMatrixPuertaIzq);
	modelMuroDer.render(modelMatrixMuroDer);
	modelMuroIzq.render(modelMatrixMuroIzq);

	// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
	glActiveTexture(GL_TEXTURE0);

	/*****************************************
	 * Objetos animados por huesos
	 * **************************************/
	glm::vec3 ejey = glm::normalize(terrain.getNormalTerrain(modelMatrixPlayer[3][0], modelMatrixPlayer[3][2]));
	glm::vec3 ejex = glm::vec3(modelMatrixPlayer[0]);
	glm::vec3 ejez = glm::normalize(glm::cross(ejex, ejey));
	ejex = glm::normalize(glm::cross(ejey, ejez));
	modelMatrixPlayer[0] = glm::vec4(ejex, 0.0);
	modelMatrixPlayer[1] = glm::vec4(ejey, 0.0);
	modelMatrixPlayer[2] = glm::vec4(ejez, 0.0);
	modelMatrixPlayer[3][1] = -GRAVITY * tmv * tmv + 5.0 * tmv + terrain.getHeightTerrain(modelMatrixPlayer[3][0], modelMatrixPlayer[3][2]);
	tmv = currTime - startTimeJump;
	if(modelMatrixPlayer[3][1] < terrain.getHeightTerrain(modelMatrixPlayer[3][0], modelMatrixPlayer[3][2])){
		isJump = false;
		modelMatrixPlayer[3][1] = terrain.getHeightTerrain(modelMatrixPlayer[3][0], modelMatrixPlayer[3][2]);
	}
	glm::mat4 modelMatrixPlayerBody = glm::mat4(modelMatrixPlayer);
	modelMatrixPlayerBody = glm::scale(modelMatrixPlayerBody, glm::vec3(0.015f));
	modelMatrixPlayerBody = glm::rotate(modelMatrixPlayerBody, glm::radians(180.0f), glm::vec3(0, 1, 0));
	playerModelAnimate.setAnimationIndex(animationPlayerIndex);
	playerModelAnimate.render(modelMatrixPlayerBody);
	//animationPlayerIndex = 1;

	// Enemigos
	modelMatrixEnemy1[3][1] = terrain.getHeightTerrain(
		modelMatrixEnemy1[3][0], modelMatrixEnemy1[3][2]);
	modelMatrixEnemy2[3][1] = terrain.getHeightTerrain(
		modelMatrixEnemy2[3][0], modelMatrixEnemy2[3][2]);
	modelMatrixEnemy3[3][1] = terrain.getHeightTerrain(
		modelMatrixEnemy3[3][0], modelMatrixEnemy3[3][2]);
	modelMatrixEnemy4[3][1] = terrain.getHeightTerrain(
		modelMatrixEnemy4[3][0], modelMatrixEnemy4[3][2]);
	modelMatrixEnemy5[3][1] = terrain.getHeightTerrain(
		modelMatrixEnemy5[3][0], modelMatrixEnemy5[3][2]);

	modelEnemy1.render(glm::scale(modelMatrixEnemy1, glm::vec3(0.01f)));
	modelEnemy2.render(glm::scale(modelMatrixEnemy2, glm::vec3(0.01f)));
	modelEnemy3.render(glm::scale(modelMatrixEnemy3, glm::vec3(0.01f)));
	modelEnemy4.render(glm::scale(modelMatrixEnemy4, glm::vec3(0.01f)));
	modelEnemy5.render(glm::scale(modelMatrixEnemy5, glm::vec3(0.01f)));

	// Fountain
	glDisable(GL_CULL_FACE);
	modelMatrixFountain[3][1] = terrain.getHeightTerrain(modelMatrixFountain[3][0] , modelMatrixFountain[3][2]) + 0.2;
	glm::mat4 modelMatrixFountainCopy = glm::scale(modelMatrixFountain, glm::vec3(10.0f, 10.0f, 10.0f));
	modelFountain.render(modelMatrixFountainCopy);
	glEnable(GL_CULL_FACE);

	// Antochas
	for (int i = 0; i < antochaPositions.size(); i++) {
		glm::mat4 modelMatrixAntocha = glm::translate(
			glm::mat4(1.0f), antochaPositions[i]);
		modelAntocha.render(modelMatrixAntocha);
	}

		// Laberinto walls additional
		modelMuro1.render(glm::scale(modelMatrixMuro1, glm::vec3(0.5f)));
		modelMuro2.render(glm::scale(modelMatrixMuro2, glm::vec3(0.5f)));
		modelMuro3.render(glm::scale(modelMatrixMuro3, glm::vec3(0.5f)));
		modelMuro4.render(glm::scale(modelMatrixMuro4, glm::vec3(0.5f)));
		modelMuro5.render(glm::scale(modelMatrixMuro5, glm::vec3(0.5f)));
		modelMuro6.render(glm::scale(modelMatrixMuro6, glm::vec3(0.5f)));
		modelMuro7.render(glm::scale(modelMatrixMuro7, glm::vec3(0.5f)));
		modelMuro8.render(glm::scale(modelMatrixMuro8, glm::vec3(0.5f)));
		modelMuro9.render(glm::scale(modelMatrixMuro9, glm::vec3(0.5f)));
		modelMuro10.render(glm::scale(modelMatrixMuro10, glm::vec3(0.5f)));
		modelMuro11.render(glm::scale(modelMatrixMuro11, glm::vec3(0.5f)));
		modelMuro12.render(glm::scale(modelMatrixMuro12, glm::vec3(0.5f)));
		modelMuro13.render(glm::scale(modelMatrixMuro13, glm::vec3(0.5f)));
		modelMuro14.render(glm::scale(modelMatrixMuro14, glm::vec3(0.5f)));
		modelMuro15.render(glm::scale(modelMatrixMuro15, glm::vec3(0.5f)));

		// Monedas del laberinto. Sus posiciones ya vienen incorporadas en los OBJ.
		if (monedasActivas[0])
			modelMoneda1.render(glm::scale(modelMatrixMoneda1, glm::vec3(0.5f)));
		if (monedasActivas[1])
			modelMoneda2.render(glm::scale(modelMatrixMoneda2, glm::vec3(0.5f)));
		if (monedasActivas[2])
			modelMoneda3.render(glm::scale(modelMatrixMoneda3, glm::vec3(0.5f)));
		if (monedasActivas[3])
			modelMoneda4.render(glm::scale(modelMatrixMoneda4, glm::vec3(0.5f)));
		if (monedasActivas[4])
			modelMoneda5.render(glm::scale(modelMatrixMoneda5, glm::vec3(0.5f)));

	/*******************************************
	 * Skybox
	 *******************************************/
	GLint oldCullFaceMode;
	GLint oldDepthFuncMode;
	// deshabilita el modo del recorte de caras ocultas para ver las esfera desde adentro
	glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
	glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
	shaderSkybox.setFloat("skybox", 0);
	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);
	glActiveTexture(GL_TEXTURE0);
	skyboxSphere.render();
	glCullFace(oldCullFaceMode);
	glDepthFunc(oldDepthFuncMode);
}

void renderAlphaScene(bool render = true){
	/**********
	 * Update the position with alpha objects
	 */
	// update the fountain
	blendingUnsorted.find("fountain")->second = glm::vec3(modelMatrixFountain[3]);

	/**********
	 * Sorter with alpha objects
	 */
	std::map<float, std::pair<std::string, glm::vec3>> blendingSorted;
	std::map<std::string, glm::vec3>::iterator itblend;
	for(itblend = blendingUnsorted.begin(); itblend != blendingUnsorted.end(); itblend++){
		float distanceFromView = glm::length(camera->getPosition() - itblend->second);
		blendingSorted[distanceFromView] = std::make_pair(itblend->first, itblend->second);
	}

	/**********
	 * Render de las transparencias
	 */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	
	for(std::map<float, std::pair<std::string, glm::vec3> >::reverse_iterator it = blendingSorted.rbegin(); it != blendingSorted.rend(); it++){
		if(render && it->second.first.compare("fountain") == 0){
			// Se renderiza el sistema de partículas de la fuente
			glm::mat4 modelMatrixParticlesFountain = glm::mat4(1.0);
			modelMatrixParticlesFountain = glm::translate(modelMatrixParticlesFountain, it->second.second);
			modelMatrixParticlesFountain[3][1] = terrain.getHeightTerrain(modelMatrixParticlesFountain[3][0], modelMatrixParticlesFountain[3][2]) + 4.2f;
			modelMatrixParticlesFountain = glm::scale(modelMatrixParticlesFountain, glm::vec3(3.0f));
			currentTimeParticles = TimeManager::Instance().GetTime();
			if(currentTimeParticles - lastTimeParticles > 10.0f){
				lastTimeParticles = currentTimeParticles;}
			glDepthMask(GL_FALSE);
			glPointSize(10.0f);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureParticleFountainID);
			shaderParticlesFountain.turnOn();
			shaderParticlesFountain.setFloat("Time", float(currentTimeParticles - lastTimeParticles));
			shaderParticlesFountain.setFloat("ParticleLifetime", 10.0f);
			shaderParticlesFountain.setInt("ParticleTex", 0);
			shaderParticlesFountain.setVectorFloat3("Gravity", glm::value_ptr(glm::vec3(0.0f, -0.01f, 0.0f)));
			shaderParticlesFountain.setMatrix4("model", 1, false, glm::value_ptr(modelMatrixParticlesFountain));
			glBindVertexArray(VAOParticles);
			glDrawArrays(GL_POINTS, 0, nParticles);
			glDepthMask(GL_TRUE);
			shaderParticlesFountain.turnOff();
		}
		else if (render
				&& it->second.first.compare(0, 4, "fire") == 0) {
			const AbstractModel::AABB &torchAABB = modelAntocha.getAAbb();
			glm::vec3 firePosition(
				(torchAABB.mins.x + torchAABB.maxs.x) * 0.5f + 0.5f,
				torchAABB.maxs.y,
				(torchAABB.mins.z + torchAABB.maxs.z) * 0.5f);
			firePosition += it->second.second;
			glm::mat4 modelMatrixFire = glm::translate(
				glm::mat4(1.0f), firePosition);

			currentTimeFire = TimeManager::Instance().GetTime();

			if (currentTimeFire - lastTimeFire > fireParticleLifetime) {
				lastTimeFire = currentTimeFire;
			}

			glDepthMask(GL_FALSE);

			shaderParticlesFire.turnOn();

			shaderParticlesFire.setFloat("Time", float(currentTimeFire - lastTimeFire));
			shaderParticlesFire.setFloat("ParticleLifetime", fireParticleLifetime);
			shaderParticlesFire.setFloat("ParticleSize", fireParticleSize);
			shaderParticlesFire.setInt("ParticleTex", 0);

			// Movimiento hacia arriba
			shaderParticlesFire.setVectorFloat3(
				"Accel",
				glm::value_ptr(glm::vec3(0.0f, 0.25f, 0.0f))
			);

			shaderParticlesFire.setMatrix4(
				"model",
				1,
				false,
				glm::value_ptr(modelMatrixFire)
			);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureParticleFireID);

			glBindVertexArray(VAOParticlesFire);
			glDrawArrays(GL_POINTS, 0, nParticlesFire);
			glBindVertexArray(0);

			shaderParticlesFire.turnOff();

			glDepthMask(GL_TRUE);
		}
	}
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	if(render){
		/************Render de imagen de frente**************/
		shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
		shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureScreen0LivesID);
		shaderTexture.setInt("outTexture", 0);
		glEnable(GL_BLEND);
		boxIntro.render();

		glBindTexture(GL_TEXTURE_2D, textureHeartID);
		for (int i = 0; i < vidasJugador; i++) {
			glm::mat4 heartMatrix = glm::translate(
				glm::mat4(1.0f),
				glm::vec3(0.68f + i * 0.14f, 0.86f, 0.0f));
			heartMatrix = glm::scale(
				heartMatrix, glm::vec3(0.12f, 0.12f, 0.05f));
			boxHeart.render(heartMatrix);
		}
		glDisable(GL_BLEND);

		modelText->render(
			"Monedas: " + std::to_string(contadorMonedas),
			-0.95f, 0.88f, 28.0f, 1.0f, 1.0f, 1.0f);
	}
}

void renderScene(){
	renderSolidScene();
	renderAlphaScene(false);
}

void applicationLoop() {
	bool psi = true;

	glm::vec3 axis;
	glm::vec3 target;
	float angleTarget;

	modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0.0f, 0.05f, -70.0f));
	modelMatrixPlayer = glm::rotate(modelMatrixPlayer, glm::radians(-90.0f), glm::vec3(0, 1, 0));

	modelMatrixEnemy1 = glm::translate(
		modelMatrixEnemy1, glm::vec3(-60.0f, 0.0f, 67.5f));// area es el que esta en la esquina superior derecha
	modelMatrixEnemy2 = glm::translate(
		modelMatrixEnemy2, glm::vec3(67.5f, 0.0f, 67.5f));// area es el que esta en la esquina superior derecha
	modelMatrixEnemy3 = glm::translate(
		modelMatrixEnemy3, glm::vec3(12.5f, 0.0f, 45.0f)); // area es el que esta en la esquina inferior derecha
	modelMatrixEnemy4 = glm::translate(
		modelMatrixEnemy4, glm::vec3(-70.0f, 0.0f, 32.5f));// area es el que esta en la esquina inferior izquierda
	modelMatrixEnemy5 = glm::translate(
		modelMatrixEnemy5, glm::vec3(-67.5f, 0.0f, -67.5f)); //Aerea es el que esta en la esquina superior izquierda
	modelMatrixEnemy5 = glm::rotate(
		modelMatrixEnemy5, glm::radians(-90.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	baseMatrixEnemy1 = modelMatrixEnemy1;
	baseMatrixEnemy2 = modelMatrixEnemy2;
	baseMatrixEnemy3 = modelMatrixEnemy3;
	baseMatrixEnemy4 = modelMatrixEnemy4;
	baseMatrixEnemy5 = modelMatrixEnemy5;

	modelMatrixFountain = glm::translate(modelMatrixFountain, glm::vec3(5.0, 0.0, -40.0));

	// Muros del laberinto
	modelMatrixMuro1 = glm::mat4(1.0f);
	modelMatrixMuro1 = glm::translate(modelMatrixMuro1, glm::vec3(0.0f, 0.0f, 0.0f));

	modelMatrixMuro2 = glm::mat4(1.0f);
	modelMatrixMuro2 = glm::translate(modelMatrixMuro2, glm::vec3(0.0f, 0.0f, 0.0f));

	modelMatrixMuro3 = glm::mat4(1.0f);
	modelMatrixMuro3 = glm::translate(modelMatrixMuro3, glm::vec3(0.0f, 0.0f, 0.0f));

	modelMatrixMuro4 = glm::mat4(1.0f);
	modelMatrixMuro4 = glm::translate(modelMatrixMuro4, glm::vec3(0.0f, 0.0f, 0.0f));

	modelMatrixMuro5 = glm::mat4(1.0f);
	modelMatrixMuro5 = glm::translate(modelMatrixMuro5, glm::vec3(0.0f, 0.0f, 0.0f));

	modelMatrixMuro6 = glm::mat4(1.0f);
	modelMatrixMuro6 = glm::translate(modelMatrixMuro6, glm::vec3(0.0f, 0.0f, 0.0f)); //-50 en z

	modelMatrixMuro7 = glm::mat4(1.0f);
	modelMatrixMuro7 = glm::translate(modelMatrixMuro7, glm::vec3(0.0f, 0.0f, 0.0f));

	modelMatrixMuro8 = glm::mat4(1.0f);
	modelMatrixMuro8 = glm::translate(modelMatrixMuro8, glm::vec3(0.0f, 0.0f, 0.0f));

	modelMatrixMuro9 = glm::mat4(1.0f);
	modelMatrixMuro9 = glm::translate(modelMatrixMuro9, glm::vec3(0.0f, 0.0f, 0.0f));

	modelMatrixMuro10 = glm::mat4(1.0f);
	modelMatrixMuro10 = glm::translate(modelMatrixMuro10, glm::vec3(0.0f, 0.0f, 0.0f));

	modelMatrixMuro11 = glm::mat4(1.0f);
	modelMatrixMuro11 = glm::translate(modelMatrixMuro11, glm::vec3(0.0f, 0.0f, 0.0f));

	modelMatrixMuro12 = glm::mat4(1.0f);
	modelMatrixMuro12 = glm::translate(modelMatrixMuro12, glm::vec3(0.0f, 0.0f, 0.0f));

	modelMatrixMuro13 = glm::mat4(1.0f);
	modelMatrixMuro13 = glm::translate(modelMatrixMuro13, glm::vec3(0.0f, 0.0f, 0.0f));

	modelMatrixMuro14 = glm::mat4(1.0f);
	modelMatrixMuro14 = glm::translate(modelMatrixMuro14, glm::vec3(0.0f, 0.0f, 0.0f));

	modelMatrixMuro15 = glm::mat4(1.0f);
	modelMatrixMuro15 = glm::translate(modelMatrixMuro15, glm::vec3(0.0f, 0.0f, 0.0f));

	// Las monedas ya contienen sus transformaciones dentro de cada archivo OBJ.
	modelMatrixMoneda1 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 0.0f));
	modelMatrixMoneda2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 0.0f));
	modelMatrixMoneda3 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 0.0f));
	modelMatrixMoneda4 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 0.0f));
	modelMatrixMoneda5 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 0.0f));
	//Limites del laberinto
	modelMatrixEntrada = glm::translate(modelMatrixEntrada, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixEntrada = glm::rotate(modelMatrixEntrada, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixEntrada = glm::scale(modelMatrixEntrada, glm::vec3(0.5f));

	modelMatrixSalida = glm::translate(modelMatrixSalida, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixSalida = glm::rotate(modelMatrixSalida, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixSalida = glm::scale(modelMatrixSalida, glm::vec3(0.5f));

	// Puertas del laberinto
	modelMatrixPuertaDer = glm::translate(modelMatrixPuertaDer, glm::vec3(0.0f, 0.0f, 0.0f));
	modelMatrixPuertaDer = glm::rotate(modelMatrixPuertaDer, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixPuertaDer = glm::scale(modelMatrixPuertaDer, glm::vec3(0.5f));

	modelMatrixPuertaIzq = glm::translate(modelMatrixPuertaIzq, glm::vec3(0.0f, 0.0f, 0.0f));
	modelMatrixPuertaIzq = glm::rotate(modelMatrixPuertaIzq, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixPuertaIzq = glm::scale(modelMatrixPuertaIzq, glm::vec3(0.5f));

	// Se conserva el area original de las puertas como zona de victoria.
	triggerPuertaDer = transformAABB(
		modelPuertaDer.getAAbb(), modelMatrixPuertaDer);
	triggerPuertaIzq = transformAABB(
		modelPuertaIzq.getAAbb(), modelMatrixPuertaIzq);
	const glm::vec3 margenTriggerPuerta(1.5f, 0.5f, 3.0f);
	triggerPuertaDer.mins -= margenTriggerPuerta;
	triggerPuertaDer.maxs += margenTriggerPuerta;
	triggerPuertaIzq.mins -= margenTriggerPuerta;
	triggerPuertaIzq.maxs += margenTriggerPuerta;

	modelMatrixMuroDer = glm::translate(modelMatrixMuroDer, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixMuroDer = glm::rotate(modelMatrixMuroDer, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixMuroDer = glm::scale(modelMatrixMuroDer, glm::vec3(0.5f));

	modelMatrixMuroIzq = glm::translate(modelMatrixMuroIzq, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixMuroIzq = glm::rotate(modelMatrixMuroIzq, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixMuroIzq = glm::scale(modelMatrixMuroIzq, glm::vec3(0.5f));

	lastTime = TimeManager::Instance().GetTime();

	textureActivaID = textureInit1ID;

	glm::vec3 lightPos = glm::vec3(10.0, 10.0, -10.0);

	shadowBox = new ShadowBox(-lightPos, camera.get(), 15.0f, 0.1f, 45.0f);

	while (psi) {
		currTime = TimeManager::Instance().GetTime();
		if(currTime - lastTime < 0.016666667){
			glfwPollEvents();
			continue;
		}
		lastTime = currTime;
		TimeManager::Instance().CalculateFrameRate(true);
		deltaTime = TimeManager::Instance().DeltaTime;
		psi = processInput(true);

		float patrolDeltaTime = glm::min(
			static_cast<float>(deltaTime), 0.1f);
		updateEnemyPatrol(
			modelEnemy1, modelMatrixEnemy1,
			baseMatrixEnemy1,
			enemyPatrol1, patrolDeltaTime);
		updateEnemyPatrol(
			modelEnemy2, modelMatrixEnemy2,
			baseMatrixEnemy2,
			enemyPatrol2, patrolDeltaTime);
		updateEnemyPatrol(
			modelEnemy3, modelMatrixEnemy3,
			baseMatrixEnemy3,
			enemyPatrol3, patrolDeltaTime);
		updateEnemyPatrol(
			modelEnemy4, modelMatrixEnemy4,
			baseMatrixEnemy4,
			enemyPatrol4, patrolDeltaTime);
		updateEnemyPatrol(
			modelEnemy5, modelMatrixEnemy5,
			baseMatrixEnemy5,
			enemyPatrol5, patrolDeltaTime);

		if (!puertasAbiertas
				&& contadorMonedas == static_cast<int>(monedasActivas.size())) {
			const AbstractModel::AABB &aabbPuertaIzq =
				modelPuertaIzq.getAAbb();
			glm::vec3 pivotePuertaIzq(
				aabbPuertaIzq.maxs.x,
				(aabbPuertaIzq.mins.y + aabbPuertaIzq.maxs.y) * 0.5f,
				(aabbPuertaIzq.mins.z + aabbPuertaIzq.maxs.z) * 0.5f);
			modelMatrixPuertaIzq = glm::translate(
				modelMatrixPuertaIzq, pivotePuertaIzq);
			modelMatrixPuertaIzq = glm::rotate(
				modelMatrixPuertaIzq, glm::radians(-90.0f),
				glm::vec3(0.0f, 1.0f, 0.0f));
			modelMatrixPuertaIzq = glm::translate(
				modelMatrixPuertaIzq, -pivotePuertaIzq);

			const AbstractModel::AABB &aabbPuertaDer =
				modelPuertaDer.getAAbb();
			glm::vec3 pivotePuertaDer(
				aabbPuertaDer.mins.x,
				(aabbPuertaDer.mins.y + aabbPuertaDer.maxs.y) * 0.5f,
				(aabbPuertaDer.mins.z + aabbPuertaDer.maxs.z) * 0.5f);
			modelMatrixPuertaDer = glm::translate(
				modelMatrixPuertaDer, pivotePuertaDer);
			modelMatrixPuertaDer = glm::rotate(
				modelMatrixPuertaDer, glm::radians(90.0f),
				glm::vec3(0.0f, 1.0f, 0.0f));
			modelMatrixPuertaDer = glm::translate(
				modelMatrixPuertaDer, -pivotePuertaDer);

			puertasAbiertas = true;
			std::cout << "Todas las monedas recolectadas. Puertas abiertas."
					<< std::endl;
		}

		std::map<std::string, bool> collisionDetection;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
				(float) screenWidth / (float) screenHeight, 0.01f, 100.0f);

		axis = glm::axis(glm::quat_cast(modelMatrixPlayer));
		angleTarget = glm::angle(glm::quat_cast(modelMatrixPlayer));
		target = modelMatrixPlayer[3];
		if (modelSelected == 0)
			target.y += THIRD_PERSON_TARGET_HEIGHT;

		if(std::isnan(angleTarget))
			angleTarget = 0.0;
		if(axis.y < 0)
			angleTarget = -angleTarget;
		camera->setCameraTarget(target);
		camera->setAngleTarget(angleTarget);
		camera->updateCamera();
		glm::mat4 view = camera->getViewMatrix();

		shadowBox->update(screenWidth, screenHeight);
		glm::vec3 centerBox = shadowBox->getCenter();

		// Projection light shadow mapping
		glm::mat4 lightProjection = glm::mat4(1.0f), lightView = glm::mat4(1.0f);
		glm::mat4 lightSpaceMatrix;
		lightProjection[0][0] = 2.0f / shadowBox->getWidth();
		lightProjection[1][1] = 2.0f / shadowBox->getHeight();
		lightProjection[2][2] = -2.0f / shadowBox->getLength();
		lightProjection[3][3] = 1.0f;
		lightView = glm::lookAt(centerBox, centerBox + glm::normalize(-lightPos), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		shaderDepth.setMatrix4("lightSpaceMatrix", 1, false, glm::value_ptr(lightSpaceMatrix));

		// Settea la matriz de vista y projection al shader con solo color
		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con skybox
		shaderSkybox.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderSkybox.setMatrix4("view", 1, false,
				glm::value_ptr(glm::mat4(glm::mat3(view))));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderMulLighting.setMatrix4("projection", 1, false,
					glm::value_ptr(projection));
		shaderMulLighting.setMatrix4("view", 1, false,
				glm::value_ptr(view));
		shaderMulLighting.setMatrix4("lightSpaceMatrix", 1, false,
				glm::value_ptr(lightSpaceMatrix));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderTerrain.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderTerrain.setMatrix4("view", 1, false,
				glm::value_ptr(view));
		shaderTerrain.setMatrix4("lightSpaceMatrix", 1, false,
				glm::value_ptr(lightSpaceMatrix));
		// Settea la matriz de vista y projection al shader para el fountain
		shaderParticlesFountain.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderParticlesFountain.setMatrix4("view", 1, false,
				glm::value_ptr(view));
		// Settea la matriz de vista y projection al shader para el fuego
		shaderParticlesFire.setMatrix4("projection", 1, false, 
			glm::value_ptr(projection));
		shaderParticlesFire.setMatrix4("view", 1, false, 
			glm::value_ptr(view));

		/*******************************************
		 * Propiedades de neblina
		 *******************************************/
		shaderMulLighting.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));
		shaderTerrain.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));
		shaderSkybox.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));

		/*******************************************
		 * Propiedades Luz direccional
		 *******************************************/
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.05f, 0.05f, 0.06f)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.15f, 0.15f, 0.18f)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.05f, 0.05f, 0.06f)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

		shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderTerrain.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.05f, 0.05f, 0.06f)));
		shaderTerrain.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.15f, 0.15f, 0.18f)));
		shaderTerrain.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.05f, 0.05f, 0.06f)));
		shaderTerrain.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

		/*******************************************
		 * Propiedades SpotLights
		 *******************************************/
		int exitSpotLightCount = puertasAbiertas ? 1 : 0;
		shaderMulLighting.setInt(
			"spotLightCount", exitSpotLightCount);
		shaderTerrain.setInt(
			"spotLightCount", exitSpotLightCount);
		if (puertasAbiertas) {
			glm::vec3 exitCenter =
				((triggerPuertaIzq.mins + triggerPuertaIzq.maxs)
				+ (triggerPuertaDer.mins + triggerPuertaDer.maxs))
				* 0.25f;
			glm::vec3 exitSpotPosition(
				exitCenter.x, 12.0f, exitCenter.z);
			glm::vec3 exitSpotDirection(0.0f, -1.0f, 0.0f);

			shaderMulLighting.setVectorFloat3(
				"spotLights[0].light.ambient",
				glm::value_ptr(glm::vec3(0.08f, 0.07f, 0.035f)));
			shaderMulLighting.setVectorFloat3(
				"spotLights[0].light.diffuse",
				glm::value_ptr(glm::vec3(1.0f, 0.82f, 0.35f)));
			shaderMulLighting.setVectorFloat3(
				"spotLights[0].light.specular",
				glm::value_ptr(glm::vec3(1.0f, 0.9f, 0.55f)));
			shaderMulLighting.setVectorFloat3(
				"spotLights[0].position",
				glm::value_ptr(exitSpotPosition));
			shaderMulLighting.setVectorFloat3(
				"spotLights[0].direction",
				glm::value_ptr(exitSpotDirection));
			shaderMulLighting.setFloat(
				"spotLights[0].constant", 1.0f);
			shaderMulLighting.setFloat(
				"spotLights[0].linear", 0.045f);
			shaderMulLighting.setFloat(
				"spotLights[0].quadratic", 0.0075f);
			shaderMulLighting.setFloat(
				"spotLights[0].cutOff", cos(glm::radians(35.0f)));
			shaderMulLighting.setFloat(
				"spotLights[0].outerCutOff", cos(glm::radians(50.0f)));

			shaderTerrain.setVectorFloat3(
				"spotLights[0].light.ambient",
				glm::value_ptr(glm::vec3(0.08f, 0.07f, 0.035f)));
			shaderTerrain.setVectorFloat3(
				"spotLights[0].light.diffuse",
				glm::value_ptr(glm::vec3(1.0f, 0.82f, 0.35f)));
			shaderTerrain.setVectorFloat3(
				"spotLights[0].light.specular",
				glm::value_ptr(glm::vec3(1.0f, 0.9f, 0.55f)));
			shaderTerrain.setVectorFloat3(
				"spotLights[0].position",
				glm::value_ptr(exitSpotPosition));
			shaderTerrain.setVectorFloat3(
				"spotLights[0].direction",
				glm::value_ptr(exitSpotDirection));
			shaderTerrain.setFloat(
				"spotLights[0].constant", 1.0f);
			shaderTerrain.setFloat(
				"spotLights[0].linear", 0.045f);
			shaderTerrain.setFloat(
				"spotLights[0].quadratic", 0.0075f);
			shaderTerrain.setFloat(
				"spotLights[0].cutOff", cos(glm::radians(35.0f)));
			shaderTerrain.setFloat(
				"spotLights[0].outerCutOff", cos(glm::radians(50.0f)));
		}

		/*******************************************
		 * PointLights de las antorchas
		 *******************************************/
		const int playerLightIndex =
			static_cast<int>(antochaPositions.size());
		shaderMulLighting.setInt(
			"pointLightCount", playerLightIndex + 1);
		shaderTerrain.setInt(
			"pointLightCount", playerLightIndex + 1);
		const AbstractModel::AABB &torchAABB = modelAntocha.getAAbb();
		glm::vec3 torchLightOffset(
			(torchAABB.mins.x + torchAABB.maxs.x) * 0.5f + 0.25f,
			torchAABB.maxs.y,
			(torchAABB.mins.z + torchAABB.maxs.z) * 0.5f);
		for (int i = 0; i < antochaPositions.size(); i++) {
			glm::vec3 torchLightPosition =
				antochaPositions[i] + torchLightOffset;
			std::string lightIndex = std::to_string(i);

			shaderMulLighting.setVectorFloat3(
				"pointLights[" + lightIndex + "].light.ambient",
				glm::value_ptr(glm::vec3(0.12f, 0.045f, 0.005f)));
			shaderMulLighting.setVectorFloat3(
				"pointLights[" + lightIndex + "].light.diffuse",
				glm::value_ptr(glm::vec3(1.0f, 0.42f, 0.08f)));
			shaderMulLighting.setVectorFloat3(
				"pointLights[" + lightIndex + "].light.specular",
				glm::value_ptr(glm::vec3(1.0f, 0.65f, 0.2f)));
			shaderMulLighting.setVectorFloat3(
				"pointLights[" + lightIndex + "].position",
				glm::value_ptr(torchLightPosition));
			shaderMulLighting.setFloat(
				"pointLights[" + lightIndex + "].constant", 1.0f);
			shaderMulLighting.setFloat(
				"pointLights[" + lightIndex + "].linear", 0.09f);
			shaderMulLighting.setFloat(
				"pointLights[" + lightIndex + "].quadratic", 0.032f);

			shaderTerrain.setVectorFloat3(
				"pointLights[" + lightIndex + "].light.ambient",
				glm::value_ptr(glm::vec3(0.12f, 0.045f, 0.005f)));
			shaderTerrain.setVectorFloat3(
				"pointLights[" + lightIndex + "].light.diffuse",
				glm::value_ptr(glm::vec3(1.0f, 0.42f, 0.08f)));
			shaderTerrain.setVectorFloat3(
				"pointLights[" + lightIndex + "].light.specular",
				glm::value_ptr(glm::vec3(1.0f, 0.65f, 0.2f)));
			shaderTerrain.setVectorFloat3(
				"pointLights[" + lightIndex + "].position",
				glm::value_ptr(torchLightPosition));
			shaderTerrain.setFloat(
				"pointLights[" + lightIndex + "].constant", 1.0f);
			shaderTerrain.setFloat(
				"pointLights[" + lightIndex + "].linear", 0.09f);
			shaderTerrain.setFloat(
				"pointLights[" + lightIndex + "].quadratic", 0.032f);
		}

		glm::vec3 playerLightPosition = glm::vec3(
			modelMatrixPlayer
			* glm::vec4(0.0f, 2.0f, 1.5f, 1.0f));
		std::string playerLight =
			"pointLights[" + std::to_string(playerLightIndex) + "]";

		shaderMulLighting.setVectorFloat3(
			playerLight + ".light.ambient",
			glm::value_ptr(glm::vec3(0.16f, 0.06f, 0.008f)));
		shaderMulLighting.setVectorFloat3(
			playerLight + ".light.diffuse",
			glm::value_ptr(glm::vec3(1.15f, 0.48f, 0.09f)));
		shaderMulLighting.setVectorFloat3(
			playerLight + ".light.specular",
			glm::value_ptr(glm::vec3(1.2f, 0.78f, 0.24f)));
		shaderMulLighting.setVectorFloat3(
			playerLight + ".position",
			glm::value_ptr(playerLightPosition));
		shaderMulLighting.setFloat(playerLight + ".constant", 1.0f);
		shaderMulLighting.setFloat(playerLight + ".linear", 0.07f);
		shaderMulLighting.setFloat(playerLight + ".quadratic", 0.017f);

		shaderTerrain.setVectorFloat3(
			playerLight + ".light.ambient",
			glm::value_ptr(glm::vec3(0.16f, 0.06f, 0.008f)));
		shaderTerrain.setVectorFloat3(
			playerLight + ".light.diffuse",
			glm::value_ptr(glm::vec3(1.15f, 0.48f, 0.09f)));
		shaderTerrain.setVectorFloat3(
			playerLight + ".light.specular",
			glm::value_ptr(glm::vec3(1.2f, 0.78f, 0.24f)));
		shaderTerrain.setVectorFloat3(
			playerLight + ".position",
			glm::value_ptr(playerLightPosition));
		shaderTerrain.setFloat(playerLight + ".constant", 1.0f);
		shaderTerrain.setFloat(playerLight + ".linear", 0.07f);
		shaderTerrain.setFloat(playerLight + ".quadratic", 0.017f);

		/************Render de imagen de frente**************/
		if(!iniciaPartida){
			shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
			shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureActivaID);
			shaderTexture.setInt("outTexture", 0);
			boxIntro.render();
			glfwSwapBuffers(window);
			continue;
		}

		/*******************************************
		 * 1.- We render the depth buffer
		 *******************************************/
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// render scene from light's point of view
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_FRONT);
		prepareDepthScene();
		renderScene();
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		/*******************************************
		 * Debug to view the texture view map
		 *******************************************/
		// reset viewport
		/*glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// render Depth map to quad for visual debugging
		shaderViewDepth.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
		shaderViewDepth.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
		shaderViewDepth.setFloat("near_plane", near_plane);
		shaderViewDepth.setFloat("far_plane", far_plane);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		boxViewDepth.setScale(glm::vec3(2.0, 2.0, 1.0));
		boxViewDepth.render();*/

		/*******************************************
		 * 2.- We render the normal objects
		 *******************************************/
		glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		prepareScene();
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		shaderMulLighting.setInt("shadowMap", 10);
		shaderTerrain.setInt("shadowMap", 10);
		renderSolidScene();

		/*******************************************
		 * Creacion de colliders
		 * IMPORTANT do this before interpolations
		 *******************************************/
		// Collider del player
		AbstractModel::OBB playerCollider;
		glm::mat4 modelmatrixColliderPlayer = glm::mat4(modelMatrixPlayer);

		// Aplicar la misma rotacion visual que se usa en el render ANTES del scale
		// El player usa rotate(180 Y) en lugar del rotate(-90 X) de Mayow
		modelmatrixColliderPlayer = glm::rotate(modelmatrixColliderPlayer,
			glm::radians(180.0f), glm::vec3(0, 1, 0));

		// Extraer el quaternion ANTES del scale (igual que Mayow)
		playerCollider.u = glm::quat_cast(modelmatrixColliderPlayer);

		// Ahora si aplicar el scale (igual que Mayow usaba 0.021)
		modelmatrixColliderPlayer = glm::scale(modelmatrixColliderPlayer,
			glm::vec3(0.015f, 0.015f, 0.015f));

		// Trasladar al centro local del OBB del modelo
		modelmatrixColliderPlayer = glm::translate(modelmatrixColliderPlayer,
			glm::vec3(playerModelAnimate.getObb().c.x,
					playerModelAnimate.getObb().c.y,
					playerModelAnimate.getObb().c.z));

		// Half-extents con el mismo scale uniforme
		playerCollider.e = playerModelAnimate.getObb().e * glm::vec3(0.0085f, 0.015f, 0.0085f);
		playerCollider.c = glm::vec3(modelmatrixColliderPlayer[3]);

		addOrUpdateColliders(collidersOBB, "player", playerCollider, modelMatrixPlayer);

		collidersEnemigosOBB.clear();
		collidersEnemigosOBB["Enemy-1"] = createModelOBB(
			modelEnemy1, modelMatrixEnemy1, 0.01f);
		collidersEnemigosOBB["Enemy-2"] = createModelOBB(
			modelEnemy2, modelMatrixEnemy2, 0.01f);
		collidersEnemigosOBB["Enemy-3"] = createModelOBB(
			modelEnemy3, modelMatrixEnemy3, 0.01f);
		collidersEnemigosOBB["Enemy-4"] = createModelOBB(
			modelEnemy4, modelMatrixEnemy4, 0.01f);
		collidersEnemigosOBB["Enemy-5"] = createModelOBB(
			modelEnemy5, modelMatrixEnemy5, 0.01f);

		// Colliders AABB de monedas. La etiqueta "Moneda-" permite
		// distinguir objetos recolectables de los obstaculos del escenario.
		collidersMonedasAABB.clear();
		if (monedasActivas[0])
			collidersMonedasAABB["Moneda-1"] = transformAABB(
				modelMoneda1.getAAbb(),
				glm::scale(modelMatrixMoneda1, glm::vec3(0.5f)));
		if (monedasActivas[1])
			collidersMonedasAABB["Moneda-2"] = transformAABB(
				modelMoneda2.getAAbb(),
				glm::scale(modelMatrixMoneda2, glm::vec3(0.5f)));
		if (monedasActivas[2])
			collidersMonedasAABB["Moneda-3"] = transformAABB(
				modelMoneda3.getAAbb(),
				glm::scale(modelMatrixMoneda3, glm::vec3(0.5f)));
		if (monedasActivas[3])
			collidersMonedasAABB["Moneda-4"] = transformAABB(
				modelMoneda4.getAAbb(),
				glm::scale(modelMatrixMoneda4, glm::vec3(0.5f)));
		if (monedasActivas[4])
			collidersMonedasAABB["Moneda-5"] = transformAABB(
				modelMoneda5.getAAbb(),
				glm::scale(modelMatrixMoneda5, glm::vec3(0.5f)));

		// Colliders AABB de los muros. Las etiquetas "Muro-" identifican
		// elementos solidos que deben bloquear el movimiento del jugador.
		collidersMurosAABB.clear();
		collidersMurosAABB["Muro-Der"] = transformAABB(
			modelMuroDer.getAAbb(), modelMatrixMuroDer);
		collidersMurosAABB["Muro-Izq"] = transformAABB(
			modelMuroIzq.getAAbb(), modelMatrixMuroIzq);
		collidersMurosAABB["Entrada"] = transformAABB(
			modelEntrada.getAAbb(), modelMatrixEntrada);
		collidersMurosAABB["Salida"] = transformAABB(
			modelSalida.getAAbb(), modelMatrixSalida);
		if (!puertasAbiertas) {
			collidersMurosAABB["Puerta-Izq"] = transformAABB(
				modelPuertaIzq.getAAbb(), modelMatrixPuertaIzq);
			collidersMurosAABB["Puerta-Der"] = transformAABB(
				modelPuertaDer.getAAbb(), modelMatrixPuertaDer);
		}
		collidersMurosAABB["Muro-1"] = transformAABB(
			modelMuro1.getAAbb(),
			glm::scale(modelMatrixMuro1, glm::vec3(0.5f)));
		collidersMurosAABB["Muro-2"] = transformAABB(
			modelMuro2.getAAbb(),
			glm::scale(modelMatrixMuro2, glm::vec3(0.5f)));
		collidersMurosAABB["Muro-3"] = transformAABB(
			modelMuro3.getAAbb(),
			glm::scale(modelMatrixMuro3, glm::vec3(0.5f)));
		collidersMurosAABB["Muro-4"] = transformAABB(
			modelMuro4.getAAbb(),
			glm::scale(modelMatrixMuro4, glm::vec3(0.5f)));
		collidersMurosAABB["Muro-5"] = transformAABB(
			modelMuro5.getAAbb(),
			glm::scale(modelMatrixMuro5, glm::vec3(0.5f)));
		collidersMurosAABB["Muro-6"] = transformAABB(
			modelMuro6.getAAbb(),
			glm::scale(modelMatrixMuro6, glm::vec3(0.5f)));
		collidersMurosAABB["Muro-7"] = transformAABB(
			modelMuro7.getAAbb(),
			glm::scale(modelMatrixMuro7, glm::vec3(0.5f)));
		collidersMurosAABB["Muro-8"] = transformAABB(
			modelMuro8.getAAbb(),
			glm::scale(modelMatrixMuro8, glm::vec3(0.5f)));
		collidersMurosAABB["Muro-9"] = transformAABB(
			modelMuro9.getAAbb(),
			glm::scale(modelMatrixMuro9, glm::vec3(0.5f)));
		collidersMurosAABB["Muro-10"] = transformAABB(
			modelMuro10.getAAbb(),
			glm::scale(modelMatrixMuro10, glm::vec3(0.5f)));
		collidersMurosAABB["Muro-11"] = transformAABB(
			modelMuro11.getAAbb(),
			glm::scale(modelMatrixMuro11, glm::vec3(0.5f)));
		collidersMurosAABB["Muro-12"] = transformAABB(
			modelMuro12.getAAbb(),
			glm::scale(modelMatrixMuro12, glm::vec3(0.5f)));
		collidersMurosAABB["Muro-13"] = transformAABB(
			modelMuro13.getAAbb(),
			glm::scale(modelMatrixMuro13, glm::vec3(0.5f)));
		collidersMurosAABB["Muro-14"] = transformAABB(
			modelMuro14.getAAbb(),
			glm::scale(modelMatrixMuro14, glm::vec3(0.5f)));
		collidersMurosAABB["Muro-15"] = transformAABB(
			modelMuro15.getAAbb(),
			glm::scale(modelMatrixMuro15, glm::vec3(0.5f)));

		/*******************************************
		 * Render de colliders
		 *******************************************/
		for (std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
				collidersOBB.begin(); it != collidersOBB.end(); it++) {
			glm::mat4 matrixCollider = glm::mat4(1.0);
			matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
			matrixCollider = matrixCollider * glm::mat4(std::get<0>(it->second).u);
			matrixCollider = glm::scale(matrixCollider, std::get<0>(it->second).e * 2.0f);
			boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
			boxCollider.enableWireMode();
			boxCollider.render(matrixCollider);
		}

		for (std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
				collidersSBB.begin(); it != collidersSBB.end(); it++) {
			glm::mat4 matrixCollider = glm::mat4(1.0);
			matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
			matrixCollider = glm::scale(matrixCollider, glm::vec3(std::get<0>(it->second).ratio * 2.0f));
			sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
			sphereCollider.enableWireMode();
			sphereCollider.render(matrixCollider);
		}

		for (std::map<std::string, AbstractModel::AABB>::iterator it =
				collidersMonedasAABB.begin();
				it != collidersMonedasAABB.end(); it++) {
			glm::vec3 center = (it->second.mins + it->second.maxs) * 0.5f;
			glm::vec3 size = it->second.maxs - it->second.mins;
			glm::mat4 matrixCollider = glm::translate(glm::mat4(1.0f), center);
			matrixCollider = glm::scale(matrixCollider, size);
			boxCollider.setColor(glm::vec4(1.0, 0.85, 0.0, 1.0));
			boxCollider.enableWireMode();
			boxCollider.render(matrixCollider);
		}

		for (std::map<std::string, AbstractModel::AABB>::iterator it =
				collidersMurosAABB.begin();
				it != collidersMurosAABB.end(); it++) {
			glm::vec3 center = (it->second.mins + it->second.maxs) * 0.5f;
			glm::vec3 size = it->second.maxs - it->second.mins;
			glm::mat4 matrixCollider = glm::translate(glm::mat4(1.0f), center);
			matrixCollider = glm::scale(matrixCollider, size);
			boxCollider.setColor(glm::vec4(1.0, 0.0, 0.0, 1.0));
			boxCollider.enableWireMode();
			boxCollider.render(matrixCollider);
		}

		for (std::map<std::string, AbstractModel::OBB>::iterator it =
				collidersEnemigosOBB.begin();
				it != collidersEnemigosOBB.end(); it++) {
			glm::mat4 matrixCollider = glm::translate(
				glm::mat4(1.0f), it->second.c);
			matrixCollider = matrixCollider * glm::mat4(it->second.u);
			matrixCollider = glm::scale(
				matrixCollider, it->second.e * 2.0f);
			boxCollider.setColor(glm::vec4(1.0, 0.45, 0.0, 1.0));
			boxCollider.enableWireMode();
			boxCollider.render(matrixCollider);
		}

		if (puertasAbiertas) {
			AbstractModel::AABB triggersPuertas[2] = {
				triggerPuertaIzq, triggerPuertaDer
			};
			for (int i = 0; i < 2; i++) {
				glm::vec3 center =
					(triggersPuertas[i].mins + triggersPuertas[i].maxs)
					* 0.5f;
				glm::vec3 size =
					triggersPuertas[i].maxs - triggersPuertas[i].mins;
				glm::mat4 matrixCollider =
					glm::translate(glm::mat4(1.0f), center);
				matrixCollider = glm::scale(matrixCollider, size);
				boxCollider.setColor(glm::vec4(0.0, 1.0, 0.0, 1.0));
				boxCollider.enableWireMode();
				boxCollider.render(matrixCollider);
			}
		}

		/**********Render de transparencias***************/
		renderAlphaScene();

		/*********************Prueba de colisiones****************************/
		AbstractModel::AABB playerAABB = obbToAABB(playerCollider);
		if (vidasJugador > 0
				&& currTime - ultimoDanioJugador
					>= TIEMPO_INVULNERABILIDAD) {
			for (std::map<std::string, AbstractModel::OBB>::iterator it =
					collidersEnemigosOBB.begin();
					it != collidersEnemigosOBB.end(); it++) {
				if (testOBBOBB(playerCollider, it->second)) {
					vidasJugador--;
					ultimoDanioJugador = currTime;
					std::cout << it->first
							<< " hizo danio al jugador. Vidas restantes: "
							<< vidasJugador << std::endl;
					if (vidasJugador == 0)
						std::cout << "El jugador se ha quedado sin vidas."
								<< std::endl;
					break;
				}
			}
		}

		for (std::map<std::string, AbstractModel::AABB>::iterator it =
				collidersMonedasAABB.begin();
				it != collidersMonedasAABB.end(); it++) {
			if (testAABBAABB(playerAABB, it->second)) {
				int monedaIndex = std::stoi(it->first.substr(7)) - 1;
				if (monedaIndex >= 0
						&& monedaIndex < monedasActivas.size()
						&& monedasActivas[monedaIndex]) {
					monedasActivas[monedaIndex] = false;
					contadorMonedas++;
					std::cout << it->first << " recolectada. Monedas: "
							<< contadorMonedas << std::endl;
				}
			}
		}

		if (puertasAbiertas && !juegoGanado
				&& (testAABBAABB(playerAABB, triggerPuertaIzq)
					|| testAABBAABB(playerAABB, triggerPuertaDer))) {
			juegoGanado = true;
			std::cout << "Ganaste" << std::endl;
		}

		for (std::map<std::string, AbstractModel::AABB>::iterator it =
				collidersMurosAABB.begin();
				it != collidersMurosAABB.end(); it++) {
			if (testAABBAABB(playerAABB, it->second)) {
				std::cout << "Colision del jugador con " << it->first
						<< std::endl;
				addOrUpdateCollisionDetection(
					collisionDetection, "player", true);
				break;
			}
		}

		for (std::map<std::string,
			std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator it =
			collidersSBB.begin(); it != collidersSBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string,
				std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator jt =
				collidersSBB.begin(); jt != collidersSBB.end(); jt++) {
				if (it != jt && testSphereSphereIntersection(
					std::get<0>(it->second), std::get<0>(jt->second))) {
					std::cout << "Hay collision entre " << it->first <<
						" y el modelo " << jt->first << std::endl;
					isCollision = true;
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		for (std::map<std::string,
			std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator it =
			collidersOBB.begin(); it != collidersOBB.end(); it++) {
			bool isColision = false;
			for (std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator jt =
				collidersOBB.begin(); jt != collidersOBB.end(); jt++) {
				if (it != jt && 
					testOBBOBB(std::get<0>(it->second), std::get<0>(jt->second))) {
					std::cout << "Hay colision entre " << it->first << " y el modelo" <<
						jt->first << std::endl;
					isColision = true;
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isColision);
		}

		for (std::map<std::string,
			std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator it =
			collidersSBB.begin(); it != collidersSBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator jt =
				collidersOBB.begin(); jt != collidersOBB.end(); jt++) {
				if (testSphereOBox(std::get<0>(it->second), std::get<0>(jt->second))) {
					std::cout << "Hay colision del " << it->first << " y el modelo" <<
						jt->first << std::endl;
					isCollision = true;
					addOrUpdateCollisionDetection(collisionDetection, jt->first, true);
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		std::map<std::string, bool>::iterator itCollision;
		for (itCollision = collisionDetection.begin(); 
			itCollision != collisionDetection.end(); itCollision++) {
			std::map<std::string, std::tuple<AbstractModel::SBB, 
				glm::mat4, glm::mat4>>::iterator sbbBuscado = 
				collidersSBB.find(itCollision->first);
			std::map<std::string, std::tuple<AbstractModel::OBB,
				glm::mat4, glm::mat4>>::iterator obbBuscado =
				collidersOBB.find(itCollision->first);
			if (sbbBuscado != collidersSBB.end()) {
				if (!itCollision->second) 
					addOrUpdateColliders(collidersSBB, itCollision->first);
			}
			if (obbBuscado != collidersOBB.end()) {
				if (!itCollision->second) 
					addOrUpdateColliders(collidersOBB, itCollision->first);
				else {
					if (itCollision->first.compare("player") == 0)
						modelMatrixPlayer = std::get<1>(obbBuscado->second);
				}
			}
		}

		glm::mat4 modelMatrixRayPlayer = glm::mat4(modelMatrixPlayer);
		modelMatrixRayPlayer = glm::translate(modelMatrixRayPlayer, glm::vec3(0, 1, 0));
		float maxDistanceRay = 10.0;
		glm::vec3 rayDirection = modelMatrixRayPlayer[2];
		glm::vec3 ori = modelMatrixRayPlayer[3];
		glm::vec3 rmd = ori + rayDirection * (maxDistanceRay / 2.0f);
		glm::vec3 targetRay = ori + rayDirection * maxDistanceRay;
		modelMatrixRayPlayer[3] = glm::vec4(rmd, 1.0);
		modelMatrixRayPlayer = glm::rotate(modelMatrixRayPlayer, glm::radians(90.0f), 
			glm::vec3(1, 0, 0));
		modelMatrixRayPlayer = glm::scale(modelMatrixRayPlayer, 
			glm::vec3(0.05, maxDistanceRay, 0.05));
		rayModel.render(modelMatrixRayPlayer);

		std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::
			iterator itSBB;
		for (itSBB = collidersSBB.begin(); itSBB != collidersSBB.end(); itSBB++) {
			float tRint;
			if (raySphereIntersect(ori, targetRay, rayDirection,
				std::get<0>(itSBB->second), tRint)) {
				std::cout << "Collision del rayo con el modelo " << itSBB->first 
				<< std::endl;
			}
		}
		/*std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::
			iterator itOBB;
		for (itOBB = collidersOBB.begin(); itOBB != collidersOBB.end(); itOBB++) {
			if (testRayOBB(ori, targetRay, std::get<0>(itOBB->second))) {
				std::cout << "Collision del rayo con el modelo " << itOBB->first
					<< std::endl;
			}
		}*/

		animationPlayerIndex = 0; // keep idle by default

		glfwSwapBuffers(window);

		/****************************+
		 * Open AL sound data
		 */
		source0Pos[0] = modelMatrixFountain[3].x;
		source0Pos[1] = modelMatrixFountain[3].y;
		source0Pos[2] = modelMatrixFountain[3].z;
		alSourcefv(source[0], AL_POSITION, source0Pos);

		/*
		 * Actualizacion de la fuente de Darth reservada:
		 * source2Pos[0] = modelMatrixDarth[3].x;
		 * source2Pos[1] = modelMatrixDarth[3].y;
		 * source2Pos[2] = modelMatrixDarth[3].z;
		 * alSourcefv(source[2], AL_POSITION, source2Pos);
		 */

		// Listener for the Third person camera
		listenerPos[0] = modelMatrixPlayer[3].x;
		listenerPos[1] = modelMatrixPlayer[3].y;
		listenerPos[2] = modelMatrixPlayer[3].z;
		alListenerfv(AL_POSITION, listenerPos);

		glm::vec3 upModel = glm::normalize(modelMatrixPlayer[1]);
		glm::vec3 frontModel = glm::normalize(modelMatrixPlayer[2]);

		listenerOri[0] = frontModel.x;
		listenerOri[1] = frontModel.y;
		listenerOri[2] = frontModel.z;
		listenerOri[3] = upModel.x;
		listenerOri[4] = upModel.y;
		listenerOri[5] = upModel.z;

		// Listener for the First person camera
		// listenerPos[0] = camera->getPosition().x;
		// listenerPos[1] = camera->getPosition().y;
		// listenerPos[2] = camera->getPosition().z;
		// alListenerfv(AL_POSITION, listenerPos);
		// listenerOri[0] = camera->getFront().x;
		// listenerOri[1] = camera->getFront().y;
		// listenerOri[2] = camera->getFront().z;
		// listenerOri[3] = camera->getUp().x;
		// listenerOri[4] = camera->getUp().y;
		// listenerOri[5] = camera->getUp().z;
		alListenerfv(AL_ORIENTATION, listenerOri);

		for(unsigned int i = 0; i < sourcesPlay.size(); i++){
			if(sourcesPlay[i]){
				sourcesPlay[i] = false;
				alSourcePlay(source[i]);
			}
		}
	}
}

int main(int argc, char **argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}
