#include "Player.h"

Player::Player()
{

	set_ship_height((PLAYER_BASE_SIZE / 2) / tanf(20 * DEG2RAD));
	set_position({ static_cast<float>(GetScreenWidth()) / 2, GetScreenHeight() / 2 - get_ship_height() / 2 });
	set_speed_X(0);
	set_speed_Y(0);
	set_acceleration(0.0f);
	set_rotation(0.0f);
	set_color(LIGHTGRAY);
	set_collider({ get_position().x + sin(get_rotation() * DEG2RAD) * (get_ship_height() / 2.5f), get_position().y - cos(get_rotation() * DEG2RAD) * (get_ship_height() / 2.5f), 12 });

};
Player::~Player()
{

};
void Player::set_position(Vector2 position)
{
	_position = position;
};
void Player::set_speed_X(float x)
{
	_speed.x = x;
};
void Player::set_speed_Y(float y)
{
	_speed.y = y;
};
void Player::set_acceleration(float acceleration)
{
	_acceleration = acceleration;
};
void Player::set_rotation(float rotation)
{
	_rotation = rotation;
};
void Player::set_ship_height(float height)
{
	_ship_height = height;
};
void Player::set_collider(Vector3 collider)
{
	_collider = collider;
};
void Player::set_color(Color color)
{
	_color = color;
};
Vector2 Player::get_position()
{
	return _position;
};
Vector2 Player::get_speed()
{
	return _speed;
};
float Player::get_acceleration()
{
	return _acceleration;
};
float Player::get_rotation()
{
	return _rotation;
};
float Player::get_ship_height()
{
	return _ship_height;
};
Vector3 Player::get_collider()
{
	return _collider;
};
Color Player::get_color()
{
	return _color;
};