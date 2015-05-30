#include "FrameBuffer.hpp"

#include <cctype>

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

  void FrameBuffer::SetCharAt(int x, int y, CharT chr) {
    if(InBounds(x, y)) {
      CharAt(x, y, chr);
    }
  }

  void FrameBuffer::Text(int x, int y, std::string const &text) {
    int xOffs = 0, yOffs = 0;

    for(auto chr : text) {
      if(chr == '\n') {
        ++yOffs;
        xOffs = 0;
      }
      else if(!isspace(chr)) {
        SetCharAt(x + xOffs, y + yOffs, chr);
        ++xOffs;
      }
      else {
        ++xOffs;
      }
    }
  }
}