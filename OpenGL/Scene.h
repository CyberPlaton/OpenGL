#pragma once

#include<string>

#include"entt/entt.hpp"
#include"yaml-cpp/yaml.h"

#include<fstream>

#include"Entity.h"

class Scene;
class Entity;
class SceneSerializer;

struct ComponentID {
	std::string m_Tag;
	uint32_t m_ID;
};



class Scene {
public:

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






	friend class Entity;
	friend class SceneSerializer;

public:

	Scene(std::string name) :m_SceneName(name){}

	template<typename T>
	auto getEnttView() {
		return m_Registry->m_EntityRegistry.view<T>();
	}

	void createEntity(std::string name);

	void destroyEntity(Entity e);
	
	EntityRegistry* getRegistry() { return m_Registry; }

protected:

	std::string m_SceneName;

	EntityRegistry* m_Registry = EntityRegistry::get();

	uint32_t m_ViewportWidth = 0;
	uint32_t m_ViewportHeight = 0;


protected:


	template <typename T>
	void onComponentAdded(Entity entity, T& cmp){}

};



class SceneSerializer {
public:

	static bool serialize(Scene* scene, std::string filepath);
	static Scene* deserialize(std::string filepath);

private:

	static void _serializeEntity(YAML::Emitter& out, Entity e);

};




class Entity {
public:

	friend class Scene;

	Entity() = default;
	Entity(entt::entity handle, Scene* scene) : m_EntitiyHandle(handle), m_AssociatedScene(scene){}
	Entity(const Entity& rhs) = default;



	template<typename T, typename... Args>
	T& addComponent(Args&... args) {

		assert(!hasComponent<T>());

		T& cmp = m_AssociatedScene->m_Registry->m_EntityRegistry.emplace<T>(m_EntitiyHandle, std::forward<Args>(args)...);

		return cmp;
	}


	template<typename T>
	bool hasComponent() {
		return m_AssociatedScene->m_Registry->m_EntityRegistry.has<T>(m_EntitiyHandle);
	}


	template<typename T>
	T& getComponent() {
		assert(hasComponent<T>());

		return m_AssociatedScene->m_Registry->m_EntityRegistry.get<T>(m_EntitiyHandle);
	}


	template <typename T>
	void removeComponent() {
		assert(hasComponent<T>());

		m_AssociatedScene->m_Registry->m_EntityRegistry.remove<T>(m_EntitiyHandle);
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
	Scene* m_AssociatedScene = nullptr;
	entt::entity m_EntitiyHandle{ entt::null };
};



class Unit : public Entity {
public:
	Unit(std::string name) : m_Name(name) {}


	std::string m_Name;
};
