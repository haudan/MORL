#include "Gameplay/World.hpp"

#include "FrameBuffer.hpp"

namespace MORL {
  namespace Gameplay {
    void World::RemoveEntity(WorldEntityId entityId) {
      auto iter = mEntities.find(entityId);
      if(iter != mEntities.end()) {
        mEntities.erase(iter);
      }
    }

    WorldEntity *World::GetEntity(WorldEntityId entityId) const {
      auto iter = mEntities.find(entityId);
      if(iter == mEntities.end()) {
        return nullptr;
      }

      return iter->second.get();
    }

    void World::Update() {
      for(auto &entity : mEntities) {
        entity.second->Update();
      }
    }

    void World::Draw(FrameBuffer &frameBuffer) const {
      for(auto const &entity : mEntities) {
        entity.second->Draw(frameBuffer);
      }
    }
  }
}