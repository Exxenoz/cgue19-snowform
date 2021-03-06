#pragma once

#include "LightComponent.h"
#include <glm\glm.hpp>

class PointLightComponent : public LightComponent
{
public:
    PointLightComponent(GameObject* owner);
    virtual ~PointLightComponent();

    glm::vec3 GetPosition() const;
    glm::vec3 GetColor() const { return color; }
    float GetIntensity() const { return intensity; }
    float GetConstant() const { return constant; }
    float GetLinear() const { return linear; }
    float GetQuadratic() const { return quadratic; }

    void SetColor(glm::vec3 value) { color = value; }
    void SetIntensity(float value) { intensity = value; }
    void SetConstant(float value) { constant = value; }
    void SetLinear(float value) { linear = value; }
    void SetQuadratic(float value) { quadratic = value; }

private:
    glm::vec3 color;
    float intensity;
    float constant;
    float linear;
    float quadratic;
};
