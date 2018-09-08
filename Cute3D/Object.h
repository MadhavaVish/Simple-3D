#pragma once

#include <string>

#include <glm/glm.hpp>

#include "Model.h"

class Object
{
public:
	Object(std::string const &path);
	~Object();
	
	void setPosition();
	void setScale();
	void setRotation();
	void draw();

private:
	Model model;
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
};

