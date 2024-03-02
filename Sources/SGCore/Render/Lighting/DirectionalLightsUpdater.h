//
// Created by ilya on 14.02.24.
//

#ifndef SUNGEARENGINE_DIRECTIONALLIGHTSUPDATER_H
#define SUNGEARENGINE_DIRECTIONALLIGHTSUPDATER_H

#include "SGCore/Scene/ISystem.h"

namespace SGCore
{
    class IUniformBuffer;
    
    struct DirectionalLightsUpdater : public ISystem
    {
        DirectionalLightsUpdater() noexcept;
        
        void fixedUpdate(const double& dt, const double& fixedDt) final;
        
        // uniform buffer for directional lights
        Ref<IUniformBuffer> m_uniformBuffer;
        
        int m_maxLightsCount = 5;
    };
}

#endif //SUNGEARENGINE_DIRECTIONALLIGHTSUPDATER_H