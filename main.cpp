
#include "ReplayFile.h"
#include <iostream>
#include "bench.h"
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
	BENCHMARK("Parse", rf.Parse(), 10);
	int o = 5;
	getchar();
	return 0;
}