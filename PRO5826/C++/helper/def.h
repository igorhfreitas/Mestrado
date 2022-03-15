/*******************************************
 * PRO5826 - Trabalho de heurísticas Construtivas
 *	
 *  Common Due Date Problema:
 * 	minimize sum(WTj*Tj+WEj*Ej) 
 *  due date = h * sum(Tj)
 * 
 *******************************************/

#ifndef MYSTRUCT_H
#define MYSTRUCT_H

#include <iostream>
#include <math.h>
#include <vector>
#include <string> 
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <cstdio>
 
using namespace std;


//struct tarefa que guarda o índice do job, seu tempo de processamento e penalidades unitárias de atraso/adiantamento
struct job{
	
	//índice do job
	int index;
	
	//tempo de processamento da tarefa (job)
	double processingTime;
	
	//peso de adiantamento, por unidade de tempo atrasada)
	double earliWeight;
	
	//peso de atraso, por unidade de tempo atrasada)
	double tardiWeight;

	//tempo de processamento da tarefa (job) normalizado em todos os jobs
	double processingTimeNorm;

	//peso de adiantamento, por unidade de tempo atrasada normalizado em todos os jobs
	double earliWeightNorm;

	//peso de atraso, por unidade de tempo atrasada normalizado em todos os jobs
	double tardiWeightNorm;

	//propocacao do peso de atraso pelo peso de adiantamento
	double penaltyBalance;

	//penalidade final ponderada pelo tempo de processamento e pesos
	double finalPenalty;
	
	//tempo final de processamento
	double finalProcessingTime;
};

struct orderAndFinishTime{
	
	//índice do job
	int index;	

	//penalidade final ponderada pelo tempo de processamento e pesos
	int finishTime;
};

//classe de instâncias do problema
class Instances{
	

	private: 
	
	string inputFileName;	//nome do arquivo texto que contém os dados de entrada
	
	int numProblems;		//número de problemas da instância
	int numJobs;      		//número de jobs de cada problema
	

	public:

	//construtor que cria instância usando o nome do arquivo de entrada
	Instances (string str)
	: inputFileName(str)
	{}
	
	//método que carrega os dados no vetor de jobs de cada problema desta instância (primeiro acessa problema, depois tarefa)
	vector< vector<job> > load();
	
};


//classe de medição de tempo
class Timer
{
public:
    Timer() : beg_(clock_::now()) {}
    void reset() { beg_ = clock_::now(); }
    double elapsed() const { 
        return std::chrono::duration_cast<second_>
            (clock_::now() - beg_).count(); }

private:
    typedef std::chrono::high_resolution_clock clock_;
    typedef std::chrono::duration<double, std::ratio<1> > second_;
    std::chrono::time_point<clock_> beg_;
};



#endif