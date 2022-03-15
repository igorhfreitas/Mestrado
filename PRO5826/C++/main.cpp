/*******************************************
 * PRO5826 - Trabalho de heurísticas Construtivas
 *	
 *  Common Due Date Problema:
 * 	minimize sum(WTj*Tj+WEj*Ej) 
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
#include "../includes/randomc.h"
#include "../includes/mother.cpp"
#include <chrono>
#include <cstdio>
#include <functional>
#include <numeric>
#include "helper/def.h"
#include "helper/io.cpp"
#include "helper/other.cpp"
#include "heuristicas/constructive_heuristica.cpp"

using namespace std;

//estrutura que guarda o tempo computacional despedido em cada instância
vector < vector< vector<double> > > computationalTimes;


//função que inicializa as instâncias com os nomes de arquivo txt
vector<Instances> initializeInstances(){
	
	vector<Instances> all;
	
	Instances tenJobs("../data/sch10.txt");
	all.push_back(tenJobs);
	
	Instances twentyJobs("../data/sch20.txt");
	all.push_back(twentyJobs);
	
	Instances fiftyJobs("../data/sch50.txt");
	all.push_back(fiftyJobs);
	
	Instances hundredJobs("../data/sch100.txt");
	all.push_back(hundredJobs);
	
	Instances twohJobs("../data/sch200.txt");
	all.push_back(twohJobs);
	
	Instances fivehJobs("../data/sch500.txt");
	all.push_back(fivehJobs);
	
	Instances thousJobs("../data/sch1000.txt");
	all.push_back(thousJobs);
	
	return all;
	
}


vector< vector< vector<double> > > calculateAllInstances(vector<Instances> allInstances, vector<double> hFactors){
	
	//estrutura que guarda o valor da FO da melhor solução encontrada pela heurística
	vector< vector< vector<double> > > solutionValues;
	

	//rotina que carrega os dados de cada instância do vetor de instâncias e roda heurística construtiva
	for (unsigned inst=0; inst< allInstances.size(); inst++){
		
		//vetor de vetor de jobs - acesso primeiro o problema, depois a tarefa
		vector< vector<job> > inputData;
		inputData = allInstances.at(inst).load();

		//vetor de vetor de inteiros - índice do job para guardar a sequência obtida pela heurística
		vector< vector< vector<int> > > sequenceOfProblemAndH;
		
		//vetor com tempos de processamento de cada problema para cada valor de h
		vector< vector< double> > compTimeOfProblemAndH;
		
		//vetor de com início de processamento de cada problema para cada h
		//vector< vector <double> > startTimeOfProblemAndH;
		vector< vector<vector <int> >> startTimeOfProblemAndH;
		
		//vetor de vetor com valores de função objetivo das soluções encontradas(acesso primeiro k, depois h)
		vector< vector<double> > solutionValuesOfInstance;
		
		//para cada problema da instância...
		for (unsigned p=0; p < inputData.size(); p++){
			
			//cria vetor para armazenar o valor de FO do problema, por h adotado
			vector<double> solutionValuesOfProblem;
			
			//vetor com tempo de processamento
			vector <double> compTimeOfH;

			//vetor de vetor de inteiros - índice do job para guardar a sequência obtida pela heurística
			vector< vector<int> > sequenceOfH;
		
			//vetor de com início de processamento de cada problema, por h adotado
			//vector <double> startTimeOfH;
			vector <vector<int>> startTimeOfH;


			//...e para cada valor de h
			for(unsigned h=0; h< hFactors.size(); h++){
				
				//variável que armazena o tempo de processamento da heurística na instância em questão
				Timer tmr;
			
				//cria vetor para armazenar sequência de jobs encontrada pela heurística 
				vector <int> sequence;
			
				//cria variável para guardar início de processamento da sequência
				double t_start=0;
			
				printf("Rodando heuristica....\n");
				//chama função da heurística e armazena resultado no vetor sequence e na variável t_start
				sequence = runNewConstructiveHeuristic(hFactors.at(h), inputData.at(p), t_start);
				
				//printf("Calculando possibilidades de atrasos programado | h = %i | p = %i | inst= %i ....\n",h,p,inst);
				vector<int> finishTime;
				vector<orderAndFinishTime> NewSeqAndFinishTime;
				NewSeqAndFinishTime=calcFinishJobTime(hFactors.at(h), sequence,  inputData.at(p), t_start);
				sequence.clear();
				for(auto& seqAndFinishTime: NewSeqAndFinishTime){
					finishTime.push_back(seqAndFinishTime.finishTime);
					sequence.push_back(seqAndFinishTime.index);
					for(int idx=0;idx<inputData.at(p).size();idx++){
						if(seqAndFinishTime.index==inputData.at(p).at(idx).index){
							inputData.at(p).at(idx).finalProcessingTime=seqAndFinishTime.finishTime;
						}
					}
				}
				sort(inputData.at(p).begin(),inputData.at(p).end(),increasingPprocessingTime);
				printf("Calculando penalidade....\n");
				//computa o resultado na FO da sequência encontrada pela heurística construtiva
				//double temp = calculateFO(sequence, inputData.at(p), (int) (hFactors.at(h)*sum_over_vec(inputData.at(p),&job::processingTime)), t_start );
				double temp = calculateFO_withDynamicT(sequence, inputData.at(p), (int) (hFactors.at(h)*sum_over_vec(inputData.at(p),&job::processingTime)),finishTime);
				
				//armazena resultados 
				compTimeOfH.push_back(tmr.elapsed());
				sequenceOfH.push_back(sequence);
				//startTimeOfH.push_back(t_start);
				startTimeOfH.push_back(finishTime);
				solutionValuesOfProblem.push_back(temp);
				
			}//fim do loop que percorre todos os valores de h
			
			//guardo valores do problema para todos os fatores h nas estruturas apropriadas
			sequenceOfProblemAndH.push_back(sequenceOfH);
			compTimeOfProblemAndH.push_back(compTimeOfH);
			startTimeOfProblemAndH.push_back(startTimeOfH);
			solutionValuesOfInstance.push_back(solutionValuesOfProblem);
	
		}//fim do loop que percorre todos os problemas com dado número de jobs

		//guardo os valores da instância
		solutionValues.push_back(solutionValuesOfInstance);
		computationalTimes.push_back(compTimeOfProblemAndH);
		
		//nome do arquivo com tabelas de resultados
		ostringstream outputFile;
		outputFile << "../Output/sch" << inputData.at(0).size() << ".csv";

		//cria arquivo de resultados
		ofstream myfile;
		myfile.open (outputFile.str());
		
		//tabela 2) n, k, h, posição sequência, índice do job
		myfile << "\nInstancia;Problema;Fator h;Posicao;Indice do Job;job index gravado;End Time;Tempo de Processamento;Peso A; Peso B;(b-a)/a; V-shape -earliness; V-shape -tardiness; dueDate;\n";
		
		int pkid;
		for(unsigned p=0; p < inputData.size(); p++){
			
			for (unsigned h=0; h< hFactors.size(); h++){
				
				for (unsigned j = 0; j < inputData.at(p).size(); j++){
					for(int k=0; k < inputData.at(p).size(); k++){
						if (inputData.at(p).at(k).index==sequenceOfProblemAndH.at(p).at(h).at(j)){
							pkid=k;
							goto find_pkid;
						}
					}
					find_pkid:
					myfile << inst+1 << ";"
						<< p+1 << ";"
						<< hFactors.at(h) << ";"
						<< j+1 << ";"
						<< sequenceOfProblemAndH.at(p).at(h).at(j)  << ";"
						<< inputData.at(p).at(pkid).index  << ";"
						//<< startTimeOfProblemAndH.at(p).at(h) << "\n";
						<< startTimeOfProblemAndH.at(p).at(h).at(j) << ";"
						<< inputData.at(p).at(pkid).processingTime << ";"
						<< inputData.at(p).at(pkid).earliWeight << ";"
						<< inputData.at(p).at(pkid).tardiWeight << ";"
						<< (inputData.at(p).at(pkid).earliWeight-inputData.at(p).at(pkid).tardiWeight)/inputData.at(p).at(pkid).earliWeight << ";"
						<< inputData.at(p).at(pkid).processingTime/inputData.at(p).at(pkid).earliWeight << ";"
						<< inputData.at(p).at(pkid).processingTime/inputData.at(p).at(pkid).tardiWeight << ";"
						<< (int) hFactors.at(h)*sum_over_vec(inputData.at(p),&job::processingTime) << "\n";

				}
				
			}			
		}
	}

	return solutionValues;

}

int main(){
	
	//cria vetor com todas as intâncias do problema
	vector< Instances > allInstances = initializeInstances();
	
	//vetor com todos os valores de h utilizados nas instâncias
	vector<double> hFactors;

	//cria estrutura com benchmarks - acesso instância, depois problema, depois h
	vector< vector< vector<double> > > benchmarks;

	//popula vetor hFactors com valores de h
	hFactors.push_back(0.2);
	hFactors.push_back(0.4);
	hFactors.push_back(0.6);
	hFactors.push_back(0.8);
	
	//cria estrutura para guardar os valores obtidos na F.O. - acesso instância, depois problema, depois h
	vector< vector< vector<double> > > solutionValues = calculateAllInstances(allInstances, hFactors);

	//carrega os dados de benchmark
	benchmarks = loadBenchmarks(allInstances);
		
	//nome do arquivo com tabelas de resultados
	ostringstream outputFile;
	outputFile << "../Output/output_Heuristica_Construtiva.csv";
						
	//cria arquivo de resultados
	ofstream myfile;
	myfile.open (outputFile.str());
		
	//tabela 1) n,k,h, idle time calculado, valor FO, benchmark, tempo computacional
	myfile << "\nInstancia;Problema;Fator h;ResultadoFO;Benchmark;Tempo Computacional\n";
	
	//preenche valores da tabela 1
	for(unsigned n=0; n < benchmarks.size(); n++){
		
		for(unsigned k=0; k < benchmarks.at(n).size(); k++){
			
			for(unsigned h=0; h < benchmarks.at(n).at(k).size(); h++){
				
				myfile << n+1 << ";"
					<< k+1 << ";"
					<< hFactors.at(h) << ";"
					<< solutionValues.at(n).at(k).at(h) << ";"
					<< benchmarks.at(n).at(k).at(h) << ";"
					<< computationalTimes.at(n).at(k).at(h) << "\n";
				
			}
			
		}
		
	}
		
	myfile.close();

	return 0;
	
}
