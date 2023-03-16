import pandas as pd


for dataFolder in range(1,6):
    for traceFile in range(1,2001):
        with open(f'E:/FYP/tempData/data{dataFolder}/data/traceLines/trace_{traceFile}.csv', 'r') as myFile:
            for line in myFile.readlines():
                array = line.split(',')
                array = array[:len(array)-1]
                
                for idx,element in enumerate(array):
                    array[idx] = float(element)
                
                df = pd.DataFrame(array)
                finalIndex = (dataFolder - 1) * (2000) + traceFile
                df.to_csv(f'E:/FYP/tempData/compiledCleanTraces/trace_{finalIndex}.csv')
        myFile.close()
        print(f'Cleaned {finalIndex} files')     
print("Trace Data cleaned for processing")       
        