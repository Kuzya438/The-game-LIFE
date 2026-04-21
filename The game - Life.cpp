#include <iostream>
#include <cstdlib>
#include <fstream>
#include "windows.h"

char** create_two_dim_array(int rows, int columns);
void delete_two_dim_array(char** matrix, int rows);
void fill_two_dim_array(char** matrix, int rows, int columns);
void print_two_dim_array(char** matrix, int rows, int columns);
void copy_two_dim_array(char** matrix1, char** matrix2, int rows, int columns);
int compare_two_dim_array(char** matrix, int rows, int columns, int row, int column);
bool compare_arrays(char** matrix1, char** matrix2, int rows, int columns);
int aliveCells_two_dim_arrays(char** matrix, int rows, int columns);

int main() {
	//Основные переменные
	int rows{}, columns{};
	char** matrixUniverse1{};
	int generation{ 1 };
	int aliveCells{};

	//Считываем данные из файла Universe.txt
	std::ifstream data{ "Universe.txt" };
	if (data.is_open()) {

		//Считываем размерности массива
		data >> rows >> columns;
		matrixUniverse1 = create_two_dim_array(rows, columns);
		fill_two_dim_array(matrixUniverse1, rows, columns);

		//Заполнение массива живыми клетками
		int aliveCellsX{}, aliveCellsY{};
		while (data >> aliveCellsX >> aliveCellsY) {
			matrixUniverse1[aliveCellsX][aliveCellsY] = '*';
			aliveCells++;
		}
	}
	data.close();

	print_two_dim_array(matrixUniverse1, rows, columns);
	std::cout << "Generation: " << generation << ". " << "Alive cells: " << aliveCells << std::endl;

	//Флаг на наличие живых клеток
	if (aliveCells == 0) {
		std::cout << "All cells are dead. Game over(" << std::endl;
		delete_two_dim_array(matrixUniverse1, rows);
		return 0;
	}
	
	//Массив для сравнения
	char** matrixUniverse2{ create_two_dim_array(rows, columns) };

	do {
		copy_two_dim_array(matrixUniverse1, matrixUniverse2, rows, columns);

		//Перебераем поочередно элементы массива и находим колличество соседних живых клеток
		for (int row{}; row < rows; ++row) {
			for (int column{}; column < columns; ++column) {
				int nextAliveCells{ compare_two_dim_array(matrixUniverse2, rows, columns, row, column) };

				//Соблюдаем условия зарождения клеток
				if (matrixUniverse2[row][column] == '-' && nextAliveCells == 3) {
					matrixUniverse1[row][column] = '*';
				}
				else if (matrixUniverse2[row][column] == '*' && nextAliveCells != 2 && nextAliveCells != 3) {
					matrixUniverse1[row][column] = '-';
				}
			}
		}

		//Задержка и очистка консоли
		Sleep(1000 * 0.5);
		std::system("cls");

		print_two_dim_array(matrixUniverse1, rows, columns);
		std::cout << "Generation: " << ++generation << ". " << "Alive cells: " << aliveCells_two_dim_arrays(matrixUniverse1, rows, columns) << std::endl;

	} while (compare_arrays(matrixUniverse1, matrixUniverse2, rows, columns));

	delete_two_dim_array(matrixUniverse2, rows);

	//Исход игры
	if (aliveCells_two_dim_arrays(matrixUniverse1, rows, columns)) {
		std::cout << "The world has stagnated. Game over)" << std::endl;
	}
	else {
		std::cout << "All cells are dead. Game over(" << std::endl;
	}

	delete_two_dim_array(matrixUniverse1, rows);

	return EXIT_SUCCESS;
}

char** create_two_dim_array(int rows, int columns) {
	char** ptrArray{ new char* [rows] {} };
	for (int row{}; row < rows; ++row) {
		ptrArray[row] = new char[columns] {};
	}
	return ptrArray;
}

void delete_two_dim_array(char** matrix, int rows) {
	for (int row{}; row < rows; ++row) {
		delete[] matrix[row];
	}
	delete[] matrix;
}

void fill_two_dim_array(char** matrix, int rows, int columns) {
	for (int row{}; row < rows; ++row) {
		for (int column{}; column < columns; ++column) {
			matrix[row][column] = '-';
		}
	}
}

void print_two_dim_array(char** matrix, int rows, int columns) {
	for (int row{}; row < rows; ++row) {
		for (int column{}; column < columns; ++column) {
			std::cout << matrix[row][column] << " ";
		}
		std::cout << std::endl;
	}
}

void copy_two_dim_array(char** matrix1, char** matrix2, int rows, int columns) {
	for (int row{}; row < rows; ++row) {
		for (int column{}; column < columns; ++column) {
			matrix2[row][column] = matrix1[row][column];
		}
	}
}

int compare_two_dim_array(char** matrix, int rows, int columns, int row, int column) {
	int counter{};
	for (int i{ row - 1 }; i <= row + 1; ++i) {
		for (int j{ column - 1 }; j <= column + 1; ++j) {
			if (i >= 0 && i < rows && j >= 0 && j < columns) {
				if (i == row && j == column) {
					continue;
				}
				else if (matrix[i][j] == '*') {
					counter++;
				}
			}
		}
	}
	return counter;
}

bool compare_arrays(char** matrix1, char** matrix2, int rows, int columns) {
	for (int row{}; row < rows; ++row) {
		for (int column{}; column < columns; ++column) {
			if (matrix1[row][column] != matrix2[row][column]) {
				return true;
			}
		}
	}
	return false;
}

int aliveCells_two_dim_arrays(char** matrix, int rows, int columns) {
	int aliveCells{};
	for (int row{}; row < rows; ++row) {
		for (int column{}; column < columns; ++column) {
			if (matrix[row][column] == '*') {
				aliveCells++;
			}
		}
	}
	return aliveCells;
}
