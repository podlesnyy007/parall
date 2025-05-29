/*
Написать параллельную программу, в которой процесс с номером 0 посылает процессу с номером 1 сообщение 
заранее неизвестной длины, а процесс 1 анализирует сообщение, определяет его длину и принимает данные. 
*/

#include <mpi.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char** argv)
{
    MPI_Init(NULL, NULL);

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size >= 2) {
        int rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        if (rank == 0) {
            int N = 3 + (rand() % 10);
            double* A = new double[N];

            for (int k = 0; k < N; k++) {
                A[k] = k + 0.7;
            }

            cout << "Process 0 sends the data:" << endl;
            for (int k = 0; k < N; k++) {
                cout << A[k] << endl;
            }

            MPI_Send(A, N, MPI_DOUBLE, 1, 5, MPI_COMM_WORLD);
            delete[] A;
        }

        if (rank == 1) {
            MPI_Status St;
            MPI_Probe(0, 5, MPI_COMM_WORLD, &St);

            int N;
            MPI_Get_count(&St, MPI_DOUBLE, &N);

            double* A = new double[N];
            MPI_Recv(A, N, MPI_DOUBLE, 0, 5, MPI_COMM_WORLD, &St);

            cout << "Process 1 has accepted the data:" << endl;
            for (int k = 0; k < N; k++) {
                cout << A[k] << endl;
            }

            delete[] A;
        }
    } else {
        cout << "Performance of the program not less than in 2 processes is required" << endl;
    }

    MPI_Finalize();
    return 0;
}
