#include"ParticleSystem.h"

std::mt19937 Random::s_RandomEngine;
std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;

float common_lerp(float x, float y, float t) {
	return x * (1.f - t) + y * t;
}

ParticleSystem::ParticleSystem(Sprite* sprite){

	m_ParticlePool.resize(1000);

	m_ParticleSprite = sprite;
}



void ParticleSystem::onUpdate(float dt, glm::vec3 camerPos) {

	
	for (Particle& particle : m_ParticlePool)
	{
		if (!particle.m_Active)
			continue;

		if (particle.m_LifeRemaining <= 0.0f)
		{
			particle.m_Active = false;
			particle.m_CurrentLifeTime = 0.0f;
			particle.m_LifeRemaining = 0.0f;
			continue;
		}

		particle.m_LifeRemaining = (particle.m_LifeRemaining - dt < 0.0f) ? 0.0f : particle.m_LifeRemaining -= dt;

		particle.m_Position += particle.m_Velocity * (float)dt;

		particle.m_Rotation += dt * particle.m_RotationDirection; // Random Range of rotation -dt to dt.
	}
}


void ParticleSystem::onRender() {


	for (auto& particle : m_ParticlePool)
	{
		if (!particle.m_Active)
			continue;

		// Fade away particles
		float life = particle.m_LifeRemaining / particle.m_CurrentLifeTime;


		//glm::vec4 color = glm::mix(particle.m_ColorEnd, particle.m_ColorBegin, life);
		//color.a = color.a * life;

		glm::vec4 color = glm::vec4(1.0f);
		color.r = common_lerp(particle.m_ColorEnd.r, particle.m_ColorBegin.r, life);
		color.g = common_lerp(particle.m_ColorEnd.g, particle.m_ColorBegin.g, life);
		color.b = common_lerp(particle.m_ColorEnd.b, particle.m_ColorBegin.b, life);
		color.a = color.a*life;


		float size = common_lerp(particle.m_SizeEnd, particle.m_SizeBegin, life);


		m_ParticleSprite->SetColor(glm::vec3(color.x, color.y, color.z));
		m_ParticleSprite->SetBrightness(color.a);
		m_ParticleSprite->SetScale(size);
		m_ParticleSprite->SetPosition(particle.m_Position);
		m_ParticleSprite->SetRotation(particle.m_Rotation);

		m_ParticleSprite->Draw();
	}
}



void ParticleSystem::emit() {

	Particle& particle = m_ParticlePool[m_PoolIndex];
	particle.m_Active = true;

	switch (m_EmitPositionMode) {
	case EmitPositionMode::Invalid:
		break;


	case EmitPositionMode::Fixed_On_Object:
		// For it we need an abstract entity-transform component
		// so that we always get the current location of Object.
		return; // Currently not implemented.
		break;



	case EmitPositionMode::Fixed_To_Space:
		// For it we need a rectangle definition (for now)
		// a Point in space and a Width,
		// thus we can define location random in the rectangle.
		if (!m_EmitModeB) return;

		particle.m_Position = glm::vec2(m_EmitModeB->m_Point.x + Random::Float()* m_EmitModeB->m_Width, m_EmitModeB->m_Point.y + Random::Float() * m_EmitModeB->m_Width);

		break;


	default:
		break;

	}


	//particle.m_Rotation = Random::Float() * (m_ParticleData->m_Rotation + (Random::Float() + m_ParticleData->m_RotationVariation));

	particle.m_Rotation = m_ParticleData->m_Rotation + Random::Float() * m_ParticleData->m_RotationVariation;
	particle.m_RotationDirection = m_ParticleData->m_RotationDir;

	// Velocity
	particle.m_Velocity = m_ParticleData->m_Velocity;
	particle.m_Velocity.x += m_ParticleData->m_VelocityVariation.x * (Random::Float() - 0.5f);
	particle.m_Velocity.y += m_ParticleData->m_VelocityVariation.y * (Random::Float() - 0.5f);


	// Color
	particle.m_ColorBegin = m_ParticleData->m_ColorBegin;
	particle.m_ColorEnd = m_ParticleData->m_ColorEnd;

	particle.m_CurrentLifeTime = m_ParticleData->m_MaxLifeTime;
	particle.m_LifeRemaining = m_ParticleData->m_MaxLifeTime;
	particle.m_SizeBegin = m_ParticleData->m_SizeBegin + m_ParticleData->m_SizeVariation * (Random::Float() - 0.5f);
	particle.m_SizeEnd = m_ParticleData->m_SizeEnd;



	m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();

	/*
	Particle& particle = m_ParticlePool[m_PoolIndex];
	particle.m_Active = true;
	particle.m_Position = m_ParticleData->m_Position;
	particle.m_Rotation = Random::Float() * 2.0f * glm::pi<float>();

	// Velocity
	particle.m_Velocity = m_ParticleData->m_Velocity;
	particle.m_Velocity.x += m_ParticleData->m_VelocityVariation.x * (Random::Float() - 0.5f);
	particle.m_Velocity.y += m_ParticleData->m_VelocityVariation.y * (Random::Float() - 0.5f);
	
	// Color
	particle.m_ColorBegin = m_ParticleData->m_ColorBegin;
	particle.m_ColorEnd = m_ParticleData->m_ColorEnd;

	particle.m_LifeTime = m_ParticleData->m_LifeTime;
	particle.m_LifeRemaining = m_ParticleData->m_LifeTime;
	particle.m_SizeBegin = m_ParticleData->m_SizeBegin + m_ParticleData->m_SizeVariation * (Random::Float() - 0.5f);
	particle.m_SizeEnd = m_ParticleData->m_SizeEnd;

	m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
	*/
}

