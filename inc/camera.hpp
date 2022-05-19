#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
private:
    glm::vec3 position;
    glm::vec3 up;
    float pitch;
    float yaw;
    float fov;
  
public:
    Camera(glm::vec3 position, float pitch, float yaw, float fov);

    void setPosition(const glm::vec3 position);  
    void incrementPosition(const glm::vec3 increment);  
    glm::vec3 getPosition() const;

    void setPitch(const float pitch);
    void incrementPitch(const float increment);
    float getPitch() const;

    void setYaw(const float pitch);
    void incrementYaw(const float increment);
    float getYaw() const;

    void setFov(const float pitch);
    void incrementFov(const float increment);
    float getFov() const;

    glm::vec3 getFrontVector() const;
    glm::vec3 getUpVector() const;
    glm::vec3 getRightVector() const;
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix(float window_width, float window_height, float clip_start, float clip_end) const;

};