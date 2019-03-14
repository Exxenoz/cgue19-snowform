#pragma once

#include "Component.h"
#include <glm\glm.hpp>

class TransformComponent;

#define MAX_SPOT_LIGHT_COUNT 2

class SpotLightComponent : public Component
{
public:
    SpotLightComponent(GameObject* owner);
    virtual ~SpotLightComponent();

    virtual void OnStart();

    glm::vec3 GetPosition() const;
    glm::vec3 GetDirection() const { return direction; }
    glm::vec3 GetColor() const { return color; }
    float GetIntensity() const { return intensity; }
    float GetInnerCutOff() const { return innerCutOff; }
    float GetOuterCutOff() const { return outerCutOff; }
    float GetConstant() const { return constant; }
    float GetLinear() const { return linear; }
    float GetQuadratic() const { return quadratic; }

    void SetDirection(glm::vec3 value) { direction = value; }
    void SetColor(glm::vec3 value) { color = value; }
    void SetIntensity(float value) { intensity = value; }
    void SetInnerCutOff(float value) { innerCutOff = value; }
    void SetOuterCutOff(float value) { outerCutOff = value; }
    void SetConstant(float value) { constant = value; }
    void SetLinear(float value) { linear = value; }
    void SetQuadratic(float value) { quadratic = value; }

private:
    TransformComponent* transform;
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
    float innerCutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
};
