#include <string>

#define MAX_CONFIG_ITEMS 5
#define MAX_KEY_LENGTH 32
#define MAX_VALUE_LENGTH 256

class ConfigurationManager
{
public:
	static ConfigurationManager * get();
	char* find_config(std::string config);
	
	std::string find_config_as_string(std::string key);
	int find_config_as_int(std::string key);
	bool find_config_as_bool(std::string key);

private:
	ConfigurationManager();
	// virtual ~ConfigurationManager();
	
	void init(ConfigurationManager * cm);					// Initializes the CM singleton
	int find_config_index(std::string config);
	
	bool initialized;										// Set to true when init() is called
	char keytable[MAX_CONFIG_ITEMS][MAX_KEY_LENGTH];		// Table of keys.
	char valuetable[MAX_CONFIG_ITEMS][MAX_VALUE_LENGTH];	// Table of values.
	int  keylen[MAX_CONFIG_ITEMS];							// Number of characters of each key
	int  valuelen[MAX_CONFIG_ITEMS];						// Number of characters of each value
};
typedef ConfigurationManager CM;