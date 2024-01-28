#include "DisplayStrings.h"
#include "Vector2Int.h"

int DisplayStrings::gridWidth = 0;
int DisplayStrings::gridHeight = 0;

const std::string DisplayStrings::gameover = R"(
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

const std::string DisplayStrings::title = R"(
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

void DisplayStrings::Init(int gridWidth, int gridHeight)
{
	DisplayStrings::gridWidth = gridWidth;
	DisplayStrings::gridHeight = gridHeight;
}

std::string DisplayStrings::Gameover() {
	return DisplayStrings::CenterStringInGrid(DisplayStrings::gameover, DisplayStrings::gridWidth, DisplayStrings::gridHeight);
}

std::string DisplayStrings::Title() {
	return DisplayStrings::CenterStringInGrid(DisplayStrings::title, DisplayStrings::gridWidth, DisplayStrings::gridHeight);
}

std::string DisplayStrings::CenterStringInGrid(std::string str, int gridWidth, int gridHeight) {
	std::istringstream iss(str);
	std::string line;
	std::string centeredStr;
	int maxWidth = 0;
	int numLines = 0;

	// First, find the maximum width and the number of lines
	while (std::getline(iss, line)) {
		maxWidth = std::max(maxWidth, static_cast<int>(line.length()));
		numLines++;
	}

	// Calculate horizontal and vertical padding
	int horizontalPadding = (gridWidth - maxWidth) / 2;
	int verticalPaddingTop = (gridHeight - numLines) / 2;
	int verticalPaddingBottom = gridHeight - numLines - verticalPaddingTop;

	// Apply vertical padding at the top
	centeredStr.append(verticalPaddingTop, '\n');

	// Reset the stream to read the string again
	iss.clear();
	iss.seekg(0, std::ios::beg);

	// Apply horizontal padding to each line
	while (std::getline(iss, line)) {
		std::string paddedLine(horizontalPadding, ' ');
		paddedLine += line;
		centeredStr += paddedLine + '\n';
	}

	// Apply vertical padding at the bottom
	centeredStr.append(verticalPaddingBottom, '\n');

	// Replace original string with centered string
	return std::move(centeredStr);
}

unsigned char DisplayStrings::MapCharacter(Vector2Int pos, std::vector<std::vector<bool>> grid)
{
	Vector2Int up = pos + Vector2Int::Up;
	Vector2Int down = pos + Vector2Int::Down;
	Vector2Int left = pos + Vector2Int::Left;
	Vector2Int right = pos + Vector2Int::Right;

	int sizeY = grid.size();
	int sizeX = grid[0].size();
	int neighborCount = 0;

	auto hasNeighbor = [sizeX, sizeY, &grid, &neighborCount](Vector2Int neigh) {
		bool res = neigh.x >= 0 && neigh.y >= 0 && neigh.x < sizeX && neigh.y < sizeY && !grid[neigh.y][neigh.x];
		if (res) neighborCount++;
		return res;
		};

	bool upNeighbor = hasNeighbor(up);
	bool downNeighbor = hasNeighbor(down);
	bool leftNeighbor = hasNeighbor(left);
	bool rightNeighbor = hasNeighbor(right);

	if (neighborCount == 1) return 254;
	
	if (neighborCount == 2)
	{
		if (upNeighbor && downNeighbor) return 186;
		if (leftNeighbor && downNeighbor) return 187;
		if (leftNeighbor && upNeighbor) return 188;
		if (rightNeighbor && upNeighbor) return 200;
		if (rightNeighbor && downNeighbor) return 201;
		if (leftNeighbor && rightNeighbor) return 205;
	}

	if (neighborCount == 3)
	{
		if (!downNeighbor) return 202;
		if (!upNeighbor) return 203;
		if (!leftNeighbor) return 204;
		if (!rightNeighbor) return 185;
	}

	if (neighborCount == 4) return 206;

	return 176;
}