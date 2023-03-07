import pandas as pd
import os 

## Top Level Folder for the Trace Project
filePath = 'E:/FYP/data/2000Rounds_1'

## Create folder for individual trace lines
if not os.path.isdir(f'{filePath}/traceLines'):
    os.makedirs(f'{filePath}/traceLines')

## Begin seperating the entire CSV into individual trace files
lineCount = 1
with open(f'{filePath}/traceFile.csv', 'r') as f:
    for line in f.readlines():
        array = line.split(',')
        array = array[:len(array)-1]

        for idx, element in enumerate(array):
            array[idx] = float(element)
        
        df = pd.DataFrame(array)
        df.to_csv(f'{filePath}/traceLines/trace_{lineCount}.csv')
        print(f'Converted {lineCount} lines')
        lineCount += 1
f.close()
        
# for roundCount in range(3,5):
    
#     traceFilePath = f'./data/1000Rounds_{roundCount}/traceLines'
#     os.mkdir(traceFilePath)
#     lineCount = 1
#     with open(f'D:/FYP/data/1000Rounds_{roundCount}/traceFile.csv', 'r') as f:

#         for line in f.readlines():
#             array = line.split(',')
#             array = array[:len(array)-1]

#             for idx, element in enumerate(array):
#                 array[idx] = float(element)
            
#             df = pd.DataFrame(array)
#             df.to_csv(f'./data/1000Rounds_{roundCount}/traceLines/trace_{lineCount}.csv')
#             print(f'Converted {lineCount} lines from {roundCount-2} files')
#             lineCount += 1
#     f.close()
    
# for fileCount in range(1,1001):
    
#     with open(f'D:/FYP/data/1000Rounds_1/trace_{fileCount}.csv', 'r') as f:
#         for line in f.readlines():
#             array = line.split(',')

#             array = array[:len(array)-1]

#             for idx, element in enumerate(array):
#                 array[idx] = float(element)
            
#             df = pd.DataFrame(array)
            
#             df.to_csv(f'D:/FYP/data/1000_Rounds_2/traceLines_ver2/trace_{fileCount}.csv')
            
#         f.close()
#     print(f"Converted {fileCount} files")

