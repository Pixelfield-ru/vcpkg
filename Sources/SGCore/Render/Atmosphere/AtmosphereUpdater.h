//
// Created by ilya on 14.02.24.
//

#ifndef SUNGEARENGINE_ATMOSPHEREUPDATER_H
#define SUNGEARENGINE_ATMOSPHEREUPDATER_H

#include <SGUtils/Timer.h>
#include "SGCore/Scene/ISystem.h"

namespace SGCore
{
    class IUniformBuffer;
    
    struct AtmosphereUpdater : public ISystem
    {
        AtmosphereUpdater() noexcept;
        
        void update(const double& dt, const double& fixedDt) final;
        
        Timer m_atmosphereUpdateTimer;
        
    private:
        void updateAtmosphere() noexcept;
        
        Ref<IUniformBuffer> m_uniformBuffer;
    };
}

#endif //SUNGEARENGINE_ATMOSPHEREUPDATER_H