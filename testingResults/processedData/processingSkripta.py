import pandas as pd
import matplotlib.pyplot as plt
import os

threadNums = [1, 2, 4, 8, 16, 32]
testName = ["Small", "Medium", "Large"]
gens = 1000
seqDict = {}
gH = 12
gW = 24
plt.style.use('fivethirtyeight')

def plotSeqData():
    plt.close("all")
    fig, axes = plt.subplots(nrows=2, ncols=3, figsize=(gW, gH), layout='constrained',
                             gridspec_kw={'wspace': 0.05, 'hspace': 0.1})
    fig.suptitle('Plot' + file.removesuffix('Formated.csv'), fontsize='xx-large')
    for col in orgCols:
        y = orgCols.index(col) % 3
        x = int(orgCols.index(col) / 3)
        violin = False

        axes[x,y].set_yscale('log')

        for i in range(0, int(len(df.columns) / len(orgCols))):
            fil = df[df >= 0].filter(like=col).iloc[:, i].dropna()
            fil /= len(df[df >= 0].filter(like='[mutatePopulationTimer]').iloc[:, i].dropna())
            fil *= gens

            if len(fil) > 100:
                axes[x,y].violinplot(fil, positions=[i])
                violin = True
            else:
                axes[x,y].plot(fil.index, fil, label=testName[i % len(testName)] + " test")
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
    fig.legend(handles[0:len(threadNums)], labels[0:len(threadNums)], loc='center',
               bbox_to_anchor=(0.5, 0.485), ncols=len(threadNums))
    plt.savefig(subdir + os.sep + 'Plot' + file.removesuffix('Formated.csv'))
    # plt.show()

def plotParData():
    plt.close("all")
    for j in range(0, int(len(df.columns) / len(orgCols) / len(threadNums))):
        fig, axes = plt.subplots(nrows=2, ncols=3, figsize=(gW, gH), layout='constrained',
                                 gridspec_kw={'wspace': 0.05, 'hspace': 0.1})
        fig.suptitle('Plot' + testName[j % len(testName)] + file.removesuffix('Formated.csv'), fontsize='xx-large')
        for col in orgCols:
            y = orgCols.index(col) % 3
            x = int(orgCols.index(col) / 3)
            violin = False

            axes[x,y].set_yscale('log')

            for i in range(j*len(threadNums), (j + 1)*len(threadNums) - 1):
                fil = df[df >= 0].filter(like=col).iloc[:, i].dropna()
                fil /= len(df[df >= 0].filter(like='[mutatePopulationTimer]').iloc[:, i].dropna())
                fil *= gens

                if len(fil) > 100:
                    axes[x,y].violinplot(fil, positions=[i - j * len(threadNums)])
                    violin = True
                else:
                    axes[x,y].plot(fil.index, fil, label=testName[j % len(testName)] + " test " + str(threadNums[i % len(threadNums)]) + 'T')

            if violin:
                axes[x,y].set_xlabel('number of threads')
                axes[x,y].set_xticks(range(len(threadNums) - 1))
                axes[x,y].set_xticklabels([str(threadNums[t]) + 'T' for t in range(5)])
            else:
                axes[x,y].set_xlabel('data points')
            axes[x,y].set_title(col)
            axes[x,y].set_ylabel('time (s)')
            axes[x,y].grid(visible=True)

        handles, labels = axes[0,0].get_legend_handles_labels()
        fig.legend(handles[0:len(threadNums)], labels[0:len(threadNums)], loc='center',
               bbox_to_anchor=(0.5, 0.485), ncols=len(threadNums))
        plt.savefig(subdir + os.sep + testName[j % len(testName)] + file.removesuffix('Formated.csv'))
        # plt.show()

def plotData():
    fig = plt.figure(figsize=(gW, gH), num=file, layout='constrained')
    fig.suptitle(file.removesuffix('Formated.csv'), fontsize='x-large')
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
            seqFil *= gens

            seqMedian = pd.Series(seqFil.median(), index=[str(threadNums[t]) + 'T' for t in range(5)])
            seqMean = pd.Series(seqFil.mean(), index=[str(threadNums[t]) + 'T' for t in range(5)])
            
            medianList = []
            meanList = []
            for i in range(j*len(threadNums), (j + 1)*len(threadNums) - 1):
                fil = df[df >= 0].filter(like=col).iloc[:, i].dropna()
                fil /= len(df[df >= 0].filter(like='[mutatePopulationTimer]').iloc[:, i].dropna())
                fil *= gens
                medianList.append(fil.median())
                meanList.append(fil.mean())
            medianSeries = pd.Series(index=[str(threadNums[t]) + 'T' for t in range(5)], data=medianList)
            meanSeries = pd.Series(index=[str(threadNums[t]) + 'T' for t in range(5)], data=meanList)

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
    fig.legend(handles[0:len(threadNums)], labels[0:len(threadNums)], loc='center',
               bbox_to_anchor=(0.5, 0.472), ncols=len(threadNums))
    plt.savefig(subdir + os.sep + 'M' + file.removesuffix('Formated.csv'))
    # plt.show()

for subdir, dirs, files in os.walk('.'):
    for file in files:
        filename = subdir + os.sep + file

        if filename.endswith(".csv"):
            print(filename)

            df = pd.read_csv(filename)

            orgCols = [col for col in df.columns if col.endswith(']')]

            if file.startswith('Par') and len(seqDict.keys()) > 0:
                plotData()
                # plotParData()
            else:
                seqDict[file.removeprefix('Seq').removesuffix('Formated.csv')] = df.copy(deep=True)
                # plotSeqData()