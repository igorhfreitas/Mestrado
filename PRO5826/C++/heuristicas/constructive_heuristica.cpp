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
#include "../helper/def.h"

using namespace std;

// função para ordenação do vetor de jobs

bool increasingPprocessingTime(job lhs, job rhs){
	return lhs.finalProcessingTime < rhs.finalProcessingTime;
}

bool increasingProcessingTime(job lhs, job rhs){
	return lhs.processingTime < rhs.processingTime;
}

bool decreasingProcessingTime(job lhs, job rhs){
	return lhs.processingTime > rhs.processingTime;
}

bool increasingFinalPenaltyProcessing(job lhs, job rhs){
	if (lhs.finalPenalty == rhs.finalPenalty)
		return lhs.processingTime < rhs.processingTime;
	else 
		return lhs.finalPenalty < rhs.finalPenalty;
}

bool decreasingPDivAProcessing(job lhs, job rhs){
	if (lhs.processingTime/lhs.earliWeight == rhs.processingTime/rhs.earliWeight)
		return lhs.processingTime > rhs.processingTime;
	else 
		return lhs.processingTime/lhs.earliWeight > rhs.processingTime/rhs.earliWeight;
}

bool decreasingFinalPenaltyProcessing(job lhs, job rhs){
	if (lhs.finalPenalty == rhs.finalPenalty)
		return lhs.processingTime > rhs.processingTime;
	else 
		return lhs.finalPenalty > rhs.finalPenalty;
}

bool increasingPenaltyBalanceProcessing(job lhs, job rhs){
	if (lhs.penaltyBalance == rhs.penaltyBalance)
		return lhs.processingTime < rhs.processingTime;
	else 
		return lhs.penaltyBalance < rhs.penaltyBalance;
}

bool decreasingPenaltyBalanceProcessing(job lhs, job rhs){
	if (lhs.penaltyBalance == rhs.penaltyBalance)
		return lhs.processingTime < rhs.processingTime;
	else 
		return lhs.penaltyBalance > rhs.penaltyBalance;
}

bool decreasingEarlyPenalty(job lhs, job rhs){
	if (lhs.earliWeight == rhs.earliWeight)
		return lhs.processingTime < rhs.processingTime;
	else 
		return lhs.earliWeight > rhs.earliWeight;
}


// função que roda heurística construtiva
vector<int> runNewConstructiveHeuristic(double h, vector<job> jobVector, double &t0){
	//Definições
	vector<int> jobOrder;
	vector<job> earlyCandidates;
	vector<job> lateCandidates;
	double dueDate = (int) (h * sum_over_vec(jobVector,&job::processingTime));
	//double dueDate = ceil(h * sum_over_vec(jobVector,&job::processingTime));
	
	//Calcula penalty Balance = (b-a)/a
	//separa jobs em early Candidates e Late Candidates
	for (auto& job_:jobVector){
		job_.penaltyBalance=(job_.tardiWeight-job_.earliWeight)/job_.earliWeight;
		if(job_.earliWeight<job_.tardiWeight){
			earlyCandidates.push_back( job_ );
		}
		else{
			lateCandidates.push_back(job_ );
		}
	}

	//Orderna Early Candidates por order decrescente de (b-a)/a para garantir que 
	//jobs com peso muito grande de tardiness em relação a earliness fiquem na frente
	sort( earlyCandidates.begin(), earlyCandidates.end(), decreasingPenaltyBalanceProcessing);

	//Ajusta Early Candidates para não ultrapassar o prazo de entrega
	int counter=0;
	while(sum_over_vec(earlyCandidates,&job::processingTime)>dueDate){
		lateCandidates.push_back(earlyCandidates.at(earlyCandidates.size()-1));
		earlyCandidates.pop_back();
		counter++;
	}
	
	//Calcula pena para ordernar em V-shape
	for (auto& job_:earlyCandidates){
		job_.finalPenalty=job_.processingTime/job_.earliWeight;
	}
	for (auto& job_:lateCandidates){
		job_.finalPenalty=job_.processingTime/job_.tardiWeight;
	}

	//orderna V-shape
	sort( earlyCandidates.begin(), earlyCandidates.end(), decreasingFinalPenaltyProcessing);
	sort( lateCandidates.begin(), lateCandidates.end(), increasingFinalPenaltyProcessing);

	//Valida se ainda é possível um job do late Candidates terminar na due date.
	goto jump_deterministic_insertion;
	counter=0;
	if (sum_over_vec(earlyCandidates,&job::processingTime)<dueDate){
		for (auto& job_:lateCandidates){
			if (job_.processingTime+sum_over_vec(earlyCandidates,&job::processingTime)== (int) dueDate){
				printf("Adiantando tarefa para acabar em due date %i, %lf \n",job_.index,dueDate);
				earlyCandidates.push_back(job_);
				lateCandidates.erase(lateCandidates.begin()+counter,lateCandidates.begin()+counter+1);
			}
			counter++;
		}
	}
	jump_deterministic_insertion:

	for (auto& job_: earlyCandidates){
		jobOrder.push_back(job_.index);
	}
	for (auto& job_: lateCandidates){
		jobOrder.push_back(job_.index);
	}
	t0=dueDate - sum_over_vec(earlyCandidates,&job::processingTime);

	if(calculateFO(jobOrder, jobVector, dueDate,0)<calculateFO(jobOrder, jobVector, dueDate,t0)){
		t0=0;
	}

	return jobOrder;
	
}


