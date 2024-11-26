//===- event.hpp ---------------------------------------------------===//
//
// Copyright (c) 2024 Junjie Shen
//
// see https://github.com/shenjunjiekoda/hunter/blob/main/LICENSE for
// license information.
//
//===------------------------------------------------------------------===//
//
//  This header defines the event concept and utility.
//
//===------------------------------------------------------------------===//

#pragma once

#include <concepts>

#include "analyzer/core/event/events.hpp"

namespace hunter {

template < typename T >
concept event = requires {
    { T::get_kind() } -> std::same_as< dfa::EventKind >;
}; // concept event

template < typename T >
struct is_event : std::bool_constant< event< T > > { // NOLINT
}; // struct is_event

} // namespace hunter