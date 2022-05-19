#include "camera.hpp"

Camera::Camera(glm::vec3 position, float pitch, float yaw, float fov)
{
    this->position = position;
    setPitch(pitch);
    this->yaw = yaw;
    setFov(fov);
    up=glm::vec3(0.0f, 1.0f, 0.0f);
}
void Camera::setPosition(const glm::vec3 position)
{
    this->position = position;
}
void Camera::incrementPosition(const glm::vec3 increment)
{
    this->position += increment;
}
glm::vec3 Camera::getPosition() const
{
    return position;
}

void Camera::setPitch(const float pitch)
{
    this->pitch = pitch;
    if(this->pitch>89.0f)
        this->pitch=89.0f;
    if(this->pitch<-89.0f)
        this->pitch=-89.0f;
}
void Camera::incrementPitch(const float increment)
{
    this->pitch += increment;
    if(this->pitch>89.0f)
        this->pitch=89.0f;
    if(this->pitch<-89.0f)
        this->pitch=-89.0f;
}
float Camera::getPitch() const
{
    return pitch;
}

void Camera::setYaw(const float yaw)
{
    this->yaw = yaw;
}
void Camera::incrementYaw(const float increment)
{
    this->yaw += increment;
}
float Camera::getYaw() const
{
    return yaw;
}

void Camera::setFov(const float fov)
{
    this->fov = fov;
    if(this->fov<=1.0f)
        this->fov = 1.0f;
    if(this->fov>=45.0f)
        this->fov = 45.0f;
}
void Camera::incrementFov(const float increment)
{
    this->fov += increment;
    if(this->fov<=1.0f)
        this->fov = 1.0f;
    if(this->fov>=45.0f)
        this->fov = 45.0f;
}
float Camera::getFov() const
{
    return fov;
}

glm::vec3 Camera::getFrontVector() const
{
    glm::vec3 cameraFront;
    cameraFront.x=cos(glm::radians(pitch))*sin(glm::radians(yaw));
    cameraFront.y=-sin(glm::radians(pitch));
    cameraFront.z=-cos(glm::radians(pitch))*cos(glm::radians(yaw));
    return glm::normalize(cameraFront);
}
glm::vec3 Camera::getUpVector() const
{
    return up;
}
glm::vec3 Camera::getRightVector() const
{
    return glm::normalize(glm::cross(getFrontVector(), up));
}
glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(position, position + getFrontVector(), up);
}
glm::mat4 Camera::GetProjectionMatrix(float window_width, float window_height, float clip_start, float clip_end) const
{
    return glm::perspective(glm::radians(fov), window_width / window_height, clip_start, clip_end);
}
