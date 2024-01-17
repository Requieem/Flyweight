#include "Gridshot.h"

// --------------------------
//		Global Variables
// --------------------------

Mutex mtx;
int height, width;
GameField* gameField;

// Thread function to check resizing
void check_resize() {
	int new_height, new_width;
	while (true) {
		ThisThread::sleep_for(Time::milliseconds(100));
		getmaxyx(stdscr, new_height, new_width);

		MutexLock lock(mtx);
		if (new_height != height || new_width != width) {
			height = new_height;
			width = new_width;
		}
	}
}

int main() {
	// Initialize random seed
	srand(static_cast<unsigned int>(time(nullptr)));

	// Determine the upper bound for the number of enemies
	const int maxEnemies = 25;  // Change this to your desired upper bound

	// Generate a random number of enemies (between 0 and maxEnemies)
	int numEnemies = rand() % (maxEnemies + 1);

	// Instantiate the player
	std::shared_ptr<Soldier> player = std::make_shared<Soldier>(PLAYER_COLOR, 1, 10, Vector2Int(0, 0), Vector2Int(1, 0));

	// Initialize ncurses
	initscr();
	cbreak();
	noecho();
	nodelay(stdscr, TRUE);

	// Create and populate the fieldObjects list
	FieldObjectList* fieldObjects = new FieldObjectList;
	fieldObjects->push_back(player);

	// Instantiate and add enemies to fieldObjects
	for (int i = 0; i < numEnemies; ++i) {
		std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>(ENEMY_COLOR, 1, 10, Vector2Int(0, 0), Vector2Int(0, 0), player);
		fieldObjects->push_back(enemy);
	}

	getmaxyx(stdscr, height, width);
	gameField = new GameField(*new Vector2Int(width, height), *new Vector2Int(2, 0), player, fieldObjects, 0.4f, 50);
	gameField->Init();

	// Start resize thread
	// Thread resize_thread(check_resize);

	bool gameRunning = true;
	bool spawnProjectile = false;

	Vector2Int playerDirection = player->Direction(); // Initial direction
	SharedProjectile newProjectile = nullptr;

	const Time::milliseconds tickDuration(50); // 100 ms for each tick

	while (gameRunning) {
		auto start = Time::high_resolution_clock::now();

		gameField->Draw();
		MutexLock lock(mtx);

		int key = getch(); // Get a single character input
		switch (key)
		{
		case ERR:
			player->SetMoving(false);
			break;
		case 119: // 'W' key
			playerDirection = -Vector2Int::Up;
			break;
		case 97: // 'A' key
			playerDirection = -Vector2Int::Right;
			break;
		case 115: // 'S' key
			playerDirection = Vector2Int::Up;
			break;
		case 100: // 'D' key
			playerDirection = Vector2Int::Right;
			break;
		case 32: // Spacebar
			// Set the flag to spawn a projectile
			spawnProjectile = true;
			break;
		default:
			break;
		}

		// Update the player's direction
		player->SetDirection(playerDirection);
		// Check if we should spawn a projectile
		if (spawnProjectile) {
			player->SetMoving(false);
			// Create a new projectile and add it to fieldObjects
			newProjectile = std::make_shared<Projectile>(PROJECTILE_COLOR, 2, 1, player->NextPosition(), playerDirection);
			gameField->AddFieldObject(newProjectile);

			// Reset the flag
			spawnProjectile = false;
		}

		gameRunning = !gameField->Tick();
		player->SetMoving(true);

		// Calculate the time to sleep to maintain a consistent tick rate
		auto end = Time::high_resolution_clock::now();
		auto elapsed = end - start;
		auto timeToWait = tickDuration - elapsed;
		if (timeToWait.count() > 0) {
			ThisThread::sleep_for(timeToWait);
		}
	}

	//resize_thread.join();
	endwin();
	return 0;
}