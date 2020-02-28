// Copyright (c) .NET Foundation and contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <atomic>
#include <memory>
#include <set>
#include <mutex>
#include <vector>
#include <condition_variable>
#include "cor.h"
#include "corhdr.h"
#include "corerror.h"
#include "corprof.h"
#include "paramsigparser.h"


#define SHORT_LENGTH    32
#define STRING_LENGTH  256
#define LONG_LENGTH   1024

template <class MetaInterface>
class COMPtrHolder
{
public:
    COMPtrHolder()
    {
        m_ptr = NULL;
    }
    
    COMPtrHolder(MetaInterface* ptr)
    {
        if (ptr != NULL)
        {
            ptr->AddRef();
        }
        m_ptr = ptr;
    }
    
    ~COMPtrHolder()
    {
        if (m_ptr != NULL)
        {
            m_ptr->Release();
            m_ptr = NULL;
        }
    }
    MetaInterface* operator->()
    {
        return m_ptr;
    }

    MetaInterface** operator&()
    {
       // _ASSERT(m_ptr == NULL);
        return &m_ptr;
    }
    
    operator MetaInterface*()
    {
        return m_ptr;
    }
private:
    MetaInterface* m_ptr;
};

class CorProfiler : public ICorProfilerCallback4
{
private:
    static CorProfiler* s_profiler;
    COMPtrHolder<ICorProfilerInfo10> m_pProfilerInfo;

public:
    std::atomic<int> m_refCount;

