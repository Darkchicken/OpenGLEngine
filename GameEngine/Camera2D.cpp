#include "Camera2D.h"

namespace GameEngine
{
	//initialization list
	Camera2D::Camera2D() :
		_position(0.0f, 0.0f),
		_cameraMatrix(1.0f),
		_orthoMatrix(1.0f),
		_scale(1.0f),
		_needsMatrixUpdate(true),
		_screenWidth(500),
		_screenHeight(500)
	{

	}


	Camera2D::~Camera2D()
	{
	}


	void Camera2D::init(int screenWidth, int screenheight)
	{
		_screenWidth = screenWidth;
		_screenHeight = screenheight;
		//builds orthographic matrix
		_orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);
	}
	void Camera2D::update()
	{
		//if matrix requires update
		if (_needsMatrixUpdate)
		{

			//Camera Translation		
			glm::vec3 translate(-_position.x + _screenWidth/2, -_position.y+ _screenHeight/2, 0.0f);
			_cameraMatrix = glm::translate(_orthoMatrix, translate);

			//Camera Scale
			glm::vec3 scale(_scale, _scale, 0.0f);
			_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * _cameraMatrix;

			_needsMatrixUpdate = false;
		}
		//change current camera matrix 

		//if players asks, return camera matrix to draw
	}
}

