#pragma once

#include "UObject/Object.h"
#include "UObject/WeakObjectPtr.h"

template <typename BusInterface>
struct ST_BusHandlersStorage;

template <typename BusInterface>
class ST_BusImpl;

template <typename BusInterface>
class ST_BusHandlerImpl : public BusInterface
{
public:
	friend class ST_BusImpl<BusInterface>;

	virtual ~ST_BusHandlerImpl()
	{
		bIsDestructing = true;
		BusDisconnect();
	}

	template <typename ConcreteType>
	void BusConnect(ConcreteType* ConcreteThis);
	void BusDisconnect();
	bool BusIsConnected() const;
	bool IsObjectValid() const;

private:
	// Tracks which storage we're registered with. 
	ST_BusHandlersStorage<BusInterface>* ConnectionStorage = nullptr;

	// Used to safely gate dispatch for UObject-derived handlers that haven't actually been destructed, but are pending GC.
	TWeakObjectPtr<UObject> WeakObject = nullptr;

	// Set while disconnecting due to destruction, so we avoid invocations during queued disconnections.
	bool bIsDestructing = false;
};

template <typename BusInterface>
struct ST_BusHandlersStorage
{
	using BusHandlerT = ST_BusHandlerImpl<BusInterface>;

	TSet<BusHandlerT*> Listeners;
	int BroadcastDepth = 0;

	TSet<BusHandlerT*> QueuedConnects;
	TSet<BusHandlerT*> QueuedDisconnects;
};

template <typename BusInterface>
struct ST_BusStorage
{
	using StorageT = ST_BusHandlersStorage<BusInterface>;

	static StorageT* GetStorage()
	{
		static StorageT BusStorage;
		return &BusStorage;
	}
};

template <typename BusInterface>
class ST_BusImpl
{
public:
	using BusHandlerImplT = ST_BusHandlerImpl<BusInterface>;
	using BusStorageT = ST_BusHandlersStorage<BusInterface>;

	friend class ST_BusHandlerImpl<BusInterface>;

	~ST_BusImpl()
	{
		BusStorageT* Storage = ST_BusStorage<BusInterface>::GetStorage();
		check(Storage->Listeners.IsEmpty());
		check(Storage->BroadcastDepth == 0);
	}

	static bool HasListeners()
	{
		BusStorageT* Storage = ST_BusStorage<BusInterface>::GetStorage();
		return !Storage->Listeners.IsEmpty();
	}

	static int32 GetNumListeners()
	{
		BusStorageT* Storage = ST_BusStorage<BusInterface>::GetStorage();
		return Storage->Listeners.Num();
	}

	template <typename RetT, typename InterfaceT, typename... ParamsT, typename... ArgsT>
	static void Broadcast(RetT(InterfaceT::* InterfaceFunc)(ParamsT...), ArgsT&&... args)
	{
		BusStorageT* Storage = ST_BusStorage<BusInterface>::GetStorage();
		
		++Storage->BroadcastDepth;

		for (BusHandlerImplT* Listener : Storage->Listeners)
		{
			if (Storage->QueuedDisconnects.Contains(Listener))
			{
				continue;
			}
			if (!Listener->IsObjectValid())
			{
				continue;
			}

			BusInterface* Interface = static_cast<BusInterface*>(Listener);
			(Interface->*InterfaceFunc)(std::forward<ArgsT>(args)...);
		}

		check(Storage->BroadcastDepth > 0);
		--Storage->BroadcastDepth;

		CheckFlushQueuedDisconnects();
		CheckFlushQueuedConnects();
	}

private:
	static void Connect(BusHandlerImplT* Handler)
	{
		BusStorageT* Storage = ST_BusStorage<BusInterface>::GetStorage();
		if (!ensure(!Handler->bIsDestructing))
		{
			return;
		}

		// Make sure we remove if it was still queued for disconnection.
		if (Storage->QueuedDisconnects.Contains(Handler))
		{
			Storage->QueuedDisconnects.Remove(Handler);
		}

		if (Handler->ConnectionStorage == Storage)
		{
			return;
		}

		if (Storage->BroadcastDepth > 0)
		{
			// We're mid-broadcast. Enqueue the connection so we can do it safely after broadcasting is complete.
			Storage->QueuedConnects.Add(Handler);
			return;
		}

		Storage->Listeners.Add(Handler);
		Handler->ConnectionStorage = Storage;
	}

