#include "TypeVisitor.h"

namespace Ceres::Typing {
void TypeVisitor::visit(Type* type) { type->accept(*this); }
} // namespace Ceres::Typing