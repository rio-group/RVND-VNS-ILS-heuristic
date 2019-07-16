#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <algorithm>    // std::random_shuffle
#include <vector>
#include<cmath>
#include "biblioteca.h"

//CONTROLE DO TEMPO DO ALGORITMO
int TEMPO_INICIAL;
float TEMPO_LIMITE = 98.0;

using namespace std;


//-------------Fun��o Custo-----------------------------------------------------------
int funcCusto(int ***custoCar, int ***aluguelCar, int **carros, int *percurso, int nCarros)
 {
 	int aluguel = 0;
	int soma = 0;
	int guardaSoma = 0;
	int custo = custoCar[carros[0][0]][0][percurso[0]];
	for(int i = 0; i < nCarros; i++)
	{
		soma = soma + carros[1][i];
		if(i == 0)
		{
			aluguel = aluguel + aluguelCar[carros[0][0]][0][percurso[soma - 1]];

			for(int j = (guardaSoma) + 1; j < carros[1][i]; j++)
			{
				custo = custo + custoCar[carros[0][i]][ percurso[j-1]][percurso[j]];
			}
		}

		else if(i == nCarros - 1)
		{
			aluguel = aluguel + aluguelCar[carros[0][i]][percurso[guardaSoma]][0];
			for(int j = (guardaSoma) + 1; j < carros[1][i] + guardaSoma + 1; j++)
			{
				custo = custo + custoCar[carros[0][i]][percurso[j-1]][percurso[j]];
				if(j == (carros[1][i] + guardaSoma ))
				{
					custo = custo + custoCar[carros[0][i]][percurso[j]][0];
				}
			}
		}

		else
		{
			aluguel = aluguel + aluguelCar[carros[0][i]][percurso[guardaSoma]][percurso[soma - 1]];

			for(int j = (guardaSoma) + 1; j < carros[1][i] + guardaSoma + 1; j++)
			{
				custo = custo + custoCar[carros[0][i] ][ percurso[j-1] ][percurso[j]];
			}
		}

		guardaSoma = soma -1;
	}
	int custoTotal = custo + aluguel;

	return custo + aluguel;
 }
 //____________________________________________________________________________________________________



 //---------------------Preenche a matriz soluTemp-----------------------------------------------
 void funcPreencheSoluTem(int ***custoCar, int **soluTemp, int tamanhoMatriz, int nCarros)
 {
 	int minimo = 10000;
	int ondeEsta = 0;
	int temp = 0;
	int carroEscolhido = -1;
	int contaCidade = 0;
	int *cidadeVisitada = (int*)malloc((tamanhoMatriz)*sizeof(int));//este vetor tem valores 0 se a cidade n�o  foi visitada e 1 se foi
	cidadeVisitada[0] = 1;
	for(int j = 1; j < tamanhoMatriz; j++)
	{
		cidadeVisitada[j] = 0;
	}
	while(contaCidade < tamanhoMatriz)
	{
		for(int k = 0; k < nCarros; k++)
		{
			for(int j = 0; j < tamanhoMatriz; j++)
			{
				if( ((custoCar[k][ondeEsta][j] ) < minimo ) && (ondeEsta != j) &&  (cidadeVisitada[j] != 1) )
				{
					minimo = custoCar[k][ondeEsta][j];
					temp = j;
					carroEscolhido = k;
				}
			}

		}
		minimo = 10000;
		ondeEsta = temp;
		cidadeVisitada[ondeEsta] = 1;//aqui diz que a cidade ondeESta foi visitada
		soluTemp[0][contaCidade] = ondeEsta;
		soluTemp[1][contaCidade] = carroEscolhido;
		contaCidade++;
		if(contaCidade == (tamanhoMatriz - 1) )
		{
			cidadeVisitada[0] = 0;//desconsiderando que a cidade de partida 0 foi visitada para que no final do percurso volte a cidade 0
		}
	}
	free(cidadeVisitada);
 }

 //-----------------Preenche as matrizes percurso e carros----------------------------------------
 void funcPreenchePercursoEcar(int **carros, int **soluTemp, int *percurso, int tamanhoMatriz, int nCarros)
 {
 	int t = 0;
	int k = 0;
	int numCar = 0;
	int cont = 0;
	bool entra = true;
	int *car = (int*)malloc(tamanhoMatriz*sizeof(int));
	int *carRepetido = (int*)malloc(nCarros*sizeof(int));
	for(int i = 0; i < nCarros;i++)
	{
		carRepetido[i] = 0;
	}
	int anterior = -1;
	while((numCar < nCarros) && (t < tamanhoMatriz-1))
	{
		int ncar = t;
		for(int i = cont; i < (tamanhoMatriz-1); i++)
		{

			if(soluTemp[1][k] == soluTemp[1][i])
			{
				car[t] = soluTemp[1][i];
				percurso[t] = soluTemp[0][i];
				t++;
			}

			else if(entra)
			{
				if(carRepetido[soluTemp[1][k]] == 0)
					carRepetido[soluTemp[1][k]] = 1;

				cont = i;
				entra = false;
				numCar++;
			}

		}
		while (  (carRepetido[soluTemp[1][cont]] == 1) && (cont <(tamanhoMatriz-2)) )
			cont++;
		k = cont;

		ncar = t - ncar;
		if(anterior ==(numCar-1))
		{
			numCar++;
		}
		carros[0][numCar-1] = car[t-1];
		carros[1][numCar-1] = ncar;
		anterior = numCar-1;
		entra = true;
	}
	free(car);
	free(carRepetido);
 }

 //------------Agrupa soluTemp---------------------
 void funcAgrupaSoluTemp(int **soluTemp, int **carros, int *percurso, int tamanhoMatriz, int nCarros)
 {
 	int k = carros[0][0];
	for(int i = 0; i < tamanhoMatriz; i++)
	{
		soluTemp[0][i] = percurso[i];
		if(i == (tamanhoMatriz - 1))
		{
			soluTemp[0][i] = 0;
			soluTemp[1][i] = carros[0][nCarros-1];
		}
	}
		int s = 0;
	for(int i = 0; i < nCarros; i++)
	{

		for(int j = 0; j < carros[1][i]; j++)
		{
			soluTemp[1][j+s] = carros[0][i];
		}
		s = s + carros[1][i];
	}
 }

 //----Imprime a matriz soluTemp---------------------------------------------
 void funcImprimeSoluTemp(int **soluTemp, int tamanhoMatriz)
 {
 	cout <<"\n";
 	for(int i = 0; i < 2; i++)
	{
		cout <<"[ ";
		for(int j = 0; j < tamanhoMatriz; j++)
		{
			cout << soluTemp[i][j] << " ";
		}
		cout << " ]" << endl;
	}
	cout << "\n";
 }

