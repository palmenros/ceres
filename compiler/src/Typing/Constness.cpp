#include "Constness.h"

namespace Ceres::Typing {

Constness::Constness(Constness::Kind kind)
    : kind(kind)
{
}

Constness::Constness()
    : kind(Invalid)
{
}

}