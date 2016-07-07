#include "stdafx.h"
#include "Core/CycleCounter.h"
#include "Core/Console.h"


int64_t DebugCycleCounter::counters[] = { 0, 0, 0, 0, 0, 0, 0 };
int64_t DebugCycleCounter::hitcount[] = { 0, 0, 0, 0, 0, 0, 0 };
float DebugCycleCounter::timer = 0.0f;

void DebugCycleCounterResetAndPrint(float dt)
{
	DebugCycleCounter::timer += dt;
	if ( DebugCycleCounter::timer > 1)
	{
		Console::Instance() << std::dec << "Debug: " << std::endl;
		for (int counterIndex = 0; counterIndex < DebugCycleCounter::arrays; ++counterIndex)
		{
			Console::Instance() << counterIndex << ":";
			Console::Instance() << " " << DebugCycleCounter::hitcount[counterIndex] << "h/s ";
			if(DebugCycleCounter::hitcount[counterIndex] != 0)
				Console::Instance() << " " << DebugCycleCounter::counters[counterIndex] / (DebugCycleCounter::hitcount[counterIndex]) << " cy/h";
			Console::Instance() << std::endl;
			
			
			DebugCycleCounter::counters[counterIndex] = 0;
			DebugCycleCounter::hitcount[counterIndex] = 0;
		}
		DebugCycleCounter::timer = 0;
	}
}