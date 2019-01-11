#ifndef LLARP_ROUTER_ID_HPP
#define LLARP_ROUTER_ID_HPP

#include <util/aligned.hpp>

namespace llarp
{
  struct RouterID : public AlignedBuffer< 32 >
  {
    static constexpr size_t SIZE = 32;

    using Data = std::array< byte_t, SIZE >;

    RouterID() : AlignedBuffer< SIZE >()
    {
    }

    RouterID(const byte_t* buf) : AlignedBuffer< SIZE >(buf)
    {
    }

    RouterID(const Data& data) : AlignedBuffer< SIZE >(data)
    {
    }

    std::string
    ToString() const;

    bool
    FromString(const std::string& str);

    RouterID&
    operator=(const byte_t* ptr)
    {
      std::copy(ptr, ptr + SIZE, begin());
      return *this;
    }

    friend std::ostream&
    operator<<(std::ostream& out, const RouterID& id)
    {
      return out << id.ToString();
    }

    using Hash = AlignedBuffer< SIZE >::Hash;
  };
}  // namespace llarp

#endif
