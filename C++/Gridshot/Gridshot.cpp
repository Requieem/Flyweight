#include "Gridshot.h"

// --------------------------
//		Global Variables
// --------------------------

int height, width;
GameField* gameField;

std::string emptyBar = "\u2591";
std::string fullBar = "\u2588";

static const std::string HealthBar(float min, float max, float current)
{
	float range = max - min;
	float percentage = current / range;
	int fullBars = percentage * 100 / range;

	std::string bar = "";

	for(int i = 0; i < fullBars; i++)
	{
		bar += fullBar;
	}

	for(int i = 0; i < max - fullBars; i++)
	{
		bar += emptyBar;
	}

	return bar;
}

void CursesInit()
{
	setlocale(LC_ALL, "");
	initscr();
	keypad(stdscr, TRUE);
	cbreak();
	noecho();
	nodelay(stdscr, TRUE);

	start_color();
	init_color(CAVE_COLOR, 83, 27, 0);

	init_pair(1, CAVE_COLOR, COLOR_BLACK);
	init_pair(2, PLAYER_COLOR, COLOR_BLACK);
	init_pair(3, TREASURE_COLOR, COLOR_BLACK);
	init_pair(4, COLOR_WHITE, TREASURE_COLOR);
	init_pair(5, ENEMY_COLOR, COLOR_BLACK);

	getmaxyx(stdscr, height, width);
	DisplayStrings::Init(width, height);
}

void CursesClose()
{
	endwin();
}

void GameLoop()
{

	std::shared_ptr<Player> player = std::make_shared<Player>(PLAYER_COLOR, 1, 10.0f , Vector2Int(0, 0), Vector2Int(1, 0));
	gameField = new GameField({ width, height - 7}, Vector2Int(2, 0), player);

	bool gameRunning = true;
	bool spawnProjectile = false;

	int key = ERR;

	Vector2Int playerDirection = player->NextDirection(); // Initial direction
	std::shared_ptr<Projectile> newProjectile = nullptr;

	const Time::milliseconds tickDuration(20); // 100 ms for each tick

	while (gameRunning) {
		auto start = Time::high_resolution_clock::now();

		gameField->Draw();
		auto roomGrid = *gameField->RoomGrid();

		for (int i = 0; i < roomGrid.size(); i++) {
			for (int j = roomGrid[i].size() - 1; j >= 0; j--) {
				if (roomGrid[i][j])
				{
					mvaddstr(height - 2 - roomGrid.size() + i, j * 2 + 2, emptyBar.c_str()); 
				}
				else
				{
					if (Vector2Int(j, i) == gameField->CurrentLevelPosition())
					{
						attron(COLOR_PAIR(2));
					}
					std::string c = DisplayStrings::MapCharacter(Vector2Int(j, i), roomGrid);
					mvaddstr(height - 2 - roomGrid.size() + i, j * 2 + 2, c.c_str()); 
					if (Vector2Int(j, i) == gameField->CurrentLevelPosition())
					{
						attroff(COLOR_PAIR(2));
					}
					if(j > 0 && !roomGrid[i][j-1]) mvaddstr(height - 2 - roomGrid.size() + i, j * 2 + 1, "\u2550");
				}
			}
		}

		auto healthString = HealthBar(0, 10, player->Health());

		attron(COLOR_PAIR(2));
		mvaddstr(height - 3, width - 10, "Health:");
		mvaddstr(height - 2, width - healthString.length() / 2 + 3, healthString.c_str());
		attroff(COLOR_PAIR(2));

		std::string scoreString = std::to_string(player->GetScore());
		// Pad with leading zeros
		if (scoreString.length() < 4) {
			scoreString = std::string(4 - scoreString.length(), '0') + scoreString;
		}

		scoreString = " " + scoreString + " \u0024 ";

		attron(COLOR_PAIR(3));
		mvaddstr(height - 6, width - 10, "Score:");
		attroff(COLOR_PAIR(3));
		attron(COLOR_PAIR(4));
		mvaddstr(height - 5, width - 3 - scoreString.length(), scoreString.c_str());
		attroff(COLOR_PAIR(4));

		wrefresh(stdscr);

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
	int u = 0;
	CursesInit();

	attron(COLOR_PAIR(2));
	addstr(DisplayStrings::Title().c_str());
	attroff(COLOR_PAIR(2));

	flushinp();
	while (getch() == ERR) {}

	clear();

	GameLoop();

	clear();

	attron(COLOR_PAIR(4));
	addstr(DisplayStrings::Gameover().c_str());
	attroff(COLOR_PAIR(4));
	
	flushinp();
	while (getch() == ERR) {}

	CursesClose();
	return 0;
}