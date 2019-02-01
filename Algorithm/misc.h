#pragma once

#include <forward_list>
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

// if an element in an MxN matrix is 0, its entire row and column are set to O
template <class T, size_t M, size_t N>
void zeroMatrix(T(&m)[M][N])
{
	if (M < 1 || N < 1)
		return;

	bool rowHasZero = false;
	bool colHasZero = false;
	
	for (size_t i = 0; i < M || !colHasZero; ++i)
		colHasZero = (m[i][0] == 0);
		
	for (size_t j = 0; j < N || !rowHasZero; ++j)
		rowHasZero = (m[0][j] == 0);

	for (size_t i = 1; i < M; ++i)
		for (size_t j = 1; j < N; ++j)
			if (m[i][j] == 0)
			{
				m[i][0] = 0;
				m[0][j] = 0;
			}

	for (size_t i = 1; i < M; ++i)
		if (m[i][0] == 0)
			for (size_t j = 1; j < N; ++j)
				m[i][j] = 0;

	for (size_t j = 1; j < N; ++j)
		if (m[0][j] == 0)
			for (size_t i = 1; i < M; ++i)
				m[i][j] = 0;
	
	if (rowHasZero)
		for (size_t j = 0; j < N; ++j)
			m[0][j] = 0;

	if (colHasZero)
		for (size_t i = 1; i < M; ++i)
			m[i][0] = 0;
}

template<class T>
void removeDups(std::forward_list<T>& list)
{
	auto prev = list.begin();
	for (auto i = ++list.begin(); i != list.end();)
	{
		bool dup = false;
		for (auto j = list.begin(); j != i; ++j)
		{
			if (dup = (*i == *j))
				break;
		}

		if (dup)
		{
			++i;
			list.erase_after(prev);
		}
		else
		{
			prev = i++;
		}
	}
}

// Implement an algorithm to determine if a string has all unique characters
bool isCharsUnique(std::wstring str);

bool isPalindrome(const std::wstring& str);

void towersOfHanoi(size_t from, size_t to, size_t mid, size_t count);

void grayCode(int n);

void generateGrayCode(/*size_t size*/);