#pragma once

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Vector2Int.h"

typedef std::vector<std::vector<bool>> BoolGrid;
typedef std::vector<Vector2Int> Vector2IntList;

class CaveGenerator
{
private:
    BoolGrid cave;
    BoolGrid minusCave;
    Vector2Int size;
    float fillProbability;
    Vector2IntList emptySpaces;

    void GenerateInitialCave();
    int CountSolidNeighbors(int x, int y);
    void CaveIteration();

public:
    CaveGenerator(Vector2Int size, float fillProbability);
    void GenerateCave(int iterations, Vector2Int* size, float* fillProbability);
    Vector2Int GetRandomEmptySpace();
    Vector2Int GetEnemySpace(Vector2Int fromPosition, int minDistance);
    const BoolGrid GetCave();
    const BoolGrid GetMinusCave();
    const Vector2Int GetSize();
};
