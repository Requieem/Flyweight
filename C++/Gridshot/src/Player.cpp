#include "../include/Player.h"

Player::Player(int teamColor, int speed, float startingHealth, Vector2Int position, Vector2Int direction) 
    : Soldier(teamColor, speed, startingHealth, position, direction), Life(startingHealth) {}