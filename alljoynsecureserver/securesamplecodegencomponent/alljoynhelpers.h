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

// The amount of time to wait (ms) for a response after sending a message before timing out.
const int c_MessageTimeoutInMilliseconds = 10000;

// The maximum length of an AllJoyn type signature allowed by the AllJoyn Core library.
const int c_MaximumSignatureLength = 255;

#define RETURN_IF_QSTATUS_ERROR(status) \
{   \
int32 alljoynStatus = static_cast<int32>(status);    \
if (Windows::Devices::AllJoyn::AllJoynStatus::Ok != alljoynStatus)  \
{   \
    return status;   \
}   \
}

class AllJoynHelpers
{
public:
    // The Windows::Devices::AllJoyn::AllJoynBusAttachment class wraps the alljoyn_busattachment type.  This
    // function gets the underlying alljoyn_busattachment.
    static alljoyn_busattachment GetInternalBusAttachment(_In_ Windows::Devices::AllJoyn::AllJoynBusAttachment^ busAttachment);

    // Create the alljoyn_interfacedescriptions described in introspectionXml and add them to the busAttachment.
    static QStatus CreateInterfaces(_Inout_ Windows::Devices::AllJoyn::AllJoynBusAttachment^ busAttachment, _In_ PCSTR introspectionXml);

    // Convert a UTF8 string to a wide character Platform::String.
    static Platform::String^ MultibyteToPlatformString(_In_ PCSTR);

    // Convert a wide character Platform::String to a UTF8 string.
    static std::vector<char> PlatformToMultibyteString(_In_ Platform::String^ str);

    // Get the service object path from an objectDescriptionArg.  The objectDescriptionArg should
    // come from an Announce signal.
    static Platform::String^ AllJoynHelpers::GetObjectPath(_In_ alljoyn_aboutobjectdescription objectDescription, _In_ PCSTR interfaceName);

    // Callback for alljoyn_about_announced_ptr.
    // This callback expects the context to be of type T, which must implement the OnAnnounce function.
    template<class T>
    static void AnnounceHandler(
        _In_ const void* context,
        _In_ PCSTR name,
        _In_ uint16_t version,
        _In_ alljoyn_sessionport port,
        _In_ alljoyn_msgarg objectDescriptionArg,
        _In_ const alljoyn_msgarg aboutDataArg)
    {
        T^ caller = static_cast<const Platform::WeakReference*>(context)->Resolve<T>();
        caller->OnAnnounce(name, version, port, objectDescriptionArg, aboutDataArg);
    }

    // Callback for alljoyn_proxybusobject_listener_propertieschanged_ptr.
    // This callback expects the context to be of type T, which must implement the OnPropertyChanged function.
    template<class T>
    static void PropertyChangedHandler(_In_ alljoyn_proxybusobject obj, _In_ PCSTR interfaceName, _In_ const alljoyn_msgarg changed, _In_ const alljoyn_msgarg invalidated, _In_ void* context)
    {
        T^ caller = static_cast<const Platform::WeakReference*>(context)->Resolve<T>();
        caller->OnPropertyChanged(obj, interfaceName, changed, invalidated);
    }

    // Callback for alljoyn_busobject_prop_get_ptr.
    template<class T>
    static QStatus PropertyGetHandler(_In_ const void* context, _In_ PCSTR interfaceName, _In_ PCSTR propertyName, _In_ alljoyn_msgarg value)
    {
        T^ caller = static_cast<const Platform::WeakReference*>(context)->Resolve<T>();
        return caller->OnPropertyGet(interfaceName, propertyName, value);
    }

    // Callback for alljoyn_busobject_prop_set_ptr.
    template<class T>
    static QStatus PropertySetHandler(_In_ const void* context, _In_ PCSTR interfaceName, _In_ PCSTR propertyName, _In_ alljoyn_msgarg value)
    {
        T^ caller = static_cast<const Platform::WeakReference*>(context)->Resolve<T>();
        return caller->OnPropertySet(interfaceName, propertyName, value);
    }

    // Callback for alljoyn_sessionlistener_sessionlost_ptr.
    template<class T>
    static void SessionLostHandler(_In_ const void* context, _In_ alljoyn_sessionid sessionId, _In_ alljoyn_sessionlostreason reason)
    {
        T^ caller = static_cast<const Platform::WeakReference*>(context)->Resolve<T>();
        caller->OnSessionLost(sessionId, reason);
    }

    // Callback for alljoyn_sessionlistener_sessionmemberadded_ptr.
    template<class T>
    static void SessionMemberAddedHandler(_In_ const void* context, _In_ alljoyn_sessionid sessionId, _In_ PCSTR uniqueName)
    {
        T^ caller = static_cast<const Platform::WeakReference*>(context)->Resolve<T>();
        caller->OnSessionMemberAdded(sessionId, uniqueName);
    }

