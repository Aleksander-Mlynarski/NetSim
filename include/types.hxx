#ifndef NETSIM_TYPES_HXX
#define NETSIM_TYPES_HXX

#include <functional>

using ElementID = unsigned;
using TimeOffset = unsigned;
using Time = unsigned;

using Time = unsigned;

using TimeOffset = Time;

using ProbabilityGenerator = std::function<double()>;

#endif //NETSIM_TYPES_HXX