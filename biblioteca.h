#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#include <iostream>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <algorithm>    // std::random_shuffle
#include <vector> 
#include<cmath>




//-----------------PROT�TIPOS DE FUN��ES --------------------------------------------------

int funcCusto(int ***custoCar, int ***aluguelCar, int **carros, int *percurso, int nCarros); //Calcula custos usando as matrizes carros e percurso
void funcPreenchePercursoEcar(int **carros, int **soluTemp, int *percurso, int tamanhoMatriz, int nCarros);//preenche as matrizes percurso e carros

void funcPreencheSoluTem(int ***custoCar, int **soluTemp, int tamanhoMatriz, int nCarros);//Preenche a matriz soluTemp
void funcAgrupaSoluTemp(int **soluTemp, int **carros, int *percurso, int tamanhoMatriz, int nCarros);//Agrupa a matriz soluTemp em rela��o aos carros
void funcImprimeSoluTemp(int **soluTemp, int tamanhoMatriz);
int funcCustoSoluTemp(int ***custoCar, int ***aluguelCar, int **soluTemp, int tamanhoMatriz);
void funcMelhoraSoluTemp(int ***custoCar, int ***aluguelCar, int **soluTemp, int tamanhoMatriz, int nCarros);
int funcSolTempFinalizado(int ***custoCar, int ***aluguelCar, int **soluTemp, int tamanhoMatriz, int nCarros);
void funcRepreenchePercursoECar(int **carros, int **soluTemp, int *percurso, int tamanhoMatriz, int nCarros);

void funcPermutaSoluTemp(int c1, int c2, int **solVenc, int ***carross);
void funcPermutaSoluTemp(int c1, int c2, int c3, int **solVenc, int ***carross);
void funcPermutaSoluTemp(int c1, int c2, int c3, int c4, int **solVenc, int ***carross);
void funcPermutaSoluTemp(int c1, int c2, int c3, int c4, int c5, int **solVenc, int ***carross);

void funcInverteSoluTemp(int **soluTemp, int tamanhoMatriz);

//Troca apenas uma cidade com outra de cada vez, e analisa o resultado buscando custo m�nimo
int funcBucaLocalTrocaOrdemDePercurso(int ***custoCar, int ***aluguelCar, int **carros, int *percurso, int nCarros, int tamanhoMatriz);

//Troca apenas um carro, ou seja se usa 10 vezes o carro A e 6 o carro B, fazendo a troca podemos ter
// usar 9 vezes o carro A e 7 vezes o carro B, ou vice e versa
int funcBuscaTrocaCarros(int ***custoCar, int ***aluguelCar, int **carros, int *percurso, int nCarros);

//Troca a ordem de uso dos carros
int funcBuscaTrocaOrdemDosCarros(int ***custoCar, int ***aluguelCar, int **carros, int *percurso, int nCarros);

//Escolhe um elemento aleat�rio usando a vizinhan�a troca de percurso
void funcElementoArbitrarioTrocaPercurso(int *percurso, int tamanhoMatriz);

//Escolhe um elemento aleat�rio usando a vizinhan�a que troca apenas um carro com outro
void funcElementoArbitrarioTrocaCarros(int **carros, int nCarros);

//Escolhe um elementos aleat�rio usando a vizinhan�a que troca a ordem dos carros
void funcElementoArbitrarioTrocaOrdemCarros(int **carros, int nCarros);

//VND com vizinhan�as escolhidas de maneira aleat�ria
void funcRVND(int ***custoCar, int ***aluguelCar, int **carros, int *percurso, int nCarros, int tamanhoMatriz);

//Troca de maneira aleat�ria o percurso
void funcTrocaOrdemDePercurso(int *percurso, int tamanhoMatriz, int numeroDeTroca);

// A fun��o que realiza uma busca usando VNS-----------------
int funcVNS(int ***custoCar, int ***aluguelCar, int **carros, int **soluTemp, int **solT, int *percurso, int nCarros, int tamanhoMatriz, int nInterSemMelhora);

//para gerar vetores aleatorios
int myrandom (int i);

//ordena vetores
void quick(int* vetor, int inicio, int fim);

//ordena um vetor em ordem aleat�ria
void funcOrdenaAleatorioVetor(int* vA, int tamanho);

//EXTREMAMENTE IMPORTANTE ESSA FUN��O PARA O ALGOR�TIMO: Usa a solu��o soluTemp1, para construir uma boa solu��o soluTemp2
void funcConstSolucao(int **soluTemp, int **soluTemp1, int tamanhoMatriz, int nCarros);

//Busca uma soli��o inicial melhor
void funcMelhorSolucaoInicial(int ***custoCar, int ***aluguelCar, int **soluTemp, int **soluTemp1, int **soluTemp0, int tamanhoMatriz, int nCarros, int nDeInteracoes);


//--------------------------A FUN��O PRINCIPAL DESTE TRABALHO O ILS---------------------------------------
int funcILS(int ***custoCar, int ***aluguelCar, int ** carros, int ** solT, int **soluTemp, int **soluTemp1, int **soluTemp0, int *percurso, int tamanhoMatriz, int nCarros);

//imprime m�dia dos resultados finais obtidos
int funcMedia(int ***custoCar, int ***aluguelCar, int ** carros, int ** solT, int **soluTemp, int **soluTemp1, int **soluTemp0, int *percurso, int tamanhoMatriz, int nCarros, int numeroInter);

#endif
