#ifndef METEOR_H
#define METEOR_H
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
	void set_position(Vector2 position);
	void set_speed(Vector2 speed);
	void set_radius(float radius);
	void set_active(bool active);
	void set_color(Color color);
	Vector2 get_position();
	Vector2 get_speed();
	float get_radius();
	bool get_active();
	Color get_color();
};
#endif