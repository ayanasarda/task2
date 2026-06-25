CPU Scheduler and Cache Simulator

This project simulates how tasks are scheduled on a CPU and how memory is managed using a 3-level cache system.

I used a Round Robin scheduler (quantum = 2) to run tasks. Each task makes memory requests, and the program checks whether the required memory block is in L1, L2, L3, or needs to be fetched from RAM.

The cache uses FIFO eviction, so when a level is full, the oldest block is removed.

In real systems, multiple programs compete for CPU time and fast memory access. The scheduler decides which task runs next, while the cache hierarchy helps reduce memory access time by keeping frequently used data closer to the CPU.

L1 is the fastest but smallest cache, followed by L2 and L3. If data is not found in any cache level, it is fetched from RAM. This simulation shows how cache hits and misses affect performance.

Input

Tasks are read from input.txt in this format:

TASK T1 BURST 5 MEM M1 M4 M7  
TASK T2 BURST 3 MEM M2 M1  

Output

- Shows which task is running at each cycle  
- Displays memory requests and cache hits/misses  
- Prints the state of L1, L2, and L3 after every cycle  
- Final summary includes total cycles, tasks completed, and RAM accesses  
