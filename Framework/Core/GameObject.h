#pragma once

class GameObject final
{
	friend class Scene;
public:
	GameObject(class Context* context);
	~GameObject();
	
	GameObject(const GameObject& rhs) = delete;
	GameObject& operator=(const GameObject& rhs) = delete;

	void Update();

	template <class T>
	T* AddComponent();
	template <class T>
	T* GetComponent() const;
	template <class T>
	void RemoveComponent();

	__declspec(property(get = GetName, put = SetName)) std::string name;
	std::string GetName() const { return m_name; }
	void SetName(std::string name) { m_name = name; }

private:
	class Context* m_context;
	class Scene* m_scene;
	uint32_t m_instanceid;

	std::string	m_name;
	std::vector<class IComponent*> m_components;
};


template<class T>
inline T* GameObject::AddComponent()
{
	assert(m_components[T::ComponentID] == nullptr, "Component already exists");

	m_components[T::ComponentID] = m_context->GetSubsystem<ComponentManager>()->CreateComponent<T>(this);
	return static_cast<T*>(m_components[T::ComponentID]);
}

template<class T>
inline T * GameObject::GetComponent() const
{
	return static_cast<T*>(m_components[T::ComponentID]);
}

template<class T>
inline void GameObject::RemoveComponent()
{
	if (m_components[T::ComponentID]) {
		m_context->GetSubsystem<ComponentManager>()->RemoveComponent<T>(m_components[T::ComponentID]);
		m_components[T::ComponentID] = nullptr;
	}
}