
#pragma once

#include <vector>
#include "common.h"
#include "paramsigparser.h"
#include "profilerstring.h"

struct ParameterType
{
    sig_elem_type typeCode;
    ClassID referenceType;
    bool byRef;
};

enum class ELTType
{
    Enter,
    Leave,
    Tailcall
};

class ManagedFunction
{
private:
    FunctionID m_funcID;
    ELTType m_eltType;
    COR_PRF_ELT_INFO m_eltInfo;
    ICorProfilerInfo10 *m_pProfilerInfo;
    COR_PRF_FRAME_INFO m_frameInfo;

    bool populated;
    std::vector<ParameterType> m_paramTypes;
    std::vector<COR_PRF_FUNCTION_ARGUMENT_RANGE> m_argRanges;

    void Populate();
    void PopulateArgs();
    void PopulateParamTypes();

    void ConvertTypeRefToTypeDef(ModuleID refModule, mdToken typeRef, ModuleID *defModule, mdToken *typeDef);
    ParameterType ConvertTypeInfoToParameterType(ModuleID moduleID, TypeInfo info);

public:
    ManagedFunction(ICorProfilerInfo10 *pProfilerInfo, FunctionID functionID, ELTType eltType = ELTType::Enter, COR_PRF_ELT_INFO eltInfo = NULL);
    ~ManagedFunction() = default;

    size_t GetParamCount();
    ParameterType GetParamAt(size_t pos);
    size_t GetArgValueCount();
    COR_PRF_FUNCTION_ARGUMENT_RANGE GetArgValueAt(size_t pos);
    String GetName();
};