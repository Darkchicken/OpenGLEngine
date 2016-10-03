#include "ParticleBatch2D.h"

namespace GameEngine
{

	void Particle2D::update(float deltaTime)
	{
		m_position += m_velocity * deltaTime;

	}

	ParticleBatch2D::ParticleBatch2D()
	{
		//empty
	}


	ParticleBatch2D::~ParticleBatch2D()
	{
		//delete particles array
		delete[] m_particles;
	}

	void ParticleBatch2D::init(int maxParticles, float decayRate, GLTexture texture)
	{
		m_maxParticles = maxParticles;
		m_particles = new Particle2D[maxParticles];
		m_decayRate = decayRate;
		m_texture = texture;
	}

	void ParticleBatch2D::update(float deltaTime)
	{
		//loop through all particles
		for (int i = 0; i < m_maxParticles; i++)
		{
			//check if particle is active 
			if (m_particles[i].m_life > 0.0f)
			{
				//update particle
				m_particles[i].update(deltaTime);
				//decrease life by decay rate
				m_particles[i].m_life -= m_decayRate * deltaTime;
			}
		}

	}

	void ParticleBatch2D::draw(SpriteBatch* spriteBatch)
	{
		//standard uvRect for all textures
		glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

		//loop through all particles
		for (int i = 0; i < m_maxParticles; i++)
		{
			//store current particle for reference
			auto& p = m_particles[i];

			//check if particle is active 
			if (m_particles[i].m_life > 0.0f)
			{
				//create destination rectangle with particle positon and size
				glm::vec4 destRect(p.m_position.x,p.m_position.y, p.m_width, p.m_width );

				//render particle
				spriteBatch->draw(destRect,uvRect,m_texture.id, 0, p.m_color);
			}
		}
	}


	void ParticleBatch2D::addParticle(const glm::vec2& position,
		const glm::vec2& velocity,
		const ColorRGBA8& color,
		float width)
	{
		//find first particle not in use, set active, set variables
		int particleIndex = findFreeParticle();
		//create reference to partile at index
		auto& p = m_particles[particleIndex];
		//set variables
		p.m_life = 1.0f;
		p.m_position = position;
		p.m_color = color;
		p.m_velocity = velocity;
		p.m_width = width;

	}

	int ParticleBatch2D::findFreeParticle()
	{
		//loop through all particles starting with last free particle
		for (int i = m_lastFreeParticle; i < m_maxParticles; i++)
		{
			//if this particle is inactive
			if (m_particles[i].m_life <= 0.0f)
			{
				//set last free particle to this index
				m_lastFreeParticle = i;
				//return the index
				return i;
			}
		}
		//loop through all particles starting with 0
		for (int i = 0; i < m_lastFreeParticle; i++)
		{
			//if this particle is inactive
			if (m_particles[i].m_life <= 0.0f)
			{
				//set last free particle to this index
				m_lastFreeParticle = i;
				//return the index
				return i;
			}

		}
		//no particles are free, overwrite first particle
		return 0;

	}
}
