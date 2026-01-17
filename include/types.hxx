
#ifndef TYPES_HXX
#define TYPES_HXX

#include <cstddef>
#include <functional>

using size_type = std::size_t;
using ElementID = int;
using TimeOffset = int;
using Time = int;
using ProbabilityGenerator = std::function<double()>;

#endif // TYPES_HXX

