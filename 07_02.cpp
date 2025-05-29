/*
Написать программу, в которой происходит обмен информацией между процессами с соседними номерами 
в соответствии с топологией кольца. Использовать неблокирующую буферизованную передачу сообщений, 
блокирующий прием сообщений и функции присоединения и отсоединения буфера для передачи сообщений. 
*/

#include "mpi.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char** argv)
{
    double a = 0, b = 0, c = 0;

    int const BufSize = sizeof(double) + MPI_BSEND_OVERHEAD;

    MPI_Init(NULL, NULL);

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int posl = (rank + 1) % size; // Номер последующего процесса
    int pred = (rank != 0) ? rank - 1 : size - 1; // Номер предшествующего процесса

    a = rank + 0.7;

    void* buf = malloc(BufSize);
    MPI_Buffer_attach(buf, BufSize);

    MPI_Bsend(&a, 1, MPI_DOUBLE, posl, 5, MPI_COMM_WORLD);

    MPI_Status st;
    MPI_Recv(&b, 1, MPI_DOUBLE, pred, 5, MPI_COMM_WORLD, &st);

    int tmp;
    MPI_Buffer_detach(buf, &tmp);

    MPI_Buffer_attach(buf, BufSize);

    MPI_Bsend(&a, 1, MPI_DOUBLE, pred, 5, MPI_COMM_WORLD);

    MPI_Recv(&c, 1, MPI_DOUBLE, posl, 5, MPI_COMM_WORLD, &st);

    MPI_Buffer_detach(buf, &tmp);

    if (buf != NULL) {
        free(buf);
    }

    cout << "Process " << rank << " a=" << a << " b=" << b << " c=" << c << endl;

    MPI_Finalize();
    return 0;
}
