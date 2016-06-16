#pragma once
#include <cstdint>
#include <iostream>

//#define DEBUG_CYCLECOUNT

#ifdef DEBUG_CYCLECOUNT
	#define BEGIN_TIMED_BLOCK(ID) int64_t startCycleCount##ID = __rdtsc(); 
	#define END_TIMED_BLOCK(ID) DebugCycleCounter::counters[DebugCycleCounter_##ID] += __rdtsc() - startCycleCount##ID; DebugCycleCounter::hitcount[DebugCycleCounter_##ID]++;
#else
	#define BEGIN_TIMED_BLOCK(ID)
	#define END_TIMED_BLOCK(ID)
#endif // DEBUG_CYCLECOUNT


enum
{
	DebugCycleCounter_gameLoop,
	DebugCycleCounter_update,
	DebugCycleCounter_handleInput,
	DebugCycleCounter_draw1,
	DebugCycleCounter_draw2,
	DebugCycleCounter_draw3,
	DebugCycleCounter_draw4
};

typedef struct DebugCycleCounter
{
	const static int arrays = 7;
	static float timer;
	static int64_t counters[arrays];
	static int64_t hitcount[arrays];
} DebugCycleCounter;

void DebugCycleCounterResetAndPrint(float dt);
