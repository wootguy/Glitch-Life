#include "StdAfx.h"
#include "Util.h"
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <iostream>
#include <Windows.h>
#include "Globals.h"
#include <algorithm>

#include <direct.h>
#define GetCurrentDir _getcwd

vector<string> printlog;


void print(const string& str)
{
    //OutputDebugString((str + '\n').c_str());
    cout << str.c_str();
	printlog.push_back(str);
}

void err(const string& str)
{
    #ifdef OS_WIN
        OutputDebugString((str + '\n').c_str());
    #else
        cout << str.c_str() << endl;
    #endif
		printlog.push_back(str + '\n');
}

void warn(const string& str)
{
    #ifdef OS_WIN
        OutputDebugString((str + '\n').c_str());
    #else
        cout << str.c_str() << endl;
    #endif
		printlog.push_back(str + '\n');
}

void println(const string& str)
{
	OutputDebugString((str + '\n').c_str());
    cout << str.c_str() << endl;
	printlog.push_back(str + '\n');
}

void writeLog()
{
	ofstream fout;
	fout.open ("gsrand.log", ios::out | ios::trunc);

	for (int i = 0; i < printlog.size(); i++)
		fout << printlog[i];

	fout.close();
}

string str(int value)
{
    stringstream ss;
    ss << value;
    return ss.str();
}

string str(unsigned int value)
{
    stringstream ss;
    ss << value;
    return ss.str();
}

string str(char value)
{
    stringstream ss;
    ss << value;
    return ss.str();
}

string str(long value)
{
    stringstream ss;
    ss << value;
    return ss.str();
}

string str(long long value)
{
    stringstream ss;
    ss << value;
    return ss.str();
}

string str(float value)
{
    stringstream ss;
    ss << value;
    return ss.str();
}

string str(double value)
{
    stringstream ss;
    ss.precision(16);
    ss << value;
    return ss.str();
}

string str(double value, int precision)
{
    stringstream ss;
    ss.precision(precision);
    ss << fixed << value;
    return ss.str();
}

string str(float value, int precision)
{
	stringstream ss;
	ss.precision(precision);
	ss << fixed << value;
	return ss.str();
}

string str(long double value)
{
    stringstream ss;
    ss.precision(16);
    ss << value;
    return ss.str();
}

string getWorkDir()
{
    char cCurrentPath[FILENAME_MAX];

    #ifdef OS_WIN
        if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath) / sizeof(TCHAR)))
        {
            return "ERROR: Could not determine working directory";
        }
    #else
        getcwd(cCurrentPath, sizeof(cCurrentPath));
    #endif

    cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */
    string path = cCurrentPath;
    int index = -1;
    while (( index = path.find('\\')) != -1)
        path[index] = '/';
    return path + "/";
}

string getSubStr(const string& s, int beginIndex, int endIndex)
{
    string result = "";
    int max = s.length();
    if (beginIndex < 0 || beginIndex >= max)
    {
        println("getSubStr: Invalid begin index " + str(beginIndex));
        return "";
    }
    if (endIndex < 0 || endIndex > max)
    {
        println("getSubStr: Invalid end index " + str(endIndex));
        return "";
    }
    if (beginIndex >= endIndex)
    {
        //println("getSubStr: end index must be larger than begin index");
        return "";
    }

    for (int i = beginIndex; i < endIndex; i++)
        result += s[i];
    return result;
}

string getSubStr(const string& s, int beginIndex)
{
    string result = "";
    int max = s.size();
    if (beginIndex < 0 || beginIndex >= max)
        println("getSubStr: Invalid begin index " + str(beginIndex) + " " + s);

    for (int i = beginIndex; i < max; i++)
        result += s.at(i);
    return result;
}

bool matchStr(const string& str, const string& str2)
{
    if (str.length() == str2.length())
    {
        for (int i = 0; i < (int)str.length(); i++)
        {
            char l1 = str.at(i);
            char l2 = str2.at(i);
            if (l1 != l2 && !matchLetter(l1, l2))
                return false;
        }
    }
    else
        return false;
    return true;
}

bool matchLetter(char l1, char l2)
{
    if (isLetter(l1) && isLetter(l2))
    {
        if (l1 == l2 || l1 + 32 == l2 || l1 - 32 == l2)
            return true;
    }
    //println("matchLetter: Both chars aren't letters!");
    return false;
}

bool matchStrCase(const string& str, const string& str2)
{
    if (str.length() == str2.length())
    {
        for (int i = 0; i < (int)str.length(); i++)
        {
            if (str.at(i) != str2.at(i))
                return false;
        }
    }
    else
        return false;
    return true;
}

