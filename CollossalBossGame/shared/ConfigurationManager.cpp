#include "ConfigurationManager.h"
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <vector>
#include <string>
#include <sstream>
#include <assert.h>


using namespace std;

#define CONFIGFILEPATH "../config.ini"

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

// Constructor - zeros out memory and sets initialized to false.
ConfigurationManager::ConfigurationManager() {
	SecureZeroMemory(&keytable, sizeof(keytable));
	SecureZeroMemory(&valuetable, sizeof(valuetable));
	SecureZeroMemory(&keylen, sizeof(keylen));
	SecureZeroMemory(&valuelen, sizeof(valuelen));
	initialized = false;
}

// Initializes the Configuration Manager. Parses the file and stores data.
void ConfigurationManager::init(ConfigurationManager * cm) {
	ifstream conf(CONFIGFILEPATH, ios::in);
	string line;
	int x = 0;
	vector<string> chunk;
	if(conf.is_open()) {
		// cout << "Begin Configuration File Parsing..." << endl;
		while(conf.good()) {
			getline(conf,line);
			if(!(line.length() == 0) && !(line.at(0) == '#')) {
				// cout << "Parsing line: " << line << endl;
				split(line, '=', chunk);
				// Assert. If the configuration file fails, it should fail hard.
				assert(chunk.size() == 2 && "Config file must have 1 key=value pair per line. All spaces are ignored. Comment with leading # symbol(no spaces before #).");
				string key = chunk[0];
				string value = chunk[1];
				// cout << "Line split by = 1) " << key << " with length :" << key.length() << endl;
				// cout << "Line split by = 2) " << value << " with length :" << value.length() << endl;
				stripSpaces(key);
				stripSpaces(value);
				// cout << "Line split, trimmed, and stored 1) " << key << " with length : " << key.length() << endl;
				// cout << "Line split, trimmed, and stored 2) " << value << " with length : " << value.length() << endl;
				memcpy(&(keytable[0][x]), key.c_str(), key.size());
				memcpy(&(valuetable[0][x]), value.c_str(), value.size());
				keylen[x] = key.size();
				valuelen[x] = value.size();
				// cout << "Line length 1) " << key.size() << endl;
				// cout << "Line length 2) " << value.size() << endl;
				x++;
			}
		}
		conf.close();
	}
}

// Fetches the singleton Configuration Manager object
ConfigurationManager * ConfigurationManager::get() {
	static ConfigurationManager ConfigManager;
	if (ConfigManager.initialized == false) {
		ConfigManager.init(&ConfigManager);
		ConfigManager.initialized = true;
	}
	return &ConfigManager;
}

// Looks for a key and returns the value.
char * ConfigurationManager::find_config(string key) {
	int strlength = key.size();
	for(int i = 0; i < MAX_CONFIG_ITEMS; i++) {
		bool a = keylen[i] == strlength;
		bool b = comp(key, &(keytable[0][i]), strlength);
		if(a && b) {
			return &(valuetable[0][i]);
		}
	}
	cout << key << endl;
	assert(false && "Could not find configuration key specified.");
	return NULL;
}
