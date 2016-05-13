#include "Util.h"
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <iostream>
#include "Globals.h"
#include <algorithm>
#include <string.h>

#if defined(WIN32) || defined(_WIN32)
#include <Windows.h>
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#define GetCurrentDir getcwd

typedef char TCHAR;

void OutputDebugString(const char *str) {}
#endif

vector<string> printlog;


void print(const string& str)
{
    OutputDebugString((str).c_str());
    cout << str.c_str();
	printlog.push_back(str);
}

void backspace(int amount)
{
	if (amount <= 0)
		return;
	string output;
	for (int i = 0; i < amount; i++)
		output += "\b \b";

	OutputDebugString((output).c_str());
    cout << output.c_str();
	if (printlog.size())
	{
		string last_print = printlog[printlog.size()-1];
		while (last_print.size() <= amount)
		{
			amount -= last_print.size();
			printlog.pop_back();
			if (!printlog.size())
				return;
			last_print = printlog[printlog.size()-1];
		}
		if (amount > 0)
			last_print = last_print.substr(0, last_print.size()-amount);
	}
}

uint64 getSystemTime()
{
    #if defined(WIN32) || defined(_WIN32)
	LARGE_INTEGER fq, li;
	QueryPerformanceFrequency(&fq);
	QueryPerformanceCounter(&li);

	double freq = (double)fq.QuadPart;
	double time = (double)li.QuadPart*1000.0*1000.0;
	return (uint64)(time/freq);
    #else
    return time(NULL);
    #endif
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

string str(double value)
{
    return str(value, 16);
}

string str(long double value)
{
    return str(value, 16);
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
        println("getSubStr: Invalid begin index " + str(beginIndex) + " '" + s + "'");
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
        println("getSubStr: Invalid begin index " + str(beginIndex) + " '" + s + "'");

    for (int i = beginIndex; i < max; i++)
        result += s.at(i);
    return result;
}

bool matchStr(const string& str, const string& str2)
{
    if (str.length() == str2.length())
    {
        for (int i = 0, len = (int)str.length(); i < len; i++)
        {
            char l1 = str[i];
            char l2 = str2[i];
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
    return ( c >= 65 && c <= 90) || (c >= 97 && c <= 122);
}

bool isCapitalLetter(char c)
{
	return c >= 65 && c <= 90;
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
    if (FILE *f = fopen(file.c_str(), "r")) {
        fclose(f);
        return true;
    } else {
        return false;
    }   
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

bool hasUppercaseLetters( const string& str )
{
	for (int i = 0; i < str.length(); i++)
		if (isCapitalLetter(str[i]))
			return true;
	return false;
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
    vector<string> results;
    
    #if defined(WIN32) || defined(_WIN32)
	path = path + "*." + extension;
    winPath(path);
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;

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
    #else
    DIR *dir = opendir(path.c_str());
    
    if(!dir)
        return results;
    
    while(true)
    {
        dirent *entry = readdir(dir);
        
        if(!entry)
            break;
        
        if(entry->d_type == DT_DIR)
            continue;
        
        string name = string(entry->d_name);
        
        if(extension.size() > name.size())
            continue;
        
        if(std::equal(extension.rbegin(), extension.rend(), name.rbegin()))
            results.push_back(name);
    }
    
    closedir(dir);
    #endif
    
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
	vector<string> results;
    
    #if defined(WIN32) || defined(_WIN32)
    path = path + "*";
    winPath(path);
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;

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
    #else
    DIR *dir = opendir(path.c_str());
    
    if(!dir)
        return results;
    
    while(true)
    {
        dirent *entry = readdir(dir);
        
        if(!entry)
            break;
        
        string name = string(entry->d_name);
        
        if(entry->d_type == DT_DIR &&
           !matchStr(name, ".") &&
           !matchStr(name, ".."))
            results.push_back(name);
    }
    
    closedir(dir);
    #endif
	
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
    #if defined(WIN32) || defined(_WIN32)
	SYSTEMTIME t;
	GetLocalTime(&t);
	return DateTime(t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
    #else
    time_t now = time(NULL);
    tm *data = localtime(&now);
    return DateTime(data->tm_year, data->tm_mon, data->tm_mday, data->tm_hour, data->tm_min, data->tm_sec);
    #endif
}

string DateTime::str()
{
    #if defined(WIN32) || defined(_WIN32)
	TIME_ZONE_INFORMATION tz;
	GetTimeZoneInformation(&tz);
	string zone = ::str(-tz.Bias / 60);
	if (-tz.Bias >= 0)
		zone = "+" + zone;
	zone = " (UTC" + zone + ")";

	string suffix = "AM";
	int s_hour = hour;
	string s_min = minute < 10 ? ("0" + ::str(minute)) : ::str(minute);
	string s_sec = second < 10 ? ("0" + ::str(second)) : ::str(second);
	if (hour > 12)
	{
		s_hour -= 12;
		suffix = "PM";
	}
	return ::str(year) + "/" + ::str(month) + "/" + ::str(day) + " " + ::str(s_hour) + ":" + s_min + " " + suffix + " " + zone;
    #else
    char buffer[256];
    time_t now = time(NULL);
    tm *data = localtime(&now);
    data->tm_year = year;
    data->tm_mon = month;
    data->tm_mday = day;
    data->tm_hour = hour;
    data->tm_min = minute;
    data->tm_sec = second;
    
    strftime(buffer, 256, "%Y/%m/%d %I:%M %p (UTC %z)", data);
    
    return string(buffer);
    #endif
}

string DateTime::compact_str()
{
    #if defined(WIN32) || defined(_WIN32)
	string s_min = minute < 10 ? ("0" + ::str(minute)) : ::str(minute);
	string s_hour = hour < 10 ? ("0" + ::str(hour)) : ::str(hour);
	string s_day = day < 10 ? ("0" + ::str(day)) : ::str(day);
	string s_month = month < 10 ? ("0" + ::str(month)) : ::str(month);
	string s_year = year < 10 ? ("0" + ::str(year % 100)) : ::str(year % 100);

	return s_year + s_month + s_day + s_hour + s_min;
    #else
    char buffer[256];
    time_t now = time(NULL);
    tm *data = localtime(&now);
    data->tm_year = year;
    data->tm_mon = month;
    data->tm_mday = day;
    data->tm_hour = hour;
    data->tm_min = minute;
    data->tm_sec = second;
    
    strftime(buffer, 256, "%y%m%d%H%M", data);
    
    return string(buffer);
    #endif
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
			if (start_dir.length())
			{
				up_dir = 0;
				start_dir = "";
			}
			else
			{
				print("Could not convert '" + path + "' to absolute path using root dir: " + start_dir); 
				return start_dir + "/" + path;
			}
		}
		if (up > 0) // some crazy person went back down a directory before going up
		{
			start_dir += getSubStr(path, 0, up-1);
			path = getSubStr(path, up);
			up = 0;
		}
		if (up_dir > 0)
			start_dir = getSubStr(start_dir, 0, up_dir);
		path = getSubStr(path, 3);
		up = path.find("../");
	}
	if (start_dir.length())
		return start_dir + "/" + path;	
	else
		return path;
}

void insert_unique(const vector<string>& insert, vector<string>& insert_into)
{
	for (uint i = 0; i < insert.size(); ++i)
	{
		bool exists = false;
		for (uint k = 0; k < insert_into.size(); ++k)
		{
			if (matchStr(insert_into[k], insert[i]))
			{
				exists = true;
				break;
			}	
		}
		if (!exists)
			insert_into.push_back(insert[i]);
	}
}

string trimSpaces(string s)
{
	// Remove white space indents
	int lineStart = s.find_first_not_of(" \t\n\r");
	if (lineStart == string::npos)
		return "";
	s = s.substr(lineStart);

	// Remove spaces after the last character
	int lineEnd = s.find_last_not_of(" \t\n\r");
	if (lineEnd != string::npos && lineEnd < s.length() - 1)
		s = s.substr(0, lineEnd+1);

	return s;
}

vector<string> splitString( string str, const char * delimitters )
{
	vector<string> split;
	if (str.size() == 0)
		return split;
	string copy = str;
	char * tok = strtok((char *)copy.c_str(), delimitters);

	while (tok != NULL)
	{
		split.push_back(tok);
		tok = strtok(NULL, delimitters);
	}
	return split;
}

string getFilename(string filename)
{
    #if defined(WIN32) || defined(_WIN32)
	vector<string> components;
	string old_filename = filename;
	std::replace( filename.begin(), filename.end(), '/', '\\'); // windows slashes required

	// SHGetFileInfoA only fixes the last part of the path. So we have to make sure each folder is correct, too
	while (filename.length())
	{
		SHFILEINFO info;
		// nuke the path separator so that we get real name of current path component

		int dir = filename.find_last_of("\\");
		string testing = filename;
		if (dir != string::npos)
			testing = getSubStr(filename, dir);

		if (!matchStr(testing, ".."))
		{
			info.szDisplayName[0] = 0;
			// TODO: So uh, this doesn't work if the user has file extensions hidden (which is on by default). That sucks.
			SHGetFileInfoA( filename.c_str(), 0, &info, sizeof(info), SHGFI_DISPLAYNAME );
			components.push_back(info.szDisplayName);  
		}
		else
			components.push_back("..");
		if (dir != string::npos)
			filename = getSubStr(filename, 0, dir);
		else
			break;
	}

	string result;
	for (int i = (int)components.size() - 1; i >= 0; --i)
	{
		result += components[i];
		if (i > 0)
			result += "/";
	}

    return result;
    #else
    //shouldn't do anything on *nix, if I understood the intention of this function correctly
	// w00tguy: Yeah, this was meant to handle the case where mappers used the incorrect case and windows was ok with that.
	//          On *nix, the map will just crash or be missing resources.
    return filename;
    #endif
}

bool dirExists(const string& path)
{
    #if defined(WIN32) || defined(_WIN32)
	DWORD dwAttrib = GetFileAttributesA(path.c_str());

	return (dwAttrib != INVALID_FILE_ATTRIBUTES && 
			(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
    #else
    struct stat data;
    return stat(path.c_str(), &data) == 0 && S_ISDIR(data.st_mode);
    #endif
}

string base36(int num)
{
	string b36;

	while (num)
	{
		b36 = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[num % 36] + b36;
		num /= 36; 
	}
	return b36;
}

void sleepMsecs(int msecs)
{
    #if defined(WIN32) || defined(_WIN32)
        Sleep(msecs);
    #else
        struct timespec spec;
        spec.tv_sec = msecs / 1000;
        spec.tv_nsec = 1000000 * (msecs % 1000);
        
        nanosleep(&spec, NULL);
    #endif
}
