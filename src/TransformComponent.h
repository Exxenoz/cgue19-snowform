#pragma once

#include "Component.h"

#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\quaternion.hpp>
#include <vector>

class TransformComponent : public Component
{
public:
    TransformComponent(GameObject* owner);
    virtual ~TransformComponent();

    virtual void LateUpdate();

    void SetParent(TransformComponent* transform);
    void AddChild(TransformComponent* child);
    void RemoveChild(TransformComponent* child);

    void SetLocalPositionX(float localPositionX);
    void SetLocalPositionY(float localPositionY);
    void SetLocalPositionZ(float localPositionZ);
    void SetLocalPosition(float localPositionX, float localPositionY, float localPositionZ)
    {
        SetLocalPositionX(localPositionX);
        SetLocalPositionY(localPositionY);
        SetLocalPositionZ(localPositionZ);
    }
    void SetLocalPosition(glm::vec3 localPosition)
    {
        SetLocalPositionX(localPosition.x);
        SetLocalPositionY(localPosition.y);
        SetLocalPositionZ(localPosition.z);
    }

    void SetLocalRotationX(float localRotationX);
    void SetLocalRotationY(float localRotationY);
    void SetLocalRotationZ(float localRotationZ);
    void SetLocalRotation(float localRotationX, float localRotationY, float localRotationZ)
    {
        SetLocalRotationX(localRotationX);
        SetLocalRotationY(localRotationY);
        SetLocalRotationZ(localRotationZ);
    }

    void SetLocalScaleX(float localScaleX);
    void SetLocalScaleY(float localScaleY);
    void SetLocalScaleZ(float localScaleZ);
    void SetLocalScale(float localScaleX, float localScaleY, float localScaleZ)
    {
        SetLocalScaleX(localScaleX);
        SetLocalScaleY(localScaleY);
        SetLocalScaleZ(localScaleZ);
    }

    void SetIgnoreParentRotation(bool _ignoreParentRotation) { ignoreParentRotation = _ignoreParentRotation; }

    TransformComponent* GetParent() const { return parent; }
    TransformComponent* GetChild(uint32_t index) { return index < children.size() ? children[index] : nullptr; }
    uint32_t GetChildCount() const { return children.size(); }

    glm::mat4 const& GetModelMatrix() const { return modelMatrix; }

    float GetLocalPositionX() const { return localPosition.x; }
    float GetLocalPositionY() const { return localPosition.y; }
    float GetLocalPositionZ() const { return localPosition.z; }
    glm::vec3 GetLocalPosition() const { return localPosition; }

    float GetLocalRotationX() const { return localRotation.x; }
    float GetLocalRotationY() const { return localRotation.y; }
    float GetLocalRotationZ() const { return localRotation.z; }

    float GetLocalScaleX() const { return localScale.x; }
    float GetLocalScaleY() const { return localScale.y; }
    float GetLocalScaleZ() const { return localScale.z; }

    float GetPositionX() const { return position.x; }
    float GetPositionY() const { return position.y; }
    float GetPositionZ() const { return position.z; }
    glm::vec3 GetPosition() const { return position; }

    float GetRotationX() const { return rotation.x; }
    float GetRotationY() const { return rotation.y; }
    float GetRotationZ() const { return rotation.z; }
    glm::quat GetRotationQ() const { return rotationQ; }

    float GetScaleX() const { return scale.x; }
    float GetScaleY() const { return scale.y; }
    float GetScaleZ() const { return scale.z; }
    glm::vec3 GetScale() const { return scale; }

    glm::vec3 GetDirectionRight() const { return directionRight; }
    glm::vec3 GetDirectionUp() const { return directionUp; }
    glm::vec3 GetDirectionBackward() const { return directionBackward; }

private:
    TransformComponent* parent;
    std::vector<TransformComponent*> children;

    glm::vec3 localPosition;
    glm::vec3 localRotation;
    glm::quat localRotationQ;
    glm::vec3 localScale;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::quat rotationQ;
    glm::vec3 scale;

    glm::vec3 directionRight;
    glm::vec3 directionUp;
    glm::vec3 directionBackward;

    glm::mat4 modelMatrix;

    bool ignoreParentPosition;
    bool ignoreParentRotation;
    bool ignoreParentScale;
};
