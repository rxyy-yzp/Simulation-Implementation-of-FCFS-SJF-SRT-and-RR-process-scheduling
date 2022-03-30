# Simulation-Implementation-of-FCFS-SJF-SRT-and-RR-process-scheduling
Simulation Implementation of FCFS, SJF, SRT and RR process scheduling(Operating system)  
（1）用VC实现FCFS，SJF，SRT和RR进程调度算法的演示。  
（2）每个用来标识进程的进程控制块PCB可用结构来描述，包括但不限于以下字段：  
进程标识数ID。  
进程需要占用CPU时间和已经运行时间。  
进程的阻塞时间STARTBLOCK，表示当进程再运行STARTBLOCK个时间片后，进程将进入阻塞状态。  
进程被阻塞的时间BLOCKTIME，表示已阻塞的进程再等待BLOCKTIME个时间片后，将转换成就绪状态。  
进程状态STATE：运行，就绪，阻塞。队列指针NEXT，用来将PCB排成队列。  
（3）为了清楚地观察每个进程的调度过程，程序应将每个时间片内的进程的情况显示出来，包括正在运行的进程，处于就绪队列中的进程和处于阻塞队列中的进程。  
