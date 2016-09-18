#include "SpriteBatch.h"
//for sort call
#include <algorithm>

namespace GameEngine
{

	

	SpriteBatch::SpriteBatch(): _vbo(0), _vao(0)
	{
	}


	SpriteBatch::~SpriteBatch()
	{
	}


	void SpriteBatch::init()
	{
		createVertexArray();
	}

	//set up state to begin rendering
	void SpriteBatch::begin(GlyphSortType sortType /* GlyphSortType::TEXTURE */)
	{
		_sortType = sortType;
		//clear vectors to save memory (changes size back to 0)
		_renderBatches.clear();
		//delete glyph pointers to prevent memory leak
		for (int i = 0; i < _glyphs.size(); i++)
		{
			delete _glyphs[i];
		}
		_glyphs.clear();

	}
	

	
	void SpriteBatch::end()
	{
		//sort glyphs
		sortGlyphs();
		//generate batches from glyphs
		CreateRenderBatches();

	}
	
	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture,float depth, const ColorRGBA8& color)
	{

		//create new glyph pointer
		//must rememeber to delete later or you have memory leak
		Glyph* newGlyph = new Glyph;
		newGlyph->texture = texture;
		newGlyph->depth = depth;

		newGlyph->topLeft.color = color;
		newGlyph->topLeft.setPosition(destRect.x,destRect.y+destRect.w);
		newGlyph->topLeft.setUV(uvRect.x, uvRect.y+uvRect.w);

		newGlyph->bottomLeft.color = color;
		newGlyph->bottomLeft.setPosition(destRect.x, destRect.y);
		newGlyph->bottomLeft.setUV(uvRect.x, uvRect.y);

		newGlyph->bottomRight.color = color;
		newGlyph->bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
		newGlyph->bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

		newGlyph->topRight.color = color;
		newGlyph->topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
		newGlyph->topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

		//add new glyph to vector
		//using pointers so only a few bytes of data will need to be sorted
		//would normally be more because Glyph struct contains a lot of data
		_glyphs.push_back(newGlyph);
		
	}
	
	void SpriteBatch::renderBatch()
	{
		glBindVertexArray(_vao);

		for (int i = 0; i < _renderBatches.size(); i++)
		{
			glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);

			glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVerticies);
		}
		glBindVertexArray(0);
	}

	void SpriteBatch::CreateRenderBatches()
	{
		std::vector<Vertex> vertices;
		//reserve memory for verticies vector and resize vector to proper size
		vertices.resize(_glyphs.size() * 6); //6 verticies per glyph
		//if no glyphs, return, no batches to create
		if (_glyphs.empty())
		{
			return;
		}
		int offset = 0;
		int currentVertex = 0;
		//offset =0 , numverts = 6, texture = glyphs[0]
		/*
		RenderBatch myBatch(0, 6, _glyphs[0]->texture);
		_renderBatches.push_back(myBatch);
		*/
		//does everything above in 1 line of code
		_renderBatches.emplace_back(offset, 6, _glyphs[0]->texture);
		//set a vertex to the glyphs vertex, then increment currentVertex
		vertices[currentVertex++] = _glyphs[0]->topLeft;
		vertices[currentVertex++] = _glyphs[0]->bottomLeft;
		vertices[currentVertex++] = _glyphs[0]->bottomRight;
		vertices[currentVertex++] = _glyphs[0]->bottomRight;
		vertices[currentVertex++] = _glyphs[0]->topRight;
		vertices[currentVertex++] = _glyphs[0]->topLeft;
		offset += 6;
		//current glyph
		for (int cg = 1; cg < _glyphs.size(); cg++)
		{
			//only do this if current texture is different from previous texture
			if (_glyphs[cg]->texture != _glyphs[cg - 1]->texture)
			{
				_renderBatches.emplace_back(offset, 6, _glyphs[cg]->texture);
			}
			else
			{
				//get last element, add 6 verticies to batch
				_renderBatches.back().numVerticies +=6;
			}
			
			//set a vertex to the glyphs vertex, then increment currentVertex
			vertices[currentVertex++] = _glyphs[cg]->topLeft;
			vertices[currentVertex++] = _glyphs[cg]->bottomLeft;
			vertices[currentVertex++] = _glyphs[cg]->bottomRight;
			vertices[currentVertex++] = _glyphs[cg]->bottomRight;
			vertices[currentVertex++] = _glyphs[cg]->topRight;
			vertices[currentVertex++] = _glyphs[cg]->topLeft;
			offset += 6;
		}
		//create vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		//orphan the buffer
		glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		//upload the data
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex),vertices.data());
		//unbind vuffewr
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	//creates vertex array object
	void SpriteBatch::createVertexArray()
	{
		//only generate these if they havent been already
		if (_vao == 0)
		{
			glGenVertexArrays(1, &_vao);
		}
		glBindVertexArray(_vao);


		if (_vbo == 0)
		{
			glGenBuffers(1, &_vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		//send on vertex attrib array for position, 0 is for first element
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);


		//this is the position attribute pointer (last value is the byte offset of the data, its 0 here because the position data is the first data)
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		//this is the color attribute pointer 
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		//this is the UV attribute pointer 
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		//unbind vertex array
		glBindVertexArray(0);
		
	}

	void SpriteBatch::sortGlyphs()
	{
		switch (_sortType)
		{
		case GlyphSortType::BACK_TO_FRONT:
			//guarantees that 2 equal elements will retain same order
			//pass iterator to beginnning and end of container, and comparator
			std::stable_sort(_glyphs.begin(), _glyphs.end(), compareBackToFront);
			break;
		case GlyphSortType::FRONT_TO_BACK:
			
			std::stable_sort(_glyphs.begin(), _glyphs.end(), compareFrontToBack);
			break;
		case GlyphSortType::TEXTURE:
		
			std::stable_sort(_glyphs.begin(), _glyphs.end(), compareTexture);
			break;
		}
		
	}


	bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b)
	{
		return (a->depth < b->depth);
	}
	bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b)
	{
		return (a->depth > b->depth);
	}
	bool SpriteBatch::compareTexture(Glyph* a, Glyph* b)
	{
		//sort by texture id int
		return (a->texture < b->texture);
	}

}