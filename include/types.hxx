#include <cstddef>
#include <list>

#ifndef TYPES_HXX
#define TYPES_HXX

using size_type = std::size_t;
using const_iterator = std::list<Package>::const_iterator;
using ElementID = int;
using TimeOffset = int;
using Time = int;
using ProbabilityGenerator = double;

#endif // TYPES_HXX