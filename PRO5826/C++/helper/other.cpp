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
#include <chrono>
#include <cstdio>
#include <functional>
#include <numeric>
#include "def.h"
 
using namespace std;

double sum_over_vec(vector<job> &tarefas, const std::function<double (const job&)>& member){
  return std::accumulate(tarefas.begin(), tarefas.end(), 0,
      [&](int sum, const job& curr) { return sum + member(curr);});
}


//função que calcula o valor de uma solução (sequência e instante de início de processamento) na função objetivo
double calculateFO(vector<int> orderedIndex, vector<job> jobData, double dueDate, double t0){
	
	double resp=0;
	double t=t0;
	
	for(unsigned pos=0; pos<orderedIndex.size(); pos++){
		
		double pj, WEj, WTj;
		
		for(unsigned j=0; j<jobData.size(); j++){
			
			if (jobData.at(j).index == orderedIndex.at(pos)){
				
				pj = jobData.at(j).processingTime;
				
				WEj = jobData.at(j).earliWeight;
				
				WTj = jobData.at(j).tardiWeight;
				
				break;
				
			}
			
		}
		
		t += pj;
		
		if(t <= dueDate) resp += WEj*(dueDate - t);
		
		else resp += WTj*(t-dueDate);
		
	}
	
	return resp;
	
}
//função que calcula o valor de uma solução (sequência e instante de final de processamento) na função objetivo
double calculateFO_withDynamicT(vector<int> orderedIndex, vector<job> jobData, double dueDate,vector<int> actualFinishTime){
	
	double resp=0;
	double t=0;
	double t_helper=0;
	
	//printf("Calculando peso para due date %lf \n",dueDate);
	for(unsigned pos=0; pos<orderedIndex.size(); pos++){
		double pj, WEj, WTj;
		
		for(unsigned j=0; j<jobData.size(); j++){
			
			if (jobData.at(j).index == orderedIndex.at(pos)){
				
				pj = jobData.at(j).processingTime;
				
				WEj = jobData.at(j).earliWeight;
				
				WTj = jobData.at(j).tardiWeight;
				
				if (actualFinishTime.at(pos)<pj){
					t_helper+=pj;
				}
				else{
					t_helper=actualFinishTime.at(pos);
				}
				t=t_helper;
				break;
			}
		}
		
		if(t <= dueDate) {
			resp += WEj*(dueDate - t);
		}
		else{
			resp += WTj*(t-dueDate);
		 }
		
	}
	
	return resp;
	
}