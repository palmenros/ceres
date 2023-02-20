#include "Node.h"

namespace Ceres::AST {

Node::Node(SourceSpan const& sourceSpan)
    : sourceSpan(sourceSpan)
{
}
} // namespace Ceres::AST