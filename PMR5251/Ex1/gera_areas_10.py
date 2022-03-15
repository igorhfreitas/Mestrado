# -*- coding: utf-8 -*-
# Este programa tem a finalidade de gerar um vetor de áreas aleatório a partir
# de uma seed

import csv
import random

random.seed(a=1)

# As áreas estão entre 0.6 e-4 m^2 e 225.8 e-4 m^2
with open('areas.csv', 'w', newline='') as f:
    writer = csv.writer(f)
    writer.writerow(['area_id', *['area{0}'.format(x) for x in range(1,11)]])
    for i in range(520):
        nums = [i]
        for j in range(10):
            num = random.random() * (225.8 - 0.6) + 0.6
            if ((num >= 225.8) or (num <= 0.6)):
                print('Error area {:4d} bar {:2d}: {:6.4f}'.format(i,j,num))
            nums.append(num)
        writer.writerow((str(x) for x in nums))