#pragma once

#include <vector>

uint32_t gcdEuclid(uint32_t a, uint32_t b);

uint32_t gcdEuclid2(uint32_t a, uint32_t b);

std::tuple<int32_t, int32_t, int32_t> gcdEuclidEx(int32_t a, int32_t b);

std::vector<size_t> sieve(size_t limit);  // of Eratosthenes

size_t sqrtFloor(size_t a);

bool isPrime(size_t number);

int phi(int n);  //Euler's function
