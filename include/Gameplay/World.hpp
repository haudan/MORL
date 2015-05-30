#pragma once

#include "Gameplay/WorldEntity.hpp"
#include "Util.hpp"

#include <unordered_map>
#include <algorithm>

namespace MORL {
  class FrameBuffer;

  namespace Gameplay {
    class World {
    public:
      using OwnedWorldEntity = std::unique_ptr<WorldEntity>;
      using WorldEntityId = uint32_t;
      using WorldEntityContainer = std::unordered_map<WorldEntityId, OwnedWorldEntity>;

      World() = default;
      World(World const &other) = default;
      World(World && other);
      ~World() = default;

      /**
       * Add a new entity to the world
       * @return WorldEntityId The id for the new entity
       */
      template <typename E, typename ...Args>
      inline WorldEntityId AddEntity(Args && ... args) {
        auto owned = MakeUnique<E>(std::forward<Args>(args)...);
        mEntities.emplace(mCurEntityId, std::move(owned));
        return mCurEntityId++;
      }

      /**
       * Remove the entity from the world
       * @param WorldEntity* entity The world to remove
       */
      void RemoveEntity(WorldEntityId entityId);

      /**
       * Return the entity with the specified id
       * @param WorldEntityId entityId The entity id
       * @return WorldEntity* A pointer to the entity, nullptr if no entity
       * with the specified id was found
       */
      WorldEntity *GetEntity(WorldEntityId entityId) const;

      /**
       * Update the world
       */
      void Update();

      /**
       * Draw the world to the framebuffer
       */
      void Draw(FrameBuffer &frameBuffer) const;
    private:
      WorldEntityId mCurEntityId = 0;
      WorldEntityContainer mEntities;
    };
  }
}