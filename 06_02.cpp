/*
Для каждого из процессов распечатать: 
1) имя процессора (т.е. компьютера, или узла), на котором он выполняется; 
2) разрешение таймера MPI; 
3) среднее время, требуемое на замер времени. 
*/

#include "mpi.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    int const NTimes = 100;
    char Proc_Name[MPI_MAX_PROCESSOR_NAME + 1];

    MPI_Init(NULL, NULL);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int NLen;
    MPI_Get_processor_name(Proc_Name, &NLen);

    double tick = MPI_Wtick();
    double time_start = MPI_Wtime();
    double time_finish;

    for (int i = 1; i <= NTimes; i++) {
        time_finish = MPI_Wtime();
    }

    cout << "Processor " << rank << "  " << Proc_Name << endl
         << "timer's tick=" << tick << "  time="
         << (time_finish - time_start) / NTimes << endl;

    MPI_Finalize();
    return 0;
}
