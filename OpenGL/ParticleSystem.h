#pragma once

#include<memory>
#include<string>
#include<vector>
#include<utility>


#include<GL/glew.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>


#include"ColorConsole.h"
#include"ShaderProgram.h"
#include<stb_image/stb_image.h>

#include <random>

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



class Texture2D {
public:

    Texture2D();
    ~Texture2D();


    bool LoadTexture(const std::string& fileName, bool genMipMaps = true);
    void Bind(GLuint texUint = 0);
    void Unbind(GLuint texUint = 0);

    GLuint GetTextureHandle() const { return m_TextureHandle; }

private:

    GLuint m_TextureHandle;

private:

};


struct ParticleData {

    glm::vec2 m_Position, m_Velocity, m_VelocityVariation;
    glm::vec4 m_ColorBegin, m_ColorEnd;
    float m_LifeTime;
    float m_SizeBegin, m_SizeEnd, m_SizeVariation;
};


class ParticleSystem {
public:
    struct Particle {

            glm::vec2 m_Position;
            glm::vec2 m_Velocity;
            glm::vec4 m_ColorBegin, m_ColorEnd;
            float m_Rotation = 0.0f;
            float m_SizeBegin, m_SizeEnd;

            float m_LifeTime = 1.0f;
            float m_LifeRemaining = 0.0f;

            bool m_Active = false;
    };



public:
    ParticleSystem(int screenwidth, int screenheight);


    void onUpdate(float dt, glm::vec3 camerPos);

    void onRender(glm::mat4 projection, glm::mat4 view, ShaderProgram* shader);

    void emit(const ParticleData& props);

    /*
    ParticleSystem(std::string texturePath, unsigned int particleCount,
        glm::vec4 color, glm::vec2 position, glm::vec2 velocity, float lifespan, float lifeVariance = 0);

    void Update(glm::vec2 objectPos, float dt); // Includes drawing function.

    void SetProjection(glm::mat4 proj) { m_Projection = proj; }
    */
private:
    std::vector<Particle> m_ParticlePool;

    uint32_t m_PoolIndex = 999;

    GLuint m_QuadVAO = 0;
    GLuint m_QuadVBO = 0;


    ShaderProgram* m_ParticleShader = nullptr;


    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewProjectionMatrix;

    /*
    ShaderProgram* m_ParticleShader = nullptr;
    std::string m_DefaultShaderVert = "particleShader.vert";
    std::string m_DefaultShaderFrag = "particleShader.frag";

    Texture2D* m_ParticleTexture = nullptr;

    std::vector<ParticleData*> m_ParticlePool;

    unsigned int m_ParticleCount = 0;

    unsigned int m_LastUsedParticle = 0;

    GLuint VAO;

    glm::mat4 m_Projection;
    */
private:


    /*
    unsigned int _firstUnusedParticle();
    void _respawnParticle(ParticleData& particle, glm::vec2& objectPos, glm::vec2 offset);

    void _draw();

    bool _loadShader();
    */
};
