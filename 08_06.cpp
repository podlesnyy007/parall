/*
Стандартными средствами MPI создать коммуникатор с виртуальной топологией графа для взаимодействия процессов по схеме 
«ведущий процесс – ведомые процессы» таким образом, чтобы все процессы в рамках данной топологии обменивались сообщениями лишь с 
нулевым процессом. Разрешить перенумерацию процессов при создании нового коммуникатора. 
*/

#include <mpi.h>
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    MPI_Init(NULL, NULL);

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int* Index = new int[size];
    for (int k = 0; k < size; k++) {
        Index[k] = size - 1 + k;
    }

    int* Edges = new int[2 * size - 2];
    for (int k = 0; k < size - 1; k++) {
        Edges[k]             = k + 1; // Рёбра от 0 к другим
        Edges[size - 1 + k]  = 0;     // Рёбра от других к 0
    }

    MPI_Comm Gr_Comm;
    MPI_Graph_create(MPI_COMM_WORLD, size, Index, Edges, true, &Gr_Comm);

    int rank;
    MPI_Comm_rank(Gr_Comm, &rank);

    int* Neighbors = new int[size - 1];
    int num;
    MPI_Graph_neighbors_count(Gr_Comm, rank, &num);
    MPI_Graph_neighbors(Gr_Comm, rank, num, Neighbors);

    for (int k = 0; k < num; k++) {
        int rank1;
        MPI_Status st;

        MPI_Sendrecv(
            &rank, 1, MPI_INT, Neighbors[k], 5,
            &rank1, 1, MPI_INT, Neighbors[k], 5,
            Gr_Comm, &st
        );

        cout << "Process " << rank << " co-operates with process " << rank1 << endl;
    }

    MPI_Comm_free(&Gr_Comm);
    delete[] Neighbors;
    delete[] Edges;
    delete[] Index;

    MPI_Finalize();
    return 0;
}
