import pandas as pd
import matplotlib.pyplot as plt
import cycler
import os

threadNums = [1, 2, 4, 8, 16]
testName = ["Small", "Medium", "Large"]
testType = ["Simple", "Complex"]
gens = 1000
rounds = 10
seqDict = {}
parDict = {}
gH = 12
gW = 24
plt.style.use('fivethirtyeight')

def plotSeqData():
    plt.close("all")
    fig, axes = plt.subplots(nrows=2, ncols=3, figsize=(gW, gH), layout='constrained',
                             gridspec_kw={'wspace': 0.05, 'hspace': 0.1})
    fig.suptitle('Plot' + file.removesuffix('.csv'), fontsize='xx-large')

    for col in orgCols:
        y = orgCols.index(col) % 3
        x = int(orgCols.index(col) / 3)
        violin = False

        axes[x,y].set_yscale('log')

        for i in range(0, int(len(df.columns) / len(orgCols))):
            fil = df[df > 0].filter(like=col).iloc[:, i].dropna()
            fil /= len(df.filter(like='[mutatePopulationTimer]').iloc[:, i].dropna())
            fil *= gens * rounds

            if len(fil) > 100:
                axes[x,y].violinplot(fil, positions=[i])
                violin = True
            else:
                label = testName[i % len(testName)] + (" " + testType[0 if i < len(testName) else 1].lower() if (int(len(df.columns) / len(orgCols) > len(testName))) else "") + " test"
                axes[x,y].plot(fil.index, fil, label=label)
        if violin:
            axes[x,y].set_xlabel('size of test')
            axes[x,y].set_xticks(range(int(len(df.columns) / len(orgCols))))
            axes[x,y].set_xticklabels([testName[ind % len(testName)] for ind in range(int(len(df.columns) / len(orgCols)))])
        else:
            axes[x,y].set_xlabel('data points')
        axes[x,y].set_title(col)
        axes[x,y].set_ylabel('time (s)')
        axes[x,y].grid(visible=True)
    handles, labels = axes[0,0].get_legend_handles_labels()
    fig.legend(handles, labels, loc='center',
               bbox_to_anchor=(0.5, 0.485), ncols=6)
    plt.savefig(subdir + os.sep + 'Plot' + file.removesuffix('.csv'))
    # plt.show()

def plotParData():
    plt.close("all")
    for j in range(0, int(len(df.columns) / len(orgCols) / len(threadNums))):
        fig, axes = plt.subplots(nrows=2, ncols=3, figsize=(gW, gH), layout='constrained',
                                 gridspec_kw={'wspace': 0.05, 'hspace': 0.1})
        fig.suptitle('Plot' + testName[j % len(testName)] + file.removesuffix('.csv'), fontsize='xx-large')
        for col in orgCols:
            y = orgCols.index(col) % 3
            x = int(orgCols.index(col) / 3)
            violin = False

            axes[x,y].set_yscale('log')

            for i in range(j*len(threadNums), (j + 1)*len(threadNums)):
                fil = df[df > 0].filter(like=col).iloc[:, i].dropna()
                fil /= len(df.filter(like='[mutatePopulationTimer]').iloc[:, i].dropna())
                fil *= gens * rounds

                if len(fil) > 100:
                    axes[x,y].violinplot(fil, positions=[i - j * len(threadNums)])
                    violin = True
                else:
                    axes[x,y].plot(fil.index, fil, label=testName[j % len(testName)] + " test " + str(threadNums[i % len(threadNums)]) + 'T')

            if violin:
                axes[x,y].set_xlabel('number of threads')
                axes[x,y].set_xticks(range(len(threadNums)))
                axes[x,y].set_xticklabels([str(t) + 'T' for t in threadNums])
            else:
                axes[x,y].set_xlabel('data points')
            axes[x,y].set_title(col)
            axes[x,y].set_ylabel('time (s)')
            axes[x,y].grid(visible=True)

        handles, labels = axes[0,0].get_legend_handles_labels()
        fig.legend(handles, labels, loc='center',
               bbox_to_anchor=(0.5, 0.485), ncols=6)
        plt.savefig(subdir + os.sep + testName[j % len(testName)] + file.removesuffix('.csv'))
        # plt.show()

