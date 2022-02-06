#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESS 20
#define QUANTUM 5
#define BUFFER_SIZE 255

const char separacao[2] = ";";

typedef int FLAG_TYPE;
#define SEM_CONSUMO 0 
#define CONSUMINDO 1     


typedef struct
{
    char nome[5];
    int duracao;
    FLAG_TYPE FLAG;

} Process;

Process listaProcesso[MAX_PROCESS];


void reorganizar_Lista(Process *listaProcesso, int finalProcesso, int size)
{
    Process aux = listaProcesso[finalProcesso];
    for (int i = finalProcesso; i < size - 1; i++)
    {
        listaProcesso[i] = listaProcesso[i + 1];
    }
    listaProcesso[size - 1] = aux;
}

int main(int argc, char const *argv[])
{
    
    FILE *file;

    Process p;
    char buffer[BUFFER_SIZE];
    char *valor;
    int c = 0;

    file = fopen("./listaProcesso.txt", "r");

    while (fgets(buffer, BUFFER_SIZE, file))
    {
        valor = strtok(buffer, separacao);
        strcpy(p.nome, valor);
        valor = strtok(NULL, separacao);
        p.duracao = atoi(valor);
        p.FLAG = SEM_CONSUMO;
        listaProcesso[c] = p;
        c++;
    }

    fclose(file);

    int semProcesso = c;
    int processoInicial = 0;

    Process *processoAtual;

    
    while (semProcesso > 0)
    {
        processoAtual = &listaProcesso[processoInicial];
        printf("%s processo atual\n", processoAtual->nome);
        if (processoAtual->FLAG == SEM_CONSUMO)
        {
            if (processoAtual->duracao <= QUANTUM)
            {
                printf("%s Encerado!!\n\n", processoAtual->nome);
                semProcesso--;
                processoInicial = (processoInicial + 1) % c;
                processoAtual->FLAG = CONSUMINDO;
                processoAtual->duracao = 0;
            }
            else
            {
                
                printf("Processo não completado %s, inserindo um novo processo...\n", processoAtual->nome);
                processoAtual->duracao -= QUANTUM;
                reorganizar_Lista(listaProcesso, processoInicial, c);
            }
        }
        else
        {
            
            processoInicial = (processoInicial + 1) % c;
        }
    }
    return 0;
}