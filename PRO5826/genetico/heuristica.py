import math
import time
import random
import copy
import numpy as np
from itertools import product
from tqdm import tqdm

timeInTabu = 3

class job():
    def __init__(self, id,  p, a, b):
        self.id = id
        self.a = a
        self.b = b
        self.p = p

    def p_div_by_b(self):
        return self.p/self.b

    def p_div_by_a(self):
        return self.p/self.a

    def b_div_by_a(self):
        return self.b/self.a

    def pena(self):
        return (self.b - self.a)/self.a


class resolve:
    def __init__(self, size, id, h, problems):
        self.size = size
        self.id = id
        self.sumP = problems["sch" + str(size)][id]["sumP"]
        self.jobs = []
        self.d = math.floor(self.sumP*h)
        self.start_time_first_job = 0
        self.get_jobs(problems)
        self.early = []
        self.late = []
        self.h = h

    def cost(self, jobs="self", start="self"):
        cost = 0
        time = self.start_time_first_job if start == "self" else start
        if jobs == "self":
            jobs = self.jobsConstrutiva

        for job in jobs:
            time += job.p
            if time < self.d:
                cost += (self.d - time)*job.a
            else:
                cost += (time - self.d)*job.b
        return cost

    def get_jobs(self, problems):
        for j in range(self.size):
            self.jobs.append(job("job" + str(j),
                                 problems["sch" +str(self.size)][self.id]["p"][j],
                                 problems["sch" + str(self.size)][self.id]["a"][j],
                                 problems["sch" + str(self.size)][self.id]["b"][j]))

    def construtiva(self):
        earlyCandidates = []
        lateCandidates = []
        totalTimeEarlyCandidates = 0

        for job in self.jobs:
            if job.a < job.b:
                earlyCandidates.append(job)
                totalTimeEarlyCandidates += job.p
            else:
                lateCandidates.append(job)

        earlyCandidates = sorted(earlyCandidates, key=lambda job: job.pena(), reverse=True)

        while totalTimeEarlyCandidates > self.d:
            totalTimeEarlyCandidates -= earlyCandidates[-1].p
            lateCandidates.append(earlyCandidates[-1])
            earlyCandidates.pop()

        earlyCandidates = sorted(earlyCandidates, key=lambda job: job.p_div_by_a(), reverse=True)
        lateCandidates = sorted(lateCandidates, key=lambda job: job.p_div_by_b())

        self.start_time_first_job = self.d - totalTimeEarlyCandidates
        self.early = earlyCandidates
        self.late = lateCandidates
        self.jobsConstrutiva = earlyCandidates + lateCandidates
        self.costConstrutiva = self.cost(self.jobsConstrutiva, self.start_time_first_job)

    def iniGenes(self):
        """
        inicializa os genes de acordo com a solução atual
        inicializa as listas full de VShape de jobs adiantados e atrasados
        """

        self.genes = self.size*[0]

        for j in self.late:
            self.genes[int(j.id[3:])] = 1

        self.VshapeFullEarly = sorted(self.jobs, key=lambda job: job.p_div_by_a(), reverse=True)
        self.VshapeFullLate = sorted(self.jobs, key=lambda job: job.p_div_by_b())

    def costGA(self,genes):
        """
        calculo do custo de acordo com ppt
        """
        earlytemp=[]
        latetemp=[]

        timeEarly = 0
    
        for j in self.VshapeFullEarly:
           if genes[int(j.id[3:])] == 0:
               # corrige factibilidade
                if timeEarly <= self.d:
                    earlytemp.append(j)
                    timeEarly += j.p
                else:
                    genes[int(j.id[3:])] = 1

        for j in self.VshapeFullLate:
            if genes[int(j.id[3:])] == 1:
                latetemp.append(j)

        jobstemp=earlytemp+latetemp

        # Testa as 3 possibilidades de Start Time

        startTime = 0
        # Start time = 0
        cost=self.cost(jobs=jobstemp, start=0)
        if timeEarly + latetemp[0].p < self.d:
            cost = 999999999999

        #Start time --> ultimo early acaba em d
        tempStart = self.d - timeEarly
        if tempStart >= 0:
            cost2=self.cost(jobs=jobstemp, start=tempStart)
            if cost2 < cost: 
                startTime = tempStart
                cost = cost2

        # Start time --> primeiro late acaba em d
        tempStart -= latetemp[0].p
        if tempStart >= 0:
            cost3=self.cost(jobs=jobstemp, start=tempStart)
            if cost3 < cost: 
                startTime = tempStart
                cost = cost3

        # retorna o novo gene após testar factibilidade
        return genes, cost, startTime

    def Mutation(self, genes, p=0.1):
        for g in range(self.size):
            # probabilidade p de um gene mudar de estado
            if random.randrange(0, 10) < 10*p:
                if genes[g] == 0:
                    genes[g] = 1
                else:
                    genes[g] = 0

        genes, costGene, startTime = self.costGA(genes)

        return genes, costGene, startTime

    def crossover(self,genes_pai1,genes_pai2):  

        genes_pai1=np.array(genes_pai1)
        genes_pai2=np.array(genes_pai2)
        genes_filho1=np.concatenate([genes_pai1[:int(len(genes_pai1)/2)],genes_pai2[int(len(genes_pai2)/2):]])
        
        genes, costGene, startTime = self.costGA(genes_filho1)

        return genes, costGene, startTime

    def crossover2(self,genes_pai1,genes_pai2):  

        genes_filho = []

        for i in range(len(genes_pai1)):
            if random.randrange(0, 2) == 0:
                genes_filho.append(genes_pai1[i])
            else:
                genes_filho.append(genes_pai2[i])
        
        genes, costGene, startTime = self.costGA(genes_filho)

        return genes, costGene, startTime

    def GA(self,
           population=1000,
           generations=100000,
           prob_ini = 8,
           prob_Pai=0.2,
           prob_mutation_gen_ini=0.5,
           prob_mutation_next_gens=0.3):
        """
        population: NÃO USA --- populacao variavel de 3x o tamanho da instancia com limite de 200
        generations: quantidade de gerações que serão criadas
        prob_ini: probabilidade de se mutar um filho ou não
        prob_Pai: probabilidade de se sortear um pai da população atual sendo os % melhores
        prob_mutation_gen_ini: probabilidade dos genes da população inicial serem mutados
        prob_mutation_next_gens: probabilidade dos genes da população atual serem mutados
        """
        population = max(self.size*3, 200)
        self.iniGenes()
        cost=[]
        starts=[]
        genesPop = []

        genes, costGene, startTime = self.costGA(self.genes)
        genesPop.append(genes)
        starts.append(startTime)
        cost.append(costGene)

        # probabilidade de mutação inicial = 50%
        prob = prob_ini

        # elegiveis a pai
        pPai = prob_Pai

        #Gera populacao inicial a partir de mutações da solução da construtiva
        for i in range(population):
            newGene, newGeneCost, newGeneStartTime = self.Mutation(copy.deepcopy(genesPop[0]), p=prob_mutation_gen_ini)
            genesPop.append(newGene)
            starts.append(newGeneStartTime)
            cost.append(newGeneCost)

        min_cost_id=np.argmin(cost)
        self.min_cost=np.min(cost)
        gen_best_gene=genesPop[min_cost_id]
        #falta sorteio dos genesPop de acordo com o custo
        if False:
            print("-------------- Construtiva --------------")
            print(self.min_cost)
            print(gen_best_gene)
        counter=0
        passou=False
        counter_passou=0
        #Inicia processo evolutivo
        for generation in tqdm(range(generations)):
            # gera nova populacao - Crossover
            
            for pop_ in range(int(population/4)):
                pai1 = np.argsort(np.array(cost))[random.randrange(0, int(pPai*population))]
                pai2 = np.argsort(np.array(cost))[random.randrange(0, int(pPai*population))]
                #pai1 = random.randrange(0, int(pPai*population))
                #pai2 = random.randrange(0, int(pPai*population))

                # pais tem que ser diferentes
                while pai1 == pai2:
                    pai2 = random.randrange(0, int(pPai*population))

                if generation % 2 == 0:
                    genes, costGene, startTime = self.crossover(genesPop[pai1], genesPop[pai2])
                else:
                    genes, costGene, startTime = self.crossover2(genesPop[pai1], genesPop[pai2])
            
                # O filho tem uma chance de sofrer mutação 
                if generation % 20 == 0:
                    prob -= 2
                    passou=True
                #if passou:
                #    counter_passou+=1
                #if counter_passou>10:
                #    passou=False
                #    counter_passou=0
                #    prob += 2
                if random.randrange(0, 10) < prob: 
                    genes, costGene, startTime = self.Mutation(copy.deepcopy(genes), p=prob_mutation_next_gens)

                # Novo individuo substitui o pior 
                max_cost_id=np.argmax(cost)
                genesPop[max_cost_id] = genes
                starts[max_cost_id] = startTime
                cost[max_cost_id] = costGene
                #break

            #f = open(f"verificando_decaimento_do_custo/validacao_dos_genes.txt", "a")
            #f.write(f"{generation} \t")
            #f.write(f"{self.size} \t")
            #f.write(f"{self.h} \t")
            #f.write(f"{self.id} \t")
            #f.write(f"{genesPop} \t")
            #f.write(f"{starts} \t")
            #f.write(f"{cost} \t")

            if np.min(cost)<self.min_cost:
                #guarda a solução incumbente
                self.min_cost=np.min(cost)
                min_cost_id=np.argmin(cost)
                gen_best_gene=genesPop[min_cost_id] 
                counter=0
            else:
                counter+=1
            if False:    
                print("-------------- Geração " + str(generation) + " --------------")     
                print("novo minimo")
                print(self.min_cost)
                print(gen_best_gene)
            if counter>100:
                break
                pass
            #f = open(f"verificando_decaimento_do_custo/decaimento_do_custo.txt", "a")
            #f.write(f"{generation} \t")
            #f.write(f"{self.size} \t")
            #f.write(f"{self.h} \t")
            #f.write(f"{self.id} \t")
            #f.write(f"{self.costConstrutiva} \t")
            #f.write(f"{self.min_cost} \n")
            #f.close()


    def otimiza(self):

        #list_generations=list(np.logspace(3,6,num=9-6+1,base=10,dtype='int'))
        #list_prob_ini=[4,6,8]
        #list_prob_Pai=[0.15,0.3,0.6]
        #list_prob_mutation_gen_ini=[0.2,0.3,0.6]
        #list_prob_mutation_next_gens=[0.1,0.3,0.6]
