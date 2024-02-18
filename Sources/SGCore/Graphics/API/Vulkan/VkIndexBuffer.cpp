//
// Created by stuka on 07.07.2023.
//

#include "VkIndexBuffer.h"

SGCore::VkIndexBuffer::~VkIndexBuffer() noexcept
{
    destroy();
}

std::shared_ptr<SGCore::IIndexBuffer> SGCore::VkIndexBuffer::create() noexcept
{
    return shared_from_this();
}

void SGCore::VkIndexBuffer::destroy() noexcept
{

}

std::shared_ptr<SGCore::IIndexBuffer> SGCore::VkIndexBuffer::putData(std::vector<std::uint32_t> data) noexcept
{
    return shared_from_this();
}

void SGCore::VkIndexBuffer::subData
(std::vector<std::uint32_t> data, const int& offset) noexcept
{

}

std::shared_ptr<SGCore::IIndexBuffer> SGCore::VkIndexBuffer::bind() noexcept
{
    return shared_from_this();
}

std::shared_ptr<SGCore::IIndexBuffer> SGCore::VkIndexBuffer::setUsage(SGGUsage) noexcept
{
    return shared_from_this();
}
