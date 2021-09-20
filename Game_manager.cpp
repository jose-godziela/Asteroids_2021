#include "Game_Manager.h"

const int METEORS_SPEED = 2;
const int MAX_BIG_METEORS = 4;
const int MAX_MEDIUM_METEORS = 8;
const int MAX_SMALL_METEORS = 16;

const int FPS = 60;

static bool game_over = true;
static bool pause = false;
static bool victory = false;

Player* player;
Bullet* bullet[PLAYER_MAX_BULLETS];
Meteor* bigMeteor[MAX_BIG_METEORS];
Meteor* mediumMeteor[MAX_MEDIUM_METEORS];
Meteor* smallMeteor[MAX_SMALL_METEORS];

int midMeteorsCount = 0;
int smallMeteorsCount = 0;
int destroyedMeteorsCount = 0;

//These are to get the rotation of the ship using the mouse
Vector2 mouse_pos = { -100.0f,-100.0f };
Vector2 ship_pos_aux;
Vector2 ship_direction;
float ship_angle;

void init_game();
void reset_game();

// Update game (one frame)
void update(void)
{

	if (!game_over)
	{

		if (!pause)
		{
			// Player logic: rotation
			float angle_fix = 1.57f;
			mouse_pos = GetMousePosition();
			std::cout << "mouse pos x: " << mouse_pos.x << std::endl;
			std::cout << "mouse pos y: " << mouse_pos.y << std::endl;
			if (mouse_pos.x > 0 || mouse_pos.y > 0)
			{
				ship_pos_aux = player->get_position();
				ship_direction = { mouse_pos.x - ship_pos_aux.x,mouse_pos.y - ship_pos_aux.y };
				ship_angle = atan2(ship_direction.y, ship_direction.x) + angle_fix;
				ship_angle = ship_angle * 180 / PI;

				//this gives back a positive number in case that the result is negative
				if (ship_direction.x > 0 && ship_direction.y < 0)
				{
					ship_angle += 360;
				}

				std::cout << ship_angle << "" << std::endl;
				player->set_rotation(ship_angle);

			}

			// Player logic: speed
			player->set_speed_X(sin(player->get_rotation() * DEG2RAD) * PLAYER_SPEED);
			player->set_speed_Y(cos(player->get_rotation() * DEG2RAD) * PLAYER_SPEED);

			//couldn't find a functional way to make this
			Vector2 position_aux = { player->get_position().x + (player->get_speed().x * player->get_acceleration()),
									 player->get_position().y - (player->get_speed().y * player->get_acceleration()) };

			player->set_position(position_aux);

			// checks if the players goes out of the screen, and then makes the "pac-man" effect
			//both in X and Y axis
			if (player->get_position().x > GetScreenWidth() + player->get_ship_height())
			{
				std::cout << "X es mayor a la pantalla" << std::endl;
				player->set_position({ 0 - (player->get_ship_height()), player->get_position().y });
			}
			else if (player->get_position().x < -(player->get_ship_height()))
			{
				std::cout << "X es menor a la pantalla" << std::endl;
				player->set_position({ GetScreenWidth() + player->get_ship_height() ,player->get_position().y });
			}

			if (player->get_position().y > GetScreenHeight() + player->get_ship_height())
			{
				std::cout << "Y es mayor a la pantalla" << std::endl;
				player->set_position({ player->get_position().x, 0 - (player->get_ship_height()) });
			}
			else if (player->get_position().y < -(player->get_ship_height()))
			{
				std::cout << "Y es menor a la pantalla" << std::endl;
				player->set_position({ player->get_position().x ,GetScreenHeight() + player->get_ship_height() });
			}

			// bullet life timer
			for (int i = 0; i < PLAYER_MAX_BULLETS; i++)
			{
				float aux_life = bullet[i]->get_life();
				if (bullet[i]->get_active()) bullet[i]->set_life(aux_life++);
			}

			// firing logic
			for (int i = 0; i < PLAYER_MAX_BULLETS; i++)
			{
				if (bullet[i]->get_active())
				{
					// Movement
					bullet[i]->set_position({ bullet[i]->get_position().x + bullet[i]->get_speed().x, bullet[i]->get_position().y - bullet[i]->get_speed().y });

					// Collision logic: bullet vs walls
					if (bullet[i]->get_position().x > GetScreenWidth() + bullet[i]->get_radius())
					{
						bullet[i]->set_active(false);
						bullet[i]->set_life(0);
					}
					else if (bullet[i]->get_position().x < 0 - bullet[i]->get_radius())
					{
						bullet[i]->set_active(false);
						bullet[i]->set_life(0);
					}
					if (bullet[i]->get_position().y > GetScreenHeight() + bullet[i]->get_radius())
					{
						bullet[i]->set_active(false);
						bullet[i]->set_life(0);
					}
					else if (bullet[i]->get_position().y < 0 - bullet[i]->get_radius())
					{
						bullet[i]->set_active(false);
						bullet[i]->set_life(0);
					}

					// Life of bullet
					if (bullet[i]->get_life() >= 60)
					{
						//reset
						bullet[i]->set_position({ 0,0 });
						bullet[i]->set_speed({ 0,0 });
						bullet[i]->set_life(0);
						bullet[i]->set_active(false);
					}
				}
			}
			// Collision logic: player vs meteors
			player->set_collider({ player->get_position().x + sin(player->get_rotation() * DEG2RAD) * (player->get_ship_height() / 2.5f),
								   player->get_position().y - cos(player->get_rotation() * DEG2RAD) * (player->get_ship_height() / 2.5f),
								   12 });

			for (int a = 0; a < MAX_BIG_METEORS; a++)
			{
				if (CheckCollisionCircles({ player->get_collider().x, player->get_collider().y }, player->get_collider().z, bigMeteor[a]->get_position(), bigMeteor[a]->get_radius()) && bigMeteor[a]->get_active())
					game_over = true;
			}

			for (int a = 0; a < MAX_MEDIUM_METEORS; a++)
			{
				if (CheckCollisionCircles({ player->get_collider().x, player->get_collider().y }, player->get_collider().z, mediumMeteor[a]->get_position(), mediumMeteor[a]->get_radius()) && mediumMeteor[a]->get_active())
					game_over = true;
			}

			for (int a = 0; a < MAX_SMALL_METEORS; a++)
			{
				if (CheckCollisionCircles({ player->get_collider().x, player->get_collider().y }, player->get_collider().z, smallMeteor[a]->get_position(), smallMeteor[a]->get_radius()) && smallMeteor[a]->get_active())
					game_over = true;
			}
			// Meteors Position logic: big meteors
			for (int i = 0; i < MAX_BIG_METEORS; i++)
			{
				if (bigMeteor[i]->get_active())
				{

					bigMeteor[i]->set_position({ bigMeteor[i]->get_position().x + bigMeteor[i]->get_speed().x,bigMeteor[i]->get_position().y + bigMeteor[i]->get_speed().y });

					if (bigMeteor[i]->get_position().x > GetScreenWidth() + bigMeteor[i]->get_radius())
						bigMeteor[i]->set_position({ -(bigMeteor[i]->get_radius()),bigMeteor[i]->get_position().y });
					else if (bigMeteor[i]->get_position().x < 0 - bigMeteor[i]->get_radius())
						bigMeteor[i]->set_position({ GetScreenWidth() + bigMeteor[i]->get_radius(),bigMeteor[i]->get_position().y });

					if (bigMeteor[i]->get_position().y > GetScreenHeight() + bigMeteor[i]->get_radius())
						bigMeteor[i]->set_position({ bigMeteor[i]->get_position().x,-(bigMeteor[i]->get_radius()) });
					else if (bigMeteor[i]->get_position().y < 0 - bigMeteor[i]->get_radius())
						bigMeteor[i]->set_position({ bigMeteor[i]->get_position().x,GetScreenHeight() + bigMeteor[i]->get_radius() });
				}
			}

			// Meteors Position  logic: medium meteors
			for (int i = 0; i < MAX_MEDIUM_METEORS; i++)
			{
				if (mediumMeteor[i]->get_active())
				{
					mediumMeteor[i]->set_position({ mediumMeteor[i]->get_position().x + mediumMeteor[i]->get_speed().x,
													mediumMeteor[i]->get_position().y + mediumMeteor[i]->get_speed().y });

					if (mediumMeteor[i]->get_position().x > GetScreenWidth() + mediumMeteor[i]->get_radius())
						mediumMeteor[i]->set_position({ -(mediumMeteor[i]->get_radius()),mediumMeteor[i]->get_position().y });

					else if (mediumMeteor[i]->get_position().x < 0 - mediumMeteor[i]->get_radius())
						mediumMeteor[i]->set_position({ GetScreenWidth() + mediumMeteor[i]->get_radius(),mediumMeteor[i]->get_position().y });

					if (mediumMeteor[i]->get_position().y > GetScreenHeight() + mediumMeteor[i]->get_radius())
						mediumMeteor[i]->set_position({ mediumMeteor[i]->get_position().x,-(mediumMeteor[i]->get_radius()) });

					else if (mediumMeteor[i]->get_position().y < 0 - mediumMeteor[i]->get_radius())
						mediumMeteor[i]->set_position({ mediumMeteor[i]->get_position().x,GetScreenHeight() + mediumMeteor[i]->get_radius() });
				}
			}


			// Meteors Position logic: small meteors
			for (int i = 0; i < MAX_MEDIUM_METEORS; i++)
			{
				if (smallMeteor[i]->get_active())
				{
					smallMeteor[i]->set_position({ smallMeteor[i]->get_position().x + smallMeteor[i]->get_speed().x,smallMeteor[i]->get_position().y + smallMeteor[i]->get_speed().y });

					if (smallMeteor[i]->get_position().x > GetScreenWidth() + smallMeteor[i]->get_radius())
						smallMeteor[i]->set_position({ -(smallMeteor[i]->get_radius()),smallMeteor[i]->get_position().y });
					else if (smallMeteor[i]->get_position().x < 0 - smallMeteor[i]->get_radius())
						smallMeteor[i]->set_position({ GetScreenWidth() + smallMeteor[i]->get_radius(),smallMeteor[i]->get_position().y });

					if (smallMeteor[i]->get_position().y > GetScreenHeight() + smallMeteor[i]->get_radius())
						smallMeteor[i]->set_position({ mediumMeteor[i]->get_position().x,-(smallMeteor[i]->get_radius()) });
					else if (smallMeteor[i]->get_position().y < 0 - smallMeteor[i]->get_radius())
						smallMeteor[i]->set_position({ smallMeteor[i]->get_position().x,GetScreenHeight() + smallMeteor[i]->get_radius() });
				}
			}

			// Collision logic: player-BULLETS vs meteors
			for (int i = 0; i < PLAYER_MAX_BULLETS; i++)
			{
				if (bullet[i]->get_active())
				{
					for (int a = 0; a < MAX_BIG_METEORS; a++)
					{
						if (bigMeteor[a]->get_active() && CheckCollisionCircles(bullet[i]->get_position(), bullet[i]->get_radius(), bigMeteor[a]->get_position(), bigMeteor[a]->get_radius()))
						{
							bullet[i]->set_active(false);
							bullet[i]->set_life(0);
							bigMeteor[a]->set_active(false);
							destroyedMeteorsCount++;

							for (int j = 0; j < 2; j++)
							{
								if (midMeteorsCount % 2 == 0)
								{
									mediumMeteor[midMeteorsCount]->set_position(bigMeteor[a]->get_position());
									mediumMeteor[midMeteorsCount]->set_speed({ cos(bullet[i]->get_rotation() * DEG2RAD) * METEORS_SPEED * -1, sin(bullet[i]->get_rotation() * DEG2RAD) * METEORS_SPEED * -1 });
								}
								else
								{
									mediumMeteor[midMeteorsCount]->set_position(bigMeteor[a]->get_position());
									mediumMeteor[midMeteorsCount]->set_speed({ cos(bullet[i]->get_rotation() * DEG2RAD) * METEORS_SPEED, sin(bullet[i]->get_rotation() * DEG2RAD) * METEORS_SPEED });
									;
								}

								mediumMeteor[midMeteorsCount]->set_active(true);
								midMeteorsCount++;
							}
							//bigMeteor[a].position = (Vector2){-100, -100};
							bigMeteor[a]->set_color(RED);
							a = MAX_BIG_METEORS;
						}
					}

					for (int b = 0; b < MAX_MEDIUM_METEORS; b++)
					{
						if (mediumMeteor[b]->get_active() && CheckCollisionCircles(bullet[i]->get_position(), bullet[i]->get_radius(), mediumMeteor[b]->get_position(), mediumMeteor[b]->get_radius()))
						{
							bullet[i]->set_active(false);
							bullet[i]->set_life(0);
							mediumMeteor[b]->set_active(false);
							destroyedMeteorsCount++;

							for (int j = 0; j < 2; j++)
							{
								if (smallMeteorsCount % 2 == 0)
								{
									smallMeteor[smallMeteorsCount]->set_position({ mediumMeteor[b]->get_position().x, mediumMeteor[b]->get_position().y });
									smallMeteor[smallMeteorsCount]->set_speed({ cos(bullet[i]->get_rotation() * DEG2RAD) * METEORS_SPEED * -1, sin(bullet[i]->get_rotation() * DEG2RAD) * METEORS_SPEED * -1 });
								}
								else
								{
									smallMeteor[smallMeteorsCount]->set_position({ mediumMeteor[b]->get_position().x, mediumMeteor[b]->get_position().y });
									smallMeteor[smallMeteorsCount]->set_speed({ sin(bullet[i]->get_rotation() * DEG2RAD) * METEORS_SPEED * -1, sin(bullet[i]->get_rotation() * DEG2RAD) * METEORS_SPEED * -1 });
								}

								smallMeteor[smallMeteorsCount]->set_active(true);
								smallMeteorsCount++;
							}
							mediumMeteor[b]->set_position({ -100,-100 });
							mediumMeteor[b]->set_color(GREEN);
							b = MAX_MEDIUM_METEORS;
						}
					}

					for (int c = 0; c < MAX_SMALL_METEORS; c++)
					{
						if (smallMeteor[c]->get_active() && CheckCollisionCircles(bullet[i]->get_position(), bullet[i]->get_radius(), smallMeteor[c]->get_position(), smallMeteor[c]->get_radius()))
						{
							bullet[i]->set_active(false);
							bullet[i]->set_life(0);
							smallMeteor[c]->set_active(false);
							destroyedMeteorsCount++;
							smallMeteor[c]->set_color(YELLOW);
							// smallMeteor[c].position = (Vector2){-100, -100};
							c = MAX_SMALL_METEORS;
						}
					}
				}
			}
		}

		if (destroyedMeteorsCount == MAX_BIG_METEORS + MAX_MEDIUM_METEORS + MAX_SMALL_METEORS) victory = true;
	}
	else
	{
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
		{
			reset_game();
			game_over = false;
		}
	}
}

