#include "InitCeres.h"
#include "log.hpp"

namespace Ceres {
InitCeres::InitCeres() { Log::setupLogging(); }

InitCeres::~InitCeres()
{
    // Add destruction here
}
} // namespace Ceres