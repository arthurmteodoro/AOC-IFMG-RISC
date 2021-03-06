/*======================================================================================*/
/*                   TRABALHO DE AOC - SIMULADOR DE PROCESSADOR RISC                    */
/* AUTOR: ARTHUR ALEXSANDER MARTINS TEODORO - 0022427                  DATA: 03/11/2016 */
/*======================================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*======================================================================================*/
/*                                   ESTRUTURAS CRIADAS                                 */
/*======================================================================================*/
struct operadores3
{
  unsigned int rc:8;
  unsigned int rb:8;
  unsigned int ra:8;
  unsigned int opcode:8;
};

typedef struct operadores3 *Operadores3;

struct operadores2
{
  unsigned int rc:8;
  unsigned int const16:16;
  unsigned int opcode:8;
};

typedef struct operadores2 *Operadores2;

struct operadores
{
  unsigned int endereco:24;
  unsigned int opcode:8;
};

typedef struct operadores *Operadores;

struct regTemp
{
  int valor;
  int destino;
};

#define HALT 0xffffffff

enum op_codes{NOP,ADD,SUB,ZEROS,XOR,OR,NOT,AND,ASL,ASR,LSL,LSR,PASSA,LCH = 14,LCL,LOAD,STORE,JAL = 32,JR,
              BEQ,BNE,J,MULT = 64,DIV, MOD, ADDI, SUBI, MULTI, DIVI, LOADD, STORED}opcode;

/*======================================================================================*/
/*                                   VARIAVEIS GLOBAIS                                  */
/*======================================================================================*/
int* memoria;
int* registradores;
int PC, IR, neg, zero, carry, overflow;
struct regTemp RegTemp;

/*======================================================================================*/
/*                                   PROTOTIPOS DE FUNCAO                               */
/*======================================================================================*/
int iniciaProcessador(const char* arquivo);
void itob(int valor, char* string, int quantBits);
void verificaFlags(Operadores3 op3);

