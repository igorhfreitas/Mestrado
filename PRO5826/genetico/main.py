import biskup_data
#import heuristica_orig as heuristica
import heuristica
from multiprocessing import Pool,current_process
import itertools

problems = biskup_data.problems
problems_results = dict()
problem_size = [10, 20, 50, 100, 200, 500, 1000]
h_factor = [0.2, 0.4, 0.6, 0.8]

#problem = heuristica.resolve(1000, str(0),  0.2, problems)
#problem.otimiza()

def start_(configs):
    print(configs[0],configs[1],configs[2],current_process())
    problem = heuristica.resolve(*configs)    
    problem.otimiza()

pool=Pool()
configs=[#(10, str(1),  0.2, problems),
         (20, str(2),  0.8, problems),
         (100, str(4),  0.8, problems),
         (50, str(1),  0.8, problems)]

#pool.map(start_,configs)

configs=[]
if True:
    for p in problem_size:
        for h in h_factor:
            for i in range(10):
                configs.append((p, str(i),  h, problems))
                #problem = heuristica.resolve(p, str(i),  h, problems)
                #problem.otimiza()

    pool.map(start_,configs)        
print("fim")
