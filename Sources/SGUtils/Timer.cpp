//
// Created by stuka on 03.05.2023.
//

#include <chrono>
#include <GLFW/glfw3.h>
#include <numeric>

#include "Timer.h"
#include "TimerCallback.h"

// TODO:  FIX rawDeltaTime between two startFrame
// timer.startFrame();
// some code...
// timer.startFrame();
// timer.getRawDeltaTime() == glfwGetTime() ! NOT CORRECT !
void SGCore::Timer::startFrame()
{
    if(!m_active) return;
    
    if(m_firstTime)
    {
        firstTimeStart();
        m_firstTime = false;
    }
    
    double lastTime = m_currentTime;
    m_currentTime = glfwGetTime();
    
    m_rawDeltaTime = m_currentTime - lastTime;
    
    m_FPSDeltaTimeAccum += m_rawDeltaTime;
    
    m_elapsedTimeForUpdate += m_rawDeltaTime;
    
    if(m_elapsedTimeForUpdate >= m_targetFrameTime)
    {
        for(const auto& callback : m_callbacks)
        {
            callback->callUpdateFunction(m_elapsedTimeForUpdate);
        }
        
        m_elapsedTimeForUpdate = 0.0;
        ++m_framesPerSecondAccum;
    }
    
    if(m_FPSDeltaTimeAccum >= 1.0)
    {
        m_framesPerSecond = m_framesPerSecondAccum;
        m_framesPerSecondAccum = 0;
        m_FPSDeltaTimeAccum = 0.0;
    }
    
    m_elapsedTime += m_rawDeltaTime;
    
    if(m_elapsedTime >= m_targetFrameTime)
    {
        if(m_useFixedUpdateCatchUp)
        {
            // normal value
            /*std::cout << "m_elapsedTime: " << m_elapsedTime << ", m_targetFrameTime: " << m_targetFrameTime
                      << std::endl;*/
            
            while(m_elapsedTime >= m_targetFrameTime)
            {
                if(!m_active) break;
                
                m_lastFixedUpdateCallTime = m_currentFixedUpdateCallTime;
                m_currentFixedUpdateCallTime = glfwGetTime();
                m_fixedUpdateCallDeltaTime = m_currentFixedUpdateCallTime - m_lastFixedUpdateCallTime;
                
                for(const auto& callback : m_callbacks)
                {
                    callback->callFixedUpdateFunction(m_fixedUpdateCallDeltaTime, m_targetFrameTime);
                }
                
                m_elapsedTime -= m_targetFrameTime;
            }
        }
        else
        {
            m_lastFixedUpdateCallTime = m_currentFixedUpdateCallTime;
            m_currentFixedUpdateCallTime = glfwGetTime();
            m_fixedUpdateCallDeltaTime = m_currentFixedUpdateCallTime - m_lastFixedUpdateCallTime;
            
            for(const auto& callback : m_callbacks)
            {
                callback->callFixedUpdateFunction(m_fixedUpdateCallDeltaTime, m_targetFrameTime);
            }
        }
        
        // reset();
        m_elapsedTime = 0;
        m_currentFixedUpdateCallTime = glfwGetTime();
        m_lastFixedUpdateCallTime = m_currentFixedUpdateCallTime;
        
        m_active = m_cyclic;
    }
}

void SGCore::Timer::resetTimer() noexcept
{
    m_elapsedTime = 0;
    m_currentFixedUpdateCallTime = glfwGetTime();
    m_lastFixedUpdateCallTime = m_currentFixedUpdateCallTime;
    m_FPSDeltaTimeAccum = 0;
    m_currentTime = glfwGetTime();
}

void SGCore::Timer::firstTimeStart()
{
    //m_currentTime = m_startTime;
    m_currentTime = glfwGetTime();
    
    for(const std::shared_ptr<TimerCallback>& callback : m_callbacks)
    {
        callback->callStartFunction();
    }
}

void SGCore::Timer::addCallback(std::shared_ptr<TimerCallback> callback)
{
    m_callbacks.push_back(std::move(callback));
}

void SGCore::Timer::removeCallback(const std::shared_ptr<TimerCallback>& callback)
{
    m_callbacks.remove(callback);
}

uint16_t SGCore::Timer::getFramesPerSecond() const noexcept
{
    return m_framesPerSecond;
}

double SGCore::Timer::getRawDeltaTime() const noexcept
{
    return m_rawDeltaTime;
}

double SGCore::Timer::getTargetFrameRate() const noexcept
{
    return m_targetFrameRate;
}

void SGCore::Timer::setTargetFrameRate(const double& frameRate) noexcept
{
    m_targetFrameRate = frameRate;
    m_targetFrameTime = 1.0 / m_targetFrameRate;
}

double SGCore::Timer::getTargetFrameTime() const noexcept
{
    return m_targetFrameTime;
}
