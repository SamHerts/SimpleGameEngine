//
// Created by SamHerts on 5/27/2025.
//

#ifndef COORDINATOR_H
#define COORDINATOR_H

#include "ComponentManager.h"
#include "EntityManager.h"
#include "EventManager.h"
#include "SystemManager.h"
#include "Types.h"
#include <memory>


class Coordinator
{
	std::unique_ptr<ComponentManager> mComponentManager;
	std::unique_ptr<EntityManager> mEntityManager;
	std::unique_ptr<EventManager> mEventManager;
	std::unique_ptr<SystemManager> mSystemManager;
public:
	void Init()
	{
		mComponentManager = std::make_unique<ComponentManager>();
		mEntityManager = std::make_unique<EntityManager>();
		mEventManager = std::make_unique<EventManager>();
		mSystemManager = std::make_unique<SystemManager>();
	}


	// Entity methods
	Entity CreateEntity() const
	{
		return mEntityManager->CreateEntity();
	}

	void DestroyEntity(const Entity entity) const
	{
		mEntityManager->DestroyEntity(entity);

		mComponentManager->EntityDestroyed(entity);

		mSystemManager->EntityDestroyed(entity);
	}


	// Component methods
	template<typename T>
	void RegisterComponent() const
	{
		mComponentManager->RegisterComponent<T>();
	}

	template<typename T>
	void AddComponent(const Entity entity, T component) const
	{
		mComponentManager->AddComponent<T>(entity, component);

		auto signature = mEntityManager->GetSignature(entity);
		signature.set(mComponentManager->GetComponentType<T>(), true);
		mEntityManager->SetSignature(entity, signature);

		mSystemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename... Components>
	void AddComponents(const Entity entity, Components&&... components) const
	{
		auto signature = mEntityManager->GetSignature(entity);

		((
		mComponentManager->AddComponent<std::decay_t<Components>>(entity, std::forward<Components>(components)),
		signature.set(mComponentManager->GetComponentType<std::decay_t<Components>>(), true)
		), ... );

		mEntityManager->SetSignature(entity, signature);
		mSystemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	void RemoveComponent(const Entity entity) const
	{
		mComponentManager->RemoveComponent<T>(entity);

		auto signature = mEntityManager->GetSignature(entity);
		signature.set(mComponentManager->GetComponentType<T>(), false);
		mEntityManager->SetSignature(entity, signature);

		mSystemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	T& GetComponent(const Entity entity) const
	{
		return mComponentManager->GetComponent<T>(entity);
	}

	template<typename T>
	bool HasComponent(const Entity entity) const
	{
		auto signature = mEntityManager->GetSignature(entity);
		return signature[mComponentManager->GetComponentType<T>()];
	}

	template<typename T>
	ComponentType GetComponentType() const
	{
		return mComponentManager->GetComponentType<T>();
	}


	// System methods
	template<typename T>
	std::shared_ptr<T> RegisterSystem() const
	{
		return mSystemManager->RegisterSystem<T>();
	}

	template<typename T>
	std::shared_ptr<T> GetSystem() const
	{
		return mSystemManager->GetSystem<T>();
	}

	template<typename T>
	void SetSystemSignature(Signature signature) const
	{
		mSystemManager->SetSignature<T>(signature);
	}


	// Event methods
	void AddEventListener(EventId eventId, std::function<void(Event&)> const& listener) const
	{
		mEventManager->AddListener(eventId, listener);
	}

	void SendEvent(Event& event) const
	{
		mEventManager->SendEvent(event);
	}

	void SendEvent(EventId eventId) const
	{
		mEventManager->SendEvent(eventId);
	}
};


#endif //COORDINATOR_H
