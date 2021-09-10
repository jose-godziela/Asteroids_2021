#include "raylib.h"

#include <cmath>

//----------------------------------------------------------------------------------
// Some Defines
//----------------------------------------------------------------------------------
#define PLAYER_BASE_SIZE    20.0f
#define PLAYER_SPEED        6.0f
#define PLAYER_MAX_SHOOTS   10

#define METEORS_SPEED       2
#define MAX_BIG_METEORS     4
#define MAX_MEDIUM_METEORS  8
#define MAX_SMALL_METEORS   16

#define FPS 60

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
class Player {
private:
	Vector2 _position;
	Vector2 _speed;
	float   _acceleration;
	float   _rotation;
	Vector3 _collider;
	Color   _color;
public:
	Player();
	~Player();
	void set_position(Vector2 position) { _position = position; };
	void set_speed(Vector2 speed) { _speed = speed; };
	void set_acceleration(float acceleration) { _acceleration = acceleration; };
	void set_rotation(float rotation) { _rotation = rotation; };
	void set_collider(Vector3 collider) { _collider = collider; };
	void set_color(Color color) { _color = color; };
	Vector2 get_position() { return _position; };
	Vector2 get_speed() { return _speed; };
	float get_acceleration() { return _acceleration; };
	float get_rotation() { return _rotation; };
	Vector3 get_collider() { return _collider; };
	Color get_color() { return _color; };

};

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
	void set_position(Vector2 position) { _position = position; };
	void set_speed(Vector2 speed) { _speed = speed; };
	void set_radius(float radius) { _radius = radius; };
	void set_rotation(float rotation) { _rotation = rotation; };
	void set_life(int life) { _life = life; };
	void set_active(bool active) { _active = active; };
	void set_color(Color color) { _color = color; };
	Vector2 get_position() { return _position; };
	Vector2	get_speed() { return _speed; };
	float	get_radius() { return _radius; };
	float	get_rotation() { return _rotation; };
	int		get_life() { return _life; };
	bool	get_active() { return _active; };
	Color	get_color() { return _color; };
};

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
	Vector2 get_position() { return _position; };
	Vector2 get_speed() { return _speed; };
	float get_radius() { return _radius; };
	bool get_active() { return _active; };
	Color get_color() { return _color; };
};

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------
static const int screenWidth = 800;
static const int screenHeight = 450;

static bool gameOver = false;
static bool pause = false;
static bool victory = false;

// NOTE: Defined triangle is isosceles with common angles of 70 degrees.
static float shipHeight = 0.0f;

Player* player = new Player();
Bullet* bullet[PLAYER_MAX_SHOOTS];
Meteor* bigMeteor[MAX_BIG_METEORS];
Meteor* mediumMeteor[MAX_MEDIUM_METEORS];
Meteor* smallMeteor[MAX_SMALL_METEORS];

static int midMeteorsCount = 0;
static int smallMeteorsCount = 0;
static int destroyedMeteorsCount = 0;

//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static void InitGame(void);         // Initialize game
static void UpdateGame(void);       // Update game (one frame)
static void DrawGame(void);         // Draw game (one frame)
static void UnloadGame(void);       // Unload game
static void UpdateDrawFrame(void);  // Update and Draw (one frame)

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
	// Initialization (Note windowTitle is unused on Android)
	//---------------------------------------------------------
	InitWindow(screenWidth, screenHeight, "un clasico: Asteroids");

	InitGame();

	SetTargetFPS(FPS);
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update and Draw
		//----------------------------------------------------------------------------------
		UpdateDrawFrame();
		//----------------------------------------------------------------------------------
	}
	// De-Initialization
	//--------------------------------------------------------------------------------------
	UnloadGame();         // Unload loaded data (textures, sounds, models...)

	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}

//------------------------------------------------------------------------------------
// Module Functions Definitions (local)
//------------------------------------------------------------------------------------

