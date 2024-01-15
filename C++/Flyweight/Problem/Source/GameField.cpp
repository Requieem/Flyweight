#include "GameField.h"
#include "Projectile.h"

const std::string RED_COLOR = "\033[1;31m";  // Red text
const std::string GREEN_COLOR = "\033[1;32m";  // Green text
const std::string RESET_COLOR = "\033[0m";  // Reset text color to default

GameField::GameField(Vector2Int size, std::shared_ptr<Soldier> player, FieldObjectList* fieldObjects) : size(size), player(player)
{
	this->fieldObjects = fieldObjects;
	
	for (const SharedFieldObject& current : *fieldObjects)
	{
		objectMap[current->Position()].push_back(current);
	}
}

bool GameField::Tick()
{
	for (auto it = fieldObjects->begin(); it != fieldObjects->end();) {
		const SharedFieldObject& obj = *it;

		if (MakeFieldMove(obj)) it = fieldObjects->erase(it);
		else ++it;
	}

	return player->IsDead() || IsOutOfBounds(player->Position());
}

#define ANSI_RED "\033[31m"
#define ANSI_GREEN "\033[32m"
#define ANSI_YELLOW "\033[33m"
#define ANSI_CYAN "\033[36m"

void GameField::Draw() const {
    // ANSI color codes corresponding to your character types
    const char* playerColor = ANSI_GREEN; // Red for player
    const char* enemyColor = ANSI_RED; // Green for enemy soldiers
    const char* projectileColor = ANSI_CYAN; // Cyan for projectiles
    const char* environmentColor = ANSI_YELLOW;

    // Create a 2D grid to represent the console (without padding)
    std::vector<std::vector<std::string>> consoleGrid(size.y * 2 + 1, std::vector<std::string>(size.x * 2 + 1, " "));

    // Fill horizontal lines
    for (int y = 0; y < size.y + 1; y += 1) {
        for (int x = 0; x < size.x * 2 + 1; ++x) {
            consoleGrid[y][x] = "_";
        }
    }

    // Fill vertical lines
    for (int y = 1; y < size.y + 1; y += 1) {
        for (int x = 0; x < size.x * 2 + 1; x += 2) {
            consoleGrid[y][x] = "|";
        }
    }

    // Iterate through fieldObjects and update the grid based on object positions
    for (const auto& obj : *fieldObjects) {
        const Vector2Int& position = obj->Position();

        // Assuming you have a character representation for FieldObjects (e.g., 'S' for Soldier)
        std::string objectChar = " "; // Default character

        // Check the type of FieldObject
        if (dynamic_cast<Soldier*>(obj.get()) != nullptr) {
            // If it's a Soldier, use 'S' character with corresponding color
            if (obj == player) {
                objectChar = 219; // Player character (full block)
                objectChar = playerColor + objectChar + environmentColor; // Add color codes
            }
            else {
                objectChar = 178; // Enemy Soldier character (medium shade block)
                objectChar = enemyColor + objectChar + environmentColor; // Add color codes
            }
        }
        else if (dynamic_cast<Projectile*>(obj.get()) != nullptr) {
            // If it's a Projectile, use 'P' character with corresponding color
            objectChar = 254;
            objectChar = projectileColor + objectChar + environmentColor; // Add color codes
        }

        // Calculate the position in the console grid (considering vertical and horizontal dividers)
        int gridX = position.x * 2 + 1;
        int gridY = position.y + 1;

        // Update the grid cell with the colored object character
        consoleGrid[gridY][gridX] = objectChar;
    }

    // Clear the console before printing the updated grid
    system("cls"); // Windows-specific, use "clear" for Unix/Linux

    // Print the console grid
    for (const std::vector<std::string>& row : consoleGrid) {
        for (const std::string& cell : row) {
            std::cout << cell;
        }

        std::cout << std::endl;
    }
}

bool GameField::IsOutOfBounds(Vector2Int position) const
{
	int x = position.x;
	int y = position.y;
	int horizontalMax = size.x;
	int verticalMax = size.y;

	return x < 0 || y < 0 || x >= horizontalMax || y >= verticalMax;
}

bool GameField::MakeFieldMove(const SharedFieldObject& obj)
{
    if (!obj->IsMoving()) return false;

	Vector2Int direction = obj->Direction();
	Vector2Int position = obj->Position();

	bool isCrossingBounds = IsCrossingBounds(position, direction);
	auto pairsIterator = objectMap.find(position);

	if (pairsIterator != objectMap.end())
	{
		FieldObjectList objectsAtThisPosition = pairsIterator->second;
		auto valueIterator = std::find(objectsAtThisPosition.begin(), objectsAtThisPosition.end(), obj);

		if (valueIterator != objectsAtThisPosition.end())
		{
			objectsAtThisPosition.erase(valueIterator);
		}
	}

	if (isCrossingBounds) return true;

	obj->Move();
	position = obj->Position();
	objectMap[position].push_back(obj);

	return false;
}

bool GameField::IsCrossingBounds(Vector2Int position, Vector2Int direction)
{
	Vector2Int nextPosition = Vector2Int::Sum(position, direction);

	int x = nextPosition.x;
	int y = nextPosition.y;
	int horizontalMax = size.x;
	int verticalMax = size.y;

	return x < 0 || y < 0 || x >= horizontalMax || y >= verticalMax;
}

