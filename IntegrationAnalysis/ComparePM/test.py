gcu_id ='8006'

with open('BELLOW2GCUABCHVS.map', 'r') as f:
    for line in f:
        line = line.split()
        # print(line[6])
        if gcu_id in line[6]:
            abc_id = line[1]
            print(abc_id)
            break