#pragma once
#include <string>
#include <vector>

using namespace std;

typedef unsigned int uint;

extern vector<string> printlog;

enum read_dir
{
    FROM_START, // parse the string from beginning to end (normal)
    FROM_END	// parse the string end to beginning (backwards)
};

/*
	Print the string to the debug output and console with '\n' appended.
	str - string to print
*/
void println(const string& str);

/*
	Print the string to the debug output and console.
	str - string to print
*/
void print(const string& str);

/*
	Print the string to the debug output and console (red). Append '\n'.
	str - string to print
*/
void err(const string& str);

/*
	Print the string to the debug output and console (yellow). Append '\n'.
	str - string to print
*/
void warn(const string& str);

void writeLog();

/*
	Convert the specified value to a string
	value - value to convert
*/
string str(int value);

string str(unsigned int value);

string str(char value);

string str(long value);

string str(long long value);

string str(float value);

string str(double value);

string str(double value, int precision);

string str(float value, int precision);

string str(long double value);

// Return a string representing the current working directory (Ex: "C:\test").
string getWorkDir();


/*
	Extract a section from a string
	str - string to extract from
	beginIndex - index to start at (inclusive)
	endIndex - index to stop at (exclusive)
*/
string getSubStr(const string& str, int beginIndex, int endIndex);

string getSubStr(const string& str, int beginIndex);


/*
	Check if two strings are idential (case insensitive)
	str - a string
	str2 - another string
*/
bool matchStr(const string& str, const string& str2);

/*
	True if two letters match (case insensitive). If non-letters are used, then
	this function will retrun false.
	l1 - a letter
	l2 - another letter
*/
bool matchLetter(char l1, char l2);


/*
	Check if two strings are identical (case sensitive)
	str - a string
	str2 - another string
*/
bool matchStrCase(const string& str, const string& str2);


// True if the character passed is a letter (a-z, A-Z)
bool isLetter(char c);

// True if the character passed is a number (0-9)
bool isNumeric(char c);

// True if the string passed is a number. Negative values are allowed (Ex: "-9")
bool isNumber(const string& str);

// Whether or not the string contains the specified character
bool contains(const string& str, char c);

// Remove the first instance of this character from the string
void remove(string& str, char c);

// Attempts to convert a string to a number
int toInt(const string& str);

string toLowerCase(string str);

/*
	Returns true if the file could be found.
	file - absolute path to the file (Ex: "C:\Project\thing.png")
*/
bool fileExists(const string& file);

// Return the absoulte path the file, excluding the file's name
// Ex: "C:\Project\thing.jpg" -> "C:\Project"
string getPath(const string& file);


/*
	Extract a double value in the string. If none can be found, 0 is returned.
	str - string to read
	dir - which direction to read (FROM_START, FROM_END)
*/
double readDouble(const string& str, int dir);

/*
	Extract an integer value in the string. If none can be found, 0 is returned.
	str - string to read
	dir - which direction to read (FROM_START, FROM_END)
*/
int readInt(const string& str, int dir);


/*
	Extract a message surrounded by quotes ("") from a string
	str - string to read
*/
string readQuote(const string& str);

// convert a value in degrees to radians.
double toRadians(double deg);

// convert a value in radians to degrees.
double toDegrees(double rad);

// returns the value rounded up to the nearest power of two
int ceilPow2(int value);

// convert / to \ in the pathname
void winPath(string& path);

vector<string> getDirFiles(string path, string extension);

vector<string> getSubdirs(string path);

char * loadFile( string file );

void recurseSubdirs(std::string path, vector<std::string>& dirs);

vector<std::string> getAllSubdirs(std::string path);

string relative_path_to_absolute(string start_dir, string path);

void insert_unique(const vector<string>& insert, vector<string>& insert_into);

struct DateTime
{
	int year, month, day;
	int hour, minute, second;

	DateTime() : year(0),  month(0), day(0), hour(0), minute(0), second(0) {}
	DateTime(int year, int month, int day, int hour, int minute, int second) 
	: year(year),  month(month), day(day), hour(hour), minute(minute), second(second) {}
	
	static DateTime now();
	string str();
	string compact_str();
};