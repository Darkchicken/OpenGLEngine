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

	glm::vec2 Camera2D::convertScreenToWorldCoords(glm::vec2 screenCoords)
	{
		//invert Y direction
		screenCoords.y = _screenHeight - screenCoords.y;
		//make it so 0 is center
		screenCoords -= glm::vec2(_screenWidth / 2, _screenHeight / 2);
		//give world coords proper scale
		screenCoords /= _scale;
		//translate with the camera position
		screenCoords += _position;

		return screenCoords;
	}

	//use tile collision to check if an object can be seen by camera (for culling)
	//arguments are position and dimensions of colliding agent
	bool Camera2D::isBoxInView(const glm::vec2& position, const glm::vec2 dimensions)
	{
		//get scale of camera
		glm::vec2 scaledScreenDimensions = glm::vec2(_screenWidth, _screenHeight)/(_scale);

		//minimum x distance between agent and tile without a collision
		const float MIN_DISTANCE_X = dimensions.x / 2.0f + scaledScreenDimensions.x/2.0f;
		//minimum y distance between agent and tile without a collision
		const float MIN_DISTANCE_Y = dimensions.y/2.0f + scaledScreenDimensions.y/2.0f;

		//center position of the agent (from parameters)
		glm::vec2 centerPos = position + dimensions/2.0f;
		//center position of camera (already stored as center)
		glm::vec2 centerCameraPos = _position;
		//find vector between center of camera and center of agent
		glm::vec2 distVec = centerPos - centerCameraPos;

		//check x collision
		float xDepth = MIN_DISTANCE_X - abs(distVec.x);
		//check y collision
		float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

		//if this is true, we are colliding
		if (xDepth > 0 && yDepth > 0)
		{
			//notify that there was a collision
			return true;
			
		}
		//if no collision happened, return false
		return false;
	}
}

