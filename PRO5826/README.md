# Common due date scheduling

Esse trabalho foi desenvolvido seguindo as etapas:
- Heurística construtiva em C++
- Atualização da Heurística construtiva porém em python
- Heurística de melhoria em python
- Meta-Heurística em python

# Fontes de consulta:

https://github.com/luba0587/Scheduling-common-dueDate/blob/master/PRO5826-Luisa_v14.cpp

compara o peso de cada tarefa para término antes (earliness) e porterior (tardiness)
se earliness < tardiness
    cadidata tarefa para ser cumprida antes
se tardiness <= earliness
    candidata a tarefa para ser cumprida depois


se sum de processing time de candidatas earliness <= due date, ok, se não:
    orderna as tarefas candidatas a earliness pela orderm decrescente do peso de tardiness
    tira um a um os elementos até que cumpra a due date.
    insere estes elementos nas candidatas a tardiness

para o grupo earliness restante, ordena o grupo em função da razão p/a - decrescente

para o grupo tardiness restante, ordena o grupo em função da razão p/b - decrescente

https://github.com/swolarz/common-due-date-scheduling/tree/master/src/main/scala

## ========================= ##

There are 7 data files


The restricted single-machine common due date problem can
be stated as follows:

A set of n jobs with deterministic processing times p(i)
and a common due date d are given. The jobs have to be processed
on one machine. For each of the jobs an individual earliness
a(i) and tardiness b(i) penalty is given, which is incurred, if
a job is finished before or after the common due date d,
respectively. The goal is to find a schedule for the n
jobs which jointly minimizes the sum of earliness and tardiness
penalties.

The data files are:
  sch10, sch20, sch50, sch100, sch200, sch500, sch1000

The format of these data files is:
    number of problems
    for each problem in turn:
       number of jobs (n)
       for each job i (i=1,...,n) in turn:
          p(i), a(i), b(i)

The common due date d is calculated by:

d = round [SUM_P * h] 

where round[X] gives the biggest integer which is smaller then or equal to X;
Sum_P denotes the sum of the processing times of the n jobs and 
the parameter h is used to calculate more or less restrictive common due dates.

For the following 280 benchmarks we used h=0.2; h=0.4; h=0.6 and h=0.8.


