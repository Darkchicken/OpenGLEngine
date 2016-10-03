#pragma once

#include <glm/glm.hpp>
#include "Vertex.h"
#include "SpriteBatch.h"
#include "GLTexture.h"
namespace GameEngine
{

	class Particle2D
	{
	public:
		friend class ParticleBatch2D;
		//update for each particle
		void update(float deltaTime);
	private:
		//position of particle
		glm::vec2 m_position = glm::vec2(0.0f);
		//velocity of particle
		glm::vec2 m_velocity = glm::vec2(0.0f);
		//color of particle
		ColorRGBA8 m_color;
		//lifetime of particle (particle is active if life is greater than 0)
		float m_life = 1.0f;
		//width of particle
		float m_width = 0.0f;
		
		

	};

	class ParticleBatch2D
	{
	public:
		ParticleBatch2D();
		~ParticleBatch2D();

		void init(	int maxParticles,
					float decayRate,
					GLTexture texture);

		void update(float deltaTime);

		void draw(SpriteBatch spriteBatch);

		void addParticle(	const glm::vec2& position,
							const ColorRGBA8& color,
							const glm::vec2& velocity,
							float width);
		
	private:

		int findFreeParticle();

		//decay rate of all particles
		float m_decayRate = 0.1f;
		//dynamic array of particles
		Particle2D* m_particles = nullptr;
		//max number of particles in batch
		int m_maxParticles = 0;
		//index of last free particle
		int m_lastFreeParticle = 0;
		//texture for particles
		GLTexture m_texture;
		

	};
}

