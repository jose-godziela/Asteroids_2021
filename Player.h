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
public:
	Player();
	~Player();
	void set_position(Vector2 position) { _position = position; };
	void set_speed_X(float x) { _speed.x = x; };
	void set_speed_Y(float y) { _speed.y = y; };
	void set_acceleration(float acceleration) { _acceleration = acceleration; };
	void set_rotation(float rotation) { _rotation = rotation; };
	void set_ship_height(float height) { _ship_height = height; };
	void set_collider(Vector3 collider) { _collider = collider; };
	void set_color(Color color) { _color = color; };
	Vector2 get_position() { return _position; };
	Vector2 get_speed() { return _speed; };
	float get_acceleration() { return _acceleration; };
	float get_rotation() { return _rotation; };
	float get_ship_height() { return _ship_height; };
	Vector3 get_collider() { return _collider; };
	Color get_color() { return _color; };

};