#include <curses.h>

#include "Gameplay/World.hpp"

#include "FrameBuffer.hpp"

namespace MORL {
  namespace Gameplay {
    void World::RemoveEntity(WorldEntity *entity) {
      auto iter = std::find_if(mEntities.begin(), mEntities.end(), [=](auto const &ent) {
        return ent.get() == entity;
      });
      if(iter != mEntities.end()) {
        mEntities.erase(iter);
      }
    }

    void World::Update() {
      for(auto &entity : mEntities) {
        entity->Update();
      }
    }

    void World::Draw(FrameBuffer &frameBuffer) const {
      for(auto const &entity : mEntities) {
        entity->Draw(frameBuffer);
      }
    }
  }
}