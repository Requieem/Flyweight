#include "Gridshot.h"

// --------------------------
//		Global Variables
// --------------------------

int height, width;
GameField* gameField;

const std::string gameover = R"(
     ____                         ___                   
    / ___| __ _ _ __ ___   ___   / _ \__   _____ _ __   
   | |  _ / _` | '_ ` _ \ / _ \ | | | \ \ / / _ | '__|  
   | |_| | (_| | | | | | |  __/ | |_| |\ V |  __| |     
    \____|\__,_|_| |_| |_|\___|  \___/  \_/ \___|_|   _ 
  _ __  _ __ ___ ___ ___   ___ _ __   __ _  ___ ___  | |
 | '_ \| '__/ _ / __/ __| / __| '_ \ / _` |/ __/ _ \ | |
 | |_) | | |  __\__ \__ \ \__ | |_) | (_| | (_|  __/ |_|
 | .__/|_|  \___|___|___/ |___| .__/ \__,_|\___\___| (_)
 |_|                          |_|                       
                                                  
)";

const std::string title = R"(
           ____      _     _ ____  _           _                  
          / ___|_ __(_) __| / ___|| |__   ___ | |_   _            
         | |  _| '__| |/ _` \___ \| '_ \ / _ \| __| (_)           
         | |_| | |  | | (_| |___) | | | | (_) | |_   _            
          \____|_|  |_|\__,_|____/|_| |_|\___/ \__| (_)           
        _____ _             ____                                  
       |_   _| |__   ___   / ___| __ _ _ __ ___   ___             
         | | | '_ \ / _ \ | |  _ / _` | '_ ` _ \ / _ \            
         | | | | | |  __/ | |_| | (_| | | | | | |  __/            
         |_| |_| |_|\___|  \____|\__,_|_| |_| |_|\___|            
  ____                         _          ____  _             _   
 / ___| _ __   __ _  ___ ___  | |_ ___   / ___|| |_ __ _ _ __| |_ 
 \___ \| '_ \ / _` |/ __/ _ \ | __/ _ \  \___ \| __/ _` | '__| __|
  ___) | |_) | (_| | (_|  __/ | || (_) |  ___) | || (_| | |  | |_ 
 |____/| .__/ \__,_|\___\___|  \__\___/  |____/ \__\__,_|_|   \__|
       |_|                                                        
)";


void CursesInit()
{
	initscr();
	cbreak();
	noecho();
	nodelay(stdscr, TRUE);
	getmaxyx(stdscr, height, width);
}
void CursesClose()
{
	endwin();
}
void GameLoop()
{
	const int minEnemies = 10;
	const int maxEnemies = 25;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distribution(minEnemies, maxEnemies);
	int numEnemies = distribution(gen);
	std::shared_ptr<Soldier> player = std::make_shared<Soldier>(PLAYER_COLOR, 1, 10, Vector2Int(0, 0), Vector2Int(1, 0));

	// Create and populate the fieldObjects list
	FieldObjectList fieldObjects = std::make_shared<std::vector<std::shared_ptr<FieldObject>>>();
	fieldObjects->push_back(player);

	// Instantiate and add enemies to fieldObjects
	for (int i = 0; i < numEnemies; ++i) {
		std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>(ENEMY_COLOR, 1, 10, Vector2Int(0, 0), Vector2Int(0, 0), player);
		fieldObjects->push_back(enemy);
	}

	gameField = new GameField({ width, height }, *new Vector2Int(2, 0), player, fieldObjects, 0.4f, 50);
	gameField->Init();

	bool gameRunning = true;
	bool spawnProjectile = false;

	int key = ERR;

	Vector2Int playerDirection = player->NextDirection(); // Initial direction
	std::shared_ptr<Projectile> newProjectile = nullptr;

	const Time::milliseconds tickDuration(20); // 100 ms for each tick

	while (gameRunning) {
		auto start = Time::high_resolution_clock::now();

		gameField->Draw();
		key = getch(); // Get a single character input
		flushinp();

		switch (key)
		{
		case ERR:
			player->SetMoving(false);
			break;
		case 119: // 'W' key
			playerDirection = Vector2Int::Down;
			break;
		case 97: // 'A' key
			playerDirection = Vector2Int::Left;
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
			newProjectile = std::make_shared<Projectile>(PROJECTILE_COLOR, 2, 1, player->NextPosition(playerDirection), playerDirection);
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
}

int main() {
	CursesInit();

	addstr( title.c_str());

	flushinp();
	while (getch() == ERR) {}

	GameLoop();

	clear();
	addstr(gameover.c_str());
	
	flushinp();
	while (getch() == ERR) {}

	CursesClose();
	return 0;
}

