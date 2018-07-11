#pragma once

#include <string>

// Implement an algorithm to determine if a string has all unique characters
bool isCharsUnique(std::wstring str);


/*
Palindrome Permutation: Given a string, write a function to check if it is a permutation of a palindrome. 
A permutation is a rearrangement of letters. The palindrome does not need to be limited to just dictionary words. 
 
Input:  Tact  Coa 
Output:  True  (permutations:  "taco  cat".  "atco  cta".  etc.) */bool isPalindrome(const std::wstring& str);