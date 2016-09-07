/*
Copyright 2016 Barrett Adair

Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
*/

#ifndef POLYVAR_HPP
#define POLYVAR_HPP

#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <type_traits>

#define POLYVAR_USE_BOOST
//#define POLYVAR_USE_MAPBOX

#if defined POLYVAR_USE_BOOST
#  include <boost/variant.hpp>
#  define POLYVAR_VARIANT_IMPL boost::variant
#  define POLYVAR_USING \
        using POLYVAR_VARIANT_IMPL<Ts...>::variant; \
        using POLYVAR_VARIANT_IMPL<Ts...>::operator=; \
        using POLYVAR_VARIANT_IMPL<Ts...>::which; \
        using POLYVAR_VARIANT_IMPL<Ts...>::empty; \
        using POLYVAR_VARIANT_IMPL<Ts...>::type; \
/**/
#  define POLYVAR_VISIT_IMPL boost::apply_visitor
#elif defined POLYVAR_USE_MAPBOX
// include it your damn self
#  define POLYVAR_VARIANT_IMPL mapbox::util::variant
#  define POLYVAR_VISIT_IMPL mapbox::util::apply_visitor
#  define POLYVAR_USING \
        using POLYVAR_VARIANT_IMPL<Ts...>::variant; \
        using POLYVAR_VARIANT_IMPL<Ts...>::operator=; \
        using POLYVAR_VARIANT_IMPL<Ts...>::which; \
        using POLYVAR_VARIANT_IMPL<Ts...>::empty; \
        using POLYVAR_VARIANT_IMPL<Ts...>::type; \
#else
#  include <variant>
#  define POLYVAR_VARIANT_IMPL std::variant
#  define POLYVAR_VISIT_IMPL std::visit
// TODO - check these
#  define POLYVAR_USING \
        using POLYVAR_VARIANT_IMPL<Ts...>::variant; \
        using POLYVAR_VARIANT_IMPL<Ts...>::operator=; \
        using POLYVAR_VARIANT_IMPL<Ts...>::which; \
        using POLYVAR_VARIANT_IMPL<Ts...>::empty; \
        using POLYVAR_VARIANT_IMPL<Ts...>::type; \
/**/
#endif

#define DEFINE_POLYVAR(name, ...) \
template<typename... Ts> \
struct name : public POLYVAR_VARIANT_IMPL<Ts...> { \
\
    using base = POLYVAR_VARIANT_IMPL<Ts...>; \
    POLYVAR_USING \
\
    BOOST_PP_REPEAT(BOOST_PP_SEQ_SIZE(__VA_ARGS__), POLYVAR_MEMFNS, __VA_ARGS__) \
} \
/**/

#define POLYVAR_MEMFNS(z, i, seq) POLYVAR_MEMFNS_I(BOOST_PP_SEQ_ELEM(i,seq))

#define POLYVAR_MEMFNS_I(name)                                 \
    template<typename... Args>                                 \
    constexpr decltype(auto) name(Args&&... args) {            \
        return POLYVAR_VISIT_IMPL([&](auto& v){                \
            return v.name(                                     \
                static_cast<Args&&>(args)...);}, *this);       \
    }                                                          \
/**/

#endif // #ifndef POLYVAR_HPP
