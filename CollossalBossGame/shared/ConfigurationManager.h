#include <string>

#define MAX_CONFIG_ITEMS 5

class ConfigurationManager
{
public:
	static ConfigurationManager * get();
	char* find_config(std::string config);
	
private:
	ConfigurationManager();
	// virtual ~ConfigurationManager();
	
	void init(ConfigurationManager * cm);
	
	bool initialized;
	char table[32][2][MAX_CONFIG_ITEMS];
	int  tlen[2][MAX_CONFIG_ITEMS];
};
typedef ConfigurationManager CM;