#define MAX_CONFIG_ITEMS 200

class ConfigurationManager
{
public:
	static ConfigurationManager get();
	char* find_config(string config);
	
private:
	ConfigurationManager();
	virtual ~ConfigurationManager();
	
	void init(ConfigurationManager * cm);
	
	bool initialized;
	char table[32][MAX_CONFIG_ITEMS][2];
	int  tlen[MAX_CONFIG_ITEMS][2];
};