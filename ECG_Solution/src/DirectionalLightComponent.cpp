#include "DirectionalLightComponent.h"

DirectionalLightComponent::DirectionalLightComponent(GameObject* owner) :
    Component::Component(owner),
    color(1.f, 1.f, 1.f),
    direction(0.f, 0.f, 0.f),
    intensity(1.f)
{
}

DirectionalLightComponent::~DirectionalLightComponent()
{
}
