#include <SFML/Graphics.hpp> //The graphics library that will be used to create a GUI
#include <iostream>
#include <vector>

//function declarations
void outputVector(std::vector<int>), backTrack(int, int, std::vector<std::vector<int>>&, std::vector<std::vector<int>>&, std::vector<std::vector<int>>, int&, int&);
std::vector<int> getRow(int, std::vector<std::vector<int>>), getCol(int, std::vector<std::vector<int>>), getBox(int, int, std::vector<std::vector<int>>);
int getNumber(int,int, std::vector<std::vector<int>>, std::vector<std::vector<int>>);

int main() {

	std::vector<std::vector<int>>  grid = { //The sudoku grid - 0s represent empty cells - whitespace to show layout of grid.
		{8,0,0,  0,0,0,  0,0,0},
		{0,0,3,  6,0,0,  0,0,0},
		{0,7,0,  0,9,0,  2,0,0},

		{0,5,0,  0,0,7,  0,0,0},
		{0,0,0,  0,4,5,  7,0,0},
		{0,0,0,  1,0,0,  0,3,0},

		{0,0,1,  0,0,0,  0,6,0},
		{0,0,0,  0,0,0,  0,0,0},
		{0,9,0,  0,0,0,  4,0,0}
	};

	std::vector<std::vector<int>>  copy = grid; //To keep track of the original numbers

	std::vector<std::vector<int>> recurs = {
		{0,0,0,  0,0,0,  0,0,0},
		{0,0,0,  0,0,0,  0,0,0},
		{0,0,0,  0,0,0,  0,0,0},

		{0,0,0,  0,0,0,  0,0,0},
		{0,0,0,  0,0,0,  0,0,0},
		{0,0,0,  0,0,0,  0,0,0},

		{0,0,0,  0,0,0,  0,0,0},
		{0,0,0,  0,0,0,  0,0,0},
		{0,0,0,  0,0,0,  0,0,0}
	}; //to keep track of the number of times a number has to be found

	int n, nx, ny;

	for (int y = 0; y < 9; y++) {
		for (int x = 0; x < 9; x++) {

			if (copy[y][x] == 0) { //checking that the cell being checked is not an original number
				n = getNumber(x, y, grid, recurs);

				if (n > 0) {
					grid[y][x] = n;
					recurs[y][x] = 1; // if a valid number can be written in this cell, it is
				}

				else {
					backTrack(x, y, grid, recurs, copy, nx, ny); // if there are no valid options for the next cell, the program backtracks
					x = nx;
					y = ny;
					grid[y][x] = getNumber(x, y, grid, recurs);
				}
			}
		}
	}

	//Outputting solution
	for (int i = 0; i < grid.size(); i++) {
		outputVector(grid[i]);
	}

	return 0;
}

std::vector <int> getRow(int y, std::vector<std::vector<int>> grid) {
	//returns all the values in the row that the current cell is in
	return grid[y];
}

std::vector <int> getCol(int x, std::vector<std::vector<int>> grid) {
	//returns all the values in the column that the current cell is in
	std::vector <int> temp;
	for (int i = 0; i < grid.size(); i++) {
		temp.push_back(grid[i][x]);
	}
	return temp;
}

std::vector <int> getBox(int x, int y, std::vector<std::vector<int>> grid) {
	//returns all the values in the 3x3 box that the current cell is in
	std::vector <int> temp;
	int startX = floor(x / 3) * 3;
	int startY = floor(y / 3) * 3;
	for (int iy = startY; iy < startY+3; iy++) {
		for (int ix = startX; ix < startX+3; ix++) {		
			temp.push_back(grid[iy][ix]);
		}
	}
	return temp;
}

int getNumber(int x, int y, std::vector<std::vector<int>> grid, std::vector<std::vector<int>> recurs) {
	//iterates over values in the row, column and current box to see if an incrementing value of n is present. Returns the first value of n that does not exist inside the vectors.

	std::vector<std::vector<int>> combined = { getRow(y, grid), getCol(x, grid), getBox(x, y, grid) };
	bool flag; // flag is used to determine whether n exists in the vector combined.

	int recurrance = recurs[y][x];

	for (int n = 1; n < 10; n++) {
		flag = false;
		for (int v = 0; v < combined.size(); v++) {
			for (int i = 0; i < combined[v].size(); i++) {
				if (n == combined[v][i]) {
					flag = true;
					break;
				}
			}
			if (flag) {
				break;
			}
		}
		if (!flag) {
			if (recurrance > 0) {
				recurrance -= 1;
			}
			else {
				return n;
			}
		}
	}
	return 0;

}

void outputVector(std::vector<int> v){
	//outputs the values in a vector of integers - used for debugging
	for (int i = 0; i < v.size(); i++) {
		std::cout << v[i] << ",  ";
	}
	std::cout << std::endl;
}

void backTrack(int x, int y, std::vector<std::vector<int>>& grid, std::vector<std::vector<int>>& recurs, std::vector<std::vector<int>> copy, int& nx, int& ny) {


	int n;
	while (true) {

		recurs[y][x] = 0; 
		if (x > 0) { // iterates backwards across the matrix
			x -= 1;
		}
		else {
			x = 8;
			y -= 1;
		}
		if (copy[y][x] == 0) {
			grid[y][x] = 0;
			if (recurs[y][x] < 9){ // makes sure the program doesn't try more possibilities than there exist
				recurs[y][x] += 1;
				n = getNumber(x, y, grid, recurs);
				if (n > 0) { // if there is another possible solution to this cell, it is used
					nx = x;
					ny = y;
					return;
				}
				else {
					recurs[y][x] -= 1; // otherwise, the while loop repeats
				}
			}
		}
	}
}