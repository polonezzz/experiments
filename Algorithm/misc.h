#pragma once

#include <iomanip>
#include <iostream>
#include <string>
#include <utility>

template<class T, size_t M, size_t N>
void printMatrix(T(&m)[M][N])
{
	for (size_t i = 0; i < N; ++i)
	{
		for (size_t j = 0; j < N; ++j)
			std::cout << std::setw(3) << m[i][j] << " ";
		
		std::cout << '\n';
	}

	std::cout << '\n';
}

template <class T, size_t N>
void rotateMatrix(T(&m)[N][N], bool left)
{
	if (left)
	{
		//transpose around the main diagonal
		for (size_t i = 0; i < N; ++i)
			for (size_t j = i; j < N; ++j)
				std::swap(m[i][j], m[j][i]);
	}
	else
	{
		//transpose around the secondary diagonal
		for (size_t i = 0; i < N; ++i)
			for (size_t j = 0; j < N - i; ++j)
				std::swap(m[i][j], m[N - (j + 1)][N - (i + 1)]);
	}
	
	for (size_t i = 0; i < N - i; ++i)
		for (size_t j = 0; j < N; ++j)
			std::swap(m[i][j], m[N-(i+1)][j]);
}

template <class T, size_t N>
void rotateMatrix2(T(&m)[N][N], bool left)
{
	if (left)
	{
		for (size_t i = 0; i < N / 2; ++i)
			for (size_t j = i; j < N - (i + 1); ++j)
			{
				auto v = m[i][j];
				std::swap(v, m[N - (j + 1)][i]);
				std::swap(v, m[N - (i + 1)][N - (j + 1)]);
				std::swap(v, m[j][N - (i + 1)]);
				std::swap(v, m[i][j]);
			}
	}
	else
	{
		for (size_t i = 0; i < N / 2; ++i)
			for (size_t j = i; j < N - (i + 1); ++j)
			{
				auto v = m[i][j];

				std::swap(v, m[j][N - (i + 1)]);
				std::swap(v, m[N - (i + 1)][N - (j + 1)]);
				std::swap(v, m[N - (j + 1)][i]);
				std::swap(v, m[i][j]);
			}
	}
}

// Implement an algorithm to determine if a string has all unique characters
bool isCharsUnique(std::wstring str);

bool isPalindrome(const std::wstring& str);

void towersOfHanoi(size_t from, size_t to, size_t mid, size_t count);