//----------Calcula o custo usando a matriz soluTemp-----------------
 int funcCustoSoluTemp(int ***custoCar, int ***aluguelCar, int **soluTemp, int tamanhoMatriz)
 {
 	int aluguel = 0;
	int k = soluTemp[1][0];
	int partida = 0;
	int chegada = 0;
	int custo = 0;
	int partidaC = 0;
	for(int i = 0; i < tamanhoMatriz; i++)
	{
		if(k != soluTemp[1][i])
		{
			chegada = soluTemp[0][i-1];
			aluguel = aluguel + aluguelCar[k][partida][chegada];
			k = soluTemp[1][i];
			partida = chegada;

		}
		if( i == (tamanhoMatriz-1))
		{
			aluguel = aluguel + aluguelCar[k][partida][0];
		}



		if( i ==  0)
		{
			custo = custo + custoCar[soluTemp[1][0]][0][soluTemp[0][i]];
		}
		else if( i != 0)
		{
			custo = custo + custoCar[soluTemp[1][i]][ soluTemp[0][i-1] ][soluTemp[0][i]];
		}

	}

	return custo + aluguel;
 }

 //---------------Fun��o que busca melhorar a matriz da soluTemp----------------------------------
 void funcMelhoraSoluTemp(int ***custoCar, int ***aluguelCar, int **soluTemp, int tamanhoMatriz, int nCarros)
 {
 	int ***carross  = (int***)malloc(nCarros*sizeof(int**));
	for(int k = 0; k < nCarros; k++)
	{
		carross[k] = (int**)malloc(2*sizeof(int*));
		for(int i = 0; i < 2; i++)
		{
			carross[k][i] = (int*)malloc((tamanhoMatriz-1)*sizeof(int));
		}
	}
 	for(int i = 0; i < nCarros; i++)
 	{
 		for(int j = 0; j < 2; j++)
 		{
 			for(int k = 0; k < tamanhoMatriz-1; k++)
 			{
 				carross[i][j][k] = -1;
			}
		}
	}

 	int **solVenc = (int**)malloc(2*sizeof(int*));//busca solu��o vencedora
	for(int i = 0; i < 2; i++)
		solVenc[i] = (int*)malloc(tamanhoMatriz*sizeof(int));
	solVenc[0][tamanhoMatriz-1] = 0;
 	for(int k = 0; k < nCarros; k++)
 	{
 		int c = 0;
 		for(int i = 0; i < tamanhoMatriz-1; i++ )
 		{
 			if(k == soluTemp[1][i])
 			{
 				carross[k][0][c] = soluTemp[0][i];
 				carross[k][1][c] = soluTemp[1][i];
 				c++;
			}
		}
	}
	//___________________________________________________

	//An�lise de cada caso de acordo com numero de carros
	//----------Com dois carros--------------------------------------------------
	int minimo = funcCustoSoluTemp(custoCar, aluguelCar, soluTemp, tamanhoMatriz);
 	if(nCarros == 2)
 	{
 		for(int i = 0; i < nCarros; i++)
 		{
			int j = 0;
			while((i-j)==0) j++;
			while(j < nCarros)
			{
				funcPermutaSoluTemp(i, j, solVenc, carross);
				if( funcCustoSoluTemp(custoCar, aluguelCar, solVenc, tamanhoMatriz) < minimo)
				{
					for(int p = 0; p < tamanhoMatriz; p++)
					{
						soluTemp[0][p] = solVenc[0][p];
						soluTemp[1][p] = solVenc[1][p];
					}
					minimo = funcCustoSoluTemp(custoCar, aluguelCar, soluTemp, tamanhoMatriz);
				}
				j++;
				while((i-j) == 0)j++;
			}
		}
	}
	//________________________________________________________________________________

	//-----------Com tr�s carros---------------------------------------------

 	else if(nCarros == 3)
 	{
 		for(int i = 0; i < nCarros; i++)
 		{
			int j = 0;
			while((i-j)==0) j++;
			while(j < nCarros)
			{
				int k = 0;
				while((i-k)*(j-k) == 0) k++;
				while(k < nCarros)
				{
					funcPermutaSoluTemp(i, j, k, solVenc, carross);
					if( funcCustoSoluTemp(custoCar, aluguelCar, solVenc, tamanhoMatriz) < minimo)
					{
						for(int p = 0; p < tamanhoMatriz; p++)
						{
							soluTemp[0][p] = solVenc[0][p];
							soluTemp[1][p] = solVenc[1][p];
						}
						minimo = funcCustoSoluTemp(custoCar, aluguelCar, soluTemp, tamanhoMatriz);
					}
					k++;
					while((i-k)*(j-k) == 0) k++;
				}
				j++;
				while((i-j) == 0)j++;
			}
		}
	}
	//________________________________________________________________________________

	//--------------Com quatro carros------------------------------------------------

 	else if(nCarros == 4)
 	{
 		for(int i = 0; i < nCarros; i++)
 		{
			int j = 0;
			while((i-j)==0) j++;
			while(j < nCarros)
			{
				int k = 0;
				while((i-k)*(j-k) == 0) k++;
				while(k < nCarros)
				{
					int w = 0;
					while((i-w)*(j-w)*(k-w) == 0 ) w++;
					while(w < nCarros)
					{
						funcPermutaSoluTemp(i, j, k, w, solVenc, carross);
						if( funcCustoSoluTemp(custoCar, aluguelCar, solVenc, tamanhoMatriz) < minimo)
						{
							for(int p = 0; p < tamanhoMatriz; p++)
							{
								soluTemp[0][p] = solVenc[0][p];
								soluTemp[1][p] = solVenc[1][p];
							}
							minimo = funcCustoSoluTemp(custoCar, aluguelCar, soluTemp, tamanhoMatriz);
						}
						w++;
						while((i-w)*(j-w)*(k-w) == 0 ) w++;
					}
					k++;
					while((i-k)*(j-k) == 0) k++;
				}
				j++;
				while((i-j) == 0)j++;
			}
		}
	}
	//_________________________________________________________________________________

	//-------------------Com cinco carros--------------------------------------------

 	else if(nCarros == 5)
 	{
 		for(int i = 0; i < nCarros; i++)
 		{
			int j = 0;
			while((i-j)==0) j++;
			while(j < nCarros)
			{
				int k = 0;
				while((i-k)*(j-k) == 0) k++;
				while(k < nCarros)
				{
					int w = 0;
					while((i-w)*(j-w)*(k-w) == 0 ) w++;
					while(w < nCarros)
					{
						int t = 0;
						while((i-t)*(j-t)*(k-t)*(w-t) == 0 ) t++;
						while(t < nCarros)
						{
							funcPermutaSoluTemp(i, j, k, w, t, solVenc, carross);
							if( funcCustoSoluTemp(custoCar, aluguelCar, solVenc, tamanhoMatriz) < minimo)
							{
								for(int p = 0; p < tamanhoMatriz; p++)
								{
									soluTemp[0][p] = solVenc[0][p];
									soluTemp[1][p] = solVenc[1][p];
								}
								minimo = funcCustoSoluTemp(custoCar, aluguelCar, soluTemp, tamanhoMatriz);
							}
							t++;
							while((i-t)*(j-t)*(k-t)*(w-t) == 0 ) t++;
						}
						w++;
						while((i-w)*(j-w)*(k-w) == 0 ) w++;
					}
					k++;
					while((i-k)*(j-k) == 0) k++;
				}
				j++;
				while((i-j) == 0)j++;
			}
		}

	}
	//--------------Libera memoria-------------------------
	for(int k = 0; k < nCarros; k++)
	{
		for(int i = 0; i < 2; i++)
			free(carross[k][i]);
	}
	for(int k = 0; k < nCarros; k++)
	{
		free(carross[k]);
	}
	free(carross);


	for(int i = 0; i < 2; i++)
		free(solVenc[i]);
	free(solVenc);
	//_______________________________________________________

 }



 //---------Fun��es que permutam------------------------------------------------------
 //------------Fun��o que permuta dois blocos----------------------------
 void funcPermutaSoluTemp(int c1, int c2, int **solVenc, int ***carross)
 {
 	int c = 0;
	int s = 0;

	while(carross[c1][0][c] != -1)
	{
		solVenc[0][s] = carross[c1][0][c];
		solVenc[1][s] = carross[c1][1][c];
		c++;
		s++;
	}
	c = 0;
	while(carross[c2][0][c] != -1)
	{
		solVenc[0][s] = carross[c2][0][c];
		solVenc[1][s] = carross[c2][1][c];
		c++;
		s++;
	}
	solVenc[1][s] = carross[c2][1][c-1];

 }
 //____________________________________________________________________________
 //--------------Fun��o que permuta 3 blocos-----------------------------------
 void funcPermutaSoluTemp(int c1, int c2, int c3, int **solVenc, int ***carross)
{
 	int c = 0;
	int s = 0;

	while(carross[c1][0][c] != -1)
	{
		solVenc[0][s] = carross[c1][0][c];
		solVenc[1][s] = carross[c1][1][c];
		c++;
		s++;
	}
	c = 0;
	while(carross[c2][0][c] != -1)
	{
		solVenc[0][s] = carross[c2][0][c];
		solVenc[1][s] = carross[c2][1][c];
		c++;
		s++;
	}
	c = 0;
	while(carross[c3][0][c] != -1)
	{
		solVenc[0][s] = carross[c3][0][c];
		solVenc[1][s] = carross[c3][1][c];
		c++;
		s++;
	}
	solVenc[1][s] = carross[c3][1][c-1];

}
//___________________________________________________________
//--------------Fun��o que permuta 4 blocos-----------------
void funcPermutaSoluTemp(int c1, int c2, int c3, int c4, int **solVenc, int ***carross)
{
	int c = 0;
	int s = 0;

	while(carross[c1][0][c] != -1)
	{
		solVenc[0][s] = carross[c1][0][c];
		solVenc[1][s] = carross[c1][1][c];
		c++;
		s++;
	}
	c = 0;
	while(carross[c2][0][c] != -1)
	{
		solVenc[0][s] = carross[c2][0][c];
		solVenc[1][s] = carross[c2][1][c];
		c++;
		s++;
	}
	c = 0;
	while(carross[c3][0][c] != -1)
	{
		solVenc[0][s] = carross[c3][0][c];
		solVenc[1][s] = carross[c3][1][c];
		c++;
		s++;
	}
	c = 0;
	while(carross[c4][0][c] != -1)
	{
		solVenc[0][s] = carross[c4][0][c];
		solVenc[1][s] = carross[c4][1][c];
		c++;
		s++;
	}
	solVenc[1][s] = carross[c4][1][c-1];

}
//________________________________________________________________________________
//------------------Fun��o que permuta 5 blocos----------------------------------
void funcPermutaSoluTemp(int c1, int c2, int c3, int c4, int c5, int **solVenc, int ***carross)
{
	int c = 0;
	int s = 0;

	while(carross[c1][0][c] != -1)
	{
		solVenc[0][s] = carross[c1][0][c];
		solVenc[1][s] = carross[c1][1][c];
		c++;
		s++;
	}
	c = 0;
	while(carross[c2][0][c] != -1)
	{
		solVenc[0][s] = carross[c2][0][c];
		solVenc[1][s] = carross[c2][1][c];
		c++;
		s++;
	}
	c = 0;
	while(carross[c3][0][c] != -1)
	{
		solVenc[0][s] = carross[c3][0][c];
		solVenc[1][s] = carross[c3][1][c];
		c++;
		s++;
	}
	c = 0;
	while(carross[c4][0][c] != -1)
	{
		solVenc[0][s] = carross[c4][0][c];
		solVenc[1][s] = carross[c4][1][c];
		c++;
		s++;
	}
	c = 0;
	while(carross[c5][0][c] != -1)
	{
		solVenc[0][s] = carross[c5][0][c];
		solVenc[1][s] = carross[c5][1][c];
		c++;
		s++;
	}
	solVenc[1][s] = carross[c5][1][c-1];

}
//______________________________________________________________
//----Fun��o para inverter a matriz no formato soluTemp--------
void funcInverteSoluTemp(int **soluTemp, int tamanhoMatriz)
{
	int temp1,temp2;
	int c = (tamanhoMatriz-1);
	for (int i = 0; i < c/2; i++)
	{
		temp1 = soluTemp[0][i];
		soluTemp[0][i] = soluTemp[0][c-i-1];
		soluTemp[0][c-i-1] = temp1;

		temp2 = soluTemp[1][i];
		soluTemp[1][i] = soluTemp[1][c-i-1];
		soluTemp[1][c-i-1] = temp2;
	}
}
//_______________________________________________________________
// ----Fun��o que faza ultima melhora de soluTemp----------------
int funcSolTempFinalizado(int ***custoCar, int ***aluguelCar, int **soluTemp, int tamanhoMatriz, int nCarros)
{
	funcMelhoraSoluTemp(custoCar, aluguelCar, soluTemp, tamanhoMatriz, nCarros);
	int **solVenc = (int**)malloc(2*sizeof(int*));//busca solu��o vencedora
	for(int i = 0; i < 2; i++)
		solVenc[i] = (int*)malloc(tamanhoMatriz*sizeof(int));
	for(int i = 0; i < tamanhoMatriz; i++)
	{
		solVenc[0][i] = soluTemp[0][i];
		solVenc[1][i] = soluTemp[1][i];
	}
	int minimo1 = funcCustoSoluTemp(custoCar, aluguelCar, solVenc, tamanhoMatriz);
	int minimo = minimo1;
	funcInverteSoluTemp(solVenc, tamanhoMatriz);
	funcMelhoraSoluTemp(custoCar, aluguelCar, solVenc, tamanhoMatriz, nCarros);
	int minimo2 = funcCustoSoluTemp(custoCar, aluguelCar, solVenc, tamanhoMatriz);
	if(minimo2 < minimo1)
	{
		funcInverteSoluTemp(soluTemp, tamanhoMatriz);
		funcMelhoraSoluTemp(custoCar, aluguelCar, soluTemp, tamanhoMatriz, nCarros);
		minimo = minimo2;
	}

	//Libera mem�ria----------
	for(int i = 0; i < 2; i++)
		free(solVenc[i]);
	free(solVenc);
	return minimo;
}
void funcRepreenchePercursoECar(int **carros, int **soluTemp, int *percurso, int tamanhoMatriz, int nCarros)
{
	int k = soluTemp[1][0];
	int nCar = 0;
	int indCar = 0;
	for(int i = 0; i < (tamanhoMatriz-1); i++)
	{
		percurso[i] = soluTemp[0][i];
		if(k == soluTemp[1][i])
		{
			nCar++;
			if(i == (tamanhoMatriz-2))
			{
				carros[0][indCar] = k;
				carros[1][indCar] = nCar;
			}
		}
		else
		{
			carros[0][indCar] = k;
			carros[1][indCar] = nCar;
			indCar++;
			nCar = 1;
			k = soluTemp[1][i];
		}
	}
}