vector<int> finish_time_vec(int delayed_index, int dueDate,vector<int> jobOrder,vector<job> jobVec){
	//função para forçar o atraso em uma tarefa e recalcular o tempo final de cada job
	//Segundo Kennedy, é melhor deixar isso para próxima entrega
	vector<int> t;
	double t_help;
	int counter=0;
	double delayed_time=0;
	int vectorDelayedIdx=0;

	//Loop para atrasar o final da tarefa escolhida
	for(auto& order:jobOrder){
		for (auto& job_:jobVec){
			if(order==job_.index&&delayed_index>0){
				if(job_.index==delayed_index){
					delayed_time=dueDate-t_help-job_.processingTime;
					vectorDelayedIdx=counter;
					t_help=dueDate;
				}
				else{
					t_help+=job_.processingTime;
				}
			t.push_back(t_help);
			counter++;
			}
		}
	}
	//loop para corrigir as tarefas anteriores a tarefa atrasada de forma a manter todos os jobs consecutivos
	counter=0;
	for(auto& order:jobOrder){
		for(auto& job_:jobVec){
			if(order==job_.index&&delayed_index>0){
				break;
			}
			else{
				if(counter<vectorDelayedIdx){
					t.at(counter)+=delayed_time;
				}
			}
		}
		counter++;
	}

	return t;
}

