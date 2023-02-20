#ifndef COMPILER_NODE_H
#define COMPILER_NODE_H

#include "../../utils/SourceSpan.h"
#include <vector>

namespace Ceres::AST {

class AbstractASTVisitor;

class Node {
public:
    SourceSpan sourceSpan;

public:
    explicit Node(SourceSpan const& sourceSpan);

    virtual void accept(AbstractASTVisitor& visitor) = 0;
    virtual std::vector<Node*> getChildren() const = 0;

    virtual ~Node() = default;
};

} // namespace Ceres::AST
#endif // COMPILER_NODE_H
