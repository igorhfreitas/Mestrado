/*******************************************
 * PRO5826 - Trabalho de heurísticas Construtivas
 *	
 *  Common Due Date Problema:
 * 	minimize sum(WTj*Tj+WEj*Ej) 
 * 
 *  due date = h * sum(Tj)
 * 
 *******************************************/
#include <iostream>
#include <math.h>
#include <vector>
#include <string> 
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <cstdio>
#include <functional>
#include <numeric>
#include "def.h"

using namespace std;


//função que lê os dados do problema e armazena num vetor de tarefas
vector< vector<job> > Instances::load(){

	//cria vetor de jobs, ainda sem dimensão, que será retornado pela função
	vector< vector<job> > instanceData;
	
	//abre o arquivo de entrada da instância
	ifstream dataFile;
	dataFile.open(inputFileName);
	
	//se der erro na abertura do arquivo, imprime aviso
	if (dataFile.bad()){
		cerr << "Nao foi possivel abrir arquivo de entrada!\n";
	}
	
	//se abrir com sucesso, lê e armazena os dados do arquivo
	else{
		
		//lê o número de problemas da instância
		dataFile >> numProblems;

		//para todo problema...
		for(int p=1; p<= numProblems; p++){
			
			//...cria vetor de job para o problema
			vector<job> problemData;
			
			//...cria tarefa que será armazenada no vetor de jobs do problema
			job read;
			
			//...lê o número de jobs do problema a armazenar
			dataFile >> numJobs;
			
			//...loop que lê os dados conforme numero de tarefas especificada no próprio arquivo (numJobs), 
			//salvando do vetor de job criado no início desse loop (problemData)
			for (int j=1; j<= numJobs; j++){
				
				//indez do job é igual ao contador j do loop
				read.index = j;
				
				//guarda valores de pj, EWj e TWj
				dataFile >> read.processingTime >> read.earliWeight >> read.tardiWeight;
				
				problemData.push_back(read);
				
			}//fim do loop que lê os dados de todas as tarefas do problema
			
			//...guarda dados do problema na matriz (vetor de vetores) instanceData
			instanceData.push_back(problemData);
	
		}//fim do loop que lê os dados de todos os problemas da instância
	
    }//fim do procedimento realizado caso o arquivo de entrada tenha sido aberto com sucesso
    
    dataFile.close();
	
	return instanceData;
	
}

//função que lê os benchmarks do problema e armazena numa estrutura - acesso primeiro inst, depois k, depois h
vector< vector< vector<double> > > loadBenchmarks(vector<Instances> &instancias){
	
	//cria estrutura de dados - primeiro acessa instância, depois problema, depois h
	vector< vector< vector<double> > > benchData;

	//abre o arquivo de entrada da instância
	ifstream dataFile;
	dataFile.open("../data/bench.txt");
	
	//se der erro na abertura do arquivo, imprime aviso
	if (dataFile.bad()){
		cerr << "Nao foi possivel abrir arquivo de entrada!\n";
	}
	
	
	//se abrir com sucesso, lê e armazena os dados do arquivo
	else{

		//variável que armazena o número lido de tarefas da instância
		int n;
		
		//lê uma a uma as instâncias do problema
		for(unsigned inst=0; inst<instancias.size(); inst++){

			//lê o número de problemas da instância
			dataFile >> n;

			//cria estrutura para armazena dados da instância (primeiro acessa problema, depois h)
			vector< vector<double> > instValues;
		
			//para todo problema...
			for(int p=1; p<= 10; p++){
			
				//...cria vetor de job para o problema
				vector<double> problemValues(4);
			
				//...lê valores de benchmark para os 4 valores de h, de 0,2 a 0,8
				dataFile >> problemValues[0] >> problemValues[1] >> problemValues[2]>> problemValues[3];

				//...guarda dados do problema na matriz (vetor de vetores) instanceData
				instValues.push_back(problemValues);
				
			}//fim do loop que lê os dados de benchmark de todos os problemas da instância

			benchData.push_back(instValues);
		
		}//fim do procedimento que percorre todas as instâncias do problema
		

    }//fim do procedimento realizado caso o arquivo de entrada tenha sido aberto com sucesso
   
    dataFile.close();

	return benchData;

}