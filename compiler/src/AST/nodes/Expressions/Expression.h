#ifndef COMPILER_EXPRESSION_H
#define COMPILER_EXPRESSION_H

#include "../../../Typing/Type.h"
#include "../Node.h"

namespace Ceres::AST {
class Expression : public Node {
public:
    Type* type;

    explicit Expression(SourceSpan const& sourceSpan);

    explicit Expression(SourceSpan const& sourceSpan, Type* type);

    virtual ~Expression() = default;
};

} // namespace Ceres::AST

#endif // COMPILER_EXPRESSION_H
