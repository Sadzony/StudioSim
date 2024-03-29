﻿#pragma once
#include <list>
#include "Component.h"
#include "SceneManager.h"

class Transform;

class GameObject
{
	GameObject();

	std::string name;
	std::list<Component*> components;
	Transform* transform {};
	bool isInitialized = false;
	bool beingDeleted = false;
	
	[[nodiscard]] Component* hasComponentInternal(const type_info &type_info) const;
	void addComponent(Component* component);
public:
	~GameObject();
	
	void start();
	void update();
	void lateUpdate();
	void destroy();
	
	[[nodiscard]] Transform* getTransform() const { return transform; }
	[[nodiscard]] bool isValidTransform() const { return transform != nullptr; }
	[[nodiscard]] std::string getName() const { return name; }
	[[nodiscard]] bool isBeingDeleted() const { return beingDeleted; }

	template<typename T, typename... Args>
	std::enable_if_t<std::is_base_of_v<Component, T>, T*> addComponent(Args... args)
	{
		T* newComponent = new T(args...);

		std::string typeName = typeid(T).name();
		//Remove "class" from this string
		typeName = typeName.erase(0, 6);

		newComponent->setName(typeName);
		newComponent->setOwner(this);
	
		if (isInitialized)
			newComponent->start();
		
		components.push_back(newComponent);
		return newComponent;
	}

	[[nodiscard]] bool hasComponent(const type_info &type_info) const;
	
	template <typename T>
	T* getComponent()
	{
		if (Component* component = hasComponentInternal(typeid(T)); component != nullptr)
		{
			return static_cast<T*>(component);
		}

		return nullptr;
	}
	
	friend class SceneManager;
	friend class ImGuiHandler;
};

