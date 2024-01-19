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

const unsigned char emptyBar = 176;
const unsigned char fullBar = 219;

static const std::string HealthBar(float min, float max, float current)
{
	float range = max - min;
	float percentage = current / range;
	int fullBars = percentage * 100 / range;

	return std::string(fullBars, fullBar) + std::string(max - fullBars, emptyBar);
}


void CursesInit()
{
	initscr();
	cbreak();
	noecho();
	nodelay(stdscr, TRUE);

	start_color();
	init_color(CAVE_COLOR, 200, 50, 25);

	init_pair(1, CAVE_COLOR, COLOR_BLACK);
	init_pair(2, PLAYER_COLOR, COLOR_WHITE);
	init_pair(3, COLOR_WHITE, TREASURE_COLOR);
	init_pair(4, COLOR_WHITE, ENEMY_COLOR);

	getmaxyx(stdscr, height, width);
}
void CursesClose()
{
	endwin();
}
void GameLoop()
{

	std::shared_ptr<Player> player = std::make_shared<Player>(PLAYER_COLOR, 1, 10.0f , Vector2Int(0, 0), Vector2Int(1, 0));
	gameField = new GameField({ width, height - 3 }, *new Vector2Int(2, 0), player, 0.45f, 50);
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

		attron(COLOR_PAIR(2));
		mvaddstr(height - 2, 5, (" Health: " + HealthBar(0, 10, player->Health())).c_str());
		attroff(COLOR_PAIR(2));

		std::string scoreString = std::to_string(player->GetScore());
		// Pad with leading zeros
		if (scoreString.length() < 4) {
			scoreString = std::string(4 - scoreString.length(), '0') + scoreString;
		}

		scoreString = " " + scoreString + "\u0024 ";

		attron(COLOR_PAIR(3));
		mvaddstr(height - 2, width - 6 - scoreString.length(), scoreString.c_str());
		attroff(COLOR_PAIR(3));

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
		if (spawnProjectile && !player->WillGoOutOfBounds(playerDirection)) {
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

	attron(COLOR_PAIR(2));
	addstr(title.c_str());
	attroff(COLOR_PAIR(2));

	flushinp();
	while (getch() == ERR) {}

	GameLoop();

	clear();

	attron(COLOR_PAIR(4));
	addstr(gameover.c_str());
	attroff(COLOR_PAIR(4));
	
	flushinp();
	while (getch() == ERR) {}

	CursesClose();
	return 0;
}

