
#include "managedargprinter.h"
#include <cinttypes>

ManagedArgPrinter::ManagedArgPrinter(ParameterType type, COR_PRF_FUNCTION_ARGUMENT_RANGE arg, ICorProfilerInfo10 *pProfilerInfo) :
    m_type(type),
    m_arg(arg),
    m_pProfilerInfo(pProfilerInfo)
{

}

void ManagedArgPrinter::PrettyPrint(size_t indentLevel)
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

    void* realStart = (void *)m_arg.startAddress;
    if (m_type.byRef)
    {
        realStart = *((void **)realStart);
    }

    switch (m_type.typeCode)
    {
    case ELEMENT_TYPE_BOOLEAN:
        //assert(m_arg.length == 1);
        boolValue = *((bool *)m_arg.startAddress);
        printf("Type: BOOL Value: %s", boolValue ? "true" : "false");
        break;

    case ELEMENT_TYPE_CHAR:
        //assert(m_arg.length == 1);
        charValue = *((char *)m_arg.startAddress);
        printf("Type: CHAR Value: %c", charValue);
        break;

    case ELEMENT_TYPE_I1:
        //assert(m_arg.length == 1);
        int8Value = *((int8_t *)m_arg.startAddress);
        printf("Type: INT8 Value: %d", int8Value);
        break;

    case ELEMENT_TYPE_U1:
        //assert(m_arg.length == 1);
        uint8Value = *((uint8_t *)m_arg.startAddress);
        printf("Type: UINT8 Value: %u", uint8Value);
        break;

    case ELEMENT_TYPE_U2:
        //assert(m_arg.length == 2);
        uint16Value = *((uint16_t *)m_arg.startAddress);
        printf("Type: UINT16 Value: %u", uint16Value);
        break;

    case ELEMENT_TYPE_I2:
        //assert(m_arg.length == 2);
        int16Value = *((int16_t *)m_arg.startAddress);
        printf("Type: INT16 Value: %d", int16Value);
        break;

    case ELEMENT_TYPE_I4:
        //assert(m_arg.length == 4);
        int32Value = *((int32_t *)m_arg.startAddress);
        printf("Type: INT32 Value: %d", int32Value);
        break;

    case ELEMENT_TYPE_U4:
        //assert(m_arg.length == 4);
        uint32Value = *((uint32_t *)m_arg.startAddress);
        printf("Type: UINT32 Value: %u", uint32Value);
        break;

    case ELEMENT_TYPE_I8:
        //assert(m_arg.length == 8);
        int64Value = *((int64_t*)m_arg.startAddress);
        printf("Type: INT64 Value: %" PRId64, int64Value);
        break;

    case ELEMENT_TYPE_U8:
        //assert(m_arg.length == 8);
        uint64Value = *((uint64_t *)m_arg.startAddress);
        printf("Type: UINT64 Value: %" PRIu64, uint64Value);
        break;

    case ELEMENT_TYPE_R4:
        //assert(m_arg.length == 4);
        floatValue = *((float *)m_arg.startAddress);
        printf("Type: FLOAT Value: %f", floatValue);
        break;

    case ELEMENT_TYPE_R8:
        //assert(m_arg.length == 8);
        doubleValue = *((double *)m_arg.startAddress);
        printf("Type: DOUBLE Value: %f", doubleValue);
        break;

    case ELEMENT_TYPE_STRING:
        //assert(m_arg.length == 8);
        stringValue = *((WCHAR **)m_arg.startAddress);
        wprintf(L"Type: STRING Value: %s", stringValue);
        break;

    case ELEMENT_TYPE_OBJECT:
        //assert(m_arg.length == 8);
        ptrValue = *((void **)m_arg.startAddress);
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