	static void Disconnect(BusHandlerImplT* Handler)
	{
		BusStorageT* Storage = ST_BusStorage<BusInterface>::GetStorage();
		if (!Storage)
		{
			// Not connected to anything.
			return;
		}

		// Make sure we remove if it was still queued for connection.
		Storage->QueuedConnects.Remove(Handler);

		if (Storage->BroadcastDepth > 0)
		{
			// We're mid-broadcast. Enqueue the disconnect so we can do it safely after broadcasting is complete.
			Storage->QueuedDisconnects.Add(Handler);
			return;
		}

		Storage->Listeners.Remove(Handler);
		Handler->ConnectionStorage = nullptr;
	}

	static void CheckFlushQueuedDisconnects()
	{
		BusStorageT* Storage = ST_BusStorage<BusInterface>::GetStorage();
		if (Storage->BroadcastDepth > 0)
		{
			return;
		}

		for (BusHandlerImplT* Handler : Storage->QueuedDisconnects)
		{
			Disconnect(Handler);
		}

		Storage->QueuedDisconnects.Empty();
	}

	static void CheckFlushQueuedConnects()
	{
		BusStorageT* Storage = ST_BusStorage<BusInterface>::GetStorage();
		if (Storage->BroadcastDepth > 0)
		{
			return;
		}

		for (BusHandlerImplT* Handler : Storage->QueuedConnects)
		{
			Connect(Handler);
		}

		Storage->QueuedConnects.Empty();
	}
};

/**
 * Bus class, inherits from underlying implementation.
 */
template <typename BusInterface>
class ST_Bus : public ST_BusImpl<BusInterface>
{
public:
	// Easy-accessor for Bus Events (the interface). e.g. Useful for calling Broadcast(&FST_MyBus::Events::SomeFunction, ...).
	using Events = BusInterface;

	// Easy-accessor for Bus' Handler class. e.g. Useful for inheriting, class MyListener : FST_MyBus::Handler {}.
	using Handler = ST_BusHandlerImpl<BusInterface>;
};

/**
 * Handy alias for a Bus' handler class, e.g. class MyListener : public ST_BusHandler<ST_SomeBus> {}.
 */
template <typename BusInterface>
using ST_BusHandler = typename ST_Bus<BusInterface>::Handler;


template <typename BusInterface>
template <typename ConcreteType>
void ST_BusHandlerImpl<BusInterface>::BusConnect(ConcreteType* ConcreteThis)
{
	ensureMsgf(this == static_cast<ST_BusHandlerImpl*>(ConcreteThis), TEXT("Supplied ConcreteThis is not the same object as the handler."));

	if constexpr (std::is_base_of<UObject, ConcreteType>())
	{
		WeakObject = ConcreteThis;
	}

	ST_BusImpl<BusInterface>::Connect(this);
}

template <typename BusInterface>
void ST_BusHandlerImpl<BusInterface>::BusDisconnect()
{
	if (ConnectionStorage == nullptr)
	{
		return;
	}

	ST_BusImpl<BusInterface>::Disconnect(this);
}

template <typename BusInterface>
bool ST_BusHandlerImpl<BusInterface>::BusIsConnected() const
{
	return ConnectionStorage != nullptr;
}

template <typename BusInterface>
bool ST_BusHandlerImpl<BusInterface>::IsObjectValid() const
{
	return !bIsDestructing && (WeakObject.IsExplicitlyNull() || WeakObject.IsValid());
}
