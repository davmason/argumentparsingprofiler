
#pragma once

#include "common.h"
#include "managedfunction.h"

class ManagedArgPrinter
{
private:
    ParameterType m_type;
    COR_PRF_FUNCTION_ARGUMENT_RANGE m_arg;
    ICorProfilerInfo10 *m_pProfilerInfo;

public:
    ManagedArgPrinter(ParameterType type, COR_PRF_FUNCTION_ARGUMENT_RANGE arg, ICorProfilerInfo10 *pProfilerInfo);
    ~ManagedArgPrinter() = default;

    void PrettyPrint(size_t indentLevel = 0);
};
