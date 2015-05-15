#pragma once

namespace MORL {
  class Game;

  /**
   * Basic interface for screen states
   */
  class ScreenState {
  public:
    ScreenState(Game &game);
    virtual ~ScreenState() {}

    /**
     * The screen state's logic
     */
    virtual void Update() = 0;

    /**
     * Draw the screen!
     */
    virtual void Draw() = 0;


    /**
     * Is the screen dirty and does it need a redraw?
     */
    inline bool IsDirty() const {
      return mIsDirty;
    }

    /**
     * Specify that the screen isn't dirty anymore
     */
    inline bool SetClean() {
      mIsDirty = false;
    }

    /**
     * Specify that the screen is dirty
     */
    inline bool SetDirty() {
      mIsDirty = true;
    }
  protected:
    /**
     * Tell the game loop that the screen is dirty and needs a redraw
     */
    inline void RequireRedraw() {
      mIsDirty = true;
    }
  protected:
    Game &mGame;
  private:
    bool mIsDirty = true;
  };
}