/*======================================================================================*/
/*                                        FUNCAO MAIN                                   */
/*======================================================================================*/
int main(int argc, char const *argv[])
{
  /*inicia a memória e o registrador*/
  registradores = (int*) malloc(4*33);
  memoria = (int*) malloc(4*65536);

  if(argc < 2)
  {
    fprintf(stderr, "ERRO - ARQUIVO DE ENTRADA NAO EXISTENTE\n");
    return 1;
  }

  char valor[33];

  /*inicia o processador e torna pc a posicao inicial de leitura*/
  PC = iniciaProcessador(argv[1]);
  printf("SIMULACAO INICIADA\n");
  printf("PC inicial %d\n", PC);

  /*busca instrucao*/
  IR = memoria[PC];
  itob(IR, valor, 32);
  printf("IR = %s\n", valor);
  PC++;
  printf("PC = %d\n", PC);

  /*executa enquanto nao for HALT(todos os bits 1)*/
  while(IR != HALT)
  {

    /*Decofica a instrucao e executa*/
    Operadores3 op3 = (Operadores3) &IR;
    Operadores2 op2 = (Operadores2) &IR;
    Operadores op = (Operadores) &IR;
    printf("\n");
    switch(op3->opcode)
    {

      case NOP:
        printf("NOP\n");
        break;

      case ADD:
        printf("ADD\n");
        RegTemp.valor = registradores[op3->ra] + registradores[op3->rb];
        RegTemp.destino = op3->rc;
        verificaFlags(op3);
        break;

      case SUB:
        printf("SUB\n");
        RegTemp.valor = registradores[op3->ra] - registradores[op3->rb];
        RegTemp.destino = op3->rc;
        verificaFlags(op3);
        break;

      case ZEROS:
        printf("ZEROS\n");
        RegTemp.valor = 0;
        RegTemp.destino = op3->rc;
        /*nao e necessario verificar flags uma vez que ja sao fixas*/
        neg = 0;
        zero = 1;
        carry = 0;
        overflow = 0;
        break;

      case XOR:
        printf("XOR\n");
        RegTemp.valor = registradores[op3->ra] ^ registradores[op3->rb];
        RegTemp.destino = op3->rc;
        /*verifica as flags mas carry e overflow sao fixas*/
        verificaFlags(NULL);
        carry = 0;
        overflow = 0;
        break;

      case OR:
        printf("OR\n");
        RegTemp.valor = registradores[op3->ra] | registradores[op3->rb];
        RegTemp.destino = op3->rc; 
        /*verifica as flags mas carry e overflow sao fixas*/
        verificaFlags(NULL);
        carry = 0;
        overflow = 0;
        break;

      case NOT:
        printf("NOT\n");
        RegTemp.valor = ~registradores[op3->ra];
        RegTemp.destino = op3->rc;
        /*verifica as flags mas carry e overflow sao fixas*/
        verificaFlags(NULL);
        carry = 0;
        overflow = 0;
        break;

      case AND:
        printf("AND\n");
        RegTemp.valor = registradores[op3->ra] & registradores[op3->rb];
        RegTemp.destino = op3->rc; 
        /*verifica as flags mas carry e overflow sao fixas*/
        verificaFlags(NULL);
        carry = 0;
        overflow = 0;
        break;

      case ASL:
        printf("ASL\n");
        RegTemp.valor = registradores[op3->ra] << registradores[op3->rb];
        RegTemp.destino = op3->rc;
        /*verifica as flags mas carry e overflow sao fixas*/
        verificaFlags(NULL);
        carry = 0;
        overflow = 0;
        break; 

      case ASR:
        printf("ASR\n");
        RegTemp.valor = registradores[op3->ra] >> registradores[op3->rb];
        RegTemp.destino = op3->rc;
        /*verifica as flags mas carry e overflow sao fixas*/
        verificaFlags(NULL);
        carry = 0;
        overflow = 0;
        break;  

      case LSL:
        printf("LSL\n");
        RegTemp.valor = (unsigned int) registradores[op3->ra] << (unsigned int) registradores[op3->rb];
        RegTemp.destino = op3->rc;
        /*verifica as flags mas carry e overflow sao fixas*/
        verificaFlags(NULL);
        carry = 0;
        overflow = 0;
        break;

      case LSR:
        printf("LSR\n");
        RegTemp.valor = (unsigned int) registradores[op3->ra] >> (unsigned int) registradores[op3->rb];
        RegTemp.destino = op3->rc;
        /*verifica as flags mas carry e overflow sao fixas*/
        verificaFlags(NULL);
        carry = 0;
        overflow = 0;
        break;  

      case PASSA:
        printf("PASSA\n");
        RegTemp.valor = registradores[op3->ra];
        RegTemp.destino = op3->rc; 
        /*verifica as flags mas carry e overflow sao fixas*/
        verificaFlags(NULL);
        carry = 0;
        overflow = 0;
        break; 

      case LCH:
        printf("LCH\n");
        RegTemp.valor = registradores[op2->rc];
        RegTemp.valor = RegTemp.valor & 0x0000ffff;
        RegTemp.valor = RegTemp.valor | (((op2->const16 << 16) & 0xffff0000) & 0xffff0000);
        RegTemp.destino = op2->rc;
        /*como nao usa flags zera todas*/
        neg = 0;
        zero = 0;
        carry = 0;
        overflow = 0;
        break;

      case LCL:
        printf("LCL\n");
        RegTemp.valor = registradores[op2->rc];
        RegTemp.valor = RegTemp.valor & 0xffff0000;
        RegTemp.valor = RegTemp.valor | (op2->const16 & 0x0000ffff);
        RegTemp.destino = op2->rc;
        /*como nao usa flags zera todas*/
        neg = 0;
        zero = 0;
        carry = 0;
        overflow = 0;
        break;

      case LOAD:
        printf("LOAD\n");
        RegTemp.valor = memoria[registradores[op3->ra]];
        RegTemp.destino = op3->rc;
        /*como nao usa flags zera todas*/
        neg = 0;
        zero = 0;
        carry = 0;
        overflow = 0;
        break;

      case STORE:
        printf("STORE\n");
        memoria[registradores[op3->rc]] = registradores[op3->ra];
        printf("Alteracao ocorrida:\n");
        printf("MEMORY[%d] = %d\n", registradores[op3->rc], registradores[op3->ra]);
        printf("\n");
        /*como nao usa flags zera todas*/
        neg = 0;
        zero = 0;
        carry = 0;
        overflow = 0;
        break;

      case JAL:
        printf("JAL\n");
        RegTemp.valor = PC;
        RegTemp.destino = 32;
        PC = op->endereco;
        printf("Alteracao ocorrida:\n");
        printf("PC = %d\n", PC);
        /*como nao usa flags zera todas*/
        neg = 0;
        zero = 0;
        carry = 0;
        overflow = 0;
        break;

      case JR:
        printf("JR\n");
        PC = registradores[32];
        printf("Alteracao ocorrida:\n");
        printf("PC = %d\n", PC);
        printf("\n");
        /*como nao usa flags zera todas*/
        neg = 0;
        zero = 0;
        carry = 0;
        overflow = 0;
        break;

      case BEQ:
        printf("BEQ\n");
        if(registradores[op3->ra] == registradores[op3->rb])
        {
          PC = op3->rc;
          printf("Alteracao ocorrida:\n");
          printf("PC = %d\n", PC);
          printf("\n");
          /*como nao usa flags zera todas*/
          neg = 0;
          zero = 0;
          carry = 0;
          overflow = 0;
        }
        else
        {
          printf("BEQ nao tomado\n");
        }
        break;

      case BNE:
        printf("BNE\n");
        if(registradores[op3->ra] != registradores[op3->rb])
        {
          PC = op3->rc;
          printf("Alteracao ocorrida:\n");
          printf("PC = %d\n", PC);
          printf("\n");
          /*como nao usa flags zera todas*/
          neg = 0;
          zero = 0;
          carry = 0;
          overflow = 0;
        }
        else
        {
          printf("BNE nao tomado\n");
        }
        break;

      case J:
        printf("J\n");
        PC = op->endereco;
        printf("Alteracao ocorrida:\n");
        printf("PC = %d\n", PC);
        printf("\n");
        /*como nao usa flags zera todas*/
        neg = 0;
        zero = 0;
        carry = 0;
        overflow = 0;
        break;

      case MULT:
        printf("MULT\n");
        RegTemp.valor = registradores[op3->ra] * registradores[op3->rb];
        RegTemp.destino = op3->rc;
        verificaFlags(op3);
        break;

      case DIV:
        printf("DIV\n");
        if(registradores[op3->rb] == 0)
        {
          fprintf(stderr, "ERRO - DIVISAO POR 0\n");
          return 1;
        }
        RegTemp.valor = registradores[op3->ra] / registradores[op3->rb];
        RegTemp.destino = op3->rc;
        verificaFlags(op3);
        break;

      case MOD:
        printf("MOD\n");
        RegTemp.valor = registradores[op3->ra] % registradores[op3->rb];
        RegTemp.destino = op3->rc;
        verificaFlags(NULL);
        carry = 0;
        overflow = 0;
        break;

      case ADDI:
        printf("ADDI\n");
        RegTemp.valor = registradores[op3->ra] + op3->rb;
        RegTemp.destino = op3->rc;
        verificaFlags(op3);
        break;

      case SUBI:
        printf("SUBI\n");
        RegTemp.valor = registradores[op3->ra] - op3->rb;
        RegTemp.destino = op3->rc;
        verificaFlags(op3);
        break;

      case MULTI:
        printf("MULTI\n");
        RegTemp.valor = registradores[op3->ra] * op3->rb;
        RegTemp.destino = op3->rc;
        verificaFlags(op3);
        break;

      case DIVI:
        printf("DIVI\n");
        if(op3->rb == 0)
        {
          fprintf(stderr, "ERRO - DIVISAO POR 0\n");
          return 1;
        }
        RegTemp.valor = registradores[op3->ra] / op3->rb;
        RegTemp.destino = op3->rc;
        verificaFlags(op3);
        break;

      case LOADD:
        printf("LOADD\n");
        RegTemp.valor = memoria[op2->const16];
        RegTemp.destino = op2->rc;
        /*como nao usa flags zera todas*/
        neg = 0;
        zero = 0;
        carry = 0;
        overflow = 0;
        break;

      case STORED:
        printf("STORED\n");
        memoria[op2->const16] = registradores[op3->rc];
        printf("Alteracao ocorrida:\n");
        printf("MEMORY[%d] = %d\n", op2->const16, registradores[op3->rc]);
        printf("\n");
        /*como nao usa flags zera todas*/
        neg = 0;
        zero = 0;
        carry = 0;
        overflow = 0;
        break;

      default:
        fprintf(stderr, "ERRO - INSTRUCAO NAO EXISTENTE\n");
        exit(1);
        break;
   }

    /*Escreve o valor nos registradores*/
    if((op3->opcode != STORE) && (op3->opcode != JR) && (op3->opcode != BEQ) && (op3->opcode != BNE) && (op3->opcode != J)
    && (op3->opcode != STORED))
    {
      registradores[RegTemp.destino] = RegTemp.valor;
      printf("Alteracao ocorrida:\n");
      printf("R%d = %d\n", RegTemp.destino, RegTemp.valor);
      printf("\n");
    }
    printf("Flags:\n");
    printf("Overflow: %d\n", overflow);
    printf("Carry: %d\n", carry);
    printf("Neg: %d\n", neg);
    printf("Zero: %d\n\n", zero);

    /*Busca a proxima isntrucao*/
    IR = memoria[PC];
    itob(IR, valor, 32);
    printf("IR = %s\n", valor);
    PC++;
    printf("PC = %d\n", PC);
  }
  printf("HALT encontrado: simulação terminada\n");

  free(memoria);
  free(registradores);

  return 0;
}

