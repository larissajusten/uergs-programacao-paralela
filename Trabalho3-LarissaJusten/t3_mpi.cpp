/**
 * 
 * MPI Trabalho 3
 * SISTEMA DE ENCONTRO DE VALORES
 * @author Larissa Justen
 * @version 1.3.0
 * 
 */


#include "mpi.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

using namespace std;

#define TAM_MAX_VETOR 1000000
#define TAM_POR_THREAD 100000
#define send_data_tag 2001
#define return_data_tag 2002
#define root_process 0

int array2[TAM_MAX_VETOR];

int main(int argc, char **argv)
{

    int count, partial_count;
    MPI_Status status;

    int rank, size, next, prev, message, aux_token;

    int node_id, num_rows_to_receive, num_rows_received, start_row, end_row, num_rows_to_send;


    int vec[TAM_MAX_VETOR] = {0};
    int numero, vezes_que_o_num_aparece, indice_thread = 0;

    srand(time(NULL));

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size != 10){
        if (rank == 0)
            cout << "\nEsse programa foi feito para 10 nodos, corrija o comando para utilizar 10 nodos e tente novamente" << endl;
    }else{
        if (root_process == rank) {
            
            /* Recebe inputs do usuario */
            cout << "\nDigite o numero a ser inserido no vetor: " << endl;
            cin >> numero;

            cout << "\nQuantidade de vezes que o número aparece no vetor: " << endl;
            cin >> vezes_que_o_num_aparece;

            while(vezes_que_o_num_aparece < 0 || vezes_que_o_num_aparece >= TAM_MAX_VETOR) {
                cout << "\nQuantidade invalida! Tente novamente, quantidade de vezes que o número aparece no vetor: " << endl;
                cin >> vezes_que_o_num_aparece;
            }

            int posicoes_do_numero[vezes_que_o_num_aparece] = {0};

            cout << "\nDigite as posições que o número aparece: [0 - 999.999]" << endl;
            for(int i = 0; i < vezes_que_o_num_aparece; i++) {
                int posicao_temporaria = TAM_MAX_VETOR;

                cout << "Posição: " << endl;

                while(posicao_temporaria < 0 || posicao_temporaria >= TAM_MAX_VETOR) {
                cin >> posicao_temporaria;
                if (posicao_temporaria < 0 || posicao_temporaria >= TAM_MAX_VETOR) {
                    cout << "Posição [" << posicao_temporaria << "] invalida, digite novamente: " << endl;
                }
                }
                vec[posicao_temporaria] = numero;
            }

            // Parte que distribui o array pros nodos
            double inicio_tempo = MPI_Wtime();
            for(node_id = 1; node_id < size; node_id++) {
                start_row = node_id*TAM_POR_THREAD ;
                end_row   = node_id*TAM_POR_THREAD + TAM_POR_THREAD-1;

                num_rows_to_send = end_row - start_row + 1;

                MPI_Send( &num_rows_to_send, 1 , MPI_INT, node_id, send_data_tag, MPI_COMM_WORLD);

                MPI_Send( &vec[start_row], num_rows_to_send, MPI_INT, node_id, send_data_tag, MPI_COMM_WORLD);
            }

            // Parte que conta pelo root
            count = 0;
            for(int i = 0; i < TAM_POR_THREAD; i++) {
                if(vec[i] == numero) {
                    count += 1;
                }
            } 
            cout <<"Contagem Parcial: "<< count << " do processo: "<< root_process << endl;

            // Parte que pega as contagens dos outros processos
            for(node_id = 1; node_id < size; node_id++) {
            
                MPI_Recv( &partial_count, 1, MPI_INT, MPI_ANY_SOURCE, return_data_tag, MPI_COMM_WORLD, &status);

                cout <<"Contagem Parcial: "<< partial_count << " do processo: "<< node_id << endl;
            
                count += partial_count;
            }

            cout << "Numero de vezes: " << count << endl;
            cout << "Tempo total: " << MPI_Wtime() - inicio_tempo << endl;

        }else
        {
            // recebendo array para fazer a contagem
            MPI_Recv( &num_rows_to_receive, 1, MPI_INT,root_process, send_data_tag, MPI_COMM_WORLD, &status);
            
            MPI_Recv( &array2, num_rows_to_receive, MPI_INT,root_process, send_data_tag, MPI_COMM_WORLD, &status);

            num_rows_received = num_rows_to_receive;

            // fazendo a contagem
            partial_count = 0;
            for(int i = 0; i < num_rows_received; i++) {
                if(array2[i] != 0) {
                    partial_count += 1;
                }
            }

            // retornando a mensagem
            MPI_Send( &partial_count, 1, MPI_INT, root_process,return_data_tag, MPI_COMM_WORLD);
        }
        
    }
    MPI_Finalize();
    return 0;
}