    CorProfiler();
    virtual ~CorProfiler();
    virtual HRESULT STDMETHODCALLTYPE Initialize(IUnknown* pProfilerInfoUnk);
    virtual HRESULT STDMETHODCALLTYPE InitializeForAttach(IUnknown* pCorProfilerInfoUnk, void* pvClientData, UINT cbClientData);
    virtual HRESULT STDMETHODCALLTYPE ProfilerAttachComplete();
    virtual HRESULT STDMETHODCALLTYPE ProfilerDetachSucceeded();
    virtual HRESULT STDMETHODCALLTYPE Shutdown();
    virtual HRESULT STDMETHODCALLTYPE AppDomainCreationStarted(AppDomainID appDomainId);
    virtual HRESULT STDMETHODCALLTYPE AppDomainCreationFinished(AppDomainID appDomainId, HRESULT hrStatus);
    virtual HRESULT STDMETHODCALLTYPE AppDomainShutdownStarted(AppDomainID appDomainId);
    virtual HRESULT STDMETHODCALLTYPE AppDomainShutdownFinished(AppDomainID appDomainId, HRESULT hrStatus);
    virtual HRESULT STDMETHODCALLTYPE AssemblyLoadStarted(AssemblyID assemblyId);
    virtual HRESULT STDMETHODCALLTYPE AssemblyLoadFinished(AssemblyID assemblyId, HRESULT hrStatus);
    virtual HRESULT STDMETHODCALLTYPE AssemblyUnloadStarted(AssemblyID assemblyId);
    virtual HRESULT STDMETHODCALLTYPE AssemblyUnloadFinished(AssemblyID assemblyId, HRESULT hrStatus);
    virtual HRESULT STDMETHODCALLTYPE ModuleLoadStarted(ModuleID moduleId);
    virtual HRESULT STDMETHODCALLTYPE ModuleLoadFinished(ModuleID moduleId, HRESULT hrStatus);
    virtual HRESULT STDMETHODCALLTYPE ModuleUnloadStarted(ModuleID moduleId);
    virtual HRESULT STDMETHODCALLTYPE ModuleUnloadFinished(ModuleID moduleId, HRESULT hrStatus);
    virtual HRESULT STDMETHODCALLTYPE ModuleAttachedToAssembly(ModuleID moduleId, AssemblyID AssemblyId);
    virtual HRESULT STDMETHODCALLTYPE ClassLoadStarted(ClassID classId);
    virtual HRESULT STDMETHODCALLTYPE ClassLoadFinished(ClassID classId, HRESULT hrStatus);
    virtual HRESULT STDMETHODCALLTYPE ClassUnloadStarted(ClassID classId);
    virtual HRESULT STDMETHODCALLTYPE ClassUnloadFinished(ClassID classId, HRESULT hrStatus);
    virtual HRESULT STDMETHODCALLTYPE FunctionUnloadStarted(FunctionID functionId);
    virtual HRESULT STDMETHODCALLTYPE JITCompilationStarted(FunctionID functionId, BOOL fIsSafeToBlock);
    virtual HRESULT STDMETHODCALLTYPE JITCompilationFinished(FunctionID functionId, HRESULT hrStatus, BOOL fIsSafeToBlock);
    virtual HRESULT STDMETHODCALLTYPE JITCachedFunctionSearchStarted(FunctionID functionId, BOOL* pbUseCachedFunction);
    virtual HRESULT STDMETHODCALLTYPE JITCachedFunctionSearchFinished(FunctionID functionId, COR_PRF_JIT_CACHE result);
    virtual HRESULT STDMETHODCALLTYPE JITFunctionPitched(FunctionID functionId);
    virtual HRESULT STDMETHODCALLTYPE JITInlining(FunctionID callerId, FunctionID calleeId, BOOL* pfShouldInline);
    virtual HRESULT STDMETHODCALLTYPE ThreadCreated(ThreadID threadID);
    virtual HRESULT STDMETHODCALLTYPE ThreadDestroyed(ThreadID threadID);
    virtual HRESULT STDMETHODCALLTYPE ThreadAssignedToOSThread(ThreadID managedThreadId, DWORD osThreadId);
    virtual HRESULT STDMETHODCALLTYPE ThreadNameChanged(ThreadID managedThreadId, ULONG cchName, WCHAR name[]);
    virtual HRESULT STDMETHODCALLTYPE RemotingClientInvocationStarted();
    virtual HRESULT STDMETHODCALLTYPE RemotingClientSendingMessage(GUID* pCookie, BOOL fIsAsync);
    virtual HRESULT STDMETHODCALLTYPE RemotingClientReceivingReply(GUID* pCookie, BOOL fIsAsync);
    virtual HRESULT STDMETHODCALLTYPE RemotingClientInvocationFinished();
    virtual HRESULT STDMETHODCALLTYPE RemotingServerReceivingMessage(GUID* pCookie, BOOL fIsAsync);
    virtual HRESULT STDMETHODCALLTYPE RemotingServerInvocationStarted();
    virtual HRESULT STDMETHODCALLTYPE RemotingServerInvocationReturned();
    virtual HRESULT STDMETHODCALLTYPE RemotingServerSendingReply(GUID* pCookie, BOOL fIsAsync);
    virtual HRESULT STDMETHODCALLTYPE UnmanagedToManagedTransition(FunctionID functionId, COR_PRF_TRANSITION_REASON reason);
    virtual HRESULT STDMETHODCALLTYPE ManagedToUnmanagedTransition(FunctionID functionId, COR_PRF_TRANSITION_REASON reason);
    virtual HRESULT STDMETHODCALLTYPE RuntimeSuspendStarted(COR_PRF_SUSPEND_REASON suspendReason);
    virtual HRESULT STDMETHODCALLTYPE RuntimeSuspendFinished();
    virtual HRESULT STDMETHODCALLTYPE RuntimeSuspendAborted();
    virtual HRESULT STDMETHODCALLTYPE RuntimeResumeStarted();
    virtual HRESULT STDMETHODCALLTYPE RuntimeResumeFinished();
    virtual HRESULT STDMETHODCALLTYPE RuntimeThreadSuspended(ThreadID threadID);
    virtual HRESULT STDMETHODCALLTYPE RuntimeThreadResumed(ThreadID threadID);
    virtual HRESULT STDMETHODCALLTYPE MovedReferences(ULONG cMovedObjectIDRanges, ObjectID oldObjectIDRangeStart[], ObjectID newObjectIDRangeStart[], ULONG cObjectIDRangeLength[]);
    virtual HRESULT STDMETHODCALLTYPE ObjectAllocated(ObjectID objectId, ClassID classId);
    virtual HRESULT STDMETHODCALLTYPE ObjectsAllocatedByClass(ULONG cClassCount, ClassID classIds[], ULONG cObjects[]);
    virtual HRESULT STDMETHODCALLTYPE ObjectReferences(ObjectID objectId, ClassID classId, ULONG cObjectRefs, ObjectID objectRefIds[]);
    virtual HRESULT STDMETHODCALLTYPE RootReferences(ULONG cRootRefs, ObjectID rootRefIds[]);
    virtual HRESULT STDMETHODCALLTYPE ExceptionThrown(ObjectID thrownObjectId);
    virtual UINT_PTR STDMETHODCALLTYPE FunctionIDMapper(FunctionID functionId, BOOL* pbHookFunction);
    virtual UINT_PTR STDMETHODCALLTYPE FunctionIDMapper2(FunctionID functionId, void* clientData, BOOL* pbHookFunction);
    virtual HRESULT STDMETHODCALLTYPE ExceptionSearchFunctionEnter(FunctionID functionId);
    virtual HRESULT STDMETHODCALLTYPE ExceptionSearchFunctionLeave();
    virtual HRESULT STDMETHODCALLTYPE ExceptionSearchFilterEnter(FunctionID functionId);
    virtual HRESULT STDMETHODCALLTYPE ExceptionSearchFilterLeave();
    virtual HRESULT STDMETHODCALLTYPE ExceptionSearchCatcherFound(FunctionID functionId);
    virtual HRESULT STDMETHODCALLTYPE ExceptionOSHandlerEnter(UINT_PTR __unused);
    virtual HRESULT STDMETHODCALLTYPE ExceptionOSHandlerLeave(UINT_PTR __unused);
    virtual HRESULT STDMETHODCALLTYPE ExceptionUnwindFunctionEnter(FunctionID functionId);
    virtual HRESULT STDMETHODCALLTYPE ExceptionUnwindFunctionLeave();
    virtual HRESULT STDMETHODCALLTYPE ExceptionUnwindFinallyEnter(FunctionID functionId);
    virtual HRESULT STDMETHODCALLTYPE ExceptionUnwindFinallyLeave();
    virtual HRESULT STDMETHODCALLTYPE ExceptionCatcherEnter(FunctionID functionId, ObjectID objectId);
    virtual HRESULT STDMETHODCALLTYPE ExceptionCatcherLeave();
    virtual HRESULT STDMETHODCALLTYPE ExceptionCLRCatcherFound();
    virtual HRESULT STDMETHODCALLTYPE ExceptionCLRCatcherExecute();
    virtual HRESULT STDMETHODCALLTYPE COMClassicVTableCreated(ClassID wrappedClassID, REFGUID implementedIID, VOID* pVTable, ULONG cSlots);
    virtual HRESULT STDMETHODCALLTYPE COMClassicVTableDestroyed(ClassID wrappedClassID, REFGUID implementedIID, VOID* pVTable);
    virtual HRESULT STDMETHODCALLTYPE GarbageCollectionStarted(INT cGenerations, BOOL generationCollected[], COR_PRF_GC_REASON reason);
    virtual HRESULT STDMETHODCALLTYPE GarbageCollectionFinished();
    virtual HRESULT STDMETHODCALLTYPE FinalizeableObjectQueued(DWORD finalizerFlags, ObjectID objectID);
    virtual HRESULT STDMETHODCALLTYPE RootReferences2(ULONG cRootRefs, ObjectID rootRefIds[], COR_PRF_GC_ROOT_KIND rootKinds[], COR_PRF_GC_ROOT_FLAGS rootFlags[], UINT_PTR rootIds[]);
    virtual HRESULT STDMETHODCALLTYPE HandleCreated(GCHandleID handleId, ObjectID initialObjectId);
    virtual HRESULT STDMETHODCALLTYPE HandleDestroyed(GCHandleID handleId);
    virtual HRESULT STDMETHODCALLTYPE SurvivingReferences(ULONG cSurvivingObjectIDRanges, ObjectID objectIDRangeStart[], ULONG cObjectIDRangeLength[]);
    virtual HRESULT STDMETHODCALLTYPE ReJITCompilationStarted(FunctionID functionId, ReJITID rejitId, BOOL fIsSafeToBlock);
    virtual HRESULT STDMETHODCALLTYPE GetReJITParameters(ModuleID moduleId, mdMethodDef methodId, ICorProfilerFunctionControl* pFunctionControl);
    virtual HRESULT STDMETHODCALLTYPE ReJITCompilationFinished(FunctionID functionId, ReJITID rejitId, HRESULT hrStatus, BOOL fIsSafeToBlock);
    virtual HRESULT STDMETHODCALLTYPE ReJITError(ModuleID moduleId, mdMethodDef methodId, FunctionID functionId, HRESULT hrStatus);
    virtual HRESULT STDMETHODCALLTYPE MovedReferences2(ULONG cMovedObjectIDRanges, ObjectID oldObjectIDRangeStart[], ObjectID newObjectIDRangeStart[], SIZE_T cObjectIDRangeLength[]);
    virtual HRESULT STDMETHODCALLTYPE SurvivingReferences2(ULONG cSurvivingObjectIDRanges, ObjectID objectIDRangeStart[], SIZE_T cObjectIDRangeLength[]);
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(const IID& iid, void** ppv);
    virtual ULONG STDMETHODCALLTYPE AddRef();
    virtual ULONG STDMETHODCALLTYPE Release();

    HRESULT STDMETHODCALLTYPE EnterCallback(FunctionIDOrClientID functionId, COR_PRF_ELT_INFO eltInfo);
    HRESULT STDMETHODCALLTYPE LeaveCallback(FunctionIDOrClientID functionId, COR_PRF_ELT_INFO eltInfo);
    HRESULT STDMETHODCALLTYPE TailcallCallback(FunctionIDOrClientID functionId, COR_PRF_ELT_INFO eltInfo);

    void PrettyPrintArgument(IMetaDataImport2 *metadataImport, TypeInfo ti, COR_PRF_FUNCTION_ARGUMENT_RANGE *arg);

    HRESULT GetFunctionIDName(FunctionID funcId, std::wstring& name);

    static CorProfiler* Instance()
    {
        return s_profiler;
    }
};