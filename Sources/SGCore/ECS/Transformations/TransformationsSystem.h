//
// Created by stuka on 02.05.2023.
//

#pragma once

#ifndef NATIVECORE_TRANSFORMATIONSSYSTEM_H
#define NATIVECORE_TRANSFORMATIONSSYSTEM_H

#include "SGCore/ECS/ISystem.h"
#include "SGCore/ECS/Transformations/TransformComponent.h"

namespace Core::ECS
{
    class TransformationsSystem : public ISystem
    {
    public:
        void update(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Core::ECS::Entity>& entity) final;

        void deltaUpdate(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Core::ECS::Entity>& entity, const double& deltaTime) final;
    };
}

#endif //NATIVECORE_TRANSFORMATIONSSYSTEM_H