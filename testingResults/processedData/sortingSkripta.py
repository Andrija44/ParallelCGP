import pandas as pd
import fileinput
import os

for subdir, dirs, files in os.walk('../rawData'):
    for file in files:
        filename = subdir + os.sep + file

        if filename.endswith('Formated.csv'):
            continue
        if filename.endswith(".csv"):
            print(filename.removeprefix('../rawData').removesuffix('.csv') + 'Formated.csv')

            ext = max(open(filename, 'r'), key=lambda s: s.count(',')).count(',')
            brTest = 0

            for line in fileinput.input(filename, inplace=1):
                brLin = fileinput.lineno()
                if(fileinput.lineno() == 2):
                    print('%s%s' % (line.rstrip('\n'), str(','*ext)))
                    dulj = len(line) + ext
                else:
                    if line.startswith("TEST"):
                        brTest += 1
                    print('%s' % line.rstrip('\n'))

            data = pd.read_csv(filename, names=range(dulj), index_col=0, skiprows=lambda x: x % 7 == 0)
            data.dropna(axis=1,how='all', inplace=True)
            data = data.transpose()
            data.reset_index(drop=True, inplace=True)
            data.rename_axis(None, axis=1, inplace=True)

            newDf = pd.DataFrame()

            for col in data.columns.unique():
                for i in range(0, int(len(data.columns) / len(data.columns.unique())), 10):
                    fil = data.filter(like=col).iloc[:, i:i+10]
                    cols = pd.Series(fil.columns)
                    cols[cols[cols == col].index.values.tolist()] = [col + '.' + str(i) if i != 0 else col for i in range(sum(cols == col))]
                    fil.columns = cols
                    fil = pd.concat([fil[col].dropna() for col in fil], ignore_index=True)
                    fil = pd.DataFrame(fil, columns=[col])
                    newDf = pd.concat([newDf, fil], axis=1)
            print(newDf)

            # os.remove(filename)
            newDf.to_csv('./' + filename.removeprefix('../rawData').removesuffix('.csv') + 'Formated.csv', index=False)