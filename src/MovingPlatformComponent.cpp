#include "MovingPlatformComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "Collision.h"
#include "PlayerComponent.h"
#include "RigidDynamicComponent.h"
#include "Time.h"

MovingPlatformComponent::MovingPlatformComponent(GameObject* owner) :
    Component::Component(owner),
    transform(owner->GetComponent<TransformComponent>()),
    passenger(nullptr),
    startLocalPosition(0.f, 0.f, 0.f),
    moveDirection(0.f, 0.f, 0.f),
    length(0.f),
    time(0.f),
    timer(0.f),
    progress(0.f),
    forward(true)
{
}

void MovingPlatformComponent::OnStart()
{
    startLocalPosition = transform->GetLocalPosition();
}

void MovingPlatformComponent::Update()
{
    glm::vec3 oldLocalPlatformPosition = transform->GetLocalPosition();

    if (length != 0.f && time > 0.f)
    {
        timer += sTime.GetDeltaTime();

        if (timer >= time)
        {
            timer = 0.f;

            if (forward)
            {
                progress = 1.f;
            }
            else
            {
                progress = 0.f;
            }

            forward = !forward;
        }
        else
        {
            if (forward)
            {
                progress = timer / time;
            }
            else
            {
                progress = 1.f - timer / time;
            }
        }

        transform->SetLocalPosition(startLocalPosition + moveDirection * length * progress);
    }

    if (passenger)
    {
        glm::vec3 platformOffset = transform->GetLocalPosition() - oldLocalPlatformPosition;
        passenger->SetLocalPosition(passenger->GetLocalPosition() + platformOffset);
    }
}

void MovingPlatformComponent::OnCollisionEnter(Collision const& collision)
{
    GameObject* colliderGameObject = collision.GetGameObject();

    if (!colliderGameObject)
    {
        return;
    }

    PlayerComponent* player = colliderGameObject->GetComponent<PlayerComponent>();

    if (!player)
    {
        return;
    }

    passenger = colliderGameObject->GetComponent<TransformComponent>();
}

void MovingPlatformComponent::OnCollisionExit(Collision const& collision)
{
    GameObject* colliderGameObject = collision.GetGameObject();

    if (!colliderGameObject)
    {
        return;
    }

    if (colliderGameObject->GetComponent<PlayerComponent>())
    {
        passenger = nullptr;
    }
}
