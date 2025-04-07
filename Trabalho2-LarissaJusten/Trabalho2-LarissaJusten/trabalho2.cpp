#include "mpi.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

using namespace std;

enum Tags { 
    T_TOKEN = 418, 
    T_MESSAGE = 100
};

/**
 * 
 * RINGULAR
 * SISTEMA DE ENVIO DE MENSAGEM EM ANEL COM TOKEN
 * @author Larissa Justen
 * @version 4.1.1
 * 
 */

int main(int argc, char **argv)
{
    int rank, size, next, prev, message, aux_token;
    bool have_token = false;

    srand(time(NULL));

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    next = (rank + 1) % size;
    prev = (rank + size - 1) % size;

    if (0 == rank) {
        have_token = true;
        message = (rand() % 10 ); 
        aux_token = 0;

        cout << "\n\nProcesso {0} envia [" << message << "] para o {" << next
                  << "} (" << size << " processos no anel) \n"
                  << endl;
        MPI_Send(&message, 1, MPI::INT, next, T_MESSAGE, MPI_COMM_WORLD);
        MPI_Send(&aux_token, 1, MPI::INT, next, T_TOKEN, MPI_COMM_WORLD);
        cout << "Processo {" << rank << "} enviou [" << message << "] para {"<< next <<"}"<< endl;
    }

    while (1) {
        MPI_Recv(&message, 1, MPI::INT, prev, T_MESSAGE, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&aux_token, 1, MPI::INT, prev, T_TOKEN, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (have_token) {
            sleep(2);
            aux_token=1;
            have_token=false;
            cout << "Processo {"<< rank <<"} vai enviar o\e[1m token\e[0m para {" << next
                      << "}" << endl;
        }else{
            if (aux_token == 1){

                cout << "Processo {"<< rank <<"} recebeu o\e[1m token\e[0m "
                        << endl;

                have_token=true;
                message = (rand() % 10 );
                
                cout << "Processo {"<< rank <<"} definiu: [" << message << "]"
                        << endl;
            }
            aux_token = 0;
        }

        cout << "Processo {" << rank << "} enviou [" << message << "] para {"<< next <<"}"<< endl;
        MPI_Send(&message, 1, MPI::INT, next, T_MESSAGE, MPI_COMM_WORLD);
        MPI_Send(&aux_token, 1, MPI::INT, next, T_TOKEN, MPI_COMM_WORLD);
    }

    MPI_Finalize();  
    return 0;
}
