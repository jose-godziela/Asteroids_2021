#pragma once
#include "raylib.h"



class Bullet {
private:
	Vector2 _position;
	Vector2 _speed;
	float   _radius;
	float   _rotation;
	int     _life;
	bool    _active;
	Color   _color;
public:
	Bullet();
	~Bullet();
	void set_position(Vector2 position);
	void set_speed(Vector2 speed);
	void set_radius(float radius);
	void set_rotation(float rotation);
	void set_life(int life);
	void set_active(bool active);
	void set_color(Color color);
	Vector2 get_position();
	Vector2	get_speed();
	float	get_radius();
	float	get_rotation();
	int		get_life();
	bool	get_active();
	Color	get_color();
};