//-----------Busca local de acordo com suas vizinhan�as------------------------------------------------------------------------------
//------------Troca apenas uma cidade com outra de cada vez, e analisa o resultado buscando custo m�nimo---------------------
int funcBucaLocalTrocaOrdemDePercurso(int ***custoCar, int ***aluguelCar, int **carros, int *percurso, int nCarros, int tamanhoMatriz)
{
	int minimo = funcCusto(custoCar, aluguelCar, carros, percurso, nCarros);

	for(int i = 0; i < (tamanhoMatriz-1); i++)
	{
		int temp1, temp2;
		for(int j = i+1; j < (tamanhoMatriz-1); j++)
		{
			temp1 = percurso[i];
			temp2 = percurso[j];
			percurso[i] = temp2;
			percurso[j] = temp1;
			if(minimo < funcCusto(custoCar, aluguelCar, carros, percurso, nCarros))
			{
				percurso[i] = temp1;
				percurso[j] = temp2;
			}
		}
	}
	return minimo;
}

//Troca apenas um carro, ou seja se usa 10 vezes o carro A e 6 o carro B, fazendo a troca podemos ter
// usar 9 vezes o carro A e 7 vezes o carro B, ou vice e versa
//Retorna o menor valor
int funcBuscaTrocaCarros(int ***custoCar, int ***aluguelCar, int **carros, int *percurso, int nCarros)
{
	int minimo = funcCusto(custoCar, aluguelCar, carros, percurso, nCarros);

	for(int i = 0; i < nCarros; i++)
	{
		for(int j = i+1; j < nCarros; j++)
		{
			if(carros[1][i] > 1)
			{
				carros[1][i] = carros[1][i] - 1;
				carros[1][j] = carros[1][j] + 1;
				if(minimo < funcCusto(custoCar, aluguelCar, carros, percurso, nCarros))
				{
					carros[1][i] = carros[1][i] + 1;
					carros[1][j] = carros[1][j] - 1;
				}
			}
		}
	}
	return minimo;
}

//-------------------Troca a ordem de uso dos carros----------------------------------------------
//Retorna o menor valor
int funcBuscaTrocaOrdemDosCarros(int ***custoCar, int ***aluguelCar, int **carros, int *percurso, int nCarros)
{
	int minimo = funcCusto(custoCar, aluguelCar, carros, percurso, nCarros);

	for(int i = 0; i < nCarros; i++)
	{
		int temp1, temp2;
		for(int j = i+1; j < nCarros; j++)
		{
			temp1 = carros[0][i];
			temp2 = carros[0][j];
			carros[0][i] = temp2;
			carros[0][j] = temp1;
			if(minimo < funcCusto(custoCar, aluguelCar, carros, percurso, nCarros))
			{
				carros[0][i] = temp1;
				carros[0][j] = temp2;
			}
		}
	}
	return minimo;
}
//________________________________________________________________________________________________________________________


//n�o retorna o menor valor apenar realiza uma perturba��o
void funcElementoArbitrarioTrocaPercurso(int *percurso, int tamanhoMatriz)
{
	bool entra = true;
	while(entra)
	{
		srand((unsigned)time(0));
		int i = rand()%(tamanhoMatriz);
		int j = rand()%(tamanhoMatriz);
		int temp;
		if(i != j)
		{
			temp = percurso[i];
			percurso[i] = percurso[j];
			percurso[j] = temp;
			entra = false;
		}
	}

}

//n�o retorna o menor valor apenas uma perturba��o
void funcElementoArbitrarioTrocaCarros(int **carros, int nCarros)
{
	bool entra = true;
	while(entra)
	{
		srand((unsigned)time(0));
		int i = rand()%(nCarros);
		int j = rand()%(nCarros);
		int temp;
		if(i != j)
		{
			if(carros[1][i] > 1 )
			{
				carros[1][i] = carros[1][i] - 1;
				carros[1][j] = carros[1][j] + 1;
				entra = false;
			}
		}
	}
}

// n�o devolve o m�nimo, apenas realiza uma perturba��o
void funcElementoArbitrarioTrocaOrdemCarros(int **carros, int nCarros)
{
	bool entra = true;
	while(entra)
	{
		srand((unsigned)time(0));
		int i = rand()%(nCarros);
		int j = rand()%(nCarros);
		int temp;
		if(i != j)
		{
			temp = carros[0][i];
			carros[0][i] = carros[0][j];
			carros[0][j] = temp;
			entra = false;
		}
	}

}


