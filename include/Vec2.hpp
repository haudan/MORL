#pragma once

#include <cmath>

template <typename S>
class Vec2 {
public:
  using Scalar = S;

  Vec2() = default;

  Vec2(Scalar a)
    : mX(a), mY(a)
  {}

  Vec2(Scalar x, Scalar y)
  : mX(x), mY(y)
  {}

  Vec2(Vec2 const &other)
    : mX(other.mX),
      mY(other.mY)
  {}

  ~Vec2() = default;

  inline Scalar X() const { return mX; }
  inline void X(Scalar x) { mX = x; }
  inline Scalar Y() const { return mY; }
  inline void Y(Scalar y) { mY = y; }

  Scalar MagnitudeSquared() const {
    return (mX * mX) + (mY * mY);
  }

  Scalar Magnitude() const {
    return std::sqrt(MagnitudeSquared());
  }

  void Normalize() {
    auto magn = Magnitude();

    // One over magnitude allows for 2 x multiplication, instead of 2 x division
    if(magn != 0) {
      magn = Scalar(1.0) / magn;
      mX *= magn;
      mY *= magn;
    }
  }

  inline Vec2 Normalized() const {
    Vec2 n{*this};
    n.Normalize();
    return n;
  }

  inline Vec2 &operator += (Vec2 const &rhs) {
    (*this) = (*this) + rhs;
    return *this;
  }

  inline Vec2 &operator -= (Vec2 const &rhs) {
    (*this) = (*this) - rhs;
    return *this;
  }

  inline Vec2 &operator *= (Vec2 const &rhs) {
    (*this) = (*this) * rhs;
    return *this;
  }

  inline Vec2 &operator /= (Vec2 const &rhs) {
    (*this) = (*this) / rhs;
    return *this;
  }



  inline Vec2 &operator += (Scalar a) {
    (*this) = (*this) + a;
    return *this;
  }

  inline Vec2 &operator -= (Scalar a) {
    (*this) = (*this) - a;
    return *this;
  }

  inline Vec2 &operator *= (Scalar a) {
    (*this) = (*this) * a;
    return *this;
  }

  inline Vec2 &operator /= (Scalar a) {
    (*this) = (*this) / a;
    return *this;
  }
private:
  Scalar mX = 0, mY = 0;
};


using Vec2i = Vec2<int>;
using Vec2f = Vec2<float>;
using Vec2d = Vec2<double>;



template <typename S>
Vec2<S> operator + (Vec2<S> const &lhs, Vec2<S> const &rhs) {
  return Vec2<S>{lhs.X() + rhs.X(), lhs.Y() + rhs.Y()};
}

template <typename S>
Vec2<S> operator - (Vec2<S> const &lhs, Vec2<S> const &rhs) {
  return Vec2<S>{lhs.X() - rhs.X(), lhs.Y() - rhs.Y()};
}

template <typename S>
Vec2<S> operator * (Vec2<S> const &lhs, Vec2<S> const &rhs) {
  return Vec2<S>{lhs.X() * rhs.X(), lhs.Y() * rhs.Y()};
}

template <typename S>
Vec2<S> operator / (Vec2<S> const &lhs, Vec2<S> const &rhs) {
  return Vec2<S>{lhs.X() / rhs.X(), lhs.Y() / rhs.Y()};
}



template <typename S>
Vec2<S> operator + (Vec2<S> const &lhs, S a) {
  return Vec2<S>{lhs.X() + a, lhs.Y() + a};
}

template <typename S>
Vec2<S> operator - (Vec2<S> const &lhs, S a) {
  return Vec2<S>{lhs.X() - a, lhs.Y() - a};
}

template <typename S>
Vec2<S> operator * (Vec2<S> const &lhs, S a) {
  return Vec2<S>{lhs.X() * a, lhs.Y() * a};
}

template <typename S>
Vec2<S> operator / (Vec2<S> const &lhs, S a) {
  return Vec2<S>{lhs.X() / a, lhs.Y() / a};
}
