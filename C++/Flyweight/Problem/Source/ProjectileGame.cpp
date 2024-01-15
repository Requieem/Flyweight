// Problem.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <thread>
#include <conio.h>
#include <iostream>
#include "GameField.h"
#include "FieldObject.h"
#include "Projectile.h"

int main() {
    auto player = std::make_shared<Soldier>(0, 1, 10, Vector2Int(0, 0), Vector2Int(1, 0));
    auto enemy = std::make_shared<Soldier>(0, 1, 10, Vector2Int(2, 2), Vector2Int(1, 0));

    FieldObjectList* fieldObjects = new FieldObjectList;
    fieldObjects->push_back(player);
    fieldObjects->push_back(enemy);

    GameField gameField = *new GameField(*new Vector2Int(50, 50), player, fieldObjects);

    bool gameRunning = true;

    Vector2Int playerDirection = player->Direction(); // Initial direction
    bool spawnProjectile = false;
    bool act = true;
    std::shared_ptr<Projectile> newProjectile = nullptr;

    while (gameRunning) {
        if(act) gameField.Draw();
        act = true;

        int key = _getch(); // Get a single character input

        switch (key) {
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
            act = false;
            break;
        }

        if (!act) continue;

        // Update the player's direction
        player->SetDirection(playerDirection);
        // Check if we should spawn a projectile
        if (spawnProjectile) {
            player->SetMoving(false);
            // Create a new projectile and add it to fieldObjects
            newProjectile = std::make_shared<Projectile>(0, 2, 1, player->NextPosition(), playerDirection);
            fieldObjects->push_back(newProjectile);

            // Reset the flag
            spawnProjectile = false;
        }

        gameRunning = !gameField.Tick();
        player->SetMoving(true);
    }

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
