#pragma once
#include <gl/glew.h>
#include "Vertex.h"
#include <vector>
#include <glm/glm.hpp>

namespace GameEngine
{
	enum class GlyphSortType
	{
		NONE,
		FRONT_TO_BACK,
		BACK_TO_FRONT,
		TEXTURE		//sort by texture so all same texture gets drawn together
	};

	struct Glyph
	{
		GLuint texture;
		float depth;

		Vertex topLeft;
		Vertex bottomLeft;
		Vertex topRight;
		Vertex bottomRight;
	};

	class RenderBatch
	{
	public:
		RenderBatch(GLuint Offset, GLuint NumVerticies, GLuint Texture): 
			offset(Offset), numVerticies(NumVerticies), texture(Texture)
		{

		}
		GLuint offset;
		//number of verticies to draw
		GLuint numVerticies;
		GLuint texture;
	};


	class SpriteBatch
	{
	public:
		SpriteBatch();
		~SpriteBatch();
		//initialize spritebatch class
		void init();
		//set up for drawing
		void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
		//post processing
		void end();
		//adds sprites to batch
		//pass by reference to avoid copying tons of data
		//use const to make sure referenced info isnt going to change
		void draw(const glm::vec4& destRect,const glm::vec4& uvRect, GLuint texture, float depth, const Color& color);
		//finishes rendering, draws sprites to screen
		void renderBatch();

	private:
		
		void CreateRenderBatches();
		//initialize vertex array object
		void createVertexArray();
		//sorting glyphs
		void sortGlyphs();

		static bool compareFrontToBack(Glyph* a, Glyph* b);
		static bool compareBackToFront(Glyph* a, Glyph* b);
		static bool compareTexture(Glyph* a, Glyph* b);

		GlyphSortType _sortType;

		GLuint _vbo;
		GLuint _vao;


		//using pointers so only a few bytes of data will need to be sorted
		//would normally be more because Glyph struct contains a lot of data
		std::vector<Glyph*> _glyphs;

		std::vector<RenderBatch> _renderBatches;

	};
}

