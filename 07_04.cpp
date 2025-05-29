/*
То же самое, что в задании 2, но с совмещенной блокирующей отправкой и приемом сообщений. 
*/

#include "mpi.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    double a = 0, b = 0, c = 0;

    MPI_Init(NULL, NULL);

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int posl = (rank + 1) % size; // Номер последующего процесса
    int pred = (rank != 0) ? rank - 1 : size - 1; // Номер предшествующего процесса

    a = rank + 0.7;

    MPI_Status st;

    MPI_Sendrecv(
        &a, 1, MPI_DOUBLE, posl, 5,
        &b, 1, MPI_DOUBLE, pred, 5,
        MPI_COMM_WORLD, &st
    );

    MPI_Sendrecv(
        &a, 1, MPI_DOUBLE, pred, 5,
        &c, 1, MPI_DOUBLE, posl, 5,
        MPI_COMM_WORLD, &st
    );

    cout << "Process " << rank << " a=" << a << " b=" << b << " c=" << c << endl;

    MPI_Finalize();
    return 0;
}
