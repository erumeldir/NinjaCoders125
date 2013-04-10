#include "ConfigurationManager.h"
#include <iostream>
using namespace std;
#define CONFIGFILEPATH "../config.ini"

ConfigurationManager::ConfigurationManager() {
	ZeroMemory(&table, sizeof(table));
	ZeroMemory(&tlen, sizeof(tlen));
	initialized = false;
}

ConfigurationManager ConfigurationManager::init(ConfigurationManager * cm) {
	ifstream conf = open(CONFIGFILEPATH, ios::in);
	string line;
	int x = 0;
	vector<string> chunk;
	if(conf.is_open()) {
		cout << "Begin Configuration File Parsing..." << endl;
		while(conf.good()) {
			getline(conf,line);
			if(!line.at(0) == '#') {
				cout << "Parsing line: " << line << endl;
				split(line, '=', chunk);
				cout << "Line split by = 1) " << chunk[0] << endl;
				cout << "Line split by = 2) " << chunk[1] << endl;
				chunk[0].erase(std::remove(chunk[0].begin(), chunk[0].end(), ' '), chunk[0].end());
				chunk[1].erase(std::remove(chunk[1].begin(), chunk[1].end(), ' '), chunk[1].end());
				cout << "Line split, trimmed, and stored 1) " << chunk[0] << endl;
				cout << "Line split, trimmed, and stored 2) " << chunk[1] << endl;
				memcpy(&(table[0][x][0]), chunk[0].c_str(), chunk[0].size());
				memcpy(&(table[0][x][1]), chunk[1].c_str(), chunk[1].size());
				tlen[x][0] = chunk[0].size();
				tlen[x][1] = chunk[1].size();
				cout << "Line length 1) " << chunk[0].size() << endl;
				cout << "Line length 2) " << chunk[1].size() << endl;
				x++;
			}
		}
		conf.close();
	}
}

ConfigurationManager ConfigurationManager::get() {
	static ConfigurationManager CM;
	if (CM.initialized == false) {
		CM.init(&CM);
		CM.initialized = true;
	}
	return CM;
}

char * ConfigurationManager::find_config(string config) {
	int strlength = config.size();
	for(int i = 0; i < MAX_CONFIG_ITEMS; i++) {
		if(tlen[i] == strlength && !config.compare(0, strlength, (const char*)&(table[0][i][0]), strlength)) {
			return &(table[0][i][1];
		}
	}
	return NULL;
}