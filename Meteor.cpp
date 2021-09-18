#include "Meteor.h"


Meteor::Meteor()
{
	_position = {};
	_speed = {};
	_radius = 0;
	_active = 0;
	_color = WHITE;
}
Meteor::~Meteor()
{

}

void Meteor::set_position(Vector2 position)
{
	_position = position;
};
void Meteor::set_speed(Vector2 speed)
{
	_speed = speed;
};

void Meteor::set_radius(float radius)
{
	_radius = radius;
};

void Meteor::set_active(bool active)
{
	_active = active;
};

void Meteor::set_color(Color color)
{
	_color = color;
};
Vector2 Meteor::get_position()
{
	return _position;
};
Vector2 Meteor::get_speed()
{
	return _speed;
};
float Meteor::get_radius()
{
	return _radius;
};
bool  Meteor::get_active()
{
	return _active;
};
Color Meteor::get_color()
{
	return _color;
};