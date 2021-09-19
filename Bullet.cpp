#include "Bullet.h"


Bullet::Bullet()
{
	_position = {};
	_speed = {};
	_radius = 0;
	_life = 0;
	_active = 0;
	_color = WHITE;
	_rotation = 0;
}
Bullet::~Bullet()
{

}
void Bullet::set_position(Vector2 position)
{
	_position = position;
}
void Bullet::set_speed(Vector2 speed)
{
	_speed = speed;
}
void Bullet::set_radius(float radius)
{
	_radius = radius;
}
void Bullet::set_rotation(float rotation)
{
	_rotation = rotation;
}
void Bullet::set_life(int life)
{
	_life = life;
}
void Bullet::set_active(bool active)
{
	_active = active;
}
void Bullet::set_color(Color color)
{
	_color = color;
}
Vector2 Bullet::get_position()
{
	return _position;
}
Vector2	Bullet::get_speed()
{
	return _speed;
}
float Bullet::get_radius()
{
	return _radius;
}
float Bullet::get_rotation()
{
	return _rotation;
}
int	Bullet::get_life()
{
	return _life;
}
bool Bullet::get_active()
{
	return _active;
}
Color Bullet::get_color()
{
	return _color;
}

void init_bullets(Bullet* &bullet, int max_bullets)
{
		bullet = new Bullet();
		Vector2 pos_init = { 0,0 };
		Vector2 speed_init = { 0,0 };
		float aux_radius = 2.0f;
		bullet->set_position(pos_init);
		bullet->set_speed(speed_init);
		bullet->set_radius(aux_radius);
		bullet->set_active(false);
		bullet->set_life(0);
		bullet->set_color(WHITE);
}