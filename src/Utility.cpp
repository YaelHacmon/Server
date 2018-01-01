#include "../include/Utility.h"
#include <iterator> //for istream_iterator
#include <sstream> //for istringstream

vector<string> Utility::split(string& line) {
	istringstream iss(line);
	vector<string> args((istream_iterator<string>(iss)), istream_iterator<string>());
	return args;
}


string Utility::toString(int a) {
	//declare stringstream
	stringstream ss;
	//add integer to stream
	ss << a;
	//make stream to string
	string str = ss.str();
	//return string
	return str;
}