#
        #grid=list(product(list_generations,list_prob_ini,list_prob_Pai,list_prob_mutation_gen_ini,list_prob_mutation_next_gens))
#
        #for generations,prob_ini,prob_Pai,prob_mutation_gen_ini,prob_mutation_next_gens in tqdm(grid):
        #    inicio = time.time()
        #    self.construtiva()
        #    self.GA(generations=generations,
        #            prob_ini=prob_ini,
        #            prob_Pai=prob_Pai,
        #            prob_mutation_gen_ini=prob_mutation_gen_ini,
        #            prob_mutation_next_gens=prob_mutation_next_gens)
        #    fim = time.time()

        #    if True:
        #        f = open(f"tunning/results_metaheuristica_new_{generations}_{prob_ini}_{prob_Pai}_{prob_mutation_gen_ini}_{prob_mutation_next_gens}.txt", "a")
        #        f.write(f"{self.size} \t")
        #        f.write(f"{self.h} \t")
        #        f.write(f"{self.id} \t")
        #        f.write(f"{fim-inicio} \t")
        #        f.write(f"{self.costConstrutiva} \t")
        #        f.write(f"{self.min_cost} \n")
        #        f.close()

        inicio = time.time()
        self.construtiva()
        gener=1000
        if self.size<50:
            gener=1000
        else:
            gener=1000
        self.GA(generations=gener,
                prob_ini=8,
                prob_Pai=0.6,
                prob_mutation_gen_ini=0.3,
                prob_mutation_next_gens=0.6)
        fim = time.time()

        if True:
            f = open(f"solucao_final_early_stop/results_metaheuristica_crossover_2.txt", "a")
            f.write(f"{self.size} \t")
            f.write(f"{self.h} \t")
            f.write(f"{self.id} \t")
            f.write(f"{fim-inicio} \t")
            f.write(f"{self.costConstrutiva} \t")
            f.write(f"{self.min_cost} \n")
            f.close()

