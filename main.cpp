
#include "ReplayFile.h"
#include <iostream>

int main()
{
	ReplayFile rf("1BE973D44E656FCC97DCD1A4E9076C36.replay");
	if (!rf.Load())
	{
		printf("Error loading replay file");
		return 0;
	}
	
	rf.DeserializeHeader();
	rf.FixParents();
	rf.Parse();
	int o = 5;
	return 0;
}