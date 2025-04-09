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
 * @version 4.1.0
 * 
 */

int main(int argc, char *argv[])
{
    int rank, size, next, prev, message, aux_token;
    bool have_token = false;

    srand(time(NULL));

    MPI::Init();
    rank = MPI::COMM_WORLD.Get_rank();
    size = MPI::COMM_WORLD.Get_size();

    next = (rank + 1) % size;
    prev = (rank + size - 1) % size;

    if (0 == rank) {
        have_token = true;
        message = (rand() % 10 ); 
        aux_token = 0;

        cout << "\n\nProcesso {0} envia [" << message << "] para o {" << next
                  << "} (" << size << " processos no anel) \n"
                  << endl;
        MPI::COMM_WORLD.Send(&message, 1, MPI::INT, next, T_MESSAGE);
        MPI::COMM_WORLD.Send(&aux_token, 1, MPI::INT, next, T_TOKEN);
        cout << "Processo {" << rank << "} enviou [" << message << "] para {"<< next <<"}"<< endl;
    }

    while (1) {
        MPI::COMM_WORLD.Recv(&message, 1, MPI::INT, prev, T_MESSAGE);
        MPI::COMM_WORLD.Recv(&aux_token, 1, MPI::INT, prev, T_TOKEN);

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
        MPI::COMM_WORLD.Send(&message, 1, MPI::INT, next, T_MESSAGE);
        MPI::COMM_WORLD.Send(&aux_token, 1, MPI::INT, next, T_TOKEN);
    }

    MPI::Finalize();
    return 0;
}
