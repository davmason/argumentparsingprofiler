
#pragma once

#include <cstddef>

#include "cor.h"
#include "corhdr.h"
#include "corerror.h"
#include "corprof.h"

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

template <class T>
class NewArrayHolder
{
public:
    NewArrayHolder()
    {
        m_pArray = NULL;
    }

    NewArrayHolder(T* pArray)
    {
        m_pArray = pArray;
    }

    NewArrayHolder<T>& operator=(T* other)
    {
        delete[] m_pArray;
        m_pArray = other;
        return *this;
    }

    operator T*()
    {
        return m_pArray;
    }

    ~NewArrayHolder()
    {
        delete[] m_pArray;
    }
private:
    NewArrayHolder(const NewArrayHolder& other)
    {
        assert(!"Unreachable");
    }

    T* m_pArray;
};
