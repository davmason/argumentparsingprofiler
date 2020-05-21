#include "paramsigparser.h"

#include <assert.h>

#define PARAMSIGPARSER_DEBUG true

using std::vector;

ParamSigParser::ParamSigParser() :
    m_paramCount(0),
    m_callingConv(),
    m_currTypeByRef(false),
    m_indexType(-1),
    m_index(-1),
    m_paramTypes(),
    m_hasUnsupportedParams(false),
    m_inParam(false),
    m_inClass(false)
{
}

void ParamSigParser::NotifyCallingConvention(sig_elem_type callingConv)
{
#if PARAMSIGPARSER_DEBUG
    printf("NotifyCallingConvention\n");
#endif // PARAMSIGPARSER_DEBUG
    m_callingConv = callingConv;
}

void ParamSigParser::NotifyParamCount(sig_count count)
{
#if PARAMSIGPARSER_DEBUG
    printf("NotifyParamCount: %d\n", count);
#endif // PARAMSIGPARSER_DEBUG
    m_paramCount = count;
}

void ParamSigParser::NotifyGenericParamCount(sig_count count)
{
#if PARAMSIGPARSER_DEBUG
    printf("NotifyGenericParamCount: %d\n", count);
#endif // PARAMSIGPARSER_DEBUG
    m_hasUnsupportedParams = true;
}

void ParamSigParser::NotifyBeginParam()
{
#if PARAMSIGPARSER_DEBUG
    printf("NotifyBeginParam\n");
#endif // PARAMSIGPARSER_DEBUG
    m_inParam = true;
}

void ParamSigParser::NotifyEndParam()
{
#if PARAMSIGPARSER_DEBUG
    printf("NotifyEndParam\n");
#endif // PARAMSIGPARSER_DEBUG
    m_inParam = true;
    m_inClass = false;
}

void ParamSigParser::NotifyBeginType()
{
}

void ParamSigParser::NotifyEndType()
{
    m_currTypeByRef = false;
}

void ParamSigParser::NotifyByref()
{
    m_currTypeByRef = true;
}

void ParamSigParser::NotifyTypeSimple(sig_elem_type elem_type)
{
#if PARAMSIGPARSER_DEBUG
    printf("NotifyTypeSimple\n");
#endif // PARAMSIGPARSER_DEBUG
    if (m_inParam)
    {
        m_paramTypes.push_back({ elem_type, (sig_index_type)-1, -1, m_currTypeByRef });
    }
}

void ParamSigParser::NotifyTypeDefOrRef(sig_index_type indexType, int index)
{
#if PARAMSIGPARSER_DEBUG
    printf("NotifyTypeDefOrRef\n");
#endif // PARAMSIGPARSER_DEBUG
    if (m_inParam && m_inClass)
    {
        m_indexType = indexType;
        m_index = index;

        m_paramTypes.push_back({ ELEMENT_TYPE_CLASS, indexType, index, m_currTypeByRef });
    }
}

void ParamSigParser::NotifyTypeGenericInst(sig_elem_type elem_type, sig_index_type indexType, sig_index index, sig_mem_number number)
{
#if PARAMSIGPARSER_DEBUG
    printf("NotifyTypeGenericInst\n");
#endif // PARAMSIGPARSER_DEBUG
    m_hasUnsupportedParams = true;
}
void ParamSigParser::NotifyTypeGenericTypeVariable(sig_mem_number number)
{
#if PARAMSIGPARSER_DEBUG
    printf("NotifyTypeGenericTypeVariable\n");
#endif // PARAMSIGPARSER_DEBUG
    m_hasUnsupportedParams = true;
}
void ParamSigParser::NotifyTypeGenericMemberVariable(sig_mem_number number)
{
#if PARAMSIGPARSER_DEBUG
    printf("NotifyTypeGenericMemberVariable\n");
#endif // PARAMSIGPARSER_DEBUG
    m_hasUnsupportedParams = true;
}

void ParamSigParser::NotifyTypeValueType()
{
#if PARAMSIGPARSER_DEBUG
    printf("NotifyTypeValueType\n");
#endif // PARAMSIGPARSER_DEBUG
    m_hasUnsupportedParams = true;
}
void ParamSigParser::NotifyTypeClass()
{
#if PARAMSIGPARSER_DEBUG
    printf("NotifyTypeClass\n");
#endif // PARAMSIGPARSER_DEBUG
    m_inClass = true;
}
void ParamSigParser::NotifyTypePointer()
{
#if PARAMSIGPARSER_DEBUG
    printf("NotifyTypePointer\n");
#endif // PARAMSIGPARSER_DEBUG
    m_hasUnsupportedParams = true;
}
void ParamSigParser::NotifyTypeFunctionPointer()
{
#if PARAMSIGPARSER_DEBUG
    printf("NotifyTypeFunctionPointer\n");
#endif // PARAMSIGPARSER_DEBUG
    m_hasUnsupportedParams = true;
}
void ParamSigParser::NotifyTypeArray()
{
#if PARAMSIGPARSER_DEBUG
    printf("NotifyTypeArray\n");
#endif // PARAMSIGPARSER_DEBUG
    m_hasUnsupportedParams = true;
}
void ParamSigParser::NotifyTypeSzArray()
{
#if PARAMSIGPARSER_DEBUG
    printf("NotifyTypeSzArray\n");
#endif // PARAMSIGPARSER_DEBUG
    m_hasUnsupportedParams = true;
}

sig_elem_type ParamSigParser::GetCallingConv()
{
    return m_callingConv;
}

int ParamSigParser::GetParamCount()
{
    return m_paramCount;
}

bool ParamSigParser::HasUnsupportedParams()
{
    return m_hasUnsupportedParams;
}

vector<TypeInfo> ParamSigParser::GetParamTypes()
{
    return m_paramTypes;
}