#pragma once
#include <memory>

namespace cycfi { namespace elements {
class element;
using element_ptr = std::shared_ptr<element>;
template <typename T> struct receiver;
template <typename T> struct sender;
}}
