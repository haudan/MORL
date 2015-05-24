#pragma once

#include <memory>

namespace MORL {
  class FrameBuffer {
  public:
    using CharT = char;

    FrameBuffer() = delete;
    FrameBuffer(int width, int height);
    FrameBuffer(FrameBuffer const &other) = default;
    FrameBuffer(FrameBuffer && other);
    ~FrameBuffer() = default;

    inline int Width() const { return mWidth; }
    inline int Height() const { return mHeight; }

    /**
     * Is the imaginary character at (x, y) inside the boundaries of the buffer?
     * @param int x X position of the character
     * @param int y Y position of the character
     * @return bool true if in bounds, false otherwise
     */
    inline bool InBounds(int x, int y) {
      return (x >= 0 && x < mWidth)
          && (y >= 0 && y < mHeight);
    }

    /**
     * Get the character at (x, y)
     *
     * Does not perform any bounds checks
     * @param int x X position
     * @param int y Y position
     * @return CharT The character at the position (x, y)
     */
    inline CharT CharAt(int x, int y) {
      return mBuffer[mWidth * y + x];
    }

    /**
     * Set character at (x, y)
     *
     * Does not perform any bounds checks
     * @param int x X position
     * @param int y Y position
     * @param CharT chr The character to write into the buffer at (x, y)
     */
    inline void CharAt(int x, int y, CharT chr) {
      mBuffer[mWidth * y + x] = chr;
    }

    /**
     * Set the entire buffer to the specific character
     * @param CharT chr What to set the buffer to
     */
    void Clear(CharT chr);
  private:
    int const mWidth = 0;
    int const mHeight = 0;
    std::unique_ptr<CharT[]> mBuffer;
  };
}