def plotData():
    fig = plt.figure(figsize=(gW, gH), num=file, layout='constrained')
    fig.suptitle(file.removesuffix('.csv'), fontsize='x-large')
    subfigs = fig.subfigures(1, 2, wspace=0.06, width_ratios=[1., 1.])
    axes0 = subfigs[0].subplots(2, 3,
                                gridspec_kw={'wspace': 0.05, 'hspace': 0.1})
    subfigs[0].suptitle('Median', fontsize='x-large')
    subfigs[0].set_facecolor('#e8e6f2')
    axes1 = subfigs[1].subplots(2, 3,
                                gridspec_kw={'wspace': 0.05, 'hspace': 0.1})
    subfigs[1].suptitle('Mean', fontsize='x-large')
    subfigs[1].set_facecolor('#efe6f2')
    for col in orgCols:
        y = orgCols.index(col) % 3
        x = int(orgCols.index(col) / 3)
        for j in range(0, int(len(df.columns) / len(orgCols) / len(threadNums))):

            for key in seqDict:
                if file.removeprefix('Par').startswith(key):
                    seqFile = key

            seqFil = seqDict[seqFile].filter(like=col).iloc[:, j].dropna()
            seqFil /= len(seqDict[seqFile].filter(like='[mutatePopulationTimer]').iloc[:, j].dropna())
            seqFil *= gens * rounds

            seqMedian = pd.Series(seqFil.median(), index=[str(t) + 'T' for t in threadNums])
            seqMean = pd.Series(seqFil.mean(), index=[str(t) + 'T' for t in threadNums])
            
            medianList = []
            meanList = []
            for i in range(j*len(threadNums), (j + 1)*len(threadNums)):
                fil = df[df > 0].filter(like=col).iloc[:, i].dropna()
                fil /= len(df.filter(like='[mutatePopulationTimer]').iloc[:, i].dropna())
                fil *= gens * rounds
                medianList.append(fil.median())
                meanList.append(fil.mean())
            medianSeries = pd.Series(index=[str(t) + 'T' for t in threadNums], data=medianList)
            meanSeries = pd.Series(index=[str(t) + 'T' for t in threadNums], data=meanList)

            axes0[x,y].set_yscale('log')
            axes0[x,y].plot(medianSeries, label=testName[j % len(testName)] + " test PAR")
            axes0[x,y].plot(seqMedian, label=testName[j % len(testName)] + " test SEQ")
            axes0[x,y].grid(visible=True)
            axes0[x,y].set_title(col, fontsize='large')
            axes0[x,y].set_xlabel('number of threads')
            axes0[x,y].set_ylabel('time (s)')

            axes1[x,y].set_yscale('log')
            axes1[x,y].plot(meanSeries, label=testName[j % len(testName)] + " test PAR")
            axes1[x,y].plot(seqMean, label=testName[j % len(testName)] + " test SEQ")
            axes1[x,y].grid(visible=True)
            axes1[x,y].set_title(col, fontsize='large')
            axes1[x,y].set_xlabel('number of threads')
            axes1[x,y].set_ylabel('time (s)')

    handles, labels = axes0[0,0].get_legend_handles_labels()
    fig.legend(handles[:len(testName) * 2], labels[:len(testName) * 2], loc='center',
               bbox_to_anchor=(0.5, 0.472), ncols=6)
    plt.savefig(subdir + os.sep + 'M' + file.removesuffix('.csv'))
    # plt.show()

