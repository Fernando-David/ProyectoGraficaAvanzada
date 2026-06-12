#ifndef AERIALCAMERA_H
#define AERIALCAMERA_H

#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_DLL
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllexport))
    #else
      #define DLL_PUBLIC __declspec(dllexport)
    #endif
  #else
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllimport))
    #else
      #define DLL_PUBLIC __declspec(dllimport)
    #endif
  #endif
  #define DLL_LOCAL
#else
  #if __GNUC__ >= 4
      #define DLL_PUBLIC __attribute__ ((visibility ("default")))
      #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
      #define DLL_PUBLIC
      #define DLL_LOCAL
  #endif
#endif

#include "Camera.h"

class DLL_PUBLIC AerialCamera : public Camera
{
public:
    AerialCamera(glm::vec3 position = glm::vec3(0.0f, 30.0f, 0.0f),
                 glm::vec3 worldUp = glm::vec3(0.0f, 0.0f, -1.0f));
    void mouseMoveCamera(float xoffset, float yoffset, float dt) override;
    void scrollMoveCamera(float soffset, float dt) override;
    void updateCamera() override;
    void moveFrontCamera(bool dir, float dt);
    void moveRightCamera(bool dir, float dt);
};

#endif // AERIALCAMERA_H
