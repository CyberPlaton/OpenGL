#pragma once

#include"entt/entt.hpp"

class EntityRegistry {
public:
	static EntityRegistry* get() {
		if (!g_RegistryObject) g_RegistryObject = new EntityRegistry();
		
	
		return g_RegistryObject;
	}

	static void del() {
		if (g_RegistryObject) delete g_RegistryObject;
	}


	entt::registry m_EntityRegistry;

private:
	static EntityRegistry* g_RegistryObject;

private:
	EntityRegistry() = default;
};



class Entity {
public:
	Entity() {
		m_EntitiyHandle = EntityRegistry::get()->m_EntityRegistry.create();
	}

	Entity(const Entity& rhs) = default;



	template<typename T, typename... Args>
	T& addComponent(Args&... args) {
		
		assert(!hasComponent<T>());

		T& cmp = EntityRegistry::get()->m_EntityRegistry.emplace<T>(m_EntitiyHandle, std::forward<Args>(args)...);

		return cmp;
	}


	template<typename T>
	bool hasComponent() {
		return EntityRegistry::get()->m_EntityRegistry.has<T>(m_EntitiyHandle);
	}


	template<typename T>
	T& getComponent() {
		assert(hasComponent<T>());

		return EntityRegistry::get()->m_EntityRegistry.get<T>(m_EntitiyHandle);
	}


	template <typename T>
	void removeComponent() {
		assert(hasComponent<T>());

		EntityRegistry::get()->m_EntityRegistry.remove<T>(m_EntitiyHandle);
	}


	operator bool()const { return m_EntitiyHandle != entt::null; }
	operator entt::entity() const { return m_EntitiyHandle; }
	operator uint32_t() const { return (uint32_t)m_EntitiyHandle; }

	bool operator == (const Entity& rhs)const {
		return m_EntitiyHandle == rhs.m_EntitiyHandle;
	}

	bool operator != (const Entity& rhs)const {
		return m_EntitiyHandle != rhs.m_EntitiyHandle;
	}


protected:
	entt::entity m_EntitiyHandle{ entt::null };
};



struct TransformComponent {
	float x, y;
};

class Unit : public Entity{
public:
	Unit(std::string name) : m_Name(name){}


	std::string m_Name;
};
