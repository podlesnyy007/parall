/*
Проиллюстрировать возможности коллективных взаимодействий процессов MPI на примере следующей задачи. 
Процесс 0 обновляет значение некоторой переменной вещественного типа и 
широковещательным образом рассылает обновленное значение всем процессам в коммуникаторе. 
Каждый процесс прибавляет к полученному значению свой номер, 
после чего данные от всех процессов собираются обратно в процессе 0. 
*/

#include "mpi.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    double a = 0;

    MPI_Init(NULL, NULL);

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        a = 0.3;
    }

    // Широковещательная рассылка сообщения
    MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double* A = new double[size];
        MPI_Gather(&a, 1, MPI_DOUBLE, A, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        for (int k = 0; k < size; k++) {
            cout << A[k] << endl;
        }

        delete[] A;
    } else {
        double b = a + rank;
        MPI_Gather(&b, 1, MPI_DOUBLE, NULL, 0, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