void funcRVND(int ***custoCar, int ***aluguelCar, int **carros, int *percurso, int nCarros, int tamanhoMatriz)
{
	//Escolhe uma vizinhan�a de forma aleat�ria
	srand ( unsigned ( time(0) ) );
  	vector<int> v;

 	// conjunto de valores
  	for (int i = 0; i < 3; ++i) v.push_back(i); // 1 2 3

  	// usando built-in random generator:
  	random_shuffle ( v.begin(), v.end() );

  	// usando myrandom:
  	random_shuffle ( v.begin(), v.end(), myrandom);
	//____________________________________
	//Realiza a busca local escolhendo cada vizinhan�a de forma aleat�ria;

	int k = 0;
	while(k < 3)
	{
		if(k == v[0])
		{
			funcBucaLocalTrocaOrdemDePercurso(custoCar, aluguelCar, carros, percurso, nCarros, tamanhoMatriz);
		}
		if(k == v[1])
		{
			funcBuscaTrocaCarros(custoCar, aluguelCar, carros, percurso, nCarros);
		}
		if(k == v[2])
		{
			funcBuscaTrocaOrdemDosCarros(custoCar, aluguelCar, carros, percurso, nCarros);
		}
		k++;
	}
}

//Troca de forma aleat�ria
//n�o retorna o menor, apenas faz uma perturba��o
void funcTrocaOrdemDePercurso(int *percurso, int tamanhoMatriz, int numeroDeTroca)
{
	srand((unsigned)time(0));
	int aleatorio = rand()%(numeroDeTroca-1) + 1;
	int aleatorio1;
	int aleatorio2;
	int temp;
	for(int i = 0; i < aleatorio; i++)
	{
		aleatorio1 = rand()%(tamanhoMatriz-2);
		aleatorio2 = rand()%(tamanhoMatriz-2);
		temp = percurso[aleatorio1];
		percurso[aleatorio1] = percurso[aleatorio2];
		percurso[aleatorio2] = temp;
	}
}