// Initialize game variables
void InitGame(void)
{
	int posX, posY;
	int velX, velY;
	bool correctRange = false;
	victory = false;
	pause = false;

	shipHeight = (PLAYER_BASE_SIZE / 2) / tanf(20 * DEG2RAD);


	Vector2 aux_player_pos = { screenWidth / 2, screenHeight / 2 - shipHeight / 2 };
	Vector2 aux_speed = { 0,0 };

	// Initialization player
	player->set_position(aux_player_pos);
	player->set_speed(aux_speed);
	player->set_acceleration(0.0f);
	player->set_rotation(0.0f);
	player->set_color(LIGHTGRAY);

	//this is to set properly the collider

	Vector3 aux_collider = {
		aux_player_pos.x + sin(player->get_rotation() * DEG2RAD) * (shipHeight / 2.5f), aux_player_pos.y - cos(player->get_rotation() * DEG2RAD) * (shipHeight / 2.5f), 12
	};//      X            + sin(rotation * DEG2RAD [transformation]) * (shipHeight /2.5f), Y			   - cos(rotation * DEG2RAD) * (shipHeight / 2.5f), 12

	player->set_collider(aux_collider);

	destroyedMeteorsCount = 0;

	// Initialization shoot
	for (int i = 0; i < PLAYER_MAX_SHOOTS; i++)
	{
		bullet[i] = new Bullet();
		Vector2 pos_init = { 0,0 };
		Vector2 speed_init = { 0,0 };
		float aux_radius = 2.0f;
		bullet[i]->set_position(pos_init);
		bullet[i]->set_speed(speed_init);
		bullet[i]->set_radius(aux_radius);
		bullet[i]->set_active(false);
		bullet[i]->set_life(0);
		bullet[i]->set_color(WHITE);
	}

	for (int i = 0; i < MAX_BIG_METEORS; i++)
	{
		bigMeteor[i] = new Meteor();
		posX = GetRandomValue(0, screenWidth);

		while (!correctRange)
		{
			if (posX > screenWidth / 2 - 150 && posX < screenWidth / 2 + 150) posX = GetRandomValue(0, screenWidth);
			else correctRange = true;
		}

		correctRange = false;

		posY = GetRandomValue(0, screenHeight);

		while (!correctRange)
		{
			if (posY > screenHeight / 2 - 150 && posY < screenHeight / 2 + 150)  posY = GetRandomValue(0, screenHeight);
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
		Vector2 pos = {-100,-100};
		Vector2 speed = {0,0};
		mediumMeteor[i] = new Meteor();
		mediumMeteor[i]->set_position(pos);
		mediumMeteor[i]->set_speed(speed) = (Vector2){ 0,0 };
		mediumMeteor[i]->set_radius() = 20;
		mediumMeteor[i]->set_active() = false;
		mediumMeteor[i]->set_color() = BLUE;
	}

	for (int i = 0; i < MAX_SMALL_METEORS; i++)
	{
		smallMeteor[i].position = (Vector2){ -100, -100 };
		smallMeteor[i].speed = (Vector2){ 0,0 };
		smallMeteor[i].radius = 10;
		smallMeteor[i].active = false;
		smallMeteor[i].color = BLUE;
	}

	midMeteorsCount = 0;
	smallMeteorsCount = 0;
}

// Update game (one frame)
void UpdateGame(void)
{
	if (!gameOver)
	{
		if (IsKeyPressed('P')) pause = !pause;

		if (!pause)
		{
			// Player logic: rotation
			if (IsKeyDown(KEY_LEFT)) player.rotation -= 5;
			if (IsKeyDown(KEY_RIGHT)) player.rotation += 5;

			// Player logic: speed
			player.speed.x = sin(player.rotation * DEG2RAD) * PLAYER_SPEED;
			player.speed.y = cos(player.rotation * DEG2RAD) * PLAYER_SPEED;

			// Player logic: acceleration
			if (IsKeyDown(KEY_UP))
			{
				if (player.acceleration < 1) player.acceleration += 0.04f;
			}
			else
			{
				if (player.acceleration > 0) player.acceleration -= 0.02f;
				else if (player.acceleration < 0) player.acceleration = 0;
			}
			if (IsKeyDown(KEY_DOWN))
			{
				if (player.acceleration > 0) player.acceleration -= 0.04f;
				else if (player.acceleration < 0) player.acceleration = 0;
			}

			// Player logic: movement
			player.position.x += (player.speed.x * player.acceleration);
			player.position.y -= (player.speed.y * player.acceleration);

			// Collision logic: player vs walls
			if (player.position.x > screenWidth + shipHeight) player.position.x = -(shipHeight);
			else if (player.position.x < -(shipHeight)) player.position.x = screenWidth + shipHeight;
			if (player.position.y > (screenHeight + shipHeight)) player.position.y = -(shipHeight);
			else if (player.position.y < -(shipHeight)) player.position.y = screenHeight + shipHeight;

			// Player shoot logic
			if (IsKeyPressed(KEY_SPACE))
			{
				for (int i = 0; i < PLAYER_MAX_SHOOTS; i++)
				{
					if (!shoot[i].active)
					{
						shoot[i].position = (Vector2){ player.position.x + sin(player.rotation * DEG2RAD) * (shipHeight), player.position.y - cos(player.rotation * DEG2RAD) * (shipHeight) };
						shoot[i].active = true;
						shoot[i].speed.x = 1.5 * sin(player.rotation * DEG2RAD) * PLAYER_SPEED;
						shoot[i].speed.y = 1.5 * cos(player.rotation * DEG2RAD) * PLAYER_SPEED;
						shoot[i].rotation = player.rotation;
						break;
					}
				}
			}

			// Shoot life timer
			for (int i = 0; i < PLAYER_MAX_SHOOTS; i++)
			{
				if (shoot[i].active) shoot[i].lifeSpawn++;
			}

			// Shot logic
			for (int i = 0; i < PLAYER_MAX_SHOOTS; i++)
			{
				if (shoot[i].active)
				{
					// Movement
					shoot[i].position.x += shoot[i].speed.x;
					shoot[i].position.y -= shoot[i].speed.y;

					// Collision logic: shoot vs walls
					if (shoot[i].position.x > screenWidth + shoot[i].radius)
					{
						shoot[i].active = false;
						shoot[i].lifeSpawn = 0;
					}
					else if (shoot[i].position.x < 0 - shoot[i].radius)
					{
						shoot[i].active = false;
						shoot[i].lifeSpawn = 0;
					}
					if (shoot[i].position.y > screenHeight + shoot[i].radius)
					{
						shoot[i].active = false;
						shoot[i].lifeSpawn = 0;
					}
					else if (shoot[i].position.y < 0 - shoot[i].radius)
					{
						shoot[i].active = false;
						shoot[i].lifeSpawn = 0;
					}

					// Life of shoot
					if (shoot[i].lifeSpawn >= 60)
					{
						shoot[i].position = (Vector2){ 0, 0 };
						shoot[i].speed = (Vector2){ 0, 0 };
						shoot[i].lifeSpawn = 0;
						shoot[i].active = false;
					}
				}
			}

			// Collision logic: player vs meteors
			player.collider = (Vector3){ player.position.x + sin(player.rotation * DEG2RAD) * (shipHeight / 2.5f), player.position.y - cos(player.rotation * DEG2RAD) * (shipHeight / 2.5f), 12 };

			for (int a = 0; a < MAX_BIG_METEORS; a++)
			{
				if (CheckCollisionCircles((Vector2) { player.collider.x, player.collider.y }, player.collider.z, bigMeteor[a].position, bigMeteor[a].radius) && bigMeteor[a].active) gameOver = true;
			}

			for (int a = 0; a < MAX_MEDIUM_METEORS; a++)
			{
				if (CheckCollisionCircles((Vector2) { player.collider.x, player.collider.y }, player.collider.z, mediumMeteor[a].position, mediumMeteor[a].radius) && mediumMeteor[a].active) gameOver = true;
			}

			for (int a = 0; a < MAX_SMALL_METEORS; a++)
			{
				if (CheckCollisionCircles((Vector2) { player.collider.x, player.collider.y }, player.collider.z, smallMeteor[a].position, smallMeteor[a].radius) && smallMeteor[a].active) gameOver = true;
			}

			// Meteors logic: big meteors
			for (int i = 0; i < MAX_BIG_METEORS; i++)
			{
				if (bigMeteor[i].active)
				{
					// Movement
					bigMeteor[i].position.x += bigMeteor[i].speed.x;
					bigMeteor[i].position.y += bigMeteor[i].speed.y;

					// Collision logic: meteor vs wall
					if (bigMeteor[i].position.x > screenWidth + bigMeteor[i].radius) bigMeteor[i].position.x = -(bigMeteor[i].radius);
					else if (bigMeteor[i].position.x < 0 - bigMeteor[i].radius) bigMeteor[i].position.x = screenWidth + bigMeteor[i].radius;
					if (bigMeteor[i].position.y > screenHeight + bigMeteor[i].radius) bigMeteor[i].position.y = -(bigMeteor[i].radius);
					else if (bigMeteor[i].position.y < 0 - bigMeteor[i].radius) bigMeteor[i].position.y = screenHeight + bigMeteor[i].radius;
				}
			}

			// Meteors logic: medium meteors
			for (int i = 0; i < MAX_MEDIUM_METEORS; i++)
			{
				if (mediumMeteor[i].active)
				{
					// Movement
					mediumMeteor[i].position.x += mediumMeteor[i].speed.x;
					mediumMeteor[i].position.y += mediumMeteor[i].speed.y;

					// Collision logic: meteor vs wall
					if (mediumMeteor[i].position.x > screenWidth + mediumMeteor[i].radius) mediumMeteor[i].position.x = -(mediumMeteor[i].radius);
					else if (mediumMeteor[i].position.x < 0 - mediumMeteor[i].radius) mediumMeteor[i].position.x = screenWidth + mediumMeteor[i].radius;
					if (mediumMeteor[i].position.y > screenHeight + mediumMeteor[i].radius) mediumMeteor[i].position.y = -(mediumMeteor[i].radius);
					else if (mediumMeteor[i].position.y < 0 - mediumMeteor[i].radius) mediumMeteor[i].position.y = screenHeight + mediumMeteor[i].radius;
				}
			}

			// Meteors logic: small meteors
			for (int i = 0; i < MAX_SMALL_METEORS; i++)
			{
				if (smallMeteor[i].active)
				{
					// Movement
					smallMeteor[i].position.x += smallMeteor[i].speed.x;
					smallMeteor[i].position.y += smallMeteor[i].speed.y;

					// Collision logic: meteor vs wall
					if (smallMeteor[i].position.x > screenWidth + smallMeteor[i].radius) smallMeteor[i].position.x = -(smallMeteor[i].radius);
					else if (smallMeteor[i].position.x < 0 - smallMeteor[i].radius) smallMeteor[i].position.x = screenWidth + smallMeteor[i].radius;
					if (smallMeteor[i].position.y > screenHeight + smallMeteor[i].radius) smallMeteor[i].position.y = -(smallMeteor[i].radius);
					else if (smallMeteor[i].position.y < 0 - smallMeteor[i].radius) smallMeteor[i].position.y = screenHeight + smallMeteor[i].radius;
				}
			}

			// Collision logic: player-shoots vs meteors
			for (int i = 0; i < PLAYER_MAX_SHOOTS; i++)
			{
				if ((shoot[i].active))
				{
					for (int a = 0; a < MAX_BIG_METEORS; a++)
					{
						if (bigMeteor[a].active && CheckCollisionCircles(shoot[i].position, shoot[i].radius, bigMeteor[a].position, bigMeteor[a].radius))
						{
							shoot[i].active = false;
							shoot[i].lifeSpawn = 0;
							bigMeteor[a].active = false;
							destroyedMeteorsCount++;

							for (int j = 0; j < 2; j++)
							{
								if (midMeteorsCount % 2 == 0)
								{
									mediumMeteor[midMeteorsCount].position = (Vector2){ bigMeteor[a].position.x, bigMeteor[a].position.y };
									mediumMeteor[midMeteorsCount].speed = (Vector2){ cos(shoot[i].rotation * DEG2RAD) * METEORS_SPEED * -1, sin(shoot[i].rotation * DEG2RAD) * METEORS_SPEED * -1 };
								}
								else
								{
									mediumMeteor[midMeteorsCount].position = (Vector2){ bigMeteor[a].position.x, bigMeteor[a].position.y };
									mediumMeteor[midMeteorsCount].speed = (Vector2){ cos(shoot[i].rotation * DEG2RAD) * METEORS_SPEED, sin(shoot[i].rotation * DEG2RAD) * METEORS_SPEED };
								}

								mediumMeteor[midMeteorsCount].active = true;
								midMeteorsCount++;
							}
							//bigMeteor[a].position = (Vector2){-100, -100};
							bigMeteor[a].color = RED;
							a = MAX_BIG_METEORS;
						}
					}

					for (int b = 0; b < MAX_MEDIUM_METEORS; b++)
					{
						if (mediumMeteor[b].active && CheckCollisionCircles(shoot[i].position, shoot[i].radius, mediumMeteor[b].position, mediumMeteor[b].radius))
						{
							shoot[i].active = false;
							shoot[i].lifeSpawn = 0;
							mediumMeteor[b].active = false;
							destroyedMeteorsCount++;

							for (int j = 0; j < 2; j++)
							{
								if (smallMeteorsCount % 2 == 0)
								{
									smallMeteor[smallMeteorsCount].position = (Vector2){ mediumMeteor[b].position.x, mediumMeteor[b].position.y };
									smallMeteor[smallMeteorsCount].speed = (Vector2){ cos(shoot[i].rotation * DEG2RAD) * METEORS_SPEED * -1, sin(shoot[i].rotation * DEG2RAD) * METEORS_SPEED * -1 };
								}
								else
								{
									smallMeteor[smallMeteorsCount].position = (Vector2){ mediumMeteor[b].position.x, mediumMeteor[b].position.y };
									smallMeteor[smallMeteorsCount].speed = (Vector2){ cos(shoot[i].rotation * DEG2RAD) * METEORS_SPEED, sin(shoot[i].rotation * DEG2RAD) * METEORS_SPEED };
								}

								smallMeteor[smallMeteorsCount].active = true;
								smallMeteorsCount++;
							}
							//mediumMeteor[b].position = (Vector2){-100, -100};
							mediumMeteor[b].color = GREEN;
							b = MAX_MEDIUM_METEORS;
						}
					}

					for (int c = 0; c < MAX_SMALL_METEORS; c++)
					{
						if (smallMeteor[c].active && CheckCollisionCircles(shoot[i].position, shoot[i].radius, smallMeteor[c].position, smallMeteor[c].radius))
						{
							shoot[i].active = false;
							shoot[i].lifeSpawn = 0;
							smallMeteor[c].active = false;
							destroyedMeteorsCount++;
							smallMeteor[c].color = YELLOW;
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
		if (IsKeyPressed(KEY_ENTER))
		{
			InitGame();
			gameOver = false;
		}
	}
}

// Draw game (one frame)
void DrawGame(void)
{
	BeginDrawing();

	ClearBackground(RAYWHITE);

	if (!gameOver)
	{
		// Draw spaceship
		Vector2 v1 = { player.position.x + sinf(player.rotation * DEG2RAD) * (shipHeight), player.position.y - cosf(player.rotation * DEG2RAD) * (shipHeight) };
		Vector2 v2 = { player.position.x - cosf(player.rotation * DEG2RAD) * (PLAYER_BASE_SIZE / 2), player.position.y - sinf(player.rotation * DEG2RAD) * (PLAYER_BASE_SIZE / 2) };
		Vector2 v3 = { player.position.x + cosf(player.rotation * DEG2RAD) * (PLAYER_BASE_SIZE / 2), player.position.y + sinf(player.rotation * DEG2RAD) * (PLAYER_BASE_SIZE / 2) };
		DrawTriangle(v1, v2, v3, MAROON);

		// Draw meteors
		for (int i = 0; i < MAX_BIG_METEORS; i++)
		{
			if (bigMeteor[i].active) DrawCircleV(bigMeteor[i].position, bigMeteor[i].radius, DARKGRAY);
			else DrawCircleV(bigMeteor[i].position, bigMeteor[i].radius, Fade(LIGHTGRAY, 0.3f));
		}

		for (int i = 0; i < MAX_MEDIUM_METEORS; i++)
		{
			if (mediumMeteor[i].active) DrawCircleV(mediumMeteor[i].position, mediumMeteor[i].radius, GRAY);
			else DrawCircleV(mediumMeteor[i].position, mediumMeteor[i].radius, Fade(LIGHTGRAY, 0.3f));
		}

		for (int i = 0; i < MAX_SMALL_METEORS; i++)
		{
			if (smallMeteor[i].active) DrawCircleV(smallMeteor[i].position, smallMeteor[i].radius, GRAY);
			else DrawCircleV(smallMeteor[i].position, smallMeteor[i].radius, Fade(LIGHTGRAY, 0.3f));
		}

		// Draw shoot
		for (int i = 0; i < PLAYER_MAX_SHOOTS; i++)
		{
			if (shoot[i].active) DrawCircleV(shoot[i].position, shoot[i].radius, BLACK);
		}

		if (victory) DrawText("VICTORY", screenWidth / 2 - MeasureText("VICTORY", 20) / 2, screenHeight / 2, 20, LIGHTGRAY);

		if (pause) DrawText("GAME PAUSED", screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, screenHeight / 2 - 40, 40, GRAY);
	}
	else DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 2 - 50, 20, GRAY);

	EndDrawing();
}

// Unload game variables
void UnloadGame(void)
{
	// TODO: Unload all dynamic loaded data (textures, sounds, models...)
}

// Update and Draw (one frame)
void UpdateDrawFrame(void)
{
	UpdateGame();
	DrawGame();
}