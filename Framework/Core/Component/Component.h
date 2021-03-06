#pragma once

class IComponent
{
protected:
	class GameObject* m_pGameObject;
	class Context* m_context;

public:
	IComponent(class Context* context, class GameObject* owner) : 
		m_context(context), m_pGameObject(owner) {}
	virtual ~IComponent() = default;

	virtual void Update() = 0;

	virtual const Util::TypeID GetComponentID() const = 0;
	virtual const std::string GetComponentName() const = 0;
	class GameObject* GetOwner() const { return m_pGameObject; }
};

template <class Class>
class Component : public IComponent
{
protected:
	Component(class Context* context = nullptr, class GameObject* owner = nullptr) : 
		IComponent(context, owner) {}
	virtual ~Component() = default;

public:
	static inline const Util::TypeID ComponentID = Util::FamilyTypeID<IComponent>::GetID<Class>();

	virtual void Update() {}

	virtual const Util::TypeID GetComponentID() const override 
	{ 
		return ComponentID; 
	}
	virtual const std::string GetComponentName() const override 
	{ 
		static const std::string name = std::string(typeid(Class).name()).substr(6); 
		return name; 
	}
};