vector<orderAndFinishTime> calcFinishJobTime(double h,vector<int> jobOrder, vector<job> jobVector, double &t0){
	// Calcula tempo final de cada job para ser levado em consideração no calculo do custo
	//Definições
	int counter=0;
	int counter_2=0;
	double dueDate = (int) (h * sum_over_vec(jobVector,&job::processingTime));
	//double dueDate = ceil(h * sum_over_vec(jobVector,&job::processingTime));
	int actualProcessingTime=t0;
	vector<int> actualFinishTime;
	vector<int> finalFinishTime;
	vector<job> JobVecotr_aux;
	vector<job> earlyCandidates;
	vector<job> lateCandidates;
	vector<job> temp_setA;
	vector<job> temp_setB;
	vector<job> temp_jobs;
	vector<int> JobOrder_aux;
	vector<int> JobOrder_aux_final;
	vector<orderAndFinishTime> NewOrderAndFinishTime;
	double penalty=0;
	double tempCost=0;
	double atualCost=0;
	int delayed_index=0;
	int maxearlyPenaltyidx=0;
	int maxearlyPenalty=0;
	int minimun_late_processing_time;
	bool stop=false;
	job job_to_be_move;
	int job_counter;
	int start_time_first_job=t0;
	int proposeFinishTime_job=0;
	vector<int> proposeFinishTime;
	bool find_minimun=false;
	bool jumped;

	printf("Calculando Heurística de Melhoria - Busca Local \n");
	//calculando o valor de pena atual
	for (auto& job_:jobOrder){		
		for(int c=0; c<jobVector.size(); c++){
			if(job_==jobVector.at(c).index){
				actualProcessingTime+=jobVector.at(c).processingTime;
				actualFinishTime.push_back(actualProcessingTime);
				if(actualProcessingTime<=dueDate){
					earlyCandidates.push_back(jobVector.at(c));
				}else{
					lateCandidates.push_back(jobVector.at(c));
				}
			}
			if(maxearlyPenalty<jobVector.at(c).earliWeight&&actualProcessingTime<dueDate){
				maxearlyPenalty=jobVector.at(c).earliWeight;
				maxearlyPenaltyidx=jobVector.at(c).index;
			}
		}
	}
	//actualProcessingTime=0;
	//start_time_first_job=dueDate-sum_over_vec(earlyCandidates,&job::processingTime);
	//penalty=calculateFO_withDynamicT(jobOrder, jobVector, dueDate,actualFinishTime);
	penalty=calculateFO(jobOrder, jobVector, dueDate,start_time_first_job);
	atualCost=penalty;
	//for(int c=0;c<actualFinishTime.size();c++){
	//	actualFinishTime.at(c)=actualFinishTime.at(c)+start_time_first_job;
	//}
	finalFinishTime=actualFinishTime;

	counter=0;
	minimun_late_processing_time=sum_over_vec(earlyCandidates,&job::processingTime);
	for (auto& job_:lateCandidates){
		if (job_.processingTime<=minimun_late_processing_time){
			minimun_late_processing_time=job_.processingTime;
		}
	}
	/*inicio busca local
	verifica recursivamente se um job atrasado (late candidates) pode ser adiantado (movido para early candidates)
	se puder, faz a movimentação e verifica o custo, se o custo é menor salva a solução;
	após todas os late candidates serem testados (all pairs), aceita a solução de menor custo e continua a busca local enquanto tiver espaço no early candidates;
	*/
	//goto pula_busca_local_nao_executar; //jump para não executar busca local
	if ((start_time_first_job<=0)||(start_time_first_job<minimun_late_processing_time)){
		printf("Jump da busca local %i \n",start_time_first_job);
		jumped=true;
		finalFinishTime=actualFinishTime;		
		goto pula_busca_local;
	}
	while (!(stop)){
		find_minimun=false;
		counter=0;
		for(auto& job_:lateCandidates){
			printf("Validando se late job pode ser adiantado - %i \n",counter);
			if (start_time_first_job - job_.processingTime > 0){
				printf("Verificando possibilidade %lf -- %i \n",(start_time_first_job - job_.processingTime),counter);
				temp_setA=lateCandidates;
				temp_setA.erase(temp_setA.begin()+counter,temp_setA.begin()+counter+1);
				temp_setB=earlyCandidates;
				temp_setB.push_back(lateCandidates.at(counter));
				sort( temp_setB.begin(), temp_setB.end(), decreasingPDivAProcessing);
				
				JobOrder_aux.clear();
				actualFinishTime.clear();
				for (auto& job_: temp_setB){
					JobOrder_aux.push_back(job_.index);
					proposeFinishTime_job+=job_.processingTime;
					actualFinishTime.push_back(proposeFinishTime_job);
				}
				for (auto& job_: temp_setA){
					JobOrder_aux.push_back(job_.index);
					proposeFinishTime_job+=job_.processingTime;
					actualFinishTime.push_back(proposeFinishTime_job);
				}
				
				//tempCost=calculateFO_withDynamicT(JobOrder_aux, jobVector, dueDate,actualFinishTime);
				tempCost=calculateFO(JobOrder_aux, jobVector, dueDate,start_time_first_job - job_.processingTime);
				printf("pena atual %lf pena nova %lf \n",atualCost,tempCost);
				if (tempCost<atualCost){
					printf("Aceitando inserção de late candidates para early candidates no %i \n", job_.index);
					job_to_be_move=job_;
					atualCost = tempCost;
					job_counter=counter;
					find_minimun=true;
				}
			}
			counter++;
		}
		if (find_minimun){
			printf("Efetivando a solução aceita \n");
			earlyCandidates.push_back(lateCandidates.at(job_counter));
			sort( earlyCandidates.begin(), earlyCandidates.end(), decreasingPDivAProcessing);
			lateCandidates.erase(lateCandidates.begin()+job_counter,lateCandidates.begin()+job_counter+1);

			start_time_first_job-=job_to_be_move.processingTime;
			penalty=atualCost;

			JobOrder_aux.clear();
			actualFinishTime.clear();
			proposeFinishTime_job=start_time_first_job;
			for (auto& job_: earlyCandidates){
				JobOrder_aux.push_back(job_.index);
				proposeFinishTime_job+=job_.processingTime;
				actualFinishTime.push_back(proposeFinishTime_job);
			}
			for (auto& job_: lateCandidates){
				JobOrder_aux.push_back(job_.index);
				proposeFinishTime_job+=job_.processingTime;
				actualFinishTime.push_back(proposeFinishTime_job);
			}

			for (auto& order_:JobOrder_aux){
				for (auto& job_aux:jobVector){
					if (job_aux.index==order_){
						JobVecotr_aux.push_back(job_aux);
					}
				}
			}
			jobOrder=JobOrder_aux;
			finalFinishTime=actualFinishTime;
		}
		else{
			stop=true;
		}
	}
	pula_busca_local:
	if(calculateFO(jobOrder, jobVector, dueDate,0)<calculateFO(jobOrder, jobVector, dueDate,start_time_first_job)){
		printf("Alterando para iniciar em 0 .....");
		start_time_first_job=0;
		penalty=calculateFO(jobOrder, jobVector, dueDate,0);
		
		finalFinishTime.clear();
		proposeFinishTime_job=start_time_first_job;
		for(auto& order_:jobOrder){
			for(auto& job_:jobVector){
				if(order_==job_.index){
					proposeFinishTime_job+=job_.processingTime;
					finalFinishTime.push_back(proposeFinishTime_job);
				}
			}
		}
	}
	pula_busca_local_nao_executar:
	orderAndFinishTime temp;
	for (int c=0;c<jobOrder.size();c++){
		temp.index=jobOrder.at(c);
		temp.finishTime=finalFinishTime.at(c);
		NewOrderAndFinishTime.push_back(temp);
	}
	t0=start_time_first_job;

	return NewOrderAndFinishTime;
}
