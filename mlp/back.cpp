/**********************************************************************
 * Algoritmo de treinamento back-propagation para redes multicamadas
**********************************************************************/

/************************* BIBLIOTECAS *******************************/
#include <iostream.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>

/************************* DEFINICOES ********************************/
#define MAXCAM          5              // Número máximo de camadas
#define MAXNEU          100            // Número máximo de neurônios
#define MAXPES          100            // Número máximo de pesos
#define MAXLIN          100            // Número máximo de linhas
#define MAXCOL          20             // Número máximo de colunas

#define NUMLIN          26             // Número de Linhas da Matriz de Entrada
#define NUMCOLENT       8              // Número de Colunas da Matriz de Entrada
#define NUMCOLSAI       5              // Número de Colunas de Saida
#define NUMITE          1              // Numero de Iteracoes
#define ESC             27

#define MI              0.6
#define TOLERANCIA	0.00001       // Número de erros consecutivos

double BETA = MI;                     // Fator de ajuste das correcoes

/************************* CLASSES **********************************/
/*********************** CLASSE NEURONIO ****************************/
class Neuronio
{
  private:
    int Numero_Pesos;                   // Número de pesos do neurônio
    double W[MAXPES];                   // Vetor de pesos

  public:
    void Inicializar_Neuronio(int Numero_Pesos);     // Inicia os valores dos pesos
    void Ajustar_Peso(double Entrada, double Erro, int Indice_Peso);   // Ajusta os valores dos pesos
    double Somatorio(double Entrada[]);             // Retorna os pesos e quantos são
    double Erro_Peso(double Erro,int Indice_Peso);
};

/*********************************************************
  Inicializa o Número de Pesos e também os valores
  iniciais dos pesos
*********************************************************/
void Neuronio :: Inicializar_Neuronio(int Numero_Pesos)
{
  int i;

  this -> Numero_Pesos = Numero_Pesos;

  randomize();

  for (i=0;i < Numero_Pesos;i++)
    W[i] = (double)(random(11)/10.0)*(random(3) - 1);
}

/*********************************************************
  Multilica o Erro da saída de um neurônio por um Peso de
  Indice_Peso
*********************************************************/
double Neuronio :: Erro_Peso(double Erro, int Indice_Peso)
{
  return(Erro*W[Indice_Peso]);
}

/*********************************************************
  Dada uma entrada, retorna a saída do neurônio
  multiplicando-a pelos pesos
*********************************************************/
double Neuronio :: Somatorio(double Entrada[])
{
  int i;
  double Som = 0;

  for (i=0;i < Numero_Pesos;i++)
    Som += Entrada[i]*W[i];

  return Som;
}

/*********************************************************
  Dado o erro da camada da frente, a saída do neurônio,
  e Indice do Peso, calcula-se o novo peso
*********************************************************/
void Neuronio :: Ajustar_Peso(double Entrada, double Erro, int Indice_Peso)
{
  W[Indice_Peso] += BETA*Erro*Entrada;
}

/*********************** CLASSE CAMADA ****************************/
class Camada
{
  private:
    int Numero_Neuronios;               // Número de neurônios na camada
    int Numero_Pesos;
    double Saida[MAXNEU];               // Saída dos neurônios da camada
    Neuronio N[MAXNEU];                 // Neurônios da camada

  public:
    void Inicializar_Camada(int Numero_Neuronios, int Numero_Pesos);  // Atribui o número de neurônios
    void Treinar_Neuronios(double Entrada[]);                           // Treina os neurônios com uma entrada
    void Funcao_Ativacao();                                           // Joga saída linear na funcao de ativacao
    void Retornar_Saida(double Linha[]);                              // Retorna a saída dos neurônios
    void Ajustar_Pesos_Neuronios(double Erros[], double Entrada[]);
    void Calcular_Erro_Camada_Saida(double Erros[], double Y[]);
    void Calcular_Erro_Camada(double Erros[]);
    double Somatorio_Erro(double Erros[]);
    void Calcular_Erro_Final(double Erros[],double Y[]);
};

