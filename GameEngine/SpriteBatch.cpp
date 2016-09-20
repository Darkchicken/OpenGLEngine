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
	
		_glyphs.clear();

	}
	

	
	void SpriteBatch::end()
	{
		//set the size of _glyphPointers to the size of _glyphs
		_glyphPointers.resize(_glyphs.size());
		//cause glyph pointers to reference glyphs
		for (int i = 0; i < _glyphs.size(); i++)
		{
			_glyphPointers[i] = &_glyphs[i];
		}
		//sort glyphs
		sortGlyphs();
		//generate batches from glyphs
		CreateRenderBatches();

	}
	
	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture,float depth, const ColorRGBA8& color)
	{

		//construct new glyph and add new glyph to vector
		_glyphs.emplace_back(destRect, uvRect, texture, depth, color);
		
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
		vertices.resize(_glyphPointers.size() * 6); //6 verticies per glyph
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
		_renderBatches.emplace_back(offset, 6, _glyphPointers[0]->texture);
		//set a vertex to the glyphs vertex, then increment currentVertex
		vertices[currentVertex++] = _glyphPointers[0]->topLeft;
		vertices[currentVertex++] = _glyphPointers[0]->bottomLeft;
		vertices[currentVertex++] = _glyphPointers[0]->bottomRight;
		vertices[currentVertex++] = _glyphPointers[0]->bottomRight;
		vertices[currentVertex++] = _glyphPointers[0]->topRight;
		vertices[currentVertex++] = _glyphPointers[0]->topLeft;
		offset += 6;
		//current glyph
		for (int cg = 1; cg < _glyphPointers.size(); cg++)
		{
			//only do this if current texture is different from previous texture
			if (_glyphPointers[cg]->texture != _glyphPointers[cg - 1]->texture)
			{
				_renderBatches.emplace_back(offset, 6, _glyphPointers[cg]->texture);
			}
			else
			{
				//get last element, add 6 verticies to batch
				_renderBatches.back().numVerticies +=6;
			}
			
			//set a vertex to the glyphs vertex, then increment currentVertex
			vertices[currentVertex++] = _glyphPointers[cg]->topLeft;
			vertices[currentVertex++] = _glyphPointers[cg]->bottomLeft;
			vertices[currentVertex++] = _glyphPointers[cg]->bottomRight;
			vertices[currentVertex++] = _glyphPointers[cg]->bottomRight;
			vertices[currentVertex++] = _glyphPointers[cg]->topRight;
			vertices[currentVertex++] = _glyphPointers[cg]->topLeft;
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
			std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareBackToFront);
			break;
		case GlyphSortType::FRONT_TO_BACK:
			
			std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareFrontToBack);
			break;
		case GlyphSortType::TEXTURE:
		
			std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareTexture);
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