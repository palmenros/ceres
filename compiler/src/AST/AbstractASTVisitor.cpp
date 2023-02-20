#include "AbstractASTVisitor.h"
#include "../utils/log.hpp"

namespace Ceres::AST {

void AbstractASTVisitor::visit(Node& node) { node.accept(*this); }

void AbstractASTVisitor::visitChildren(Node& node)
{
    for (auto* childPtr : node.getChildren()) {
        ASSERT(childPtr != nullptr);
        visit(*childPtr);
    }
}

} // namespace Ceres::AST