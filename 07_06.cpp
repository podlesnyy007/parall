/*
Написать программу, иллюстрирующую возможности организации схемы «ведущий процесс – ведомые процессы (masterslave)» 
стандартными средствами MPI. Процесс с номером 0 считать ведущим, а остальные – ведомыми. 
В ведущем процессе использовать неблокирующий прием сообщений и функцию MPI_WAITSOME (MPI::Request::Waitsome). 
В «ведомых» процессах использовать блокирующую передачу сообщений.  
*/

#include "mpi.h"
#include <iostream>
#include <cmath>

using namespace std;

// Проверка всех элементов
bool Is_Finished(bool* Prizn, int N)
{
    bool Res = true;
    for (int k = 0; k < N; k++) {
        Res = Prizn[k] && Res;
    }
    return Res;
}

// Для корректной работы
// Функция Master должна быть быстрее, чем Slave
// Делаем число операций в ней пропорциональным длине массива
double Master(double* A, int N)
{
    double S = 0;
    for (int k = 0; k < N; k++) {
        S += A[k];
    }
    return S;
}

// Для корректной работы
// Функция Master должна быть быстрее, чем Slave
// Делаем число операций в ней пропорциональным кубу длины массива
void Slave(double* A, int N, int Rank)
{
    static int Count = 0;
    double* Tmp1 = new double[N];
    double* Tmp2 = new double[N];

    Tmp1[0] = cos(1.0 + Count + Rank);
    Tmp2[0] = cos(1.0 + Count - Rank);

    for (int k = 1; k < N; k++) {
        Tmp1[k] = sin(Tmp1[k - 1] + Tmp2[k - 1]);
        Tmp2[k] = sin(Tmp1[k - 1] - Tmp2[k - 1]);
    }

    // Длинный трехкратно вложенный цикл
    for (int k = 0; k < N; k++) {
        A[k] = 0;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                A[k] += Tmp1[i] * Tmp2[j] / (1 + k + (i - j) * (i - j));
            }
        }
    }

    Count++;
    delete[] Tmp2;
    delete[] Tmp1;
}

int const NN = 1000, NNN = 3;

int main(int argc, char** argv)
{
    MPI_Init(NULL, NULL);

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size < 2) {
        cout << "It is required not less than two processes!" << endl;
    } else {
        if (rank == 0) {
            // Ведущий процесс

            bool* Prizn = new bool[size - 1];
            double* A = new double[(size - 1) * NN];
            double* R = new double[size - 1];

            for (int k = 0; k < size - 1; k++) {
                R[k] = 0;
            }

            MPI_Request* Req = new MPI_Request[size - 1];
            MPI_Status* St = new MPI_Status[size - 1];
            int* Indx = new int[size - 1];

            double Tms = MPI_Wtime();

            for (int m = 0; m < NNN; m++) {
                for (int k = 1; k < size; k++) {
                    Prizn[k - 1] = false;
                    MPI_Irecv(&A[NN * (k - 1)], NN, MPI_DOUBLE, k, 5,
                              MPI_COMM_WORLD, &Req[k - 1]);
                }

                while (!Is_Finished(Prizn, size - 1)) {
                    // Какой-то из ведомых процессов не переслал очередные данные
                    int Num;
                    MPI_Waitsome(size - 1, Req, &Num, Indx, St);

                    for (int k = 0; k < Num; k++) {
                        Prizn[Indx[k]] = true;
                        R[Indx[k]] += Master(&A[NN * Indx[k]], NN);
                    }
                }
            }

            Tms = MPI_Wtime() - Tms;

            for (int k = 0; k < size - 1; k++) {
                cout << "R[" << k << "]=" << R[k] << endl;
            }

            cout << "Time=" << Tms << "  s" << endl;

            delete[] Indx;
            delete[] St;
            delete[] Req;
            delete[] R;
            delete[] A;
            delete[] Prizn;
        } else {
            // Ведомые процессы

            double* A = new double[NN];

            for (int m = 0; m < NNN; m++) {
                Slave(A, NN, rank);
                MPI_Send(A, NN, MPI_DOUBLE, 0, 5, MPI_COMM_WORLD);
            }

            delete[] A;
        }
    }

    MPI_Finalize();
    return 0;
}
