#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

std::vector<std::string> readWordSearch(const std::string& filename) {
	std::vector<std::string> grid;
	std::ifstream file(filename);
	std::string line;

	if (!file.is_open()) {
		std::cout << "Err opening file" << std::endl;
		return grid;
	}

	while(getline(file, line)) {
		if (!line.empty()) {
			grid.push_back(line);
		}
	}

	file.close();
	return grid;
}

int countXMAS(const std::vector<std::string>& grid) {
	int rows = grid.size();
	int cols = grid[0].size();
	int count = 0;

	const int directions[8][2] = {
		{0, 1},   // right
        	{1, 1},   // down-right
         	{1, 0},   // down
          	{1, -1},  // down-left
           	{0, -1},  // left
            	{-1, -1}, // up-left
             	{-1, 0},  // up
              	{-1, 1}   // up-right
	};

	auto checkDirection = [&](int row, int col, int dx, int dy) -> bool {
		if (row + 3*dx < 0 || row + 3*dx >= rows || col + 3*dy < 0 || col + 3*dy >= cols) {
			return false;
		}

	return (grid[row][col] == 'X' && grid[row + dx][col + dy] == 'M' && grid[row + 2*dx][col + 2*dy] == 'A' && grid[row + 3*dx][col + 3*dy] == 'S') ;
	};

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			for (const auto& dir : directions) {
				if (checkDirection(i, j, dir[0], dir[1])) {
					count++;
				}
			}
		}
	}

	return count;
}

int countXMAS_Part2(const std::vector<std::string>& grid) {
	int rows = grid.size();
    	int cols = grid[0].size();
     	int count = 0;

      	// Check each position as the center of potential X-MAS
       	for (int i = 1; i < rows - 1; i++) {  // Start from 1 and end before last row
        	for (int j = 1; j < cols - 1; j++) {  // Start from 1 and end before last column
            	// Check if this position is 'A' (center of X)
             	if (grid[i][j] != 'A') continue;

              	// Check all possible combinations of MAS in X shape
               	// For each arm of the X, MAS can be forward or backward

                // Arrays to store the characters in each diagonal
                char topLeft[3] = {grid[i-1][j-1], grid[i][j], grid[i+1][j+1]};
                char topRight[3] = {grid[i-1][j+1], grid[i][j], grid[i+1][j-1]};

                // Check if either diagonal forms MAS (forward or backward)
                bool isValidX = false;

                // Check all possible combinations
                if ((topLeft[0] == 'M' && topLeft[2] == 'S' &&
                 topRight[0] == 'M' && topRight[2] == 'S') ||
                (topLeft[0] == 'M' && topLeft[2] == 'S' &&
                 topRight[2] == 'M' && topRight[0] == 'S') ||
                (topLeft[2] == 'M' && topLeft[0] == 'S' &&
                 topRight[0] == 'M' && topRight[2] == 'S') ||
                (topLeft[2] == 'M' && topLeft[0] == 'S' &&
                 topRight[2] == 'M' && topRight[0] == 'S')) {
                 	count++;
                 	}
         	}
        }

        return count;
}

int main() {
	std::vector<std::string> grid = readWordSearch("input.txt");

	if (grid.empty()) {
		std::cout << " no data lil jit" << std::endl;
		return 1;
	}

	int xmasCount = countXMAS(grid);
	int xmasCount2 = countXMAS_Part2(grid);

	std::cout << "part 1 sol: " << xmasCount << std::endl;
	std::cout << "part 2 sol: " << xmasCount2 << std::endl;

	return 0;
}
