#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <algorithm>    // std::random_shuffle
#include <vector> 
#include <stdio.h>
#include <stdlib.h>
#include<cmath>
#include "biblioteca.h"
//FINAL



using namespace std;


int main(int argc, char** argv) 
{
	ifstream txtFile;
	txtFile.open("inst/Aracaju200n.car");
	if(!txtFile.is_open())
	{
		cout << "Não foi possível abrir o arquivo " << endl;
	}

	int nCarros, tamanhoMatriz; // numero de carros e tamanho da matriz custo
	
	//--BLOCO RESPONSAVEL POR PEGAR TAMANHO DA MATRIZ E NUMERO DE CARROS
	string leituraLinha; //faz a leitura de cada linha
	for(int nLinhas = 0; nLinhas < 27; nLinhas++)
	{
			txtFile >> leituraLinha;
			if(nLinhas == 17)
			{
				txtFile >> tamanhoMatriz;
			}
			
			if(nLinhas == 19)
			{
				txtFile >> nCarros;
			}
	}
	//___________________________________________________________________
	
	//---------Custo de cada carro para se deslocar de uma cidade a outra
	int ***custoCar  = (int***)malloc(nCarros*sizeof(int**));
	for(int k = 0; k < nCarros; k++)
	{
		custoCar[k] = (int**)malloc(tamanhoMatriz*sizeof(int*));
		for(int i = 0; i < tamanhoMatriz; i++)
		{
			custoCar[k][i] = (int*)malloc(tamanhoMatriz*sizeof(int));
		}
	}
	//___________________________________________________________________
	
	
	//Valor do aluguel de cada carro, ao se entregar o carro em outra didade
	int ***aluguelCar  = (int***)malloc(nCarros*sizeof(int**));
	for(int k = 0; k < nCarros; k++)
	{
		aluguelCar[k] = (int**)malloc(tamanhoMatriz*sizeof(int*));
		for(int i = 0; i < tamanhoMatriz; i++)
		{
			aluguelCar[k][i] = (int*)malloc(tamanhoMatriz*sizeof(int));
		}
	}
	//___________________________________________________________________
	
	//----------------PREENCHE A MATRIZ CUSTO DOS CARROS----------------
	for(int k = 0; k < nCarros; k++)
	{
		int c;
		txtFile >> c;
		for(int i = 0; i < tamanhoMatriz; i++)
	    {
			for(int j = 0; j < tamanhoMatriz; j++)
			{
				txtFile >> custoCar[k][i][j];
			}
		}			
	}
	txtFile >> leituraLinha;	
	//____________________________________________________________________
	
	
	//-----------PREENCHE A MATRIZ DOS ALUGUEIS DE CADA CARRO-------------
	for(int k = 0; k < nCarros; k++)
	{
		int c;
		txtFile >> c;
		for(int i = 0; i < tamanhoMatriz; i++)
	    {
			for(int j = 0; j < tamanhoMatriz; j++)
			{
				txtFile >> aluguelCar[k][i][j];
			}
		}			
	}	
	//____________________________________________________________________
	txtFile.close();
//________________________________________________________________________________________________________________________________________________________

//OS CODIGOS ACIMA FORAM RESPONSÁVEIS POR CARREGAR OS DADOS PROVENIENTES DE UM ARQUIVO
//--------------------------------------------------------------------------------------------------------------------------------------------------------


//------------BLOCO RESPONSÁVEL POR REALIZAR A BUSCA INICIAL----------------

//-----------------------Matriz soluTemp------------------------------------------
//Essa matriz representa um vetor que guarda uma solução temporária com 2 linhas, 
//e tamanhoMatriz colunas, onde a primeira linha são as cidades de chegada e a segunda 
//linha representa o carro usado nesse trajeto

	//uma primeira solução inicial
	int **soluTemp = (int**)malloc(2*sizeof(int*));
	for(int i = 0; i < 2; i++)
		soluTemp[i] = (int*)malloc(tamanhoMatriz*sizeof(int));

	
	//uma segunda solução inicial
	int **soluTemp1 = (int**)malloc(2*sizeof(int*));
	for(int i = 0; i < 2; i++)
		soluTemp1[i] = (int*)malloc(tamanhoMatriz*sizeof(int));
		
		
	//Essa matriz será usada apenas na função VND que tem a mesma função da matriz soluTemp
	int **solT = (int**)malloc(2*sizeof(int*));
	for(int i = 0; i < 2; i++)
		solT[i] = (int*)malloc(tamanhoMatriz*sizeof(int));	
		
		
	//Cópia de soluTemp inicial, para ser usado na construção de soluTemp1
	int **soluTemp0 = (int**)malloc(2*sizeof(int*));
	for(int i = 0; i < 2; i++)
		soluTemp0[i] = (int*)malloc(tamanhoMatriz*sizeof(int));
	//____________________________________________________	
		
	//Essa função visa construir um percurso de menor valor, apenas levando em conta
	//o custo do carro, e não considerando a restrição que devolvendo um carro, não
	// será possível usalo novamente	
	//Será preenchido soluTemp e sua cópia soluTemp0
	funcPreencheSoluTem(custoCar, soluTemp, tamanhoMatriz, nCarros);
	funcPreencheSoluTem(custoCar, soluTemp0, tamanhoMatriz, nCarros);
	
	//funcImprimeSoluTemp(soluTemp, tamanhoMatriz);
	
	funcConstSolucao(soluTemp0, soluTemp1, tamanhoMatriz, nCarros);//Usa a soluTemp0 para construir de maneira aleatória soluTemp1
	
	int *percurso = (int*)malloc((tamanhoMatriz - 1)*sizeof(int)); //solucao contendo apenas a roda das cidades
	
	int **carros = (int**)malloc(2*sizeof(int*)); // a primeira linha representa a ordem dos carros escolhidos 
	for(int i = 0; i < 2; i++)                    //a segunda linha o número de vezes que o carro foi usado
		carros[i] = (int*)malloc((nCarros)*sizeof(int));

	
	funcPreenchePercursoEcar(carros, soluTemp, percurso, tamanhoMatriz, nCarros);
	
	//Por causa das restrições de não poder pegar novamente um carro já usado,  foi necessário agrupar os blocos com carros iguais
	funcAgrupaSoluTemp(soluTemp, carros, percurso, tamanhoMatriz, nCarros);
	//funcImprimeSoluTemp(soluTemp, tamanhoMatriz);
	
	//permuta os blocos e até inverte a matriz caso necessário, para encontar um tempo melhor
	funcSolTempFinalizado(custoCar, aluguelCar, soluTemp, tamanhoMatriz, nCarros);
	//funcMelhorSolucaoInicial(custoCar, aluguelCar, soluTemp, soluTemp1, soluTemp0, tamanhoMatriz, nCarros, 1000);
	funcRepreenchePercursoECar(carros, soluTemp, percurso, tamanhoMatriz, nCarros);
	funcBucaLocalTrocaOrdemDePercurso(custoCar, aluguelCar, carros, percurso, nCarros, tamanhoMatriz);
	//_________________________________________________________________________________________________________________________________
	
	
	//----------------------Algoritmos para minimizar custos---------------------------------------------------------------------------
	//é esse
	
	//funcBuscaTrocaCarros(custoCar, aluguelCar, carros, percurso, nCarros);
	//cout << " agora vale = " << funcCusto(custoCar, aluguelCar, carros, percurso, nCarros) << endl;
	//funcBuscaTrocaOrdemDosCarros(custoCar, aluguelCar, carros, percurso, nCarros);
	//cout << " no fim  vale = " << funcCusto(custoCar, aluguelCar, carros, percurso, nCarros) << endl;
	//funcRVND(custoCar, aluguelCar, carros, percurso, nCarros, tamanhoMatriz);
	
	//cout << " agora vale = " << funcCusto(custoCar, aluguelCar, carros, percurso, nCarros) << endl;
	//funcVNS(custoCar, aluguelCar, carros, soluTemp, solT, percurso, nCarros, tamanhoMatriz, 120);
	//funcILS(custoCar, aluguelCar, carros, solT, soluTemp, soluTemp1, soluTemp0, percurso, tamanhoMatriz, nCarros, 12);
	//funcImprimeSoluTemp(soluTemp, tamanhoMatriz);
	//cout << "MEDIA  = " << funcMedia(custoCar, aluguelCar, soluTemp, tamanhoMatriz, 30) << endl;
	
	
	//CONTROLE DAS VARIÁVEIS TORNANDO O ALGORITMO AUTOMATIZADO
	//
	
	
	//cout <<  funcILS(custoCar, aluguelCar, carros, solT, soluTemp, soluTemp1, soluTemp0, percurso, tamanhoMatriz, nCarros) << endl;
	cout << funcMedia(custoCar, aluguelCar, carros, solT, soluTemp, soluTemp1, soluTemp0, percurso, tamanhoMatriz, nCarros, 30) << endl;
	
	//FILE *arq = fopen("resultadosT/BrasilRS32n.txt", "w");


/*	fprintf(arq,"\n");
 	for(int i = 0; i < 2; i++)
	{
		fprintf(arq,"[ ");
		for(int j = 0; j < tamanhoMatriz; j++)
		{
			fprintf(arq, "%d ",soluTemp[i][j] );
		}
		fprintf(arq,"] \n");
	}
	fprintf(arq,"\n");
	fprintf(arq, "No fim vale : %d \n",funcCustoSoluTemp(custoCar, aluguelCar, soluTemp, tamanhoMatriz));*/
	return 0;
	
}
