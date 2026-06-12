#include "Headers/AerialCamera.h"

AerialCamera::AerialCamera(glm::vec3 position, glm::vec3 worldUp) {
    this->position = position;
    this->worldUp = worldUp;
    this->front = glm::vec3(0.0f, -1.0f, 0.0f);
    this->up = glm::vec3(0.0f, 0.0f, 1.0f);
    this->right = glm::normalize(glm::cross(this->front, this->worldUp));
    this->yaw = -90.0f;
    this->pitch = -90.0f;
    this->speed = SPEED;
    this->sensitivity = SENSITIVTY;
    updateCamera();
}

void AerialCamera::mouseMoveCamera(float xoffset, float yoffset, float dt) {
    // No rotation for the fixed top-down camera by default
}

void AerialCamera::scrollMoveCamera(float soffset, float dt) {
    float cameraSpeed = sensitivity * dt;
    position.y += soffset * cameraSpeed;
    updateCamera();
}

void AerialCamera::updateCamera() {
    this->front = glm::vec3(0.0f, -1.0f, 0.0f);
    this->right = glm::normalize(glm::cross(this->front, this->worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}

void AerialCamera::moveFrontCamera(bool dir, float dt) {
    float velocity = dt * speed;
    if (!dir) velocity = -velocity;
    this->position += glm::vec3(0.0f, 0.0f, -velocity);
    updateCamera();
}

void AerialCamera::moveRightCamera(bool dir, float dt) {
    float velocity = dt * speed;
    if (!dir) velocity = -velocity;
    this->position += this->right * velocity;
    updateCamera();
}
