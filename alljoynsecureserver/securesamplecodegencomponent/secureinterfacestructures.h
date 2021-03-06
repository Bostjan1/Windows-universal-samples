//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the Microsoft Public License.
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#pragma once

namespace org { namespace alljoyn { namespace Bus { namespace Samples { namespace Secure {

public ref class AllJoynMessageArgStructure sealed : Windows::Foundation::Collections::IVector<Platform::Object^>
{
public:
    AllJoynMessageArgStructure()
    {
        m_vector = ref new Platform::Collections::Vector<Platform::Object^>();
    }

    virtual Windows::Foundation::Collections::IIterator<Platform::Object^>^ First()
    {
        return m_vector->First();
    }

    virtual Platform::Object^ GetAt(unsigned int index)
    {
        return m_vector->GetAt(index);
    }

    virtual property unsigned int Size {
        virtual unsigned int get()
        {
            return m_vector->Size;
        }
    }

    virtual bool IndexOf(Platform::Object^ value, unsigned int * index)
    {
        return m_vector->IndexOf(value, index);
    }

    virtual unsigned int GetMany(unsigned int startIndex, Platform::WriteOnlyArray<Platform::Object^>^ dest)
    {
        return m_vector->GetMany(startIndex, dest);
    }

    virtual Windows::Foundation::Collections::IVectorView<Platform::Object^>^ GetView()
    {
        return m_vector->GetView();
    }

    virtual void SetAt(unsigned int index, Platform::Object^ item)
    {
        return m_vector->SetAt(index, item);
    }

    virtual void InsertAt(unsigned int index, Platform::Object^ item)
    {
        return m_vector->InsertAt(index, item);
    }

    virtual void Append(Platform::Object^ item)
    {
        return m_vector->Append(item);
    }

    virtual void RemoveAt(unsigned int index)
    {
        return m_vector->RemoveAt(index);
    }

    virtual void RemoveAtEnd()
    {
        return m_vector->RemoveAtEnd();
    }

    virtual void Clear()
    {
        return m_vector->Clear();
    }

    virtual void ReplaceAll(const Platform::Array<Platform::Object^>^ arr)
    {
        return m_vector->ReplaceAll(arr);
    }

private:
    Platform::Collections::Vector<Platform::Object^>^ m_vector;
};
} } } } } 

partial ref class TypeConversionHelpers
{
internal:
};