/*********************************************************
  Inicializa o Numero de Neuronios e o Numero de Pesos
  e invoca a inicialização dos neurônios
*********************************************************/
void Camada :: Inicializar_Camada(int Numero_Neuronios, int Numero_Pesos)
{
  int i;

  this -> Numero_Neuronios = Numero_Neuronios;
  this -> Numero_Pesos = Numero_Pesos;

  for(i=0;i < Numero_Neuronios;i++)
    N[i].Inicializar_Neuronio(Numero_Pesos);
}

/*********************************************************
  Calcula os erros da camada de saída com base na saída
  desejada Y, retornando os erros
*********************************************************/
void Camada :: Calcular_Erro_Final(double Erros[], double Y[])
{
  int i;

  for(i=0;i < Numero_Neuronios;i++)
    Erros[i] = (Y[i] - Saida[i]);
}

/*********************************************************
  Dispara o somatório de um neurônio para uma certa entrada
  armazenando a sua saída
*********************************************************/
void Camada :: Treinar_Neuronios(double Entrada[])
{
  int i;

  for(i=0;i < Numero_Neuronios;i++)
    Saida[i] = N[i].Somatorio(Entrada);
}

/*********************************************************
  Calcula os erros da camada de saída com base na saída
  desejada Y, retornando os erros
*********************************************************/
void Camada :: Calcular_Erro_Camada_Saida(double Erros[], double Y[])
{
  int i;

  for(i=0;i < Numero_Neuronios;i++)
    Erros[i] = (Y[i] - Saida[i]) * Saida[i] * (1-Saida[i]);
}

/*********************************************************
  Calcula os erros da camada escondida com base no erro
  da camada da frente, retornando os erros
*********************************************************/
void Camada :: Calcular_Erro_Camada(double Erros[])
{
  int i;

  for(i=0;i < Numero_Neuronios;i++)
    Erros[i] = Saida[i] * (1-Saida[i]) * Erros[i];
}

/*********************************************************
  Ajusta os pesos dos neurônios da camada de acordo com os
  erros da camada da frente, e retorna o somárório de erros
  da própria camada
*********************************************************/
void Camada :: Ajustar_Pesos_Neuronios(double Erros[], double Entrada[])
{
  int i,j;
  double Temp, Erro_Aux[MAXNEU];

  /* Cálculo do Somatório que será usado para o cálculo do erro
     da camada anterior */

  for(i=1;i < Numero_Pesos;i++)
  {
    Temp  = 0;
    for(j=0;j < Numero_Neuronios;j++)
    {
      Temp += N[j].Erro_Peso(Erros[j],i);
    }
    Erro_Aux[i-1] = Temp;
  }

  /* Ajusta os pesos de cada neurônio  de acordo com o erro
     da camada da frente e a saída da própria camada */

  for(i=0;i < Numero_Neuronios;i++)
    for(j=0;j < Numero_Pesos;j++)
      N[i].Ajustar_Peso(Entrada[j],Erros[i],j);

  /* Atribui o vetor de erros calculado, para o vetor erro
     que será retornado */

  for(i=0;i < (Numero_Pesos -1);i++)
    Erros[i] = Erro_Aux[i];
}

/*********************************************************
  Atualiza a saída da camada passando-a por uma função
  de ativação
*********************************************************/
void Camada :: Funcao_Ativacao()
{
  int i;

  for(i=0;i < Numero_Neuronios;i++)
    Saida[i] = 1/(1+ exp(-Saida[i]));
}

/*********************************************************
  Retorna a Saída da Camada
*********************************************************/
void Camada :: Retornar_Saida(double Linha[])
{
  int i;

  Linha[0] = 1;
  for(i=1;i <= Numero_Neuronios;i++)
    Linha[i] = Saida[i-1];
}

/*********************** CLASSE REDE ******************************/
class Rede
{
  private:
    int Numero_Camadas;                 // Número de camadas da rede
    int Numero_Linhas;                  // Número de linhas de entrada
    int Numero_Colunas_Entrada;         // Número de colunas de entrada
    int Numero_Colunas_Saida;           // Número de colunas da saída

    Camada C[MAXCAM];                   // Camadas da rede
    double X[MAXLIN][MAXCOL];           // Matriz de entrada da rede
    double Y[MAXLIN][MAXCOL];           // Matriz de saída da rede