int funcVNS(int ***custoCar, int ***aluguelCar, int **carros, int **soluTemp, int **solT, int *percurso, int nCarros, int tamanhoMatriz, int nInterSemMelhora)
{
	int aceita = 1;
	int controle = 100; //usado apenas quando tamanhoMatriz > 200

	//______________________________________________

	//Inicializa a matriz perT
	int *perT = (int*)malloc((tamanhoMatriz - 1)*sizeof(int));
	for(int i = 0; i < (tamanhoMatriz -1); i++)
		perT[i] = percurso[i];

	//Inicializa a matriz carT
	int **carT = (int**)malloc(2*sizeof(int*));
	for(int i = 0; i < 2; i++)
		carT[i] = (int*)malloc((nCarros)*sizeof(int));
	for(int i = 0; i < nCarros; i++)
	{
		carT[0][i] = carros[0][i];
		carT[1][i] = carros[1][i];
	}

	for(int i = 0; i < tamanhoMatriz; i ++)
	{
		solT[0][i] = soluTemp[0][i];
		solT[1][i] = soluTemp[1][i];
	}

	int inter = 0;
	int minimo ;
	int minimoT;
	while(inter < nInterSemMelhora)
	{
		for(int i = 0; i < (tamanhoMatriz -1); i++)
		perT[i] = percurso[i];

		for(int i = 0; i < nCarros; i++)
		{
			carT[0][i] = carros[0][i];
			carT[1][i] = carros[1][i];
		}

		if(tamanhoMatriz < 200)
		{

			int k = 0;
			while(k < 3)
			{
				if(k == 0)       //if((k == 0)&&(inter <= controle) )
				{
					for(int i = 0; i < 10; i++)
						funcBucaLocalTrocaOrdemDePercurso(custoCar, aluguelCar, carT, perT, nCarros, tamanhoMatriz);

					funcRVND(custoCar, aluguelCar, carT, perT, nCarros, tamanhoMatriz);

					for(int i = 0; i < 6; i++)
						minimoT = funcBuscaTrocaCarros(custoCar, aluguelCar, carT, perT, nCarros);

					funcAgrupaSoluTemp(solT, carT, perT, tamanhoMatriz, nCarros);
					//funcSolTempFinalizado(custoCar, aluguelCar, solT, tamanhoMatriz, nCarros);

					minimo = funcCusto(custoCar, aluguelCar, carros, percurso, nCarros);
					//minimoT = funcCustoSoluTemp(custoCar, aluguelCar, solT, tamanhoMatriz);

					//CONTROLE DO TEMPO
					if(( (clock() - TEMPO_INICIAL)/1000.0) >= TEMPO_LIMITE)
					{
						int variavel = (minimo < minimoT) ? minimo : minimoT;
						return variavel;
					}
					//__________________

					if( minimoT < minimo + aceita)
					{
						if(minimoT < minimo)
						{
							inter = 0; k = 0;
						}

						else
						{
							inter++; k++;
						}
						funcRepreenchePercursoECar(carT, solT, perT, tamanhoMatriz, nCarros);
						for(int i = 0; i < (tamanhoMatriz -1); i++)
							percurso[i] = perT[i];

						for(int i = 0; i < nCarros; i++)
						{
							carros[0][i] = carT[0][i];
							carros[1][i] = carT[1][i];
						}
						funcAgrupaSoluTemp(soluTemp, carT, perT, tamanhoMatriz, nCarros);
						//cout << "minimoT = " << minimoT << "    minimo = " << minimo << endl;
					}
					else
					{
						k++; inter++;
					}
				}

				else if(k == 1 )      // else if((k == 1)&&(inter <= controle) )
				{
					funcElementoArbitrarioTrocaOrdemCarros(carT, nCarros);
					funcElementoArbitrarioTrocaOrdemCarros(carT, nCarros);
					funcRVND(custoCar, aluguelCar, carT, perT, nCarros, tamanhoMatriz);

					funcAgrupaSoluTemp(solT, carT, perT, tamanhoMatriz, nCarros);
					minimoT = funcSolTempFinalizado(custoCar, aluguelCar, solT, tamanhoMatriz, nCarros);

					minimo = funcCusto(custoCar, aluguelCar, carros, percurso, nCarros);
					//minimoT = funcCustoSoluTemp(custoCar, aluguelCar, solT, tamanhoMatriz);

					//CONTROLE DO TEMPO
					if(( (clock() - TEMPO_INICIAL)/1000.0) >= TEMPO_LIMITE)
					{
						int variavel = (minimo < minimoT) ? minimo : minimoT;
						return variavel;
					}
					//__________________

					if( minimoT < minimo + aceita)
					{
						if(minimoT < minimo)
						{
							inter = 0; k = 0;
						}

						else
						{
							inter++; k++;
						}
						funcRepreenchePercursoECar(carT, solT, perT, tamanhoMatriz, nCarros);
						for(int i = 0; i < (tamanhoMatriz -1); i++)
							percurso[i] = perT[i];

						for(int i = 0; i < nCarros; i++)
						{
							carros[0][i] = carT[0][i];
							carros[1][i] = carT[1][i];
						}
						funcAgrupaSoluTemp(soluTemp, carT, perT, tamanhoMatriz, nCarros);
						//cout << "minimoT = " << minimoT << "    minimo = " << minimo << endl;
					}
					else
					{
						k++; inter++;
					}
				}

				else if(k == 2)            //else if( (k == 2)&&(inter <= controle)  )
				{
					funcElementoArbitrarioTrocaCarros(carT,  nCarros);
					funcElementoArbitrarioTrocaCarros(carT,  nCarros);
					funcElementoArbitrarioTrocaCarros(carT,  nCarros);
					funcRVND(custoCar, aluguelCar, carT, perT, nCarros, tamanhoMatriz);
					minimoT = funcBucaLocalTrocaOrdemDePercurso(custoCar, aluguelCar, carT, perT, nCarros, tamanhoMatriz);

					funcAgrupaSoluTemp(solT, carT, perT, tamanhoMatriz, nCarros);
					//funcSolTempFinalizado(custoCar, aluguelCar, solT, tamanhoMatriz, nCarros);

					minimo = funcCusto(custoCar, aluguelCar, carros, percurso, nCarros);
					//minimoT = funcCustoSoluTemp(custoCar, aluguelCar, solT, tamanhoMatriz);

					//CONTROLE DO TEMPO
					if(( (clock() - TEMPO_INICIAL)/1000.0) >= TEMPO_LIMITE)
					{
						int variavel = (minimo < minimoT) ? minimo : minimoT;
						return variavel;
					}
					//__________________

					if( minimoT < minimo + aceita)
					{
						if(minimoT < minimo)
						{
							inter = 0; k = 0;
						}

						else
						{
							inter++; k++;
						}
						funcRepreenchePercursoECar(carT, solT, perT, tamanhoMatriz, nCarros);
						for(int i = 0; i < (tamanhoMatriz -1); i++)
							percurso[i] = perT[i];

						for(int i = 0; i < nCarros; i++)
						{
							carros[0][i] = carT[0][i];
							carros[1][i] = carT[1][i];
						}
						funcAgrupaSoluTemp(soluTemp, carT, perT, tamanhoMatriz, nCarros);
						//cout << "minimoT = " << minimoT << "    minimo = " << minimo << endl;
					}
					else
					{
						k++; inter++;
					}
				}
			}
			//ate aqui
		}

		//para tamanhoMatriz > 200
		else
		{
			//daqui

			int k = 0;


			while(k < 3)
			{
				if((k == 0)&&(inter <= controle) )
				{
					for(int i = 0; i < 6; i++)
					{
						funcBucaLocalTrocaOrdemDePercurso(custoCar, aluguelCar, carT, perT, nCarros, tamanhoMatriz);
					}

					for(int i = 0; i < 6; i++)
					{
							funcBuscaTrocaCarros(custoCar, aluguelCar, carT, perT, nCarros);
					}

					funcRVND(custoCar, aluguelCar, carT, perT, nCarros, tamanhoMatriz);

					funcAgrupaSoluTemp(solT, carT, perT, tamanhoMatriz, nCarros);

					minimo = funcCusto(custoCar, aluguelCar, carros, percurso, nCarros);
					minimoT = funcSolTempFinalizado(custoCar, aluguelCar, solT, tamanhoMatriz, nCarros);
					if( minimoT < minimo + aceita)
					{
						if(minimoT < minimo)
						{
							inter = 0; k = 0;
						}

						else
						{
							inter++; k++;
						}
						funcRepreenchePercursoECar(carT, solT, perT, tamanhoMatriz, nCarros);
						for(int i = 0; i < (tamanhoMatriz -1); i++)
							percurso[i] = perT[i];

						for(int i = 0; i < nCarros; i++)
						{
							carros[0][i] = carT[0][i];
							carros[1][i] = carT[1][i];
						}
						funcAgrupaSoluTemp(soluTemp, carT, perT, tamanhoMatriz, nCarros);
						//cout << "minimoT = " << minimoT << "    minimo = " << minimo << endl;
					}
					else
					{
						k++; inter++;
					}
				}
				else if( (k == 2)&&(inter <= controle)  )
				{
					funcElementoArbitrarioTrocaCarros(carT,  nCarros);
					funcElementoArbitrarioTrocaCarros(carT,  nCarros);
					funcElementoArbitrarioTrocaCarros(carT,  nCarros);
					funcRVND(custoCar, aluguelCar, carT, perT, nCarros, tamanhoMatriz);

					funcAgrupaSoluTemp(solT, carT, perT, tamanhoMatriz, nCarros);

					minimo = funcCusto(custoCar, aluguelCar, carros, percurso, nCarros);
					minimoT = funcSolTempFinalizado(custoCar, aluguelCar, solT, tamanhoMatriz, nCarros);
					if( minimoT < minimo + aceita)
					{
						if(minimoT < minimo)
						{
							inter = 0; k = 0;
						}

						else
						{
							inter++; k++;
						}
						funcRepreenchePercursoECar(carT, solT, perT, tamanhoMatriz, nCarros);
						for(int i = 0; i < (tamanhoMatriz -1); i++)
							percurso[i] = perT[i];

						for(int i = 0; i < nCarros; i++)
						{
							carros[0][i] = carT[0][i];
							carros[1][i] = carT[1][i];
						}
						funcAgrupaSoluTemp(soluTemp, carT, perT, tamanhoMatriz, nCarros);
						//cout << "minimoT = " << minimoT << "    minimo = " << minimo << endl;
					}
					else
					{
						k++; inter++;
					}
				}
				else if((k == 1)&&(inter <= controle) )
				{
					funcElementoArbitrarioTrocaOrdemCarros(carT, nCarros);
					funcElementoArbitrarioTrocaOrdemCarros(carT, nCarros);
					funcRVND(custoCar, aluguelCar, carT, perT, nCarros, tamanhoMatriz);

					funcAgrupaSoluTemp(solT, carT, perT, tamanhoMatriz, nCarros);

					minimo = funcCusto(custoCar, aluguelCar, carros, percurso, nCarros);
					minimoT =funcSolTempFinalizado(custoCar, aluguelCar, solT, tamanhoMatriz, nCarros);
					if( minimoT < minimo + aceita)
					{
						if(minimoT < minimo)
						{
							inter = 0; k = 0;
						}

						else
						{
							inter++; k++;
						}
						funcRepreenchePercursoECar(carT, solT, perT, tamanhoMatriz, nCarros);
						for(int i = 0; i < (tamanhoMatriz -1); i++)
							percurso[i] = perT[i];

						for(int i = 0; i < nCarros; i++)
						{
							carros[0][i] = carT[0][i];
							carros[1][i] = carT[1][i];
						}
						funcAgrupaSoluTemp(soluTemp, carT, perT, tamanhoMatriz, nCarros);
						//cout << "minimoT = " << minimoT << "    minimo = " << minimo << endl;
					}
					else
					{
						k++; inter++;
					}
				}





				else if(  (k == 2) )
				{
					for( int i = 0; i < 6; i++)
					{
						funcBucaLocalTrocaOrdemDePercurso(custoCar, aluguelCar, carT, perT, nCarros, tamanhoMatriz);
					}

					for(int i = 0; i < 6; i++)
					{
							funcBuscaTrocaCarros(custoCar, aluguelCar, carT, perT, nCarros);
					}

					funcRVND(custoCar, aluguelCar, carT, perT, nCarros, tamanhoMatriz);

					funcAgrupaSoluTemp(solT, carT, perT, tamanhoMatriz, nCarros);

					minimo = funcCusto(custoCar, aluguelCar, carros, percurso, nCarros);
					minimoT = funcSolTempFinalizado(custoCar, aluguelCar, solT, tamanhoMatriz, nCarros);
					if( minimoT < minimo + aceita)
					{
						if(minimoT < minimo)
						{
							inter = 0; k = 0;
						}

						else
						{
							inter++; k++;
						}
						funcRepreenchePercursoECar(carT, solT, perT, tamanhoMatriz, nCarros);
						for(int i = 0; i < (tamanhoMatriz -1); i++)
							percurso[i] = perT[i];

						for(int i = 0; i < nCarros; i++)
						{
							carros[0][i] = carT[0][i];
							carros[1][i] = carT[1][i];
						}
						funcAgrupaSoluTemp(soluTemp, carT, perT, tamanhoMatriz, nCarros);
						//cout << "minimoT = " << minimoT << "    minimo = " << minimo << endl;
					}
					else
					{
						k++; inter++;
					}
				}
				else if( (k == 1) )
				{
					funcElementoArbitrarioTrocaCarros(carT,  nCarros);
					funcElementoArbitrarioTrocaCarros(carT,  nCarros);
					funcElementoArbitrarioTrocaCarros(carT,  nCarros);

					funcRVND(custoCar, aluguelCar, carT, perT, nCarros, tamanhoMatriz);

					funcAgrupaSoluTemp(solT, carT, perT, tamanhoMatriz, nCarros);

					minimo = funcCusto(custoCar, aluguelCar, carros, percurso, nCarros);
					minimoT = funcSolTempFinalizado(custoCar, aluguelCar, solT, tamanhoMatriz, nCarros);
					if( minimoT < minimo + aceita)
					{
						if(minimoT < minimo)
						{
							inter = 0; k = 0;
						}

						else
						{
							inter++; k++;
						}
						funcRepreenchePercursoECar(carT, solT, perT, tamanhoMatriz, nCarros);
						for(int i = 0; i < (tamanhoMatriz -1); i++)
							percurso[i] = perT[i];

						for(int i = 0; i < nCarros; i++)
						{
							carros[0][i] = carT[0][i];
							carros[1][i] = carT[1][i];
						}
						funcAgrupaSoluTemp(soluTemp, carT, perT, tamanhoMatriz, nCarros);
						//cout << "minimoT = " << minimoT << "    minimo = " << minimo << endl;

					}
					else
					{
						k++; inter++;
					}
				}
				else if( (k == 0))
				{
					funcElementoArbitrarioTrocaOrdemCarros(carT, nCarros);

					funcElementoArbitrarioTrocaOrdemCarros(carT, nCarros);

					funcRVND(custoCar, aluguelCar, carT, perT, nCarros, tamanhoMatriz);

					funcAgrupaSoluTemp(solT, carT, perT, tamanhoMatriz, nCarros);


					minimo = funcCusto(custoCar, aluguelCar, carros, percurso, nCarros);
					minimoT = funcSolTempFinalizado(custoCar, aluguelCar, solT, tamanhoMatriz, nCarros);
					if( minimoT < minimo + aceita)
					{
						if(minimoT < minimo)
						{
							inter = 0; k = 0;
						}

						else
						{
							inter++; k++;
						}

						funcRepreenchePercursoECar(carT, solT, perT, tamanhoMatriz, nCarros);
						for(int i = 0; i < (tamanhoMatriz -1); i++)
							percurso[i] = perT[i];

						for(int i = 0; i < nCarros; i++)
						{
							carros[0][i] = carT[0][i];
							carros[1][i] = carT[1][i];
						}
						funcAgrupaSoluTemp(soluTemp, carT, perT, tamanhoMatriz, nCarros);
						//cout << "minimoT = " << minimoT << "    minimo = " << minimo << endl;

					}
					else
					{
						k++; inter++;
					}
				}

			}

			//at� aqui
		}
	}


	//Libera mem�ria--------------
	free(perT);
	for(int i = 0; i < 2; i++)
		free(carT[i]);
	free(carT);
	//___________________________

	return minimo;

}

