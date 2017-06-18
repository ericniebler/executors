#ifndef STD_EXPERIMENTAL_BITS_REQUIRE_H
#define STD_EXPERIMENTAL_BITS_REQUIRE_H

#include <experimental/bits/has_require_member.h>
#include <experimental/bits/is_twoway_executor.h>
#include <experimental/bits/is_oneway_executor.h>
#include <experimental/bits/is_bulk_oneway_executor.h>
#include <experimental/bits/is_bulk_twoway_executor.h>
#include <utility>

namespace std {
namespace experimental {
inline namespace concurrency_v2 {
namespace execution {

struct oneway_t;
struct twoway_t;
struct single_t;
struct bulk_t;
struct always_blocking_t;
struct possibly_blocking_t;
struct is_continuation_t;
struct is_not_continuation_t;
struct is_work_t;
struct is_not_work_t;

namespace require_impl {

template<class Executor, class... Args>
constexpr auto require(Executor&& ex, Args&&... args)
  -> decltype(std::forward<Executor>(ex).require(std::forward<Args>(args)...))
{
  return std::forward<Executor>(ex).require(std::forward<Args>(args)...);
}

// Forward declare the default adaptations.
template<class Executor>
  constexpr typename std::enable_if<
    (is_oneway_executor<Executor>::value || is_bulk_oneway_executor<Executor>::value)
    && !has_require_member<Executor, oneway_t>::value, Executor>::type
      require(Executor ex, oneway_t);
template<class Executor> class twoway_adapter;
template<class Executor>
  typename std::enable_if<
    (is_oneway_executor<Executor>::value || is_bulk_oneway_executor<Executor>::value)
    && !(is_twoway_executor<Executor>::value || is_bulk_twoway_executor<Executor>::value)
    && !has_require_member<Executor, twoway_t>::value, twoway_adapter<Executor>>::type
      require(Executor ex, twoway_t);
template<class Executor> class bulk_adapter;
template<class Executor>
  constexpr typename std::enable_if<
    (is_twoway_executor<Executor>::value || is_bulk_twoway_executor<Executor>::value)
    && !has_require_member<Executor, twoway_t>::value, Executor>::type
      require(Executor ex, twoway_t);
template<class Executor>
  typename std::enable_if<is_oneway_executor<Executor>::value
    && !(is_bulk_oneway_executor<Executor>::value || is_bulk_twoway_executor<Executor>::value)
    && !has_require_member<Executor, bulk_t>::value, bulk_adapter<Executor>>::type
      require(Executor ex, bulk_t);
template<class Executor>
  constexpr typename std::enable_if<
    (is_bulk_oneway_executor<Executor>::value || is_bulk_twoway_executor<Executor>::value)
    && !has_require_member<Executor, bulk_t>::value, Executor>::type
      require(Executor ex, bulk_t);
template<class Executor> class always_blocking_adapter;
template<class Executor>
  constexpr typename std::enable_if<!has_require_member<Executor, always_blocking_t>::value,
    always_blocking_adapter<Executor>>::type
      require(Executor ex, always_blocking_t);
template<class Executor>
  constexpr typename std::enable_if<!has_require_member<Executor, possibly_blocking_t>::value, Executor>::type
    require(Executor ex, possibly_blocking_t);
template<class Executor>
  constexpr typename std::enable_if<!has_require_member<Executor, is_continuation_t>::value, Executor>::type
    require(Executor ex, is_continuation_t);
template<class Executor>
  constexpr typename std::enable_if<!has_require_member<Executor, is_not_continuation_t>::value, Executor>::type
    require(Executor ex, is_not_continuation_t);
template<class Executor>
  constexpr typename std::enable_if<!has_require_member<Executor, is_work_t>::value, Executor>::type
    require(Executor ex, is_work_t);
template<class Executor>
  constexpr typename std::enable_if<!has_require_member<Executor, is_not_work_t>::value, Executor>::type
    require(Executor ex, is_not_work_t);
template<class Executor, class ProtoAllocator>
  constexpr typename std::enable_if<!has_require_member<Executor, std::allocator_arg_t, ProtoAllocator>::value, Executor>::type
    require(Executor ex, std::allocator_arg_t, const ProtoAllocator&);

struct require_fn
{
  template<class Executor, class... Args>
  constexpr auto operator()(Executor&& ex, Args&&... args) const
    noexcept(noexcept(require(std::forward<Executor>(ex), std::forward<Args>(args)...)))
    -> decltype(require(std::forward<Executor>(ex), std::forward<Args>(args)...))
  {
    return require(std::forward<Executor>(ex), std::forward<Args>(args)...);
  }
};

template<class T = require_fn> constexpr T customization_point{};

} // namespace require_impl
} // namespace execution
} // inline namespace concurrency_v2
} // namespace experimental
} // namespace std

#endif // STD_EXPERIMENTAL_BITS_REQUIRE_H