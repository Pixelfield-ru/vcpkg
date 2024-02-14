//
// Created by stuka on 05.02.2024.
//

#ifndef SUNGEARENGINE_ILIGHTPASS_H
#define SUNGEARENGINE_ILIGHTPASS_H

#include "IRenderPass.h"

namespace SGCore
{
    struct ILightPass : public IRenderPass
    {
        Ref<IUniformBuffer> m_lightsUniformBuffer;
        
        int m_maxLightsCount = 5;
    };
}

#endif //SUNGEARENGINE_ILIGHTPASS_H