//Imprime uma matriz nCarrosXtamanhoMatriz
void funImprimeMat(int **mat, int tamanhoMatriz, int nCarros)
{
	for(int i = 0; i < nCarros; i++)
	{
		cout <<"\n";
		cout << "[ ";
		for(int j = 0; j < tamanhoMatriz; j++)
		{
			cout << mat[i][j] << " ";
		}
		cout << " ]" << endl;
		cout << "\n";
	}
}

//fun��o gera numeros aleatorios
int myrandom (int i) { return rand()%i;};

//Fun��o extremamente importante para o algor�tmos, usa a matriz soluTemp para construir uma boa solu��o soluTemp1
void funcConstSolucao(int **soluTemp, int **soluTemp1, int tamanhoMatriz, int nCarros)
{
	int indF0 = 0; int indF1 = 0; int indF2 = 0; int indF3 = 0; int indF4 = 0;
 	int indI0 = 0; int indI1 = 0; int indI2 = 0; int indI3 = 0; int indI4 = 0;
 	bool entra0 = true; bool entra1 = true; bool entra2 = true; bool entra3 = true; bool entra4 = true;

 	int termina;//� a �ltima posi��o do vetor soluTemp1

	int **posCarI = (int**)malloc(nCarros*sizeof(int*));
	for(int i = 0; i < nCarros; i++)
		posCarI[i] = (int*)malloc(tamanhoMatriz*sizeof(int));
	for(int i = 0; i < nCarros; i++)
	{
		for(int j = 0; j < tamanhoMatriz; j++)
			posCarI[i][j] = -1;
	}
	int **posCarF = (int**)malloc(nCarros*sizeof(int*));
	for(int i = 0; i < nCarros; i++)
		posCarF[i] = (int*)malloc(tamanhoMatriz*sizeof(int));
	for(int i = 0; i < nCarros; i++)
	{
		for(int j = 0; j < tamanhoMatriz; j++)
			posCarF[i][j] = -1;
	}


 	for(int i = 0; i < (tamanhoMatriz-1); i++ )
 	{
 		if(i == (tamanhoMatriz -2))
 		{
 			if(soluTemp[1][i] == 0)
 				posCarF[0][indF0] = (tamanhoMatriz-2);
 			if(soluTemp[1][i] == 1)
 				posCarF[1][indF1] = (tamanhoMatriz-2);
 			if(soluTemp[1][i] == 2)
 				posCarF[2][indF2] = (tamanhoMatriz-2);
 			if(soluTemp[1][i] == 3)
 				posCarF[3][indF3] = (tamanhoMatriz-2);
 			if(soluTemp[1][i] == 4)
 				posCarF[4][indF4] = (tamanhoMatriz-2);
		}

 		switch (soluTemp[1][i])
 		{
 			case 0:
 				if(entra0)
				{
					posCarI[0][indI0] = i;
					indI0++;
			 		entra0 = false;

				}

				if(soluTemp[1][i] != soluTemp[1][i-1])
				{
					switch (soluTemp[1][i-1])
					{
						case 1:
							posCarF[1][indF1] = i-1;
							indF1++;
							entra1 = true;
						break;

						case 2:
							posCarF[2][indF2] = i-1;
							indF2++;
							entra2 = true;
						break;

						case 3:
							posCarF[3][indF3] = i-1;
							indF3++;
							entra3 = true;
						break;

						case 4:
							posCarF[4][indF4] = i-1;
							indF4++;
							entra4 = true;
						break;
					}

				}
			break;

			case 1:
 				if(entra1)
				{
					posCarI[1][indI1] = i;
					indI1++;
					entra1 = false;
				}


				if(soluTemp[1][i] != soluTemp[1][i-1])
				{
					switch (soluTemp[1][i-1])
					{
						case 0:
							posCarF[0][indF0] = i-1;
							indF0++;
							entra0 = true;
						break;

						case 2:
							posCarF[2][indF2] = i-1;
							indF2++;
							entra2 = true;
						break;

						case 3:
							posCarF[3][indF3] = i-1;
							indF3++;
							entra3 = true;
						break;

						case 4:
							posCarF[4][indF4] = i-1;
							indF4++;
							entra4 = true;
						break;
					}
				}
			break;

			case 2:
 				if(entra2)
				{
			 		posCarI[2][indI2] = i;
			 		indI2++;
			 		entra2 = false;
				}

				if(soluTemp[1][i] != soluTemp[1][i-1])
				{
					switch (soluTemp[1][i-1])
					{
						case 0:
							posCarF[0][indF0] = i-1;
							indF0++;
							entra0 = true;
						break;

						case 1:
							posCarF[1][indF1] = i-1;
							indF1++;
							entra1 = true;
						break;

						case 3:
							posCarF[3][indF3] = i-1;
							indF3++;
							entra3 = true;
						break;

						case 4:
							posCarF[4][indF4] = i-1;
							indF4++;
							entra4 = true;
						break;
					}
				}
			break;

			case 3:
 				if(entra3)
				{
			 		posCarI[3][indI3] = i;
			 		indI3++;
			 		entra3 = false;
				}

				if(soluTemp[1][i] != soluTemp[1][i-1])
				{
					switch (soluTemp[1][i-1])
					{
						case 0:
							posCarF[0][indF0] = i-1;
							indF0++;
							entra0 = true;
						break;

						case 1:
							posCarF[1][indF1] = i-1;
							indF1++;
							entra1 = true;
						break;

						case 2:
							posCarF[2][indF2] = i-1;
							indF2++;
							entra2 = true;
						break;

						case 4:
							posCarF[4][indF4] = i-1;
							indF4++;
							entra4 = true;
						break;
					}
				}
			break;

			case 4:
 				if(entra4)
				{
			 		posCarI[4][indI4] = i;
			 		indI4++;
			 		entra4 = false;
				}

				if(soluTemp[1][i] != soluTemp[1][i-1])
				{
					switch (soluTemp[1][i-1])
					{
						case 0:
							posCarF[0][indF0] = i-1;
							indF0++;
							entra0 = true;
						break;

						case 1:
							posCarF[1][indF1] = i-1;
							indF1++;
							entra1 = true;
						break;

						case 2:
							posCarF[2][indF2] = i-1;
							indF2++;
							entra2 = true;
						break;

						case 3:
							posCarF[3][indF3] = i-1;
							indF3++;
							entra3 = true;
						break;
					}
				}
			break;
		}
	}

	int *blocoCar = (int*)malloc(nCarros*sizeof(int));
	for(int i = 0; i < nCarros; i++)
	{
		for(int j = 0; j < tamanhoMatriz; j++)
		{
			if(posCarI[i][j] != -1)
			{
				blocoCar[i] = j+1;
			}
			else
				break;
		}
	}


	if( nCarros == 2)
	{
		int indice[nCarros]; funcOrdenaAleatorioVetor(indice,nCarros);

  		int v0[blocoCar[indice[0]]]; int v1[blocoCar[indice[1]]];

		funcOrdenaAleatorioVetor(v0, blocoCar[indice[0]]);
		funcOrdenaAleatorioVetor(v1, blocoCar[indice[1]]);

		termina = indice[nCarros-1];

		int cont = 0;
		while(cont < (tamanhoMatriz-1))
		{
			for(int i = 0; i < blocoCar[indice[0]]; i++)
			{
				for(int j = 0; j < (posCarF[indice[0]][v0[i]]+1 - posCarI[indice[0]][v0[i]]); j++)
				{
					soluTemp1[0][cont] = soluTemp[0][posCarI[indice[0]][v0[i]]+ j];
					soluTemp1[1][cont] = soluTemp[1][posCarI[indice[0]][v0[i]]+ j];
					cont++;
				}
			}
			for(int i = 0; i < blocoCar[indice[1]]; i++)
			{
				for(int j = 0; j < (posCarF[indice[1]][v1[i]]+1 - posCarI[indice[1]][v1[i]]); j++)
				{
					soluTemp1[0][cont] = soluTemp[0][posCarI[indice[1]][v1[i]]+ j];
					soluTemp1[1][cont] = soluTemp[1][posCarI[indice[1]][v1[i]]];
					cont++;
				}
			}
		}
		soluTemp1[0][tamanhoMatriz-1] = 0;
		soluTemp1[1][tamanhoMatriz-1] = termina;
	}

	else if( nCarros == 3)
	{
		int indice[nCarros]; funcOrdenaAleatorioVetor(indice,nCarros);

  		int v0[blocoCar[indice[0]]]; int v1[blocoCar[indice[1]]]; int v2[blocoCar[indice[2]]];

		funcOrdenaAleatorioVetor(v0, blocoCar[indice[0]]);
		funcOrdenaAleatorioVetor(v1, blocoCar[indice[1]]);
		funcOrdenaAleatorioVetor(v2, blocoCar[indice[2]]);

		termina = indice[nCarros-1];

		int cont = 0;
		while(cont < (tamanhoMatriz-1))
		{
			for(int i = 0; i < blocoCar[indice[0]]; i++)
			{
				for(int j = 0; j < (posCarF[indice[0]][v0[i]]+1 - posCarI[indice[0]][v0[i]]); j++)
				{
					soluTemp1[0][cont] = soluTemp[0][posCarI[indice[0]][v0[i]]+ j];
					soluTemp1[1][cont] = soluTemp[1][posCarI[indice[0]][v0[i]]+ j];
					cont++;
				}
			}
			for(int i = 0; i < blocoCar[indice[1]]; i++)
			{
				for(int j = 0; j < (posCarF[indice[1]][v1[i]]+1 - posCarI[indice[1]][v1[i]]); j++)
				{
					soluTemp1[0][cont] = soluTemp[0][posCarI[indice[1]][v1[i]]+ j];
					soluTemp1[1][cont] = soluTemp[1][posCarI[indice[1]][v1[i]]];
					cont++;
				}
			}
			for(int i = 0; i < blocoCar[indice[2]]; i++)
			{
				for(int j = 0; j < (posCarF[indice[2]][v2[i]]+1 - posCarI[indice[2]][v2[i]]); j++)
				{
					soluTemp1[0][cont] = soluTemp[0][posCarI[indice[2]][v2[i]]+ j];
					soluTemp1[1][cont] = soluTemp[1][posCarI[indice[2]][v2[i]]];
					cont++;
				}
			}
		}
		soluTemp1[0][tamanhoMatriz-1] = 0;
		soluTemp1[1][tamanhoMatriz-1] = termina;
	}

	else if(nCarros == 4)
	{
		int indice[nCarros]; funcOrdenaAleatorioVetor(indice,nCarros);

  		int v0[blocoCar[indice[0]]]; int v1[blocoCar[indice[1]]]; int v2[blocoCar[indice[2]]]; int v3[blocoCar[indice[3]]];

		funcOrdenaAleatorioVetor(v0, blocoCar[indice[0]]);
		funcOrdenaAleatorioVetor(v1, blocoCar[indice[1]]);
		funcOrdenaAleatorioVetor(v2, blocoCar[indice[2]]);
		funcOrdenaAleatorioVetor(v3, blocoCar[indice[3]]);

		termina = indice[nCarros-1];

		int cont = 0;
		while(cont < (tamanhoMatriz-1))
		{
			for(int i = 0; i < blocoCar[indice[0]]; i++)
			{
				for(int j = 0; j < (posCarF[indice[0]][v0[i]]+1 - posCarI[indice[0]][v0[i]]); j++)
				{
					soluTemp1[0][cont] = soluTemp[0][posCarI[indice[0]][v0[i]]+ j];
					soluTemp1[1][cont] = soluTemp[1][posCarI[indice[0]][v0[i]]+ j];
					cont++;
				}
			}
			for(int i = 0; i < blocoCar[indice[1]]; i++)
			{
				for(int j = 0; j < (posCarF[indice[1]][v1[i]]+1 - posCarI[indice[1]][v1[i]]); j++)
				{
					soluTemp1[0][cont] = soluTemp[0][posCarI[indice[1]][v1[i]]+ j];
					soluTemp1[1][cont] = soluTemp[1][posCarI[indice[1]][v1[i]]];
					cont++;
				}
			}
			for(int i = 0; i < blocoCar[indice[2]]; i++)
			{
				for(int j = 0; j < (posCarF[indice[2]][v2[i]]+1 - posCarI[indice[2]][v2[i]]); j++)
				{
					soluTemp1[0][cont] = soluTemp[0][posCarI[indice[2]][v2[i]]+ j];
					soluTemp1[1][cont] = soluTemp[1][posCarI[indice[2]][v2[i]]];
					cont++;
				}
			}
			for(int i = 0; i < blocoCar[indice[3]]; i++)
			{
				for(int j = 0; j < (posCarF[indice[3]][v3[i]]+1 - posCarI[indice[3]][v3[i]]); j++)
				{
					soluTemp1[0][cont] = soluTemp[0][posCarI[indice[3]][v3[i]]+ j];
					soluTemp1[1][cont] = soluTemp[1][posCarI[indice[3]][v3[i]]];
					cont++;
				}
			}
		}
		soluTemp1[0][tamanhoMatriz-1] = 0;
		soluTemp1[1][tamanhoMatriz-1] = termina;
	}

	else if(nCarros == 5)
	{
		int indice[nCarros]; funcOrdenaAleatorioVetor(indice,nCarros);

  		int v0[blocoCar[indice[0]]]; int v1[blocoCar[indice[1]]]; int v2[blocoCar[indice[2]]]; int v3[blocoCar[indice[3]]]; int v4[blocoCar[indice[4]]];

		funcOrdenaAleatorioVetor(v0, blocoCar[indice[0]]);
		funcOrdenaAleatorioVetor(v1, blocoCar[indice[1]]);
		funcOrdenaAleatorioVetor(v2, blocoCar[indice[2]]);
		funcOrdenaAleatorioVetor(v3, blocoCar[indice[3]]);
		funcOrdenaAleatorioVetor(v4, blocoCar[indice[4]]);

		termina = indice[nCarros-1];

		int cont = 0;
		while(cont < (tamanhoMatriz-1))
		{
			for(int i = 0; i < blocoCar[indice[0]]; i++)
			{
				for(int j = 0; j < (posCarF[indice[0]][v0[i]]+1 - posCarI[indice[0]][v0[i]]); j++)
				{
					soluTemp1[0][cont] = soluTemp[0][posCarI[indice[0]][v0[i]]+ j];
					soluTemp1[1][cont] = soluTemp[1][posCarI[indice[0]][v0[i]]+ j];
					cont++;
				}
			}
			for(int i = 0; i < blocoCar[indice[1]]; i++)
			{
				for(int j = 0; j < (posCarF[indice[1]][v1[i]]+1 - posCarI[indice[1]][v1[i]]); j++)
				{
					soluTemp1[0][cont] = soluTemp[0][posCarI[indice[1]][v1[i]]+ j];
					soluTemp1[1][cont] = soluTemp[1][posCarI[indice[1]][v1[i]]];
					cont++;
				}
			}
			for(int i = 0; i < blocoCar[indice[2]]; i++)
			{
				for(int j = 0; j < (posCarF[indice[2]][v2[i]]+1 - posCarI[indice[2]][v2[i]]); j++)
				{
					soluTemp1[0][cont] = soluTemp[0][posCarI[indice[2]][v2[i]]+ j];
					soluTemp1[1][cont] = soluTemp[1][posCarI[indice[2]][v2[i]]];
					cont++;
				}
			}
			for(int i = 0; i < blocoCar[indice[3]]; i++)
			{
				for(int j = 0; j < (posCarF[indice[3]][v3[i]]+1 - posCarI[indice[3]][v3[i]]); j++)
				{
					soluTemp1[0][cont] = soluTemp[0][posCarI[indice[3]][v3[i]]+ j];
					soluTemp1[1][cont] = soluTemp[1][posCarI[indice[3]][v3[i]]];
					cont++;
				}
			}
			for(int i = 0; i < blocoCar[indice[4]]; i++)
			{
				for(int j = 0; j < (posCarF[indice[4]][v4[i]]+1 - posCarI[indice[4]][v4[i]]); j++)
				{
					soluTemp1[0][cont] = soluTemp[0][posCarI[indice[4]][v4[i]]+ j];
					soluTemp1[1][cont] = soluTemp[1][posCarI[indice[4]][v4[i]]];
					cont++;
				}
			}
		}
		soluTemp1[0][tamanhoMatriz-1] = 0;
		soluTemp1[1][tamanhoMatriz-1] = termina;
	}


	//Libera memoria
	for(int i = 0; i < nCarros; i++)
		free(posCarI[i]);
	free(posCarI);
	for(int i = 0; i < nCarros; i++)
		free(posCarF[i]);
	free(posCarF);
	free(blocoCar);
}