def finalPlot():
    fig = plt.figure(figsize=(gW, gH), num=file, layout='constrained')
    fig.suptitle('Final comparison', fontsize='x-large')
    subfigs = fig.subfigures(2, 1, hspace=0.1, height_ratios=[1., 1.])
    axes0 = subfigs[0].subplots(1, 5,
                                gridspec_kw={'wspace': 0.05, 'hspace': 0.1})
    subfigs[0].suptitle('Median', fontsize='x-large')
    subfigs[0].set_facecolor('#e8e6f2')
    axes1 = subfigs[1].subplots(1, 5,
                                gridspec_kw={'wspace': 0.05, 'hspace': 0.1})
    subfigs[1].suptitle('Mean', fontsize='x-large')
    subfigs[1].set_facecolor('#efe6f2')
    prop_cycle = plt.rcParams['axes.prop_cycle']
    prop_cycle = prop_cycle.concat(cycler.cycler('color', ["#01b7b7"]))
    x = 0
    for test in seqDict:
        seqDf = seqDict[test].filter(like='[' + test.removesuffix('Test').casefold()).dropna(how="all")
        seqDf = seqDf[seqDf > 0]
        seqSer = pd.concat([seqDf[col].dropna() for col in seqDf], ignore_index=True)
        seqMut = seqDict[test].filter(like='[mutatePopulationTimer]').dropna(how='all')
        seqMutNum = len(pd.concat([seqMut[col] for col in seqMut]))
        seqSer /= seqMutNum
        seqSer *= (len(seqDf.columns) * rounds * gens)
        seqMedian = pd.Series(seqSer.median(), index=[str(t) + 'T' for t in threadNums])
        seqMean = pd.Series(seqSer.mean(), index=[str(t) + 'T' for t in threadNums])
        
        axes0[x].set_prop_cycle(prop_cycle)
        axes0[x].plot(seqMedian, label='Seq')
        axes0[x].set_title(test, fontsize='large')
        axes0[x].set_xlabel('number of threads')
        axes0[x].set_ylabel('time (s)')

        axes1[x].set_prop_cycle(prop_cycle)
        axes1[x].plot(seqMean, label='Seq')
        axes1[x].set_title(test, fontsize='large')
        axes1[x].set_xlabel('number of threads')
        axes1[x].set_ylabel('time (s)')

        for parTest in parDict:
            parDf = parDict[parTest][test].filter(like='[' + test.removesuffix('Test').casefold()).dropna(how="all")
            parDf = parDf[parDf > 0]
            mutDf = parDict[parTest][test].filter(like='[mutatePopulationTimer]').dropna(how="all")

            medianList = []
            meanList = []
            for t in range(len(threadNums)):
                parSer = pd.concat([parDf[col].dropna() for col in parDf.columns if (parDf.columns.get_loc(col) % len(threadNums) == t)], ignore_index=True)
                genNum = len(pd.concat([mutDf[col].dropna() for col in mutDf.columns if (mutDf.columns.get_loc(col) % len(threadNums) == t)], ignore_index=True))
                parSer /= genNum
                parSer *= (len([col for col in parDf.columns if (parDf.columns.get_loc(col) % len(threadNums) == t)]) * rounds * gens)
                medianList.append(parSer.median())
                meanList.append(parSer.mean())
            medianSeries = pd.Series(index=[str(t) + 'T' for t in threadNums], data=medianList)
            meanSeries = pd.Series(index=[str(t) + 'T' for t in threadNums], data=meanList)

            axes0[x].plot(medianSeries, label='Par' + parTest)
            axes0[x].grid(visible=True)

            axes1[x].plot(meanSeries, label='Par' + parTest)
            axes1[x].grid(visible=True)

        x += 1

    handles, labels = axes0[0].get_legend_handles_labels()
    fig.legend(handles, labels, loc='center',
               bbox_to_anchor=(0.5, 0.485), ncols=len(parDict) + 1)
    plt.savefig('FinalComp')
    # plt.show()

for subdir, dirs, files in os.walk('.'):
    pDict = {}

    excelFile = subdir + os.sep + "data.xlsx"
    writer = pd.ExcelWriter(excelFile, engine="xlsxwriter")

    for file in files:
        filename = subdir + os.sep + file

        if filename.endswith(".csv"):
            print(filename)

            sheetName = file.removesuffix('.csv')

            df = pd.read_csv(filename)

            orgCols = [col.removeprefix('[').removesuffix('r]') for col in df.columns if col.endswith(']')]

            if file.startswith('Par') and len(seqDict.keys()) > 0:
                pDict[file.removeprefix('Par').removesuffix(subdir.removeprefix('./par') + '.csv')] = df.copy(deep=True)
                plotData()
                plotParData()
                if (int(len(df.columns) / len(orgCols) > len(testName) * len(threadNums))):
                    index = pd.MultiIndex.from_product([orgCols, testType, testName, threadNums])
                    rowToDel = 4
                else:
                    index = pd.MultiIndex.from_product([orgCols, testName, threadNums])
                    rowToDel = 3
            else:
                seqDict[file.removeprefix('Seq').removesuffix('.csv')] = df.copy(deep=True)
                plotSeqData()
                if (int(len(df.columns) / len(orgCols) > len(testName))):
                    index = pd.MultiIndex.from_product([orgCols, testType, testName])
                    rowToDel = 3
                else:
                    index = pd.MultiIndex.from_product([orgCols, testName])
                    rowToDel = 2
            df.columns=index
            df.index += 1
            df.to_excel(writer, sheet_name=sheetName)
            writer.sheets[sheetName].set_row(rowToDel, None, None, {"hidden": True})
    
    writer.close()
    if(subdir.startswith('./par')):
        parDict[subdir.removeprefix('./par')] = pDict

finalPlot()