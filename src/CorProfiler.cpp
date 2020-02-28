// SampleProfiler.cpp : Defines the exported functions for the DLL application.
//

#include <inttypes.h>
#include <iostream>
#include <atomic>
#include <assert.h>
#include "corprofiler.h"

using std::cout;
using std::endl;
using std::wstring;
using std::wcout;
using std::shared_ptr;
using std::vector;

#define PROFILER_STUB EXTERN_C void STDMETHODCALLTYPE

CorProfiler* CorProfiler::s_profiler;


PROFILER_STUB EnterStub(FunctionIDOrClientID functionId, COR_PRF_ELT_INFO eltInfo)
{
    CorProfiler::Instance()->EnterCallback(functionId, eltInfo);
}

PROFILER_STUB LeaveStub(FunctionIDOrClientID functionId, COR_PRF_ELT_INFO eltInfo)
{
    CorProfiler::Instance()->LeaveCallback(functionId, eltInfo);
}

PROFILER_STUB TailcallStub(FunctionIDOrClientID functionId, COR_PRF_ELT_INFO eltInfo)
{
    CorProfiler::Instance()->TailcallCallback(functionId, eltInfo);
}


CorProfiler::CorProfiler()
{

}

CorProfiler::~CorProfiler()
{
}

HRESULT STDMETHODCALLTYPE CorProfiler::Initialize(IUnknown* pProfilerInfoUnk)
{
    CorProfiler::s_profiler = this;

    printf("In SampleProfiler::Initialize!");

    HRESULT hr = pProfilerInfoUnk->QueryInterface(IID_ICorProfilerInfo10, (void**)& m_pProfilerInfo);
    if (hr != S_OK)
    {
        printf("Got HR %X from QI for ICorProfilerInfo4", hr);
        return E_FAIL;
    }

    m_pProfilerInfo->SetEventMask2(COR_PRF_MONITOR_ENTERLEAVE | COR_PRF_ENABLE_FUNCTION_ARGS | COR_PRF_ENABLE_FUNCTION_RETVAL | COR_PRF_ENABLE_FRAME_INFO, 0);
    m_pProfilerInfo->SetEnterLeaveFunctionHooks3WithInfo(EnterStub, LeaveStub, TailcallStub);

    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::InitializeForAttach(IUnknown* pCorProfilerInfoUnk, void* pvClientData, UINT cbClientData)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ProfilerAttachComplete()
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ProfilerDetachSucceeded()
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::Shutdown()
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::AppDomainCreationStarted(AppDomainID appDomainId)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::AppDomainCreationFinished(AppDomainID appDomainId, HRESULT hrStatus)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::AppDomainShutdownStarted(AppDomainID appDomainId)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::AppDomainShutdownFinished(AppDomainID appDomainId, HRESULT hrStatus)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::AssemblyLoadStarted(AssemblyID assemblyId)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::AssemblyLoadFinished(AssemblyID assemblyId, HRESULT hrStatus)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::AssemblyUnloadStarted(AssemblyID assemblyId)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::AssemblyUnloadFinished(AssemblyID assemblyId, HRESULT hrStatus)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ModuleLoadStarted(ModuleID moduleId)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ModuleLoadFinished(ModuleID moduleId, HRESULT hrStatus)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ModuleUnloadStarted(ModuleID moduleId)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ModuleUnloadFinished(ModuleID moduleId, HRESULT hrStatus)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ModuleAttachedToAssembly(ModuleID moduleId, AssemblyID AssemblyId)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ClassLoadStarted(ClassID classId)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ClassLoadFinished(ClassID classId, HRESULT hrStatus)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ClassUnloadStarted(ClassID classId)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ClassUnloadFinished(ClassID classId, HRESULT hrStatus)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::FunctionUnloadStarted(FunctionID functionId)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::JITCompilationStarted(FunctionID functionId, BOOL fIsSafeToBlock)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::JITCompilationFinished(FunctionID functionId, HRESULT hrStatus, BOOL fIsSafeToBlock)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::JITCachedFunctionSearchStarted(FunctionID functionId, BOOL* pbUseCachedFunction)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::JITCachedFunctionSearchFinished(FunctionID functionId, COR_PRF_JIT_CACHE result)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::JITFunctionPitched(FunctionID functionId)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::JITInlining(FunctionID callerId, FunctionID calleeId, BOOL* pfShouldInline)
{
    wstring inlinerName;
    wstring inlineeName;

    GetFunctionIDName(callerId, inlinerName);
    GetFunctionIDName(calleeId, inlineeName);

    //wcout << L"Function " << inlineeName << L" inlined in to function " << inlinerName << endl;

    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ThreadCreated(ThreadID threadID)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ThreadDestroyed(ThreadID threadID)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ThreadAssignedToOSThread(ThreadID managedThreadId, DWORD osThreadId)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ThreadNameChanged(ThreadID managedThreadId, ULONG cchName, WCHAR name[])
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::RemotingClientInvocationStarted()
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::RemotingClientSendingMessage(GUID* pCookie, BOOL fIsAsync)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::RemotingClientReceivingReply(GUID* pCookie, BOOL fIsAsync)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::RemotingClientInvocationFinished()
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::RemotingServerReceivingMessage(GUID* pCookie, BOOL fIsAsync)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::RemotingServerInvocationStarted()
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::RemotingServerInvocationReturned()
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::RemotingServerSendingReply(GUID* pCookie, BOOL fIsAsync)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::UnmanagedToManagedTransition(FunctionID functionId, COR_PRF_TRANSITION_REASON reason)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ManagedToUnmanagedTransition(FunctionID functionId, COR_PRF_TRANSITION_REASON reason)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::RuntimeSuspendStarted(COR_PRF_SUSPEND_REASON suspendReason)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::RuntimeSuspendFinished()
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::RuntimeSuspendAborted()
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::RuntimeResumeStarted()
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::RuntimeResumeFinished()
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::RuntimeThreadSuspended(ThreadID threadID)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::RuntimeThreadResumed(ThreadID threadID)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::MovedReferences(ULONG cMovedObjectIDRanges, ObjectID oldObjectIDRangeStart[], ObjectID newObjectIDRangeStart[], ULONG cObjectIDRangeLength[])
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ObjectAllocated(ObjectID objectId, ClassID classId)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ObjectsAllocatedByClass(ULONG cClassCount, ClassID classIds[], ULONG cObjects[])
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ObjectReferences(ObjectID objectId, ClassID classId, ULONG cObjectRefs, ObjectID objectRefIds[])
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::RootReferences(ULONG cRootRefs, ObjectID rootRefIds[])
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ExceptionThrown(ObjectID thrownObjectId)
{
    return S_OK;
}

UINT_PTR STDMETHODCALLTYPE CorProfiler::FunctionIDMapper(FunctionID functionId, BOOL* pbHookFunction)
{
    return S_OK;
}

UINT_PTR STDMETHODCALLTYPE CorProfiler::FunctionIDMapper2(FunctionID functionId, void* clientData, BOOL* pbHookFunction)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ExceptionSearchFunctionEnter(FunctionID functionId)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ExceptionSearchFunctionLeave()
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ExceptionSearchFilterEnter(FunctionID functionId)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ExceptionSearchFilterLeave()
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ExceptionSearchCatcherFound(FunctionID functionId)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ExceptionOSHandlerEnter(UINT_PTR __unused)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ExceptionOSHandlerLeave(UINT_PTR __unused)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ExceptionUnwindFunctionEnter(FunctionID functionId)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ExceptionUnwindFunctionLeave()
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ExceptionUnwindFinallyEnter(FunctionID functionId)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ExceptionUnwindFinallyLeave()
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ExceptionCatcherEnter(FunctionID functionId, ObjectID objectId)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ExceptionCatcherLeave()
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ExceptionCLRCatcherFound()
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ExceptionCLRCatcherExecute()
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::COMClassicVTableCreated(ClassID wrappedClassID, REFGUID implementedIID, VOID* pVTable, ULONG cSlots)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::COMClassicVTableDestroyed(ClassID wrappedClassID, REFGUID implementedIID, VOID* pVTable)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::GarbageCollectionStarted(INT cGenerations, BOOL generationCollected[], COR_PRF_GC_REASON reason)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::GarbageCollectionFinished()
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::FinalizeableObjectQueued(DWORD finalizerFlags, ObjectID objectID)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::RootReferences2(ULONG cRootRefs, ObjectID rootRefIds[], COR_PRF_GC_ROOT_KIND rootKinds[], COR_PRF_GC_ROOT_FLAGS rootFlags[], UINT_PTR rootIds[])
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::HandleCreated(GCHandleID handleId, ObjectID initialObjectId)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::HandleDestroyed(GCHandleID handleId)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::SurvivingReferences(ULONG cSurvivingObjectIDRanges, ObjectID objectIDRangeStart[], ULONG cObjectIDRangeLength[])
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ReJITCompilationStarted(FunctionID functionId, ReJITID rejitId, BOOL fIsSafeToBlock)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::GetReJITParameters(ModuleID moduleId, mdMethodDef methodId, ICorProfilerFunctionControl* pFunctionControl)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ReJITCompilationFinished(FunctionID functionId, ReJITID rejitId, HRESULT hrStatus, BOOL fIsSafeToBlock)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ReJITError(ModuleID moduleId, mdMethodDef methodId, FunctionID functionId, HRESULT hrStatus)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::MovedReferences2(ULONG cMovedObjectIDRanges, ObjectID oldObjectIDRangeStart[], ObjectID newObjectIDRangeStart[], SIZE_T cObjectIDRangeLength[])
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::SurvivingReferences2(ULONG cSurvivingObjectIDRanges, ObjectID objectIDRangeStart[], SIZE_T cObjectIDRangeLength[])
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::QueryInterface(const IID& iid, void** ppv)
{
    if (iid == IID_IUnknown)
    {
        printf("In TestProfiler::QueryInterface Matched IUnknown\n");
        *ppv = static_cast<ICorProfilerCallback4*>(this); // original implementation
    }
    else if (iid == IID_ICorProfilerCallback4)
    {
        printf("In TestProfiler::QueryInterface Matched ICorProfilerCallback4\n");
        *ppv = static_cast<ICorProfilerCallback4*>(this);
    }
    else if (iid == IID_ICorProfilerCallback3)
    {
        printf("In TestProfiler::QueryInterface Matched ICorProfilerCallback3\n");
        *ppv = static_cast<ICorProfilerCallback3*>(this);
    }
    else if (iid == IID_ICorProfilerCallback2)
    {
        printf("In TestProfiler::QueryInterface Matched ICorProfilerCallback2\n");
        *ppv = static_cast<ICorProfilerCallback2*>(this);
    }
    else if (iid == IID_ICorProfilerCallback)
    {
        printf("In TestProfiler::QueryInterface Matched ICorProfilerCallback1\n");
        *ppv = static_cast<ICorProfilerCallback*>(this);
    }
    else
    {
        *ppv = NULL;
        printf("In TestProfiler::QueryInterface, returning E_NOINTERFACE\n");
        return E_NOINTERFACE;
    }

    printf("Leave TestProfiler::QueryInterface (Success)\n");
    reinterpret_cast<IUnknown*>(*ppv)->AddRef();
    return S_OK;
}

ULONG STDMETHODCALLTYPE CorProfiler::AddRef()
{
    m_refCount++;
    return m_refCount;
}

ULONG STDMETHODCALLTYPE CorProfiler::Release()
{
    m_refCount--;
    printf("SampleProfiler::Release() called m_refCount=%d\n", m_refCount.load());
    return m_refCount;
}

HRESULT STDMETHODCALLTYPE CorProfiler::EnterCallback(FunctionIDOrClientID functionId, COR_PRF_ELT_INFO eltInfo)
{
    ULONG pcbArgumentInfo = 0;
    COR_PRF_FRAME_INFO frameInfo;
    HRESULT hr = m_pProfilerInfo->GetFunctionEnter3Info(functionId.functionID, eltInfo, &frameInfo, &pcbArgumentInfo, NULL);
    if (hr != HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER) && FAILED(hr))
    {
        printf("Error in GetFunctionEnter3Info!\n");
        return E_FAIL;
    }

    char* pArgumentInfo = new char[pcbArgumentInfo];
    if (FAILED(m_pProfilerInfo->GetFunctionEnter3Info(functionId.functionID, eltInfo, &frameInfo, &pcbArgumentInfo, (COR_PRF_FUNCTION_ARGUMENT_INFO*)pArgumentInfo)))
    {
        printf("Error in GetFunctionEnter3Info call 2!\n");;
        return E_FAIL;
    }

    COR_PRF_FUNCTION_ARGUMENT_INFO* ptr = (COR_PRF_FUNCTION_ARGUMENT_INFO*)pArgumentInfo;

    ModuleID moduleID;
    mdToken token;
    const ULONG32 maxTypeArgs = 100;
    ULONG32 numTypeArgs;
    ClassID typeArgs[maxTypeArgs];

    if (FAILED(m_pProfilerInfo->GetFunctionInfo2(functionId.functionID, frameInfo, NULL, &moduleID, &token, maxTypeArgs, &numTypeArgs, typeArgs)))
    {
        printf("Error in GetFunctionInfo2!\n");
        return E_FAIL;
    }

    COMPtrHolder<IMetaDataImport2> metadataImport;
    if (FAILED(m_pProfilerInfo->GetModuleMetaData(moduleID, ofWrite, IID_IMetaDataImport2, reinterpret_cast<IUnknown **>(&metadataImport))))
    {
        printf("Error getting metadata.\n");
        return E_FAIL;
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
        return E_FAIL;
    }

    // TODO: get full type information
    wprintf(L"Function entered: %s\n", wszMethod);

    ParamSigParser parser;
    if (!parser.Parse((sig_byte*)pSig, pSigBytes))
    {
        DebugBreak();
        printf("Signature wasn't parsed by the parser.\n");
        return E_FAIL;
    }
   
    ULONG32 paramCount = parser.getParamCount();
    ULONG32 numRanges = ptr->numRanges;
    
    bool hasThis = false;
    if (numRanges == paramCount + 1)
    {
        hasThis = true;
    }
    else
    {
        assert(paramCount == numRanges);
    }

    vector<TypeInfo> infos = parser.getParamTypes();

    if (hasThis)
    {
        printf("    This*=");
        // TODO: is the index the token?
        PrettyPrintArgument(metadataImport, { ELEMENT_TYPE_CLASS, SIG_INDEX_TYPE_TYPEDEF, (int)token, false }, &ptr->ranges[0]);
        printf("\n");
    }

    for (ULONG i = 0; i < paramCount; ++i)
    {
        ULONG rangeIndex = i;
        if (hasThis)
        {
            // Account for the secret this arg
            rangeIndex++;
        }

        printf("    Argument %d=", i);
        PrettyPrintArgument(metadataImport, infos.at(i), &ptr->ranges[rangeIndex]);
    }

    return S_OK;
}

void CorProfiler::PrettyPrintArgument(IMetaDataImport2 *metadataImport, TypeInfo ti, COR_PRF_FUNCTION_ARGUMENT_RANGE* arg)
{
    bool        boolValue;
    char        charValue;
    int8_t      int8Value;
    uint8_t     uint8Value;
    uint16_t    uint16Value;
    int16_t     int16Value;
    int32_t     int32Value;
    uint32_t    uint32Value;
    int64_t     int64Value;
    uint64_t    uint64Value;
    float       floatValue;
    double      doubleValue;
    WCHAR       *stringValue;
    void        *ptrValue;

    void* realStart = (void *)arg->startAddress;
    if (ti.byref)
    {
        realStart = *((void **)realStart);
    }

    switch (ti.type)
    {
    case ELEMENT_TYPE_BOOLEAN:
        //assert(arg->length == 1);
        boolValue = *((bool *)arg->startAddress);
        printf("Type: BOOL Value: %s", boolValue ? "true" : "false");
        break;

    case ELEMENT_TYPE_CHAR:
        //assert(arg->length == 1);
        charValue = *((char *)arg->startAddress);
        printf("Type: CHAR Value: %c", charValue);
        break;

    case ELEMENT_TYPE_I1:
        //assert(arg->length == 1);
        int8Value = *((int8_t *)arg->startAddress);
        printf("Type: INT8 Value: %d", int8Value);
        break;

    case ELEMENT_TYPE_U1:
        //assert(arg->length == 1);
        uint8Value = *((uint8_t *)arg->startAddress);
        printf("Type: UINT8 Value: %u", uint8Value);
        break;

    case ELEMENT_TYPE_U2:
        //assert(arg->length == 2);
        uint16Value = *((uint16_t *)arg->startAddress);
        printf("Type: UINT16 Value: %u", uint16Value);
        break;

    case ELEMENT_TYPE_I2:
        //assert(arg->length == 2);
        int16Value = *((int16_t *)arg->startAddress);
        printf("Type: INT16 Value: %d", int16Value);
        break;

    case ELEMENT_TYPE_I4:
        //assert(arg->length == 4);
        int32Value = *((int32_t *)arg->startAddress);
        printf("Type: INT32 Value: %d", int32Value);
        break;

    case ELEMENT_TYPE_U4:
        //assert(arg->length == 4);
        uint32Value = *((uint32_t *)arg->startAddress);
        printf("Type: UINT32 Value: %u", uint32Value);
        break;

    case ELEMENT_TYPE_I8:
        //assert(arg->length == 8);
        int64Value = *((int64_t*)arg->startAddress);
        printf("Type: INT64 Value: %" PRId64, int64Value);
        break;

    case ELEMENT_TYPE_U8:
        //assert(arg->length == 8);
        uint64Value = *((uint64_t *)arg->startAddress);
        printf("Type: UINT64 Value: %" PRIu64, uint64Value);
        break;

    case ELEMENT_TYPE_R4:
        //assert(arg->length == 4);
        floatValue = *((float *)arg->startAddress);
        printf("Type: FLOAT Value: %f", floatValue);
        break;

    case ELEMENT_TYPE_R8:
        //assert(arg->length == 8);
        doubleValue = *((double *)arg->startAddress);
        printf("Type: DOUBLE Value: %f", doubleValue);
        break;

    case ELEMENT_TYPE_STRING:
        //assert(arg->length == 8);
        stringValue = *((WCHAR **)arg->startAddress);
        wprintf(L"Type: STRING Value: %s", stringValue);
        break;

    case ELEMENT_TYPE_OBJECT:
        //assert(arg->length == 8);
        ptrValue = *((void **)arg->startAddress);
        printf("Type: OBJREF Value: 0x%p", ptrValue);
        break;

    case ELEMENT_TYPE_PTR:
    case ELEMENT_TYPE_CLASS:
    case ELEMENT_TYPE_VALUETYPE:
    case ELEMENT_TYPE_ARRAY:
    case ELEMENT_TYPE_I: // IntPtr
    case ELEMENT_TYPE_U: // UIntPtr
    case ELEMENT_TYPE_FNPTR:
    case ELEMENT_TYPE_SZARRAY:
    case ELEMENT_TYPE_GENERICINST:
    case ELEMENT_TYPE_VAR:
    case ELEMENT_TYPE_MVAR:
        //assert(false && "Not yet impletmented");
        printf("Type not implemented yet");
        break;
    }

    //if (false /* this is the string case */)
    //{
    //    wcout << "Argument " << i << endl;
    //    ULONG pStringLengthOffset;
    //    ULONG pBufferOffset;
    //    if (FAILED(m_pProfilerInfo->GetStringLayout2(&pStringLengthOffset, &pBufferOffset)))
    //    {
    //        wcout << L"Error in GetStringLayout2!" << endl;
    //        return E_FAIL;
    //    }

    //    //Assume some max string size
    //    WCHAR tempString[128];
    //    DWORD stringLength;

    //    byte** stringAddr = reinterpret_cast<byte**>(ptr->ranges[i].startAddress);;
    //    wcout << "stringaddr=" << std::hex << stringAddr << endl;
    //    wcout << "*stringaddr=" << std::hex << *stringAddr << endl;

    //    // It's perfectly valid for a reference to be null, and the code below will AV 
    //    // if it is null.
    //    if (*stringAddr == NULL)
    //    {
    //        wcout << "NULL argument" << endl;
    //        continue;
    //    }

    //    // Assume objectOfInterestPosition as the position of the argument of interest
    //    // NOTE: pStringLengthOffset and pBufferOffset are in bytes
    //    memcpy(&stringLength, ((const void*)(*stringAddr + pStringLengthOffset)), sizeof(DWORD));
    //    wcout << "stringlength=" << std::dec << stringLength << endl;

    //    memcpy(tempString, ((const void*)(*stringAddr + pBufferOffset)), stringLength * sizeof(DWORD));
    //    tempString[stringLength * sizeof(DWORD)] = '\0';
    //    //tempString will now have the string value at objectOfInterestPosition of the method entered into 
    //}
}

HRESULT STDMETHODCALLTYPE CorProfiler::LeaveCallback(FunctionIDOrClientID functionId, COR_PRF_ELT_INFO eltInfo)
{
    wstring name;
    GetFunctionIDName(functionId.functionID, name);

    wcout << L"Function Left: " << name << endl;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::TailcallCallback(FunctionIDOrClientID functionId, COR_PRF_ELT_INFO eltInfo)
{
    wstring name;
    GetFunctionIDName(functionId.functionID, name);

    wcout << L"Function Tailcall: " << name << endl;
    return S_OK;
}

HRESULT CorProfiler::GetFunctionIDName(FunctionID funcId, wstring & name)
{
    // If the FunctionID is 0, we could be dealing with a native function.
    if (funcId == NULL)
    {
        name += L"Unknown_Native_Function";
        return S_OK;
    }

    ClassID classId = NULL;
    ModuleID moduleId = NULL;
    mdToken token = NULL;
    ULONG32 nTypeArgs = NULL;
    ClassID typeArgs[SHORT_LENGTH];

    m_pProfilerInfo->GetFunctionInfo2(funcId,
        0,
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

    return S_OK;
}
