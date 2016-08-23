#pragma once

#include "std/iostream.hpp"
#include "std/type_traits.hpp"

namespace my
{
namespace impl
{
template <typename From, typename To>
using IsConvertibleGuard = typename enable_if<is_convertible<From, To>::value>::type *;
}  // namespace impl

/// Creates a typesafe alias to a given numeric Type.
template <typename Type, typename Tag>
class NewType
{
  static_assert(is_integral<Type>::value || is_floating_point<Type>::value,
                "NewType can be used only with integral and floating point type.");

public:
  using RepType = Type;

  template <typename V, impl::IsConvertibleGuard<V, Type> = nullptr>
  constexpr explicit NewType(V const & v) : m_value(v)
  {
  }

  constexpr NewType() = default;

  template <typename V, impl::IsConvertibleGuard<V, Type> = nullptr>
  NewType & Set(V const & v)
  {
    m_value = static_cast<Type>(v);
    return *this;
  }

  Type const & Get() const { return m_value; }
  Type & Get() { return m_value; }

  NewType & operator=(NewType const & v)
  {
    m_value = v.m_value;
    return *this;
  }

  NewType & operator++()
  {
    ++m_value;
    return *this;
  }

  NewType const operator++(int)
  {
    auto const copy = *this;
    ++m_value;
    return copy;
  }

  NewType & operator--()
  {
    --m_value;
    return *this;
  }

  NewType const operator--(int)
  {
    auto const copy = *this;
    --m_value;
    return copy;
  }

  NewType & operator+=(NewType const & v)
  {
    m_value += v.m_value;
    return *this;
  }

  NewType & operator-=(NewType const & v)
  {
    m_value -= v.m_value;
    return *this;
  }

  NewType & operator*=(NewType const & v)
  {
    m_value *= v.m_value;
    return *this;
  }

  NewType & operator/=(NewType const & v)
  {
    m_value /= v.m_value;
    return *this;
  }

  NewType & operator%=(NewType const & v)
  {
    m_value %= v.m_value;
    return *this;
  }

  NewType & operator^=(NewType const & v)
  {
    m_value ^= v.m_value;
    return *this;
  }

  NewType & operator|=(NewType const & v)
  {
    m_value |= v.m_value;
    return *this;
  }

  NewType & operator&=(NewType const & v)
  {
    m_value &= v.m_value;
    return *this;
  }

  // TODO(mgsergio): Is it meaningful for a newtype to have << operator ?
  // NewType & operator<<=(NewType<V, VTag> const & v)
  // NewType & operator>>=(NewType<V, VTag> const & v)

  bool operator==(NewType const & o) const { return m_value == o.m_value; }
  bool operator!=(NewType const & o) const { return !(m_value == o.m_value); }
  bool operator<(NewType const & o) const { return m_value < o.m_value; }
  bool operator>(NewType const & o) const { return m_value > o.m_value; }
  bool operator<=(NewType const & o) const { return !(m_value > o.m_value); }
  bool operator>=(NewType const & o) const { return !(m_value < o.m_value); }
  NewType operator+(NewType const & o) const { return NewType(m_value + o.m_value); }
  NewType operator-(NewType const & o) const { return NewType(m_value - o.m_value); }
  NewType operator*(NewType const & o) const { return NewType(m_value * o.m_value); }
  NewType operator/(NewType const & o) const { return NewType(m_value / o.m_value); }
  NewType operator%(NewType const & o) const { return NewType(m_value % o.m_value); }
  NewType operator^(NewType const & o) const { return NewType(m_value ^ o.m_value); }
  NewType operator|(NewType const & o) const { return NewType(m_value | o.m_value); }
  NewType operator&(NewType const & o) const { return NewType(m_value & o.m_value); }

private:
  Type m_value;
};

namespace newtype_default_output
{
template <typename Type, typename Tag>
string SimpleDebugPrint(NewType<Type, Tag> const & nt)
{
  return DebugPrint(nt.Get());
}
}  // namespace newtype_default_output
}  // namespace my

#define NEWTYPE(REPR, NAME)                     \
  struct NAME ## _tag;                          \
  using NAME = my::NewType<REPR, NAME ## _tag>

#define NEWTYPE_SIMPLE_OUTPUT(NAME)                                     \
  inline string DebugPrint(NAME const & nt)                             \
  {                                                                     \
    return my::newtype_default_output::SimpleDebugPrint(nt);            \
  }                                                                     \
  inline ostream & operator<<(ostream & ost, NAME const & nt)           \
  {                                                                     \
    return ost << my::newtype_default_output::SimpleDebugPrint(nt);     \
  }