/*======================================================================================*/
/*INICIA PROCESSADOR - FUNCAO QUE CARRAGA CODIGO PARA MEMORIA E INICIA OS REGISTRADORES */
/*IN: ARQUIVO COM CODIGO                     OUT: POSICAO INICIAL DO PROGRAMA NA MEMORIA*/
/*======================================================================================*/
int iniciaProcessador(const char* arquivo)
{
  /*Coloca o valor 0 no registrador r0*/
  registradores[0] = 0;

  /*abre o arquivo e verifica se existe*/
  FILE *arq = fopen(arquivo, "rt");
  if(arq == NULL)
  {
    fprintf(stderr, "ERRO - ARQUIVO NAO EXISTENTE\n");
    exit(1);
  }

  char linha[34];

  /*le a primeira linha e verifica se é address*/
  int posicaoInicial = 0;
  fgets(linha, 34, arq);
  char* token = strtok(linha, " ");
  if(!strcmp(token, "address"))
  {
    char* token = strtok(NULL, " ");
    posicaoInicial = strtol(token, NULL, 2);
    if(posicaoInicial < 0 || posicaoInicial >= 65535)
    {
      fprintf(stderr, "ERRO - DIRETIVA ADDRESS INVALIDA\n");
      exit(1);
    }
  }
  else
  {
    fclose(arq);
    arq = fopen(arquivo, "rt");
  }

  /*carrega os dados da memoria*/
  int posicaoMemoria = posicaoInicial;
  while(fgets(linha, 34, arq) != NULL)
  {
    memoria[posicaoMemoria] = strtol(linha, NULL, 2);
    posicaoMemoria = (posicaoMemoria+1)%65536;
    if(posicaoMemoria == posicaoInicial)
    {
      fprintf(stderr, "ERRO - QUANTIDADE DA DADOS MAIOR QUE A MEMORIA\n");
      exit(1);
    }
  }

  fclose(arq);
  return posicaoInicial;
}

