#include "paramsigparser.h"

#include <assert.h>

using std::vector;

ParamSigParser::ParamSigParser() :
    m_paramCount(0),
    m_callingConv(),
    m_currTypeByRef(false),
    m_indexType(-1),
    m_index(-1),
    m_paramTypes()
{
}

void ParamSigParser::NotifyCallingConvention(sig_elem_type callingConv)
{
    m_callingConv = callingConv;
}

void ParamSigParser::NotifyParamCount(sig_count count)
{
    m_paramCount = count;
}

void ParamSigParser::NotifyBeginParam()
{
}

void ParamSigParser::NotifyEndParam()
{
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
    m_paramTypes.push_back({ elem_type, m_indexType, m_index, m_currTypeByRef });
}

void ParamSigParser::NotifyTypeDefOrRef(sig_index_type indexType, int index)
{
    m_indexType = indexType;
    m_index = index;
}

sig_elem_type ParamSigParser::getCallingConv()
{
    return m_callingConv;
}

int ParamSigParser::getParamCount()
{
    return m_paramCount;
}

vector<TypeInfo> ParamSigParser::getParamTypes()
{
    return m_paramTypes;
}