    // Callback for alljoyn_sessionlistener_sessionmemberremoved_ptr.
    template<class T>
    static void SessionMemberRemovedHandler(_In_ const void* context, _In_ alljoyn_sessionid sessionId, _In_ PCSTR uniqueName)
    {
        T^ caller = static_cast<const Platform::WeakReference*>(context)->Resolve<T>();
        caller->OnSessionMemberRemoved(sessionId, uniqueName);
    }

    // Create an AllJoyn bus object.
    template<class T>
    static QStatus CreateBusObject(_Inout_ Platform::WeakReference* target)
    {
        alljoyn_busobject_callbacks callbacks =
        {
            PropertyGetHandler<T>,
            PropertySetHandler<T>,
            nullptr,
            nullptr,
        };

        T^ caller = target->Resolve<T>();

        auto serviceObjectPath = PlatformToMultibyteString(caller->ServiceObjectPath);
        alljoyn_busobject busObject = alljoyn_busobject_create(serviceObjectPath.data(), false, &callbacks, target);
        if (busObject == nullptr)
        {
            return ER_FAIL;
        }

        caller->BusObject = busObject;

        return ER_OK;
    }

    // Callback for alljoyn_sessionportlistener_acceptsessionjoiner_ptr.
    // This callback expects the context to be of type T, which must implement the OnAcceptSessionJoiner function.
    template<class T>
    static QCC_BOOL AcceptSessionJoinerHandler(
        _In_ const void* context,
        _In_ alljoyn_sessionport sessionPort,
        _In_ PCSTR joiner,
        _In_ const alljoyn_sessionopts opts)
    {
        T^ caller = static_cast<const Platform::WeakReference*>(context)->Resolve<T>();
        return caller->OnAcceptSessionJoiner(sessionPort, joiner, opts);
    }

    // Callback for alljoyn_sessionportlistener_sessionjoined_ptr.
    // This callback expects the context to be of type T, which must implement the OnSessionJoined function.
    template<class T>
    static void SessionJoinedHandler(
        _In_ const void* context,
        _In_ alljoyn_sessionport sessionPort,
        _In_ alljoyn_sessionid id,
        _In_ PCSTR joiner)
    {
        T^ caller = static_cast<const Platform::WeakReference*>(context)->Resolve<T>();
        caller->OnSessionJoined(sessionPort, id, joiner);
    }

    // Create the session for an AllJoyn producer.  
    template<class T>
    static QStatus CreateProducerSession(_Inout_ Windows::Devices::AllJoyn::AllJoynBusAttachment^ busAttachment, _Inout_ Platform::WeakReference* target)
    {
        alljoyn_sessionopts opts = alljoyn_sessionopts_create(ALLJOYN_TRAFFIC_TYPE_MESSAGES, true, ALLJOYN_PROXIMITY_ANY, ALLJOYN_TRANSPORT_ANY);

        alljoyn_sessionportlistener_callbacks callbacks =
        {
            AcceptSessionJoinerHandler<T>,
            SessionJoinedHandler<T>
        };

        T^ producer = target->Resolve<T>();
        producer->SessionPortListener = alljoyn_sessionportlistener_create(&callbacks, target);

        alljoyn_sessionport sessionPort = 42;
        alljoyn_busattachment_unbindsessionport(AllJoynHelpers::GetInternalBusAttachment(busAttachment), sessionPort);
        RETURN_IF_QSTATUS_ERROR(alljoyn_busattachment_bindsessionport(AllJoynHelpers::GetInternalBusAttachment(busAttachment), &sessionPort, opts, producer->SessionPortListener));
        producer->SessionPort = sessionPort;

        alljoyn_sessionopts_destroy(opts);

        return ER_OK;
    }
};

// Passed to property get callbacks to allow them to report when the async operation is completed.
template<class T>
class PropertyGetContext
{
public:
    void SetEvent()
    {
        m_event.set();
    }

    void Wait()
    {
        m_event.wait();
    }

    QStatus GetStatus()
    {
        return m_status;
    }

    void SetStatus(QStatus value)
    {
        m_status = value;
    }

    T GetValue()
    {
        return m_value;
    }

    void SetValue(T value)
    {
        m_value = value;
    }

private:
    Concurrency::event m_event;
    QStatus m_status;
    T m_value;
};

// Passed to property set callbacks to allow them to report when the async operation is completed.
class PropertySetContext
{
public:
    void SetEvent()
    {
        m_event.set();
    }

    void Wait()
    {
        m_event.wait();
    }

    QStatus GetStatus()
    {
        return m_status;
    }

    void SetStatus(QStatus value)
    {
        m_status = value;
    }

private:
    Concurrency::event m_event;
    QStatus m_status;
};
