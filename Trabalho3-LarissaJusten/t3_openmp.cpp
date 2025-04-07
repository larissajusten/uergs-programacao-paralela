#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <time.h>
#include <vector>
#include <algorithm>

#define NUM_THREADS 10
#define TAM_MAX_VETOR 1000000
#define TAM_POR_THREAD 100000

using namespace std;

int main(int argc, char *argv[]) {

  int vetor[TAM_MAX_VETOR] = {0};
  int numero, vezes_que_o_num_aparece, indice_thread = 0;

  /* Recebe inputs do usuario */
  cout << "\nDigite o número a ser inserido no vetor: " << endl;
  cin >> numero;

  while(numero <= 0) {
    cout << "\nNúmero inválido! Tente novamente, digite o número a ser inserido no vetor: " << endl;
    cin >> numero;
  }

  cout << "\nQuantidade de vezes que o número aparece no vetor: " << endl;
  cin >> vezes_que_o_num_aparece;

  while(vezes_que_o_num_aparece <= 0 || vezes_que_o_num_aparece >= TAM_MAX_VETOR) {
    cout << "\nQuantidade inválida! Tente novamente, quantidade de vezes que o número aparece no vetor: " << endl;
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
        cout << "Posição [" << posicao_temporaria << "] inválida, digite novamente: " << endl;
      }
    }
    vetor[posicao_temporaria] = numero;
  }


  /* Variaveis das threads */
  int contar_x_num_aparece, pos = 0;
  double inicio_tempo, fim_tempo, total_tempo = 0;
  vector<int> vetor_de_posicoes;

  /* Inicia threads */
  omp_set_num_threads(NUM_THREADS);
  #pragma omp parallel shared(contar_x_num_aparece), private(pos, inicio_tempo, fim_tempo)
  {
    inicio_tempo = omp_get_wtime();
    for(pos = (omp_get_thread_num() * TAM_POR_THREAD);
        pos < (omp_get_thread_num() * TAM_POR_THREAD + TAM_POR_THREAD-1);
        pos++) {
      if(vetor[pos] == numero) {
        contar_x_num_aparece++;
        #pragma omp critical
        {
          vetor_de_posicoes.push_back(pos);
        }
      }
    }
    fim_tempo = omp_get_wtime();
    #pragma omp critical
    {
      total_tempo += fim_tempo - inicio_tempo;
    }
  }

  cout << "\n Tempo decorrido (omp_get_wtime): " << total_tempo*1000 << " (milisegundos)\n" << endl;

  cout << "\n Número de vezes que o número [" << numero << "] aparece: " << contar_x_num_aparece << endl;
  cout << "\n Posições: ";
  for(int posicao : vetor_de_posicoes){
    cout << posicao << ",";
  }
  cout << endl;
}
