
#include "managedfunction.h"

using std::vector;

ManagedFunction::ManagedFunction(ICorProfilerInfo10 *pProfilerInfo, FunctionID functionID, ELTType eltType, COR_PRF_ELT_INFO eltInfo) :
    m_funcID(functionID),
    m_eltType(eltType),
    m_eltInfo(eltInfo),
    m_pProfilerInfo(pProfilerInfo),
    m_frameInfo(),
    m_paramTypes(),
    m_argRanges()
{
    PopulateArgs();
    PopulateParamTypes();
}

void ManagedFunction::PopulateArgs()
{
    ULONG pcbArgumentInfo = 0;
    HRESULT hr = m_pProfilerInfo->GetFunctionEnter3Info(m_funcID, m_eltInfo, &m_frameInfo, &pcbArgumentInfo, NULL);
    if (hr != HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER) && FAILED(hr))
    {
        printf("Error in GetFunctionEnter3Info!\n");
        return;
    }

    NewArrayHolder<char> pArgumentInfo = new char[pcbArgumentInfo];
    if (FAILED(m_pProfilerInfo->GetFunctionEnter3Info(m_funcID, m_eltInfo, &m_frameInfo, &pcbArgumentInfo, (COR_PRF_FUNCTION_ARGUMENT_INFO*)((char *)pArgumentInfo))))
    {
        printf("Error in GetFunctionEnter3Info call 2!\n");;
        return;
    }

    COR_PRF_FUNCTION_ARGUMENT_INFO* ptr = (COR_PRF_FUNCTION_ARGUMENT_INFO*)((char *)pArgumentInfo);

    ULONG32 numRanges = ptr->numRanges;

    m_argRanges.reserve(numRanges);
    for (size_t i = 0; i < numRanges; ++i)
    {
        m_argRanges.push_back(ptr->ranges[i]);
    }
}

void ManagedFunction::PopulateParamTypes()
{
    ModuleID moduleID;
    mdToken token;
    const ULONG32 maxTypeArgs = 100;
    ULONG32 numTypeArgs;
    ClassID typeArgs[maxTypeArgs];

    if (FAILED(m_pProfilerInfo->GetFunctionInfo2(m_funcID, m_frameInfo, NULL, &moduleID, &token, maxTypeArgs, &numTypeArgs, typeArgs)))
    {
        printf("Error in GetFunctionInfo2!\n");
        return;
    }

    COMPtrHolder<IMetaDataImport2> metadataImport;
    if (FAILED(m_pProfilerInfo->GetModuleMetaData(moduleID, ofWrite, IID_IMetaDataImport2, reinterpret_cast<IUnknown **>(&metadataImport))))
    {
        printf("Error getting metadata.\n");
        return;
    }

    mdTypeDef td;
    constexpr ULONG32 methodNameLen = 500;
    WCHAR wszMethod[methodNameLen];
    ULONG cchMethod;
    DWORD dwAttr;
    PCCOR_SIGNATURE pSig;
    ULONG pSigBytes;
    ULONG ulCodeRVA;
    DWORD dwImplFlags;
    if (FAILED(metadataImport->GetMethodProps(token, &td, wszMethod, methodNameLen, &cchMethod, &dwAttr, &pSig, &pSigBytes, &ulCodeRVA, &dwImplFlags)))
    {
        printf("Error getting MethodProps.\n");
        return;
    }

    // TODO: get full type information
    wprintf(L"Function entered: %s\n", String(wszMethod).ToWString().c_str());

    ParamSigParser parser;
    if (!parser.Parse((sig_byte*)pSig, pSigBytes))
    {
        printf("Signature wasn't parsed by the parser.\n");
        return;
    }

    vector<TypeInfo> infos = parser.getParamTypes();
    m_paramTypes.reserve(infos.size());
    for (auto &&info : infos)
    {
        ParameterType paramType = ConvertTypeInfoToParameterType(moduleID, info);
        m_paramTypes.push_back(paramType);
    }
}

ParameterType ManagedFunction::ConvertTypeInfoToParameterType(ModuleID moduleID, TypeInfo info)
{
    ParameterType paramType;
    if (info.type == ELEMENT_TYPE_CLASS)
    {
        // TODO: generics, the GetClassFromToken below won't work right for generics. Also
        // need to update the sig parser to read generics
        mdToken tok = TokenFromRid(info.index, info.indexType);
        ClassID classID;
        HRESULT hr = m_pProfilerInfo->GetClassFromToken(moduleID, tok, &classID);
        if (FAILED(hr))
        {
            printf("Failed to get ClassID from token");
        }

        paramType.referenceType = classID;
    }
    else
    {
        paramType.referenceType = NULL;
    }

    paramType.byRef = info.byref;
    paramType.typeCode = info.type;
    return paramType;
}

size_t ManagedFunction::GetParamCount()
{
    return m_paramTypes.size();
}

ParameterType ManagedFunction::GetParamAt(size_t pos)
{
    return ParameterType();
}

size_t ManagedFunction::GetArgValueCount()
{
    return m_argRanges.size();
}

COR_PRF_FUNCTION_ARGUMENT_RANGE ManagedFunction::GetArgValueAt(size_t pos)
{
    return COR_PRF_FUNCTION_ARGUMENT_RANGE();
}

String ManagedFunction::GetName()
{
    // If the FunctionID is 0, we could be dealing with a native function.
    if (m_funcID == NULL)
    {
        return WCHAR("Unknown_Native_Function");
    }

    String name;

    ClassID classId = NULL;
    ModuleID moduleId = NULL;
    mdToken token = NULL;
    ULONG32 nTypeArgs = NULL;
    ClassID typeArgs[SHORT_LENGTH];

    m_pProfilerInfo->GetFunctionInfo2(m_funcID,
        NULL,
        &classId,
        &moduleId,
        &token,
        SHORT_LENGTH,
        &nTypeArgs,
        typeArgs);

    COMPtrHolder<IMetaDataImport> pIMDImport;
    m_pProfilerInfo->GetModuleMetaData(moduleId,
        ofRead,
        IID_IMetaDataImport,
        (IUnknown * *)& pIMDImport);

    WCHAR funcName[STRING_LENGTH];
    pIMDImport->GetMethodProps(token,
        NULL,
        funcName,
        STRING_LENGTH,
        0,
        0,
        NULL,
        NULL,
        NULL,
        NULL);

    name += funcName;

    return name;
}
