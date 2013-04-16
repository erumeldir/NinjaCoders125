#include "ConfigurationManager.h"
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <vector>
#include <string>
#include <sstream>
#include <assert.h>
#include <algorithm>

#define CONFIGFILEPATH "../config.ini"
#define USERCONFIGFILEPATH "../config_dev.ini"

#pragma region Helper Functions.

/* Split - Takes in a string, a delimiter, and a vector.
 * returns the vector with the string split by the delimiter.
 */
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

// Very simple local function to strip all spaces from a string.
void stripSpaces(string & s) {
	for(unsigned int i = 0; i < s.length(); i++) {
		if(s[i] == ' ') {
			s.erase(i,1);
			i--;
		}
	}
}

// Very simple local string compare function.
bool comp(string s, char * c, int length) {
	for(int i = 0; i < length; i++) {
		if(s.at(i) != c[i]) {
			return false;
		}
	}
	return true;
}

#pragma endregion split, stripSpaces, and comp[are]

// Constructor - zeros out memory and sets initialized to false.
ConfigurationManager::ConfigurationManager() {
	SecureZeroMemory(&keytable, sizeof(keytable));
	SecureZeroMemory(&valuetable, sizeof(valuetable));
	SecureZeroMemory(&keylen, sizeof(keylen));
	SecureZeroMemory(&valuelen, sizeof(valuelen));
	initialized = false;
}

// Initializes the Configuration Manager. Parses the file and stores data.
void ConfigurationManager::initializefile(char * filepath) {
	ifstream conf(filepath, ios::in);
	string line;
	int x = 0;
	vector<string> chunk;
	if(conf.is_open()) {
		// cout << "Begin Configuration File Parsing..." << endl;
		while(conf.good()) {
			getline(conf,line);
			if(!(line.length() < 2) && !(line.at(0) == '#')) {
				// cout << "Parsing line: " << line << endl;
				chunk.clear();
				split(line, '=', chunk);
				// cout << "Number of Chunks: " << chunk.size() << endl;
				// Assert. If the configuration file fails, it should fail hard.
                printf("Chunk size = %d, len = %d (%s)\n", chunk.size(), line.length(), line.c_str());
				assert(chunk.size() == 2 && "Config file must have 1 key=value pair per line. All spaces are ignored. Comment with leading # symbol(no spaces before #).");
				string key = chunk[0];
				string value = chunk[1];
				// cout << "Line split by = 1) " << key << " with length :" << key.length() << endl;
				// cout << "Line split by = 2) " << value << " with length :" << value.length() << endl;
				stripSpaces(key);
				stripSpaces(value);
				// cout << "Line split, trimmed, and stored 1) " << key << " with length : " << key.length() << endl;
				// cout << "Line split, trimmed, and stored 2) " << value << " with length : " << value.length() << endl;
				memcpy(&(keytable[x][0]), key.c_str(), key.size());
				memcpy(&(valuetable[x][0]), value.c_str(), value.size());
				keylen[x] = key.size();
				valuelen[x] = value.size();

printf(__FILE__" %d: (k,v) = (%s,%s)\n", __LINE__, &(keytable[x][0]), &(valuetable[x][0]));


				x++;
			}
		}
		conf.close();
	}
}

// Initializes the static Configuration Manager.
void ConfigurationManager::init() {
	ifstream ifile(USERCONFIGFILEPATH);
	if (!ifile) {	// If config.user doesn't exist, we should copy config.ini and make one.
		ifile.close();
		ifstream inFile(CONFIGFILEPATH, ios::binary);
		ofstream outFile(USERCONFIGFILEPATH, ios::binary);
		while (inFile.good()) { 
			outFile.put(inFile.get());
		}
		inFile.close();
		outFile.close();
	}
	initializefile(USERCONFIGFILEPATH);
}

// Fetches the singleton Configuration Manager object
ConfigurationManager * ConfigurationManager::get() {
	static ConfigurationManager ConfigManager;
	if (ConfigManager.initialized == false) {
		ConfigManager.init();
		ConfigManager.initialized = true;
	}
	return &ConfigManager;
}

#pragma region Functions to find a configuration value given a key

// Looks for a key and returns the value.
char * ConfigurationManager::find_config(string key) {
	return &(valuetable[find_config_index(key)][0]);
}

int ConfigurationManager::find_config_index(string key) {
	int strlength = key.size();
	for(int i = 0; i < MAX_CONFIG_ITEMS; i++) {
		bool a = keylen[i] == strlength;
		bool b = comp(key, &(keytable[i][0]), strlength);
		if(a && b) {
			return i;
		}
	}
	cout << key << endl;
	assert(false && "Could not find configuration key specified.");
	return NULL;
}

string ConfigurationManager::find_config_as_string(string key) {
	int index = find_config_index(key);
	return string((const char *)&(valuetable[index][0]), valuelen[index]);
}

int ConfigurationManager::find_config_as_int(string key) {
	int index = find_config_index(key);
	return atoi(&(valuetable[index][0]));
}

bool ConfigurationManager::find_config_as_bool(string key) {
	int index = find_config_index(key);
	string boolean((const char *)&(valuetable[index][0]), valuelen[index]);
	std::transform(boolean.begin(), boolean.end(), boolean.begin(), ::tolower);
	if(boolean.compare("true")) {
		return true;
	}
	if(boolean.compare("false")) {
		return false;
	}
	assert(false && "Invalid Configuration value as boolean.");
	return false;
}

Vec3f ConfigurationManager::find_config_as_point(string key) {
	vector<string> chunk;
	string point = find_config_as_string(key);
	split(point, ',', chunk);
	assert(chunk.size() == 3 && "Config Error: Points are represented as <floatx> , <floaty> , <floatz>. Spaces will ignored.");
	string x = chunk[0], y = chunk[1], z = chunk[2];
	stripSpaces(x); stripSpaces(y); stripSpaces(z);
	float fx = (float)atof(x.c_str()), fy = (float)atof(y.c_str()), fz = (float)atof(z.c_str());
	return Vec3f(fx, fy, fz);
}

#pragma endregion