  public:
    void Inicializar_Rede(int,int,int,int, int Numero_Neuronio_Camada[]);  // Inicializa os valores das variáveis
    void Treinar();                          // Treina toda a rede
    void Calcular_Resultado(double Entrada[], double Saida[]);
};

/*********************************************************
  Inicializa todas as variáveis da rede, inclusive a
  leitura das entradas e saídas da rede
*********************************************************/
void Rede :: Inicializar_Rede(int Numero_Camadas, int Numero_Linhas,
             int Numero_Colunas_Entrada, int Numero_Colunas_Saida, int Numero_Neuronio_Camada[])
{
  int i,j;
  FILE *Entrada, *Saida;

  this -> Numero_Camadas = Numero_Camadas;
  this -> Numero_Linhas = Numero_Linhas;
  this -> Numero_Colunas_Entrada = Numero_Colunas_Entrada;
  this -> Numero_Colunas_Saida = Numero_Colunas_Saida;

  Entrada = fopen("X.txt","rb");
  Saida = fopen("Y.txt","rb");

  for(i=0;i < Numero_Linhas;i++)
    for(j=0;j < Numero_Colunas_Entrada;j++)
      fread(&X[i][j],sizeof(double), 1, Entrada);

  for(i=0;i < Numero_Linhas;i++)
    for(j=0;j < Numero_Colunas_Saida;j++)
      fread(&Y[i][j],sizeof(double), 1, Saida);

  fclose(Entrada);
  fclose(Saida);

  C[0].Inicializar_Camada(Numero_Neuronio_Camada[0],Numero_Colunas_Entrada);

  for(i=1;i < Numero_Camadas;i++)
     C[i].Inicializar_Camada(Numero_Neuronio_Camada[i], (Numero_Neuronio_Camada[i-1]+1));
}

/*********************************************************
  Calcula a resposta da rede para uma certa entrada,
  retornando a saída
*********************************************************/
void Rede :: Calcular_Resultado(double Entrada[], double Saida[])
{
  int i,j;

  for (i=0; i < Numero_Camadas; i++)
  {
    C[i].Treinar_Neuronios(Entrada);
    C[i].Funcao_Ativacao();
    C[i].Retornar_Saida(Saida);

    for (j=0; j < MAXNEU; j++)
      Entrada[j] = Saida[j];
  }
}

