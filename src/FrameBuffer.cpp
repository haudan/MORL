#include "FrameBuffer.hpp"

namespace MORL {
  FrameBuffer::FrameBuffer(int width, int height)
    : mWidth(width), mHeight(height),
      mBuffer(new CharT[width * height])
  {}

  FrameBuffer::FrameBuffer(FrameBuffer &&other)
    : mWidth(other.mWidth), mHeight(other.mHeight),
      mBuffer(std::move(other.mBuffer))
  {}

  void FrameBuffer::Clear(FrameBuffer::CharT chr) {
    for(int i = 0; i < mWidth * mHeight; ++i) {
      mBuffer[i] = chr;
    }
  }
}