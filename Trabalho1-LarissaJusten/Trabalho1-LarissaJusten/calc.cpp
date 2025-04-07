#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <time.h>

using namespace std;


int main(int argc, char *argv[]) {

  srand(time(NULL));

  int numDeCalculos_sum,numDeCalculos_sub,numDeCalculos_div,numDeCalculos_mult, i;
  int threadVencedora = 0;
  double variavelA, variavelB;
  double resultadoSom, resultadoSub, resultadoDiv, resultadoMult;
  double startSom, endSom, tempoSom;
  double startSub, endSub, tempoSub;
  double startDiv, endDiv, tempoDiv;
  double startMult, endMult, tempoMult;

  variavelA = (rand() % 100 );
  variavelB = (rand() % 1000 );
  numDeCalculos_sum = (rand() % 100000 + 100000);
  numDeCalculos_sub = (rand() % 100000 + 100000);
  numDeCalculos_div = (rand() % 100000 + 100000);
  numDeCalculos_mult = (rand() % 100000 + 100000);
  cout << endl;
  cout << "----------------------------------"<< endl;
  cout << "-\t\tSAM"<< endl;
  cout << "- SISTEMA DE APOSTAS MATEMATICAS"<< endl;
  cout << "- CRIADORA: LARISSA JUSTEN"<< endl;
  cout << "- VERSÃO: 2.1.1" << endl;
  cout << "----------------------------------"<< endl;
  cout << endl;
  cout << "Tendo o numero: [" << variavelA << "] e o numero [" << variavelB << "]." << endl;
  cout << "Sabendo que as operacoes serão realizadas entre [" << numDeCalculos_sum <<"] e ["<< numDeCalculos_mult << "] vezes." << endl;
  cout << "Qual operação você acha que calcular mais rapido?\n" << endl;

  cout << "Escolha.\n" << endl;

  while(threadVencedora != 1 && threadVencedora != 2 && threadVencedora != 3 && threadVencedora != 4){
    
    cout << "\t[1] Soma" << endl;
    cout << "\t[2] Subtração" << endl;
    cout << "\t[3] Multiplicação" << endl;
    cout << "\t[4] Divisão\n" << endl;

    cout << "Digite qual sera a operação vencedora: " << endl;

    cin >> threadVencedora;
    if (threadVencedora != 1 && threadVencedora != 2 && threadVencedora != 3 && threadVencedora != 4){
        cout << "\nOPÇÃO INVALIDA, escolha novamente!" << endl;
    }

  }

  omp_set_num_threads(2);
  cout << "" << endl;
  #pragma omp parallel shared(resultadoSom, resultadoSub, resultadoDiv, resultadoMult), private(i)
  {
    #pragma omp sections
    {
      #pragma omp section
      {
        startSom = omp_get_wtime();
        for(i = 0; i < numDeCalculos_sum; i++){
          resultadoSom += variavelA + variavelB;
        }
        endSom = omp_get_wtime();
        cout << "Soma:: " << (endSom - startSom)*1000 << "ms" << endl;
        tempoSom = endSom - startSom;
      }
      #pragma omp section
      {
        startSub = omp_get_wtime();
        for(i = 0; i < numDeCalculos_sub; i++){
          resultadoSub = variavelA - variavelB;
        }
        endSub = omp_get_wtime();
        cout << "Subtração:: " << (endSub - startSub)*1000 << "ms" << endl;
        tempoSub = endSub - startSub;
      }
      #pragma omp section
      {
        startDiv = omp_get_wtime();
        for(i = 0; i < numDeCalculos_div; i++){
          resultadoDiv = variavelA / variavelB;
        }
        endDiv = omp_get_wtime();
        cout << "Divisão:: " << (endDiv - startDiv)*1000 << "ms" << endl;
        tempoDiv = endDiv - startDiv;
      }
      #pragma omp section
      {
        startMult = omp_get_wtime();
        for(i = 0; i < numDeCalculos_mult; i++){
          resultadoMult = variavelA * variavelB;
        }
        endMult = omp_get_wtime();

        cout << "Mutiplicação:: " << (endMult - startMult)*1000 << "ms" << endl;
        tempoMult = endMult - startMult;
      }
    }
  }

  cout << "" << endl;
  if(tempoSom < tempoSub && tempoSom < tempoMult && tempoSub < tempoDiv) {
    if (threadVencedora == 1) {
      cout << "A thread de soma foi a vencedora! Parabéns!" << endl;
    } else {
      cout << "A thread de soma foi a vencedora! Voce apostou errado!" << endl;
    }
  } else if (tempoSub < tempoSom && tempoSub < tempoMult && tempoSub < tempoDiv) {
    if (threadVencedora == 2) {
      cout << "A thread de subtração foi a vencedora! Parabéns!" << endl;
    } else {
      cout << "A thread de subtração foi a vencedora! Voce apostou errado!" << endl;
    }
  } else if (tempoMult < tempoSom && tempoMult < tempoSub && tempoMult < tempoDiv) {
    if (threadVencedora == 3) {
      cout << "A thread de multiplicação foi a vencedora! Parabéns!" << endl;
    } else {
      cout << "A thread de multiplicação foi a vencedora! Voce apostou errado!" << endl;
    }
  } else if (tempoDiv < tempoSom && tempoDiv < tempoSub && tempoDiv < tempoMult) {
    if (threadVencedora == 4) {
      cout << "A thread de divisão foi a vencedora! Parabéns!" << endl;
    } else {
      cout << "A thread de divisão foi a vencedora! Voce apostou errado!" << endl;
    }
  } else {
    cout << "Nao houve thread vencedora!" << endl;
  }
}
