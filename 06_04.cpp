#include <mpi.h>
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    double a = 0, b = 0;

    MPI_Init(NULL, NULL);

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        // Блокирующая посылка сообщения
        a = 7.7;
        MPI_Send(&a, 1, MPI_DOUBLE, 1, 5, MPI_COMM_WORLD);

        MPI_Status status;
        MPI_Recv(&b, 1, MPI_DOUBLE, 1, 5, MPI_COMM_WORLD, &status);
    }

    if (rank == 1) {
        // Блокирующий приём сообщения
        b = 3.3;
        MPI_Status status;
        MPI_Recv(&a, 1, MPI_DOUBLE, 0, 5, MPI_COMM_WORLD, &status);

        MPI_Send(&b, 1, MPI_DOUBLE, 0, 5, MPI_COMM_WORLD);
    }

    cout << "Process " << rank << " a=" << a << " b=" << b << endl;

    MPI_Finalize();
    return 0;
}