[#########################################################################]

UPPER BOUNDS FOR 280 INSTANCES

Upper Bounds for the 10 job example
(optimal objective function values are indicated by *):

n=10        SUM_P    h = 0.2    h = 0.4     h = 0.6     h = 0.8

k = 1       116      1,936      1,025       841*        818*
k = 2       129      1,042      615*        615*        615*
k = 3       125      1,586      917         793*        793*
k = 4       102      2,139      1,230       815*        803
k = 5       94       1,187      630         521*        521*
k = 6       88       1,521      908*        755*        755*
k = 7       103      2,170      1,374*      1,101       1,083*
k = 8       79       1,720      1,020       610*        540*
k = 9       92       1,574      876*        582*        554*
k = 10      127      1,869      1,136       710         671*


Upper bounds for the 20 job examples:

n=20        h = 0.2     h = 0.4     h = 0.6     h = 0.8

k = 1       4,431       3,066       2,986       2,986
k = 2       8,567       4,897       3,260       2,980
k = 3       6,331       3,883       3,600       3,600
k = 4       9,478       5,122       3,336       3,040
k = 5       4,340       2,571       2,206       2,206
k = 6       6,766       3,601       3,016       3,016
k = 7       11,10       16,357      4,175       3,900
k = 8       4,203       2,151       1,638       1,638
k = 9       3,530       2,097       1,992       1,992
k = 10      5,545       3,192       2,116       1,995


Upper bounds for the 50 job examples:

n=50        h = 0.2     h = 0.4     h = 0.6     h = 0.8

k = 1       42,363      24,868      17,990      17,990
k = 2       33,637      19,279      14,231      14,132
k = 3       37,641      21,353      16,497      16,497
k = 4       30,166      17,495      14,105      14,105
k = 5       32,604      18,441      14,650      14,650
k = 6       36,920      21,497      14,251      14,075
k = 7       44,277      23,883      17,715      17,715
k = 8       46,065      25,402      21,367      21,367
k = 9       36,397      21,929      14,298      13,952
k = 10      35,797      20,048      14,377      14,377


Upper bounds for the 100 job examples:

n=100       h = 0.2     h = 0.4     h = 0.6     h = 0.8

k = 1       156,103     89,588      72,019      72,019
k = 2       132,605     74,854      59,351      59,351
k = 3       137,463     85,363      68,537      68,537
k = 4       137,265     87,730      69,231      69,231
k = 5       136,761     76,424      55,291      55,277
k = 6       151,938     86,724      62,519      62,519
k = 7       141,613     79,854      62,213      62,213
k = 8       168,086     95,361      80,844      80,844
k = 9       125,153     73,605      58,771      58,771
k = 10      124,446     72,399      61,419      61,419


Upper bounds for the 200 job examples:

n=200       h = 0.2     h = 0.4     h = 0.6     h = 0.8

k = 1       526,666     301,449     254,268     254,268
k = 2       566,643     335,714     266,028     266,028
k = 3       529,919     308,278     254,647     254,647
k = 4       603,709     360,852     297,269     297,269
k = 5       547,953     322,268     260,455     260,455
k = 6       502,276     292,453     236,160     236,160
k = 7       479,651     279,576     247,555     247,555
k = 8       530,896     288,746     225,572     225,572
k = 9       575,353     331,107     255,029     255,029
k = 10      572,866     332,808     269,236     269,236


Upper bounds for the 500 job examples:

n=500       h = 0.2         h = 0.4             h = 0.6         h = 0.8

k = 1       3,113,088       1,839,902           1,581,233       1,581,233
k = 2       3,569,058       2,064,998           1,715,332       1,715,322
k = 3       3,300,744       1,909,304           1,644,947       1,644,947
k = 4       3,408,867       1,930,829           1,640,942       1,640,942
k = 5       3,377,547       1,881,221           1,468,325       1,468,325
k = 6       3,024,082       1,658,411           1,413,345       1,413,345
k = 7       3,381,166       1,971,176           1,634,912       1,634,912
k = 8       3,376,678       1,924,191           1,542,090       1,542,090
k = 9       3,617,807       2,065,647           1,684,055       1,684,055
k = 10      3,315,019       1,928,579           1,520,515       1,520,515


Upper bounds for the 1000 job examples:

n=1000      h = 0.2         h = 0.4         h = 0.6         h = 0.8

k = 1       15,190,371      8,570,154       6,411,581       6,411,581
k = 2       13,356,727      7,592,040       6,112,598       6,112,598
k = 3       12,919,259      7,313,736       5,985,538       5,985,538
k = 4       12,705,290      7,300,217       6,096,729       6,096,729
k = 5       13,276,868      7,738,367       6,348,242       6,348,242
k = 6       12,236,080      7,144,491       6,082,142       6,082,142
k = 7       14,160,773      8,426,024       6,575,879       6,575,879
k = 8       13,314,723      7,508,507       6,069,658       6,069,658
k = 9       12,433,821      7,299,271       6,188,416       6,188,416
k = 10      13,395,234      7,617,658       6,147,295       6,147,295

[##########################################################################]

These upper bounds together with the problem generator 
(see the file schpascal)
are given in 'Benchmarks for scheduling on a single-machine against 
restrictive and unrestrictive common due dates' 
by D. Biskup and M. Feldmann, Discussion Paper No. 397, August 1998, 
University of Bielefeld.

For further information please contact:

Dirk Biskup or Martin Feldmann
University of Bielefeld
Faculty of Economics and Business Administration
Postbox 10 01 31
33501 Bielefeld
Phone: ++49 521 106 3929 / 3933
Fax: ++49 521 106 6036
Email: dbiskup@wiwi.uni-bielefeld.de and mfeldmann@wiwi.uni-bielefeld.de

The largest file is sch1000 of size 200Kb (approximately)
The entire set of files is 400Kb (approximately)
Click here to access these files







