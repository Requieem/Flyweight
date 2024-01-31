#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>

#include "Vector2Int.h"

class DisplayStrings
{
private:
    static int gridWidth;
    static int gridHeight;
    static const std::string title;
    static const std::string gameover;
	static std::string CenterStringInGrid(std::string str, int gridWidth, int gridHeight);
public:
    static std::string Title();
    static std::string Gameover();
    static void Init(int gridWidth, int gridHeight);
    static std::string MapCharacter(Vector2Int pos, std::vector<std::vector<bool>> grid);
};