//Busca uma solu��o inicial melhor
void funcMelhorSolucaoInicial(int ***custoCar, int ***aluguelCar, int **soluTemp, int **soluTemp1, int **soluTemp0, int tamanhoMatriz, int nCarros, int nDeInteracoes)
{
	int inter = 0;
	int minimo, minimo1;
	while(inter < nDeInteracoes)
	{
		funcConstSolucao(soluTemp0, soluTemp1, tamanhoMatriz, nCarros);
		funcSolTempFinalizado(custoCar, aluguelCar, soluTemp1, tamanhoMatriz, nCarros);
		minimo = funcCustoSoluTemp(custoCar, aluguelCar, soluTemp, tamanhoMatriz);
		minimo1 = funcCustoSoluTemp(custoCar, aluguelCar, soluTemp1, tamanhoMatriz);
		//cout << "minimo = " << funcCustoSoluTemp(custoCar, aluguelCar, soluTemp, tamanhoMatriz) << endl;

		if(minimo1 < minimo)
		{
			for(int i = 0; i < tamanhoMatriz; i++)
			{
				soluTemp[0][i] = soluTemp1[0][i];
				soluTemp[1][i] = soluTemp1[1][i];
				inter = 0;
			}
		//	cout  << " minimo1 =  " << funcCustoSoluTemp(custoCar, aluguelCar, soluTemp1, tamanhoMatriz) << endl;
		}
		inter++;
	}

}


