#pragma once
#include "raylib.h"

class Meteor {
private:
	Vector2 _position;
	Vector2 _speed;
	float   _radius;
	bool    _active;
	Color   _color;
public:
	Meteor();
	~Meteor();
	void set_position(Vector2 position) { _position = position; };
	void set_speed(Vector2 speed) { _speed = speed; };
	void set_radius(float radius) { _radius = radius; };
	void set_active(bool active) { _active = active; };
	void set_color(Color color) { _color = color; };
	Vector2 get_position();
	Vector2 get_speed();
	float get_radius();
	bool get_active();
	Color get_color();
};