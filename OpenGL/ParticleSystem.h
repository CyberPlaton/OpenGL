#pragma once

#include<memory>
#include<string>
#include<vector>
#include<utility>


#include"Sprite.h"
#include <random>

#include<glm/gtx/quaternion.hpp>

#include"yaml-cpp/yaml.h"

float common_lerp(float x, float y, float t);

class Random
{
public:
    static void Init()
    {
        s_RandomEngine.seed(std::random_device()());
    }

    static float Float()
    {
        return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
    }

private:
    static std::mt19937 s_RandomEngine;
    static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
};


struct ParticleData {

    glm::vec2 m_Position, m_Velocity, m_VelocityVariation;
    glm::vec4 m_ColorBegin, m_ColorEnd;
    float m_MaxLifeTime;
    float m_SizeBegin, m_SizeEnd, m_SizeVariation;
    float m_Rotation;
    float m_RotationVariation;
    int m_RotationDir = 1; // -1 or 1 for direction..
    float m_RotationSpeed = 1.0f;
};


class ParticleSystem {
public:
    struct Particle {

            glm::vec2 m_Position;
            glm::vec2 m_Velocity;
            glm::vec4 m_ColorBegin, m_ColorEnd;
            float m_Rotation = 0.0f;
            float m_SizeBegin, m_SizeEnd;

            float m_CurrentLifeTime = 1.0f;
            float m_LifeRemaining = 0.0f;

            int m_RotationDirection = 1; // Default.

            bool m_Active = false;
    };


    enum class EmitPositionMode {
        Invalid = -1,
        Fixed_On_Object = 0, // Psition of particles is relative to the position of some given object (expressed as a 2D-vec).
        Fixed_To_Space = 1, // Position of particles is some definde 2D-space.
    };


public:
    static ParticleSystem* createFromFile(std::string file);
    ParticleSystem(Sprite* sprite);

    void onUpdate(float dt, glm::vec3 camerPos);

    void onRender();

    void emit();




    ParticleData* m_ParticleData = nullptr;

private:
    std::vector<Particle> m_ParticlePool;

    uint32_t m_PoolIndex = 999;

    Sprite* m_ParticleSprite = nullptr;

    EmitPositionMode m_EmitPositionMode = EmitPositionMode::Invalid;




    struct EmitModeA { // Fixed on object.


    };

    struct EmitModeB { // Fixed to space.

        glm::vec2 m_Point;
        float m_Width;
    };

    EmitModeA* m_EmitModeA = nullptr;
    EmitModeB* m_EmitModeB = nullptr;
private:

};