bool isLetter(char c)
{
    if (( c >= 65 && c <= 90) || (c >= 97 && c <= 122) )
        return true;
    return false;
}

bool isNumeric(char c)
{
    if (c >= 48 && c <= 57)
        return true;
    return false;
}

bool isNumber(const string& str)
{
    for (int i = 0 ; i < (int)str.length(); i++)
    {
        if (i == 0 && str.at(0) == '-')
            continue;
        if (!isNumeric(str.at(i)))
            return false;
    }
    return true;
}

bool contains(const string& str, char c)
{
	for (size_t i = 0, size = str.size(); i < size; i++)
	{
		if (str.at(i) == c)
			return true;
	}
	return false;
}


void remove( string& str, char c )
{
	string answer = "";
	bool removed = false;
	for (unsigned int i = 0; i < str.length(); i++)
	{
		if (removed == false)
		{
			if (str[i] != c)
				answer += str[i];
			else
				removed = true;
		}
		else
			answer += str[i];
	}

	str = answer;
}


int toInt(const string& str)
{
    int num = 0;
    bool negative = false;
    int digits = str.length();
    int startAt = 0;
    if (str.at(0) == '-')
    {
        negative = true;
        startAt = 1;
    }

    for (int i = startAt; i < digits; i++)
    {
        int power = (int)pow(10.0, (digits - i - 1));
        num += (str.at(i)-48) * power;
    }
    if (negative)
        num *= -1;
    return num;
}

string toLowerCase(string str)
{
	transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}

bool fileExists(const string& file)
{
    ifstream ifile(file.c_str());
    return ifile;
}

string getPath(const string& file)
{
    int index = file.find_last_of('/');

    if (index == -1)
        return "";
    else if ((int) file.length() > index)
        return getSubStr(file, 0, index + 1);
    else
        return file;
}

double readDouble(const string& line, int dir)
{
    int numBegin = -1;
    int numEnd = -1;
    bool hasPoint = false;

    for (int i = 0; i < (int)line.length(); i++)
    {
        char c;
        if (dir == FROM_START)
            c = line.at(i);
        else
            c = line.at(line.length() - 1 - i);

        if (numBegin == -1)
        {
            if (isNumeric(c) || (dir == FROM_START && c == '-') )
                numBegin = i;
        }
        else
        {
            if (dir == FROM_START && !hasPoint && c == '.')
                hasPoint = true;
            else if ( !isNumeric(c) )
            {
                numEnd = i;
                break;
            }
        }
    }
    if (numEnd == -1)
        numEnd = line.length();
    if (dir == FROM_END)
    {
        int temp = numEnd;
        numEnd = line.length() - numBegin;
        numBegin = line.length() - temp;
    }

    if (numBegin != -1 && numEnd != -1)
        return atof(getSubStr(line, numBegin, numEnd).c_str());
    else
        err("readDouble: No digits were found in '" + line + "'");

    return 0;
}

int readInt(const string& line, int dir)
{
    int numBegin = -1;
    int numEnd = -1;

    for (int i = 0; i < (int)line.length(); i++)
    {
        char c;
        if (dir == FROM_START)
            c = line.at(i);
        else
            c = line.at(line.length() - 1 - i);
        if (numBegin == -1)
        {
            if (isNumeric(c) || (dir == FROM_START && c == '-') )
                numBegin = i;
        }
        else
        {
            if (dir == FROM_END && c == '-')
            {
                numEnd = i + 1;
                break;
            }
            if ( !isNumeric(c) )
            {
                numEnd = i;
                break;
            }
        }
    }
    if (numEnd == -1)
        numEnd = line.length();
    if (dir == FROM_END)
    {
        int temp = numEnd;
        numEnd = line.length() - numBegin;
        numBegin = line.length() - temp;
    }

    if (numBegin != -1 && numEnd != -1)
        return atoi(getSubStr(line, numBegin, numEnd).c_str());
    else
        err("readInt: No digits were found in '" + line + "'");

    return 0;
}

string readQuote(const string& str)
{
    int begin = -1;
    int end = -1;

    for (int i = 0; i < (int)str.length(); i++)
    {
        char c = str.at(i);
        if (begin == -1)
        {
            if (c == '"')
                begin = i + 1;
        }
        else
        {
            if (c == '"')
                end = i;
        }
    }
    if (begin != -1 && end != -1)
        return getSubStr(str, begin, end);
    else
        err("readQuote: Could not find quote in '" + str + "'");

    return 0;
}

/*
double toRadians(double deg)
{
    return deg*(PI/180.0);
}

double toDegrees(double rad)
{
    return rad*(180.0/PI);
}
*/

