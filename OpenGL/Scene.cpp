#include"Scene.h"

Scene::EntityRegistry* Scene::EntityRegistry::g_RegistryObject = nullptr;


void Scene::createEntity(std::string name) {

	Entity e;
	e.m_EntitiyHandle = m_Registry->m_EntityRegistry.create();
	
	e.m_AssociatedScene = this;

	// Construct blank entity with tag(Debug name) and id(assigned by entt).
	auto& id = e.addComponent< ComponentID >();
	id.m_Tag = name;
	id.m_ID = uint32_t(e);
}


void Scene::destroyEntity(Entity e) {

	m_Registry->m_EntityRegistry.destroy(e);
}


bool SceneSerializer::serialize(Scene* scene, std::string filepath) {

	using namespace YAML;
	YAML::Emitter out;

	out << BeginMap;
	out << Key << "Scene" << Value << scene->m_SceneName;

	out << Key << "Entities" << Value << BeginSeq;


	

	scene->m_Registry->m_EntityRegistry.each([&](auto entity) {

		//Entity e = scene->m_Registry->m_EntityRegistry.get(entity);
		//if (!e) return;

		Entity e = { entity , scene};

		_serializeEntity(out,  e);

	});

	out << EndSeq;
	out << EndMap;

	std::ofstream fout(filepath);
	fout << out.c_str();

	return true;
}


Scene* SceneSerializer::deserialize(std::string filepath) {


	return nullptr;
}


void SceneSerializer::_serializeEntity(YAML::Emitter& out, Entity e){

	using namespace YAML;

	out << BeginMap; // An entity map
	out << Key << "Entity" << Value << uint32_t(e);

	if (e.hasComponent<ComponentID>()) {

		out << Key << "ComponentID";
		out << BeginMap;

		auto& id = e.getComponent<ComponentID>();

		out << Key << "Tag" << Value << id.m_Tag;
		out << Key << "ID" << Value << id.m_ID;

		out << EndMap;
	}


	out << EndMap;
}