/*********************************************************
  Algoritmmo de Treinamento Back Propagation
*********************************************************/
void Rede :: Treinar()
{
  int i, j, Linha_Escolhida, Iteracoes, Camada_Saida, Marcados[MAXLIN];
  double Vetor_Saida[MAXNEU], Erros[MAXNEU], Somatorio_Erro, Maior;
  long Contador, Dinamico;
  char Sair;

  /* Inicializando variáveis */
  for(i=0; i < MAXLIN; i++)
    Marcados[i] = 0;

  Dinamico = 0;
  Sair = 0;
  Contador = 0;
  Maior = 1;
  Iteracoes = 0;
  Camada_Saida = Numero_Camadas - 1;

  do
  {
    Linha_Escolhida = random(NUMLIN);

    j = 0;
    while (Marcados[Linha_Escolhida] == 1)
    {
      Linha_Escolhida++;
      j++;

      if(Linha_Escolhida == NUMLIN)
        Linha_Escolhida = 0;

      if(j == NUMLIN)
        for(i=0; i < MAXLIN; i++)
          Marcados[i] = 0;
    }

    Marcados[Linha_Escolhida] = 1;
    Contador++;

    // FEED-FORWARD
    C[0].Treinar_Neuronios(X[Linha_Escolhida]);
    C[0].Funcao_Ativacao();
    C[0].Retornar_Saida(Vetor_Saida);

    for (i=1; i < Numero_Camadas; i++)
    {
      C[i].Treinar_Neuronios(Vetor_Saida);
      C[i].Funcao_Ativacao();
      C[i].Retornar_Saida(Vetor_Saida);
    }

    // BACK-PROPAGATION
    /* Ajustando pesos da camada de saída */
    C[Camada_Saida].Calcular_Erro_Camada_Saida(Erros,Y[Linha_Escolhida]);
    C[Camada_Saida -1].Retornar_Saida(Vetor_Saida);
    C[Camada_Saida].Ajustar_Pesos_Neuronios(Erros,Vetor_Saida);

    /* Ajustando pesos das camadas intermediárias */
    for (i=Camada_Saida - 1; i > 0; i--)
    {
      C[i].Calcular_Erro_Camada(Erros);
      C[i-1].Retornar_Saida(Vetor_Saida);
      C[i].Ajustar_Pesos_Neuronios(Erros,X[Linha_Escolhida]);
    }

    /* Ajustando pesos da primeira camada */
    C[0].Calcular_Erro_Camada(Erros);
    C[0].Ajustar_Pesos_Neuronios(Erros,X[Linha_Escolhida]);

    /* Calculando o erro global */
    C[Camada_Saida].Calcular_Erro_Final(Erros,Y[Linha_Escolhida]);

    Somatorio_Erro = 0;
    for(i=0; i < Numero_Colunas_Saida;i++)
      Somatorio_Erro += pow(Erros[i],2);

    Somatorio_Erro /= 2;

    /* Verificando condições */
    if( Somatorio_Erro < Maior)
    {
      Dinamico = 0;
      gotoxy(1,10);
      cout << "\n\nErro = " << Somatorio_Erro << "   ";
      Maior = Somatorio_Erro;
    }
    else
      Dinamico++;

    if(Somatorio_Erro <= TOLERANCIA)
      Iteracoes++;
    else
      Iteracoes = 0;

    /* Beta dinâmico */
    if(Dinamico == 200000)
    {
      Dinamico = 0;
      BETA += (random(6)/10.0)*(random(3) - 1);
    }

    if(Dinamico == 50000)
      BETA = MI;

    /* Exibição na tela */
    if(Contador%10000 == 0)
    {
      gotoxy(1,10);
      cout << "\nIteracoes = " << Contador;
      cout << "\n\nBeta = " << BETA << "  ";
    }

    /* Opção de escape */
//    if(Contador%10000000 == 0)
//      Sair = (char)getch();

  } while( Iteracoes < NUMITE && Sair != ESC);
}

/****************** PROGRAMA PRINCIPAL *****************************/
void main(void)
{
  int Numero_Camadas;                 // Número de camadas da rede
  int Numero_Linhas;                  // Número de linhas de entrada
  int Numero_Colunas_Entrada;         // Número de colunas de entrada
  int Numero_Colunas_Saida;           // Número de colunas da saída
  int Numero_Neuronio_Camada[MAXCAM];
  int i;

  double Entrada[MAXNEU];
  double Saida[MAXNEU];

  char Continua = 'r';
  Rede R;

  Numero_Linhas = NUMLIN;
  Numero_Colunas_Entrada = NUMCOLENT;
  Numero_Colunas_Saida = NUMCOLSAI;

  while(Continua != 'n')
  {
    clrscr();

    if(Continua == 'r')
    {
      cout << "\n\nDigite o numero de camadas: ";
      cin >> Numero_Camadas;

      for(i=0; i < Numero_Camadas;i++)
      {
        cout << "\n\nDigite o numero de neuronios da camada " << i << " : ";
        cin >> Numero_Neuronio_Camada[i];
      }

      R.Inicializar_Rede(
			  Numero_Camadas,
			  Numero_Linhas,
			  Numero_Colunas_Entrada,
			  Numero_Colunas_Saida,
			  Numero_Neuronio_Camada
	  );
      R.Treinar();
    }

    cout << "\n\nDigite as entradas da rede:\n";

    for(i=0; i < Numero_Colunas_Entrada;i++)
    {
      cout << "\nEntrada " << i << " : ";
      cin >> Entrada[i];
    }

    R.Calcular_Resultado(Entrada,Saida);

    for(i=1; i <= Numero_Colunas_Saida;i++)
    {
      cout << "\nSaida " << i << " : " << Saida[i];
    }

    cout << "\n\nContinua ? (s/n/r)";
    cin >> Continua;
  }
}