// Draw game (one frame)
void draw()
{
	BeginDrawing();

	ClearBackground(RAYWHITE);

	if (!game_over)
	{
		// Draw spaceship
		Vector2 v1 = { player->get_position().x + sinf(player->get_rotation() * DEG2RAD) * (player->get_ship_height()), player->get_position().y - cosf(player->get_rotation() * DEG2RAD) * (player->get_ship_height()) };
		Vector2 v2 = { player->get_position().x - cosf(player->get_rotation() * DEG2RAD) * (PLAYER_BASE_SIZE / 2), player->get_position().y - sinf(player->get_rotation() * DEG2RAD) * (PLAYER_BASE_SIZE / 2) };
		Vector2 v3 = { player->get_position().x + cosf(player->get_rotation() * DEG2RAD) * (PLAYER_BASE_SIZE / 2), player->get_position().y + sinf(player->get_rotation() * DEG2RAD) * (PLAYER_BASE_SIZE / 2) };
		DrawTriangle(v1, v2, v3, MAROON);

		// Draw meteors
		for (int i = 0; i < MAX_BIG_METEORS; i++)
		{
			if (bigMeteor[i]->get_active())
				DrawCircleV(bigMeteor[i]->get_position(), bigMeteor[i]->get_radius(), DARKGRAY);
			else DrawCircleV(bigMeteor[i]->get_position(), bigMeteor[i]->get_radius(), Fade(LIGHTGRAY, 0.3f));
		}

		for (int i = 0; i < MAX_MEDIUM_METEORS; i++)
		{
			if (mediumMeteor[i]->get_active())
				DrawCircleV(mediumMeteor[i]->get_position(), mediumMeteor[i]->get_radius(), GRAY);
			else
				DrawCircleV(mediumMeteor[i]->get_position(), mediumMeteor[i]->get_radius(), Fade(LIGHTGRAY, 0.3f));
		}

		for (int i = 0; i < MAX_SMALL_METEORS; i++)
		{
			if (smallMeteor[i]->get_active()) DrawCircleV(smallMeteor[i]->get_position(), smallMeteor[i]->get_radius(), GRAY);
			else DrawCircleV(smallMeteor[i]->get_position(), smallMeteor[i]->get_radius(), Fade(LIGHTGRAY, 0.3f));
		}

		// Draw bullet
		for (int i = 0; i < PLAYER_MAX_BULLETS; i++)
		{
			if (bullet[i]->get_active()) DrawCircleV(bullet[i]->get_position(), bullet[i]->get_radius(), BLACK);
		}

		if (victory) DrawText("VICTORY", GetScreenWidth() / 2 - MeasureText("VICTORY", 20) / 2, GetScreenHeight() / 2, 20, LIGHTGRAY);

		if (pause) DrawText("GAME PAUSED", GetScreenWidth() / 2 - MeasureText("GAME PAUSED", 40) / 2, GetScreenHeight() / 2 - 40, 40, GRAY);
	}
	else DrawText("PRESS [LEFT MOUSE BUTTON] TO PLAY AGAIN", GetScreenWidth() / 2 - MeasureText("PRESS [LEFT MOUSE BUTTON] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 2 - 50, 20, GRAY);

	EndDrawing();
}

// Unload game variables
void de_init(void)
{
	
}


void init_player()
{
	player = new Player();

	Vector2 aux_speed = { 0,0 };

	destroyedMeteorsCount = 0;

	for (int i = 0; i < PLAYER_MAX_BULLETS; i++)
	{
		init_bullets(bullet[i], PLAYER_MAX_BULLETS);
	}
}

void init_meteors()
{
	//aux variables
	int posX, posY;
	int velX, velY;
	bool correctRange = false;

	for (int i = 0; i < MAX_BIG_METEORS; i++)
	{
		bigMeteor[i] = new Meteor();
		posX = GetRandomValue(0, GetScreenWidth());

		while (!correctRange)
		{
			if (posX > GetScreenWidth() / 2 - 150 && posX < GetScreenWidth() / 2 + 150) posX = GetRandomValue(0, GetScreenWidth());
			else correctRange = true;
		}

		correctRange = false;

		posY = GetRandomValue(0, GetScreenHeight());

		while (!correctRange)
		{
			if (posY > GetScreenHeight() / 2 - 150 && posY < GetScreenHeight() / 2 + 150)  posY = GetRandomValue(0, GetScreenHeight());
			else correctRange = true;
		}

		Vector2 pos = { posX,posY };

		bigMeteor[i]->set_position(pos);

		correctRange = false;
		velX = GetRandomValue(-METEORS_SPEED, METEORS_SPEED);
		velY = GetRandomValue(-METEORS_SPEED, METEORS_SPEED);

		while (!correctRange)
		{
			if (velX == 0 && velY == 0)
			{
				velX = GetRandomValue(-METEORS_SPEED, METEORS_SPEED);
				velY = GetRandomValue(-METEORS_SPEED, METEORS_SPEED);
			}
			else correctRange = true;
		}

		Vector2 speed = { velX,velY };
		float radius = 40.0f;

		bigMeteor[i]->set_speed(speed);
		bigMeteor[i]->set_radius(radius);
		bigMeteor[i]->set_active(true);
		bigMeteor[i]->set_color(BLUE);
	}

	for (int i = 0; i < MAX_MEDIUM_METEORS; i++)
	{

		//these meteors won't appear until a big one gets destroyed
		Vector2 pos = { -100,-100 };
		Vector2 speed = { 0,0 };
		float radius = 20;
		bool active = false;
		Color color = BLUE;
		mediumMeteor[i] = new Meteor();
		mediumMeteor[i]->set_position(pos);
		mediumMeteor[i]->set_speed(speed);
		mediumMeteor[i]->set_radius(radius);
		mediumMeteor[i]->set_active(active);
		mediumMeteor[i]->set_color(color);
	}

	for (int i = 0; i < MAX_SMALL_METEORS; i++)
	{
		Vector2 pos = { -100,-100 };
		Vector2 speed = { 0,0 };
		float radius = 10;
		bool active = false;
		Color color = BLUE;
		smallMeteor[i] = new Meteor();
		smallMeteor[i]->set_position(pos);
		smallMeteor[i]->set_speed(speed);
		smallMeteor[i]->set_radius(radius);
		smallMeteor[i]->set_active(active);
		smallMeteor[i]->set_color(color);
	}

	midMeteorsCount = 0;
	smallMeteorsCount = 0;
}

void reset_game()
{
	player->set_position({ 0, 0 });
	int posX, posY;
	int velX, velY;
	bool correctRange = false;
	for (int i = 0; i < MAX_BIG_METEORS; i++)
	{
		posX = GetRandomValue(0, GetScreenWidth());

		while (!correctRange)
		{
			if (posX > GetScreenWidth() / 2 - 150 && posX < GetScreenWidth() / 2 + 150) posX = GetRandomValue(0, GetScreenWidth());
			else correctRange = true;
		}

		correctRange = false;

		posY = GetRandomValue(0, GetScreenHeight());

		while (!correctRange)
		{
			if (posY > GetScreenHeight() / 2 - 150 && posY < GetScreenHeight() / 2 + 150)  posY = GetRandomValue(0, GetScreenHeight());
			else correctRange = true;
		}

		Vector2 pos = { posX,posY };

		bigMeteor[i]->set_position(pos);

		correctRange = false;
		velX = GetRandomValue(-METEORS_SPEED, METEORS_SPEED);
		velY = GetRandomValue(-METEORS_SPEED, METEORS_SPEED);

		while (!correctRange)
		{
			if (velX == 0 && velY == 0)
			{
				velX = GetRandomValue(-METEORS_SPEED, METEORS_SPEED);
				velY = GetRandomValue(-METEORS_SPEED, METEORS_SPEED);
			}
			else correctRange = true;
		}

		Vector2 speed = { velX,velY };
		float radius = 40.0f;

		bigMeteor[i]->set_speed(speed);
		bigMeteor[i]->set_radius(radius);
		bigMeteor[i]->set_active(true);
		bigMeteor[i]->set_color(BLUE);
	}

	for (int i = 0; i < MAX_MEDIUM_METEORS; i++)
	{

		//these meteors won't appear until a big one gets destroyed
		Vector2 pos = { -100,-100 };
		Vector2 speed = { 0,0 };
		float radius = 20;
		bool active = false;
		Color color = BLUE;
		mediumMeteor[i]->set_position(pos);
		mediumMeteor[i]->set_speed(speed);
		mediumMeteor[i]->set_radius(radius);
		mediumMeteor[i]->set_active(active);
		mediumMeteor[i]->set_color(color);
	}

	for (int i = 0; i < MAX_SMALL_METEORS; i++)
	{
		Vector2 pos = { -100,-100 };
		Vector2 speed = { 0,0 };
		float radius = 10;
		bool active = false;
		Color color = BLUE;
		smallMeteor[i]->set_position(pos);
		smallMeteor[i]->set_speed(speed);
		smallMeteor[i]->set_radius(radius);
		smallMeteor[i]->set_active(active);
		smallMeteor[i]->set_color(color);
	}

	midMeteorsCount = 0;
	smallMeteorsCount = 0;
}

void input()
{
	float player_accel_aux = player->get_acceleration();

	// Player's acceleration
	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
	{
		if (player->get_acceleration() < 1)
			player->set_acceleration(player_accel_aux += 0.04f);
	}
	else
	{
		if (player->get_acceleration() > 0) player->set_acceleration(player_accel_aux -= 0.02f);
		else if (player->get_acceleration() < 0) player->set_acceleration(0);
	}

	// Player firing bullets logic
	if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
	{
		for (int i = 0; i < PLAYER_MAX_BULLETS; i++)
		{
			if (!bullet[i]->get_active())
			{
				bullet[i]->set_position({ player->get_position().x + sin(player->get_rotation() * DEG2RAD) * (player->get_ship_height()),
										  player->get_position().y - cos(player->get_rotation() * DEG2RAD) * (player->get_ship_height()) });
				bullet[i]->set_active(true);
				bullet[i]->set_speed({ static_cast<float>(1.5 * sin(player->get_rotation() * DEG2RAD) * PLAYER_SPEED),
									   static_cast<float>(1.5 * cos(player->get_rotation() * DEG2RAD) * PLAYER_SPEED) });
				bullet[i]->set_rotation(player->get_rotation());
				break;
			}
		}
	}

	if (IsKeyPressed('P')) pause = !pause;
}


void play()
{
	init_game();
	while (!WindowShouldClose())
	{
		input();
		update();
		draw();
	}
	de_init();
	CloseWindow();
}

void init_game()
{
	InitWindow(800, 450, "un clasico: Asteroids");
	SetTargetFPS(FPS);

	victory = false;
	pause = false;

	init_player();

	init_meteors();
}