//ordena vetores
void quick(int* vetor, int inicio, int fim)
{

   int pivo, aux, i, j, meio;

   i = inicio;
   j = fim;

   meio = (int) ((i + j) / 2);
   pivo = vetor[meio];

   do{
      while (vetor[i] < pivo) i = i + 1;
      while (vetor[j] > pivo) j = j - 1;

      if(i <= j){
         aux = vetor[i];
         vetor[i] = vetor[j];
         vetor[j] = aux;
         i = i + 1;
         j = j - 1;
      }
   }while(j > i);

   if(inicio < j) quick(vetor, inicio, j);
   if(i < fim) quick(vetor, i, fim);

}

//ordena um vetor em ordem aleat�ria
void funcOrdenaAleatorioVetor(int* vA, int tamanho)
{
	int v[tamanho];

	for(int i = 0; i < tamanho; i++)
	{
		vA[i] = v[i] = rand();
	}

	quick(v, 0, tamanho-1);

	for(int i = 0; i < tamanho; i++)
	{
		for(int j = 0; j < tamanho; j++)
		{
			if(vA[i] == v[j])
			{
				vA[i] = j;
				break;
			}
		}
	}
}

// -------------A FUN��O PRINCIPAL DO TRABALHO O ILS----------------------
int funcILS(int ***custoCar, int ***aluguelCar, int ** carros, int ** solT, int **soluTemp, int **soluTemp1, int **soluTemp0, int *percurso, int tamanhoMatriz, int nCarros)
{
	int numIntVNS = 120;
	int minimo, minimo1;
	int resultado = minimo = funcVNS(custoCar, aluguelCar, carros, soluTemp, solT, percurso, nCarros, tamanhoMatriz, numIntVNS);
	int nDeInteracoes = 1;

	//minimo = funcCustoSoluTemp(custoCar, aluguelCar, soluTemp, tamanhoMatriz);
	int inter = 0;
	bool entra = true;
	while( inter < nDeInteracoes)
	{
		funcConstSolucao(soluTemp0, soluTemp1, tamanhoMatriz, nCarros);
		funcSolTempFinalizado(custoCar, aluguelCar, soluTemp1, tamanhoMatriz, nCarros);
		funcRepreenchePercursoECar(carros, soluTemp1, percurso, tamanhoMatriz, nCarros);
		minimo1 = funcVNS(custoCar, aluguelCar, carros, soluTemp1, solT, percurso, nCarros, tamanhoMatriz, numIntVNS);

		//minimo1 = funcCustoSoluTemp(custoCar, aluguelCar, soluTemp1, tamanhoMatriz);

		//CONTROLE DO TEMPO
		if(( (clock() - TEMPO_INICIAL)/1000.0) >= TEMPO_LIMITE)
		{
			int variavel = (minimo < minimo1) ? minimo : minimo1;
			return variavel;
		}
		//__________________
		if(minimo1 < minimo)
		{
			for(int i = 0; i < tamanhoMatriz; i++)
			{
				soluTemp[0][i] = soluTemp1[0][i];
				soluTemp[1][i] = soluTemp1[1][i];
			}
			minimo = funcCustoSoluTemp(custoCar, aluguelCar, soluTemp, tamanhoMatriz);
		}
		inter++;
		if(entra)
		{
			if(resultado != minimo1)
			{
				if(tamanhoMatriz < 40)
					nDeInteracoes=+2;
				else if( (tamanhoMatriz >= 40)&&(tamanhoMatriz < 200) )
					nDeInteracoes++;

				entra = false;
				if(resultado > minimo)
				{
					resultado = minimo;
					entra = true;
				}
			}
		}
		else if(resultado > minimo)
		{
			if(tamanhoMatriz < 40)
				nDeInteracoes += 5;
			else if( (tamanhoMatriz >= 40)&&(tamanhoMatriz < 200) )
			{
				nDeInteracoes += 4;
			}

			resultado = minimo;
		}
	}
	return minimo;
}

int funcMedia(int ***custoCar, int ***aluguelCar, int ** carros, int ** solT, int **soluTemp, int **soluTemp1, int **soluTemp0, int *percurso, int tamanhoMatriz, int nCarros, int numeroInter)
{
	double tempo;
	double soma = 0;
	double somaT = 0;
	int valor;
	int minimo = 999999;
	double minimoT = 99999999.0;

	int valores[numeroInter];
	double tempos[numeroInter];
	FILE *arq = fopen("Aracaju200n.txt", "w");

	fprintf(arq,"\n");

	for(int i = 0; i < numeroInter; i++)
	{
		TEMPO_INICIAL = clock();
		valor = funcILS(custoCar, aluguelCar, carros, solT, soluTemp, soluTemp1, soluTemp0, percurso, tamanhoMatriz, nCarros);
		tempo = (clock() - TEMPO_INICIAL)/1000.0;
		if(minimo > valor)
		{
			minimo = valor;
		}
		if(minimoT > tempo)
		{
			minimoT = tempo;
		}
		valores[i] = valor;
		tempos[i] = tempo;
		soma = soma + valor;
		somaT = somaT + tempo;
	}
	double media = soma/(double)numeroInter;
	double mediaT = somaT/(double)numeroInter;
	double soma2 = 0;
	double soma2T = 0;
	fprintf(arq,"\n");
	fprintf(arq,"Valores encontrados\n");
	fprintf(arq,"[");
	for(int i = 0; i < numeroInter; i++)
	{
		soma2 +=  (valores[i] - media)*(valores[i] - media);
		if(i < numeroInter-1)
			fprintf(arq," %5d, ",valores[i]);
		else
			fprintf(arq," %5d] \n ",valores[i]);
	}
	fprintf(arq,"\n");
	fprintf(arq,"Tempos encontrados\n");
	fprintf(arq,"[");
	for(int i = 0; i < numeroInter; i++)
	{
		soma2T += (tempos[i] - mediaT)*(tempos[i] - mediaT);
		if(i < numeroInter-1)
			fprintf(arq," %f, ",tempos[i]);
		else
			fprintf(arq," %f]",tempos[i]);
	}
	fprintf(arq,"\n");
	fprintf(arq,"\n");
	fprintf(arq,"A m�dia dos valores vale = %f\n",media);
	fprintf(arq,"A m�dia dos tempos vale = %f\n",mediaT);
	double desvioPadrao = sqrt(soma2/(double)numeroInter);
	double desvioPadraoT = sqrt(soma2T/(double)numeroInter);
	fprintf(arq,"Desvio padr�o dos valores = %f\n",desvioPadrao);
	fprintf(arq,"Desvio padr�o dos Tempos = %f\n",desvioPadraoT);
	fprintf(arq,"Menor Valor = %d\n",minimo);
	fprintf(arq,"Menor Tempo = %f\n",minimoT);
	for(int i = 0; i < numeroInter; i++)
	{
		if(minimo == valores[i])
		{
			fprintf(arq,"A solu��o que deu o menor valor\n");
			fprintf(arq,"\n");
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
			break;
		}
	}

	return media;
}
