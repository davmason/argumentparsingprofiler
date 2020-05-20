
#pragma once

#include <vector>

class ManagedFunction
{

public:
    ManagedFunction(FunctionID functionID, ICorProfilerInfo10 *pProfilerInfo);
    ~ManagedFunction() = default;

    size_t GetParamCount();
    std::vector<
};