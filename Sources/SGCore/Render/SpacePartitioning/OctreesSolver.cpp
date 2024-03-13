//
// Created by ilya on 10.03.24.
//

#include "OctreesSolver.h"

#include "SGCore/Scene/Scene.h"
#include "Octree.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/Transformations/TransformationsUpdater.h"
#include "IgnoreOctrees.h"
#include "SGCore/Render/Camera3D.h"
#include "ObjectsCullingOctree.h"
#include "OctreeCullableInfo.h"

SGCore::OctreesSolver::OctreesSolver()
{
    startThread();
}

void SGCore::OctreesSolver::setScene(const SGCore::Ref<SGCore::Scene>& scene) noexcept
{
    m_scene = scene;
    
    auto transformationsUpdater = scene->getSystem<TransformationsUpdater>();
    if(transformationsUpdater)
    {
        (*transformationsUpdater->m_transformChangedEvent) += m_transformChangedListener;
    }
}

void SGCore::OctreesSolver::fixedUpdate(const double& dt, const double& fixedDt) noexcept
{
    auto lockedScene = m_scene.lock();
    
    if(!lockedScene) return;
    
    auto& registry = lockedScene->getECSRegistry();
    
    auto octreesView = registry.view<Ref<Octree>>();
    
    octreesView.each([this, &registry](const entt::entity& entity, Ref<Octree> octree) {
        // octree->clearNodesBranchEntities(octree->m_root);
        if(octree->m_root->m_isSubdivided)
        {
            for(const auto& p : m_changedTransforms)
            {
                if(registry.all_of<IgnoreOctrees>(p.first))
                {
                    continue;
                }
                
                auto* tmpCullableInfo = registry.try_get<Ref<OctreeCullableInfo>>(p.first);
                auto cullableInfo = (tmpCullableInfo ? *tmpCullableInfo : nullptr);
                
                if(cullableInfo)
                {
                    /*auto lockedParentNode = cullableInfo->m_parentNode.lock();
                    if(lockedParentNode)
                    {
                        lockedParentNode->m_overlappedEntities.erase(p.first);
                    }
                    auto foundNode = octree->getOverlappingNode(p.second->m_ownTransform.m_aabb);
                    cullableInfo->m_parentNode = foundNode;
                    if(foundNode)
                    {
                        foundNode->m_overlappedEntities.insert(p.first);
                    }*/
                    auto lockedParentNode = cullableInfo->m_parentNode.lock();
                    if(lockedParentNode)
                    {
                        lockedParentNode->m_overlappedEntities.erase(p.first);
                    }
                    auto foundNode =  octree->subdivideWhileOverlap(p.first, p.second->m_ownTransform.m_aabb, octree->m_root);
                    cullableInfo->m_parentNode = foundNode;
                    if(foundNode)
                    {
                        foundNode->m_overlappedEntities.insert(p.first);
                    }
                }
            }
            
            m_changedTransforms.clear();
        }
        else // check all transformations
        {
            auto transformationsView = registry.view<Ref<Transform>>();
            transformationsView.each([&octree, &registry](const entt::entity& transformEntity, Ref<Transform> transform) {
                if(!registry.all_of<IgnoreOctrees>(transformEntity))
                {
                    auto* tmpCullableInfo = registry.try_get<Ref<OctreeCullableInfo>>(transformEntity);
                    auto cullableInfo = (tmpCullableInfo ? *tmpCullableInfo : nullptr);
                    
                    if(cullableInfo)
                    {
                        /*auto lockedParentNode = cullableInfo->m_parentNode.lock();
                        if(lockedParentNode)
                        {
                            lockedParentNode->m_overlappedEntities.erase(transformEntity);
                        }
                        auto foundNode = octree->getOverlappingNode(transform->m_ownTransform.m_aabb);
                        cullableInfo->m_parentNode = foundNode;
                        if(foundNode)
                        {
                            foundNode->m_overlappedEntities.insert(transformEntity);
                        }
                        */
                        auto lockedParentNode = cullableInfo->m_parentNode.lock();
                        if(lockedParentNode)
                        {
                            lockedParentNode->m_overlappedEntities.erase(transformEntity);
                        }
                        auto foundNode =  octree->subdivideWhileOverlap(transformEntity, transform->m_ownTransform.m_aabb, octree->m_root);
                        cullableInfo->m_parentNode = foundNode;
                        if(foundNode)
                        {
                            foundNode->m_overlappedEntities.insert(transformEntity);
                        }
                        
                        /*octree->subdivideWhileOverlap(transformEntity, transform->m_ownTransform.m_aabb,
                                                      octree->m_root);*/
                    }
                }
            });
            
            m_changedTransforms.clear();
        }
    });
    
    IParallelSystem::fixedUpdate(dt, fixedDt);
}

void SGCore::OctreesSolver::onTransformChanged(const entt::entity& entity, const SGCore::Ref<const SGCore::Transform>& transform) noexcept
{
    m_changedTransforms.emplace_back(entity, transform);
}
