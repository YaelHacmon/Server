/*
 * Utility.h
 * For helper methods, static
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include <vector>
#include <string>

using namespace std;

class Utility {
public:
	/**
	 * Helper function for splitting a command by space
	 */
	static vector<string> split(string& line);

	/**
	 * Helper function for cleaner code: converts integer to string
	 */
	static string toString(int a);
};


#endif /* UTILITY_H_ */
