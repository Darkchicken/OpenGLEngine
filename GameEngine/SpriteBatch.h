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
	//same as a struct
	class Glyph
	{
	public:
		//default constructor
		Glyph() {};
		//constructor with arguments
		Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color):
			texture(Texture), 
			depth(Depth)
		{
			topLeft.color = color;
			topLeft.setPosition(destRect.x, destRect.y + destRect.w);
			topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

			bottomLeft.color = color;
			bottomLeft.setPosition(destRect.x, destRect.y);
			bottomLeft.setUV(uvRect.x, uvRect.y);

			bottomRight.color = color;
			bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
			bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

			topRight.color = color;
			topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
			topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
		};

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
		void draw(const glm::vec4& destRect,const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color);
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


		//vector of glyph pointers for sorting
		std::vector<Glyph*> _glyphPointers;
		//vector of actual glyphs
		std::vector<Glyph> _glyphs;

		std::vector<RenderBatch> _renderBatches;

	};
}

