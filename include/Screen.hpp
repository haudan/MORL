#pragma once

#include "ScreenState.hpp"

#include <memory>
#include <stack>

namespace MORL {
  /**
   * Used for initializing and finalizing ncurses
   */
  class Screen {
  public:
    using OwnedScreenState = std::unique_ptr<ScreenState>;
    using ScreenStateContainer = std::stack<OwnedScreenState>;

    Screen(OwnedScreenState && initialState);
    Screen(Screen const & other) = default;
    Screen(Screen && other);
    ~Screen();

    /**
     * Get the current screen state
     */
    inline ScreenState &CurrentScreenState() const {
      return *(mScreenStates.top().get());
    }

    /**
     * Open a new screen, making it the current one
     */
    void GotoScreen(OwnedScreenState && screenState);

    /**
     * Go back to the last open screen
     */
    bool GoBack();

    /**
     * The update logic for the screen
     */
    void Update();

    /**
     * Refresh the screen
     */
    void Refresh();
  private:
    void InitCurse();
    void FinaCurse();
  private:
    ScreenStateContainer mScreenStates;
    int mScreenWidth = 0;
    int mScreenHeight = 0;
  };
}