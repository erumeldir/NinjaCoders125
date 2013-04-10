#include <string>

#define MAX_CONFIG_ITEMS 5
#define MAX_KEY_LENGTH 32
#define MAX_VALUE_LENGTH 256

class ConfigurationManager
{
public:
	static ConfigurationManager * get();
	char* find_config(std::string config);
	
private:
	ConfigurationManager();
	// virtual ~ConfigurationManager();
	
	void init(ConfigurationManager * cm);					// Initializes the CM singleton
	
	bool initialized;										// Set to true when init() is called
	char keytable[MAX_KEY_LENGTH][MAX_CONFIG_ITEMS];		// Table of keys.
	char valuetable[MAX_VALUE_LENGTH][MAX_CONFIG_ITEMS];	// Table of values.
	int  keylen[MAX_CONFIG_ITEMS];							// Number of characters of each key
	int  valuelen[MAX_CONFIG_ITEMS];						// Number of characters of each value
};
typedef ConfigurationManager CM;