ParticleSystem* ParticleSystem::createFromFile(std::string file) {

	using namespace std;

	Sprite* sprite = nullptr;
	ParticleSystem* ps = nullptr;

	try {

		YAML::Node node = YAML::LoadFile(file);

		// Create Particle system.
		sprite = new Sprite("shaderTexture", node["texture"].as<std::string>());
		ps = new ParticleSystem(sprite);

		// Initialize data for population.
		ps->m_ParticleData = new ParticleData();

		ps->m_ParticleData->m_Position = glm::vec2(node["position"][0].as<float>(), node["position"][1].as<float>());

		ps->m_ParticleData->m_Velocity = glm::vec2(node["velocity"][0].as<float>(), node["velocity"][1].as<float>());

		ps->m_ParticleData->m_VelocityVariation = glm::vec2(node["velocityVar"][0].as<float>(), node["velocityVar"][1].as<float>());

		ps->m_ParticleData->m_ColorBegin = glm::vec4(node["colorBegin"][0].as<float>(), node["colorBegin"][1].as<float>(), node["colorBegin"][2].as<float>(), node["colorBegin"][3].as<float>());

		ps->m_ParticleData->m_ColorEnd = glm::vec4(node["colorEnd"][0].as<float>(), node["colorEnd"][1].as<float>(), node["colorEnd"][2].as<float>(), node["colorEnd"][3].as<float>());

		ps->m_ParticleData->m_MaxLifeTime = node["lifeTime"].as<float>();

		ps->m_ParticleData->m_SizeBegin = node["sizeBegin"].as<float>();

		ps->m_ParticleData->m_SizeEnd = node["sizeEnd"].as<float>();

		ps->m_ParticleData->m_SizeVariation = node["sizeVar"].as<float>();

		ps->m_ParticleData->m_Rotation = node["rotation"].as<float>();

		ps->m_ParticleData->m_RotationVariation = node["rotationVar"].as<float>();

		ps->m_ParticleData->m_RotationDir = node["rotationDir"].as<int>();

		// Get the emit mode.
		if (node["emitMode"].as<std::string>().compare("A") == 0) { // Emit mode A

			// Not implemented.
		}
		else { // Emit mode B

			ps->m_EmitPositionMode = EmitPositionMode::Fixed_To_Space;
			ps->m_EmitModeB = new EmitModeB();
			ps->m_EmitModeB->m_Point = glm::vec2(node["modeB"]["point"][0].as<float>(), node["modeB"]["point"][1].as<float>());
			ps->m_EmitModeB->m_Width = node["modeB"]["width"].as<float>();
		}

		return ps;
	}
	catch (std::exception e) {
		cout << color(colors::RED);
		cout << "Exception: " << e.what() << "\nin (ParticleSystem::createFromFile) \n" << white;

		delete sprite;
		delete ps;
		return nullptr;
	}



	/*
	cout << color(colors::RED);
	cout << "position: " << node["position"][0].as<float>() << node["position"][1].as<float>() << endl;
	cout << "velocity: " << node["velocity"][0].as<float>() << node["velocity"][1].as<float>() << endl;
	cout << "velocityVar: " << node["velocityVar"][0].as<float>() << node["velocityVar"][1].as<float>() << endl;
	cout << "colorBegin: " << node["colorBegin"][0].as<float>() << node["colorBegin"][1].as<float>() << node["colorBegin"][2].as<float>() << node["colorBegin"][3].as<float>() << endl;
	cout << "colorEnd: " << node["colorEnd"][0].as<float>() << node["colorEnd"][1].as<float>() << node["colorEnd"][2].as<float>() << node["colorEnd"][3].as<float>() << endl;
	cout << "lifeTime: " << node["lifeTime"].as<float>() << endl;
	cout << "sizeBegin: " << node["sizeBegin"].as<float>() << endl;
	cout << "sizeEnd: " << node["sizeEnd"].as<float>() << endl;
	cout << "sizeVar: " << node["sizeVar"].as<float>() << endl;
	cout << "texture: " << node["texture"].as<std::string>() << endl;
	*/

	/*
	cout << color(colors::CYAN);
	cout << "host: " << config["host"].as<std::string>() << endl;
	cout << "port: " << config["port"].as<int>() << endl;
	cout << "password: " << config["password"].as<std::string>() << endl;
	cout << "servicename: " << config["servicename"].as<std::string>() << white << endl;
	*/
}
