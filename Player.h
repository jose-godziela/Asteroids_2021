#ifndef PLAYER_H
#define PLAYER_H
#include <cmath>

#include "raylib.h"

const float PLAYER_BASE_SIZE = 20.0f;
const float PLAYER_SPEED = 6.0f;
const int PLAYER_MAX_BULLETS = 10;

class Player {
private:
	Vector2 _position;
	Vector2 _speed;
	float   _acceleration;
	float   _rotation;
	float _ship_height;
	Vector3 _collider;
	Color   _color;
	Texture2D _texture;
public:
	Player();
	~Player();
	void set_position(Vector2 position);
	void set_speed_X(float x);
	void set_speed_Y(float y);
	void set_acceleration(float acceleration);
	void set_rotation(float rotation);
	void set_ship_height(float height);
	void set_collider(Vector3 collider);
	void set_color(Color color);
	void set_texture(Texture2D texture);
	Vector2 get_position();
	Vector2 get_speed();
	float get_acceleration();
	float get_rotation();
	float get_ship_height();
	Vector3 get_collider();
	Color get_color();
	Texture2D get_texture();
};

#endif