/*======================================================================================*/
/*ITOB - FUNCAO QUE TRANSFORMA INTEIRO EM BINARIO                                       */
/*IN: VALOR A CONVERTER, VETOR DESTINO E QUANTIDADE DE BITS                    OUT: VOID*/
/*======================================================================================*/
void itob(int valor, char* string, int quantBits)
{
  int i;//contador
  int j = 0;//indice do vetor resultado
  int r;//deslocamento

  for(i = quantBits-1; i >= 0; i--)
  {
    r = valor >> i;
    if(r & 1)
      string[j] = '1';
    else
      string[j] = '0';
    j++;
  }
  string[j] = '\0';
}

/*======================================================================================*/
/*VERIFICA FLAGS - FUNCAO QUE VERIFICA AS FLAGS DO REGISTRADOR TEMPORARIO               */
/*IN: VOID                                                                     OUT: VOID*/
/*======================================================================================*/
void verificaFlags(Operadores3 op3)
{

  /*caso o valor for negativo*/
  if(RegTemp.valor < 0)
    neg = 1;

  /*verifica se o valor eh 0*/
  if(RegTemp.valor == 0)
    zero = 1;

  /*verifica se existe carry e overflow caso op3 nao for nulo*/
  if(op3 != NULL)
  {

    /*verifica overflow*/
    overflow = 0;
    /*verifica o overflow que pode aconter em somas*/
    if(op3->opcode == ADD || op3->opcode == MULT || op3->opcode == ADDI || op3->opcode == MULTI)
    {
      if((registradores[op3->ra] >= 0) && (registradores[op3->rb] >= 0) && (RegTemp.valor < 0))
        overflow = 1;
      else if((registradores[op3->ra] < 0) && (registradores[op3->rb] < 0) && (RegTemp.valor >= 0))
             overflow = 1;
    }

    /*verifica o overflow que pode acontecer em subtracao*/
    if(op3->opcode == SUB || op3->opcode == DIV || op3->opcode == SUBI || op3->opcode == DIVI)
    {
      if((registradores[op3->ra] >= 0) && (registradores[op3->rb] < 0) && (RegTemp.valor < 0))
        overflow = 1;
      else if((registradores[op3->ra] < 0) && (registradores[op3->rb] >= 0) && (RegTemp.valor >= 0))
             overflow = 1;
    }

    /*verifica carry*/
    carry = 0;

    if(registradores[op3->ra] < 0 && registradores[op3->rb] < 0)
      carry = 1;
    else if(registradores[op3->ra] < 0 && registradores[op3->rb] < 1 && RegTemp.valor < 0)
      carry = 1;
    else if(registradores[op3->ra] < 1 && registradores[op3->rb] < 0 && RegTemp.valor < 0)
      carry = 1;
  }

}