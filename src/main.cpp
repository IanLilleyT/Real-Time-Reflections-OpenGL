#include <string>

#include <gl3w/gl3w.h>
#include <time.h>

#include "Singleton.h"
#include "SFMLCore.h"
#include "GlobalPaths.h"

int main(int argc, char **argv)
{
	srand((unsigned int)time(NULL));
	std::string executablePath = std::string(argv[0]);
	std::string dataPath = executablePath + "/../../data/";
	Singleton<GlobalPaths>::Instance()->setDataPath(dataPath);
	Singleton<SFMLCore>::Instance();
	return 0;
}

/* 
Things to fix:
-Test removing event listeners
-Same mesh with different shaders
*/