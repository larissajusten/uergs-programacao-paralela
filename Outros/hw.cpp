#include <stdio.h>
#include <mpi.h>

void chama_nois()
{
    printf("t1");
}


int main(int argc, char **argv)
{
    printf("\n");
    int size, rank;
    MPI_Init(&argc, &argv);

    //MPI_Comm_size(MPI_COMM_WORLD, &size);
    //MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    //printf("SIZE = %d RANK = %d\n",size,rank);
    chama_nois();

    MPI_Finalize();   
    return(0);
}