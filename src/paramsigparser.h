#pragma once

#include "sigparser.h"

#include <vector>

struct TypeInfo
{
    sig_elem_type   type;
    sig_index_type  indexType;
    int             index;
    bool            byref;
};

class ParamSigParser :
    public SigParser
{
private:
    int m_paramCount;
    sig_elem_type m_callingConv;
    bool m_currTypeByRef;
    sig_index_type  m_indexType;
    int m_index;
    std::vector<TypeInfo> m_paramTypes;
    bool m_hasGenericParams;

protected:
    virtual void NotifyCallingConvention(sig_elem_type callingConv);

    virtual void NotifyParamCount(sig_count count);
    virtual void NotifyGenericParamCount(sig_count count);

    virtual void NotifyBeginParam();
    virtual void NotifyEndParam();

    virtual void NotifyBeginType();
    virtual void NotifyEndType();

    virtual void NotifyTypeSimple(sig_elem_type elem_type);

    virtual void NotifyByref();

    virtual void NotifyTypeDefOrRef(sig_index_type indexType, int index);
public:
    ParamSigParser();
    ~ParamSigParser() = default;

    sig_elem_type getCallingConv();
    int getParamCount();
    bool hasGenericParams();
    std::vector<TypeInfo> getParamTypes();
};

