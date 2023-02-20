#ifndef COMPILER_CONSTNESS_H
#define COMPILER_CONSTNESS_H

namespace Ceres::Typing {

struct Constness {
    enum Kind { Const, NonConst, Invalid };

    Kind kind;
    Constness(Kind kind);
    Constness();
};

}
#endif // COMPILER_CONSTNESS_H