int ceilPow2( int value )
{
    int pow = 1;
    for (int i = 0; i < 32; i++) // 32 bits to shift through
    {
        if (value <= pow)
            return pow;
        pow = (pow << 1);
    }
    err("No power of two exists greater than " + str(value) + " as an integer");
    return -1;
}


vector<string> getDirFiles( string path, string extension )
{
	path = path + "*." + extension;
	winPath(path);
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	vector<string> results;

	//println("Target file is " + path);
	hFind = FindFirstFile(path.c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) 
	{
		//println("FindFirstFile failed " + str((int)GetLastError()) + " " + path);
		return results;
	} 
	else 
	{
		results.push_back(FindFileData.cFileName);

		while (FindNextFile(hFind, &FindFileData) != 0)
			results.push_back(FindFileData.cFileName);

		FindClose(hFind);
	}
	return results;
}

void winPath( string& path )
{
	for (int i = 0, size = path.size(); i < size; i++)
	{
		if (path[i] == '/')
			path[i] = '\\';
	}
}

vector<string> getSubdirs( string path )
{
	path = path + "*";
	winPath(path);
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	vector<string> results;

	//println("Target file is " + path);
	hFind = FindFirstFile(path.c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) 
	{
		//println("FindFirstFile failed " + str((int)GetLastError()) + " " + path);
		return results;
	} 
	else 
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && 
			!matchStr(FindFileData.cFileName, ".") && 
			!matchStr(FindFileData.cFileName, ".."))
			results.push_back(FindFileData.cFileName);

		while (FindNextFile(hFind, &FindFileData) != 0)
		{
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && 
				!matchStr(FindFileData.cFileName, ".") && 
				!matchStr(FindFileData.cFileName, ".."))
				results.push_back(FindFileData.cFileName);
		}

		FindClose(hFind);
	}
	return results;
}

char * loadFile( string file )
{
	if (!fileExists(file))
	{
		err("file does not exist " + file);
		return NULL;
	}
	ifstream fin(file.c_str(), ifstream::in|ios::binary);
	int begin = fin.tellg();
	fin.seekg (0, ios::end);
	int size = (int)fin.tellg() - begin;
	char * buffer = new char[size];
	fin.seekg(0);
	fin.read(buffer, size);
	fin.close();
	//println("read " + str(size));
	return buffer;
}

DateTime DateTime::now()
{
	SYSTEMTIME t;
	GetLocalTime(&t);
	return DateTime(t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
}

string DateTime::str()
{
	string suffix = "AM";
	int s_hour = hour;
	string s_min = minute < 10 ? ("0" + ::str(minute)) : ::str(minute);
	string s_sec = second < 10 ? ("0" + ::str(second)) : ::str(second);
	if (hour > 12)
	{
		s_hour -= 12;
		suffix = "PM";
	}
	return ::str(month) + "/" + ::str(day) + "/" + ::str(year) + " " + ::str(s_hour) + ":" + s_min + ":" + s_sec + " " + suffix;
}

string DateTime::compact_str()
{
	string s_min = minute < 10 ? ("0" + ::str(minute)) : ::str(minute);
	string s_hour = hour < 10 ? ("0" + ::str(hour)) : ::str(hour);
	string s_day = day < 10 ? ("0" + ::str(day)) : ::str(day);
	string s_month = month < 10 ? ("0" + ::str(month)) : ::str(month);
	string s_year = year < 10 ? ("0" + ::str(year % 100)) : ::str(year % 100);

	return s_year + s_month + s_day + s_hour + s_min;
}

void recurseSubdirs(string path, vector<string>& dirs)
{
	dirs.push_back(path);
	vector<string> files = getSubdirs(path);
	for (uint i = 0; i < files.size(); i++)
		recurseSubdirs(path + files[i] + '/', dirs);
}

vector<string> getAllSubdirs(string path)
{
	vector<string> dirs;
	recurseSubdirs(path, dirs);
	return dirs;
}

// no slashes allowed at the end of start_dir
string relative_path_to_absolute(string start_dir, string path)
{
	int up = path.find("../");
	if (up == string::npos)
		return start_dir + "/" + path;
	
	while (up != string::npos)
	{
		int up_dir = start_dir.find_last_of("\\/");
		if (up_dir == string::npos)
		{
			print("Could not convert '" + path + "' to absolute path using root dir: " + start_dir); 
			return start_dir + "/" + path;
		}
		if (up > 0) // some crazy person went back down a directory before going up
		{
			start_dir += getSubStr(path, 0, up-1);
			path = getSubStr(path, up);
			up = 0;
		}
		start_dir = getSubStr(start_dir, 0, up_dir);
		path = getSubStr(path, 3);
		up = path.find("../");
	}
	
	return start_dir + "/" + path;	
}
