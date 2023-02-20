
#ifndef COMPILER_VISIBILITY_H
#define COMPILER_VISIBILITY_H

namespace Ceres::Typing {

struct VariableVisibility {
public:
    enum Kind { Public, Private, Invalid };
    Kind kind;

    VariableVisibility(Kind kind);
    VariableVisibility();
};

struct FunctionVisibility {
public:
    enum Kind { Public, Private, Invalid };
    Kind kind;

    FunctionVisibility(Kind kind);
    FunctionVisibility();
};
}

#endif // COMPILER_VISIBILITY_H
