#ifndef COMPILER_STATEMENT_H
#define COMPILER_STATEMENT_H

#include "../../../Typing/Type.h"
#include "../Node.h"

namespace Ceres::AST {
class Statement : public Node {
public:
    Type* type = nullptr;

    explicit Statement(SourceSpan const& sourceSpan);

    virtual ~Statement() = default;
};

} // namespace Ceres::AST

#endif // COMPILER_STATEMENT_H
