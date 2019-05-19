#include "DirectionalLightComponent.h"

DirectionalLightComponent::DirectionalLightComponent(GameObject* owner) :
    LightComponent::LightComponent(owner),
    color(1.f, 1.f, 1.f),
    direction(0.f, 0.f, 0.f),
    intensity(1.f)
{
}

DirectionalLightComponent::~DirectionalLightComponent()
{
    while (affectedMeshRenderers.size())
    {
        affectedMeshRenderers[0]->RemoveLight(this);
    }

}
