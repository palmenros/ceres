#ifndef COMPILER_INITCERES_H
#define COMPILER_INITCERES_H

namespace Ceres {

// The purpose of this class is to perform initialization and destruction
// actions for the Ceres compiler
class InitCeres {
public:
    InitCeres();
    ~InitCeres();

    // Delete assignment operator
    InitCeres& operator=(InitCeres const&) = delete;
    InitCeres(InitCeres const&) = delete;

    // Delete move assignment operator
    InitCeres(InitCeres&&) = delete;
    InitCeres& operator=(InitCeres&&) = delete;
};

} // namespace Ceres

#endif // COMPILER_INITCERES_H
