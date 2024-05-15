#ifndef GRAPH_H
#define GRAPH_H

#include <QVector>
#include <QMap>
#include <QString>
#include <QQueue>
#include "Tree.h"

struct ElementOfMatrix
{
    int value;
    bool isNonAvailible = false;
};

struct ZeroElementInfo
{
    int rowZE;
    int columZE;
    int valueZE;

    ZeroElementInfo() {};

    ZeroElementInfo(int row, int colum, int value)
    {
        rowZE = row;
        columZE = colum;
        valueZE = value;
    };
};

template <typename T>
class Graph
{
private:

    QVector<T> VertexList;

    QVector<QVector<int>> AdjMatrix;

    int GetVertexIndex(T vertex)
    {
        int result = -1;

        for (int i = 0; i < VertexList.size() && result == -1; ++i)
        {
            if (VertexList[i] == vertex)
            {
                result = i;
            }
        }

        return result;
    }

    QVector<T> GetNeibors(T vertex)
    {
        QVector<T> Neibors;

        int index = GetVertexIndex(vertex);

        if (index == -1)
        {
            return Neibors;
        }

        for (int i = 0; i < VertexList.size(); ++i)
        {
            if (AdjMatrix[index][i] > 0)
            {
                Neibors.push_back(VertexList[i]);
            }
        }

        return Neibors;
    }

    void DepthReadHelper(T vertex, bool* visitVertex, QQueue<T>& VertexPassedList)
    {
        VertexPassedList.push_back(vertex);

        visitVertex[GetVertexIndex(vertex)] = true;

        QVector<T> neibors = GetNeibors(vertex);

        for (int i = 0; i < neibors.size(); i++)
        {
            if (!visitVertex[GetVertexIndex(neibors[i])])
            {
                DepthReadHelper(neibors[i], visitVertex, VertexPassedList);
            }
        }

    }

    void WidthReadHelper(T vertex, bool* visitVertex, QQueue<T>& queueHelper, QQueue<T>& VertexPassedList)
    {
        int index = GetVertexIndex(vertex);

        if (!visitVertex[index])
        {
            queueHelper.push_back(vertex);
            VertexPassedList.push_back(vertex);
            visitVertex[index] = true;
        }

        QVector<T> neibors = GetNeibors(vertex);

        queueHelper.pop_front();

        for (int i = 0; i < neibors.size(); i++)
        {
            int curVertex = neibors[i];

            if (!visitVertex[GetVertexIndex(curVertex)])
            {
                queueHelper.push_back(curVertex);
                VertexPassedList.push_back(vertex);
                visitVertex[GetVertexIndex(curVertex)] = true;
            }
        }

        if (!queueHelper.empty())
        {
            WidthReadHelper(queueHelper.front(), visitVertex, queueHelper, VertexPassedList);
        }
    }

    bool CheckAllPathsExist()
    {
        bool AllPathsExist = true;

        for (int i = 0; i < VertexList.size() && AllPathsExist; i++)
        {
            for (int j = 0; j < VertexList.size() && AllPathsExist; j++)
            {
                if (i != j && (AdjMatrix[i][j] == 0 || AdjMatrix[i][j] == -1))
                {
                    AllPathsExist = false;
                }
            }
        }

        if (VertexList.size() < 3) AllPathsExist = false;

        return AllPathsExist;
    }

public:

    QVector<T> GetVertexList() {return VertexList;}

    void AddVertex(T vertex)
    {
        VertexList.push_back(vertex);

        int newSize = VertexList.size();

        AdjMatrix.push_back(QVector<int>(newSize));

        for (int i = 0; i < newSize; ++i)
        {
            AdjMatrix[newSize - 1][i] = 0;
        }

        for (int i = 0; i < newSize - 1; ++i)
        {
            AdjMatrix[i].append(0);
        }
    }

    int RemoveVertex(T vertex)
    {
        int rIndex = GetVertexIndex(vertex);

        if (rIndex == -1)
        {
            return -1;
        }

        int removedCount = 0;

        //auto rIter = AdjMatrix.begin();

        //rIter += rIndex;

        for (int i = 0; i < AdjMatrix.size(); i++)
        {
            //auto rIterInner = AdjMatrix[i].begin();

            //rIterInner += rIndex;
            removedCount++;

            AdjMatrix[i].remove(rIndex);
        }

        AdjMatrix.remove(rIndex);

        VertexList.remove(rIndex);

        return removedCount;
    }

    bool SetEdge(T vertex1, T vertex2, int weight)
    {
        int sourceIndex = GetVertexIndex(vertex1);
        int targetIndex = GetVertexIndex(vertex2);

        if (sourceIndex == -1 || targetIndex == -1 || sourceIndex == targetIndex)
        {
            return false;
        }

        AdjMatrix[sourceIndex][targetIndex] = weight;

        return true;
    }

    bool RemoveEdge(T vertex1, T vertex2)
    {
        int sourceIndex = GetVertexIndex(vertex1);
        int targetIndex = GetVertexIndex(vertex2);

        if (sourceIndex == -1 || targetIndex == -1 || sourceIndex == targetIndex)
        {
            return false;
        }

        AdjMatrix[sourceIndex][targetIndex] = 0;

        return true;
    }

    int GetWeight (T vertex1, T vertex2)
    {
        int sourceIndex = GetVertexIndex(vertex1);
        int targetIndex = GetVertexIndex(vertex2);

        if (sourceIndex == -1 || targetIndex == -1)
        {
            return -1;
        }

        if (sourceIndex == targetIndex)
        {
            return 0;
        }

        return AdjMatrix[sourceIndex][targetIndex];
    }

    QQueue<T> DepthRead(T startvertex)
    {
        QQueue<T> VertexPassed;

        if (GetVertexIndex(startvertex) == -1)
        {
            return VertexPassed;
        }

        bool* visitedVertexes = new bool[VertexList.size()] {};

        DepthReadHelper(startvertex, visitedVertexes, VertexPassed);

        delete[] visitedVertexes;
    }

    QQueue<T> WidthRead(T startvertex)
    { 
        QQueue<T> VertexPassed;

        if (GetVertexIndex(startvertex) == -1)
        {
            return VertexPassed;
        }

        bool* visitedVertexes = new bool[VertexList.size()] {};

        WidthReadHelper(startvertex, visitedVertexes, VertexPassed);

        delete[] visitedVertexes;

    }

    void Clear()
    {
        VertexList.clear();
        AdjMatrix.clear();
    }

    QMap<T, T> FindShortestPath(T vertex1, T vertex2);

    QMap<T, T> KommivoyagerTask()
    {

        if (!CheckAllPathsExist())
        {
            return QMap<T, T>();
        }

        QVector<QVector<ElementOfMatrix>> KomAdjMatrix1
                (AdjMatrix.size(), QVector<ElementOfMatrix>(AdjMatrix.size()));

        for (int i = 0; i < AdjMatrix.size(); i++)
        {
            for (int j = 0; j < AdjMatrix.size(); j++)
            {
                if (i == j)
                {
                    KomAdjMatrix1[i][i].value = 0;
                    KomAdjMatrix1[i][i].isNonAvailible = true;
                }
                else
                {
                    KomAdjMatrix1[i][j].value = AdjMatrix[i][j];
                }
            }
        }

        int lowerBorder = 0;

        int matrixSize = KomAdjMatrix1.size();

        int startMinValue = INT16_MAX + 1;

        //Finding mins in rows + reduction
        QVector<int> minValues1 (matrixSize);
        QVector<int> minValues2 (matrixSize);

        for(int i = 0; i < minValues1.size(); i++)
        {
            minValues1[i] = startMinValue;
        }
        for(int i = 0; i < minValues2.size(); i++)
        {
            minValues2[i] = startMinValue;
        }


        for (int i = 0; i < matrixSize; i++)
        {
            for (int j = 0; j < matrixSize; j++)
            {
                if (!KomAdjMatrix1[i][j].isNonAvailible)
                {
                    if (minValues1[i] > KomAdjMatrix1[i][j].value)
                    {
                        minValues1[i] = KomAdjMatrix1[i][j].value;
                    }
                }
            }
        }

        for (int i = 0; i < matrixSize; i++)
        {
            for (int j = 0; j < matrixSize; j++)
            {
                if (!KomAdjMatrix1[i][j].isNonAvailible)
                {
                    KomAdjMatrix1[i][j].value -= minValues1[i];
                }
            }
        }

        //Finding mins in colums + reduction

        for (int i = 0; i < matrixSize; i++)
        {
            for (int j = 0; j < matrixSize; j++)
            {
                if (!KomAdjMatrix1[j][i].isNonAvailible)
                {
                    if (minValues2[i] > KomAdjMatrix1[j][i].value)
                    {
                        minValues2[i] = KomAdjMatrix1[j][i].value;
                    }
                }
            }
        }

        for (int i = 0; i < matrixSize; i++)
        {
            for (int j = 0; j < matrixSize; j++)
            {
                if (!KomAdjMatrix1[j][i].isNonAvailible)
                {
                    KomAdjMatrix1[j][i].value -= minValues2[i];
                }
            }
        }

        //Calculating new lower border
        for(int i = 0; i < minValues1.size(); i++)
        {
            lowerBorder += minValues1[i];
        }

        for(int i = 0; i < minValues2.size(); i++)
        {
            lowerBorder += minValues2[i];
        }

        Tree<QVector<QVector<ElementOfMatrix>>, T>* WorkTreeBase = new Tree<QVector<QVector<ElementOfMatrix>>, T>;

        QMap<T, T> SolvationPath;

        WorkTreeBase->set_data(KomAdjMatrix1);
        WorkTreeBase->set_border(lowerBorder);
        WorkTreeBase->set_path(SolvationPath);

        while (WorkTreeBase->find_min().second->get_path().size() != KomAdjMatrix1.size())
        {
            Tree<QVector<QVector<ElementOfMatrix>>, T>* WorkTree = WorkTreeBase->find_min().second;

            QVector<QVector<ElementOfMatrix>> KomAdjMatrix = WorkTree->get_data();

            SolvationPath = WorkTree->get_path();

            lowerBorder = WorkTree->get_border();

            int matrixSize = AdjMatrix.size();

            //Finding ZeroElement with highest rating 7 8
            ZeroElementInfo ZeroElementToDelete;

            ZeroElementToDelete.valueZE = 0;

            for (int i = 0; i < matrixSize; i++)
            {
                for (int j = 0; j < matrixSize; j++)
                {
                    if (!KomAdjMatrix[i][j].isNonAvailible && KomAdjMatrix[i][j].value == 0)
                    {
                        int minRow = startMinValue;
                        int minColum = startMinValue;

                        for (int k = 0; k < matrixSize; k++)
                        {
                            if (!KomAdjMatrix[i][k].isNonAvailible &&
                                    minRow > KomAdjMatrix[i][k].value)
                            {
                                minRow = KomAdjMatrix[i][k].value;
                            }
                        }

                        for (int k = 0; k < matrixSize; k++)
                        {
                            if (!KomAdjMatrix[k][j].isNonAvailible &&
                                    minColum > KomAdjMatrix[k][j].value)
                            {
                                minColum = KomAdjMatrix[k][j].value;
                            }
                        }

                        if (minRow == startMinValue)
                        {
                            minRow = 0;
                        }
                        if (minColum == startMinValue)
                        {
                            minColum = 0;
                        }

                        if (ZeroElementToDelete.valueZE <= minRow + minColum)
                        {
                            ZeroElementToDelete.valueZE = minRow + minColum;
                            ZeroElementToDelete.rowZE = i;
                            ZeroElementToDelete.columZE = j;
                        }
                    }
                }
            }

            int lowerBorderNotSelected = lowerBorder /*+ ZeroElementToDelete.valueZE*/;

            QVector<QVector<ElementOfMatrix>> KomAdjMatrixOther = KomAdjMatrix;

            KomAdjMatrixOther[ZeroElementToDelete.rowZE][ZeroElementToDelete.columZE].isNonAvailible=true;

            for(int i = 0; i < minValues1.size(); i++)
            {
                minValues1[i] = startMinValue;
            }
            for(int i = 0; i < minValues2.size(); i++)
            {
                minValues2[i] = startMinValue;
            }

            //Reduction to matrix with path not included
            //Finding mins in rows + reduction
            for (int i = 0; i < matrixSize; i++)
            {
                for (int j = 0; j < matrixSize; j++)
                {
                    if (!KomAdjMatrixOther[i][j].isNonAvailible)
                    {
                        if (minValues1[i] > KomAdjMatrixOther[i][j].value)
                        {
                            minValues1[i] = KomAdjMatrixOther[i][j].value;
                        }
                    }
                }
            }
            for (int i = 0; i < matrixSize; i++)
            {
                for (int j = 0; j < matrixSize; j++)
                {
                    if (!KomAdjMatrixOther[i][j].isNonAvailible)
                    {
                        KomAdjMatrixOther[i][j].value -= minValues1[i];
                    }
                }
            }

            //Finding mins in colums + reduction
            for (int i = 0; i < matrixSize; i++)
            {
                for (int j = 0; j < matrixSize; j++)
                {
                    if (!KomAdjMatrixOther[j][i].isNonAvailible)
                    {
                        if (minValues2[i] > KomAdjMatrixOther[j][i].value)
                        {
                            minValues2[i] = KomAdjMatrixOther[j][i].value;
                        }
                    }
                }
            }
            for (int i = 0; i < matrixSize; i++)
            {
                for (int j = 0; j < matrixSize; j++)
                {
                    if (!KomAdjMatrixOther[j][i].isNonAvailible)
                    {
                        KomAdjMatrixOther[j][i].value -= minValues2[i];
                    }
                }
            }

            //Calculating new lower border
            for(int i = 0; i < minValues1.size(); i++)
            {
                lowerBorderNotSelected += minValues1[i];
            }

            for(int i = 0; i < minValues2.size(); i++)
            {
                lowerBorderNotSelected += minValues2[i];
            }

            WorkTree->insert_left(lowerBorderNotSelected, KomAdjMatrixOther, SolvationPath);

            for (int i = 0; i < matrixSize; i++)
            {
                KomAdjMatrix[ZeroElementToDelete.rowZE][i].isNonAvailible = true;
                KomAdjMatrix[i][ZeroElementToDelete.columZE].isNonAvailible = true;
            }
            KomAdjMatrix[ZeroElementToDelete.columZE]
                    [ZeroElementToDelete.rowZE].isNonAvailible = true;

            SolvationPath[VertexList[ZeroElementToDelete.rowZE]] =
                    VertexList[ZeroElementToDelete.columZE];

            for(int i = 0; i < minValues1.size(); i++)
            {
                minValues1[i] = startMinValue;
            }
            for(int i = 0; i < minValues2.size(); i++)
            {
                minValues2[i] = startMinValue;
            }

            //Reduction to matrix with path included
            //Finding mins in rows + reduction
            for (int i = 0; i < matrixSize; i++)
            {
                for (int j = 0; j < matrixSize; j++)
                {
                    if (!KomAdjMatrix[i][j].isNonAvailible)
                    {
                        if (minValues1[i] > KomAdjMatrix[i][j].value)
                        {
                            minValues1[i] = KomAdjMatrix[i][j].value;
                        }
                    }
                }
            }
            for (int i = 0; i < matrixSize; i++)
            {
                for (int j = 0; j < matrixSize; j++)
                {
                    if (!KomAdjMatrix[i][j].isNonAvailible)
                    {
                        KomAdjMatrix[i][j].value -= minValues1[i];
                    }
                }
            }

            //Finding mins in colums + reduction
            for (int i = 0; i < matrixSize; i++)
            {
                for (int j = 0; j < matrixSize; j++)
                {
                    if (!KomAdjMatrix[j][i].isNonAvailible)
                    {
                        if (minValues2[i] > KomAdjMatrix[j][i].value)
                        {
                            minValues2[i] = KomAdjMatrix[j][i].value;
                        }
                    }
                }
            }
            for (int i = 0; i < matrixSize; i++)
            {
                for (int j = 0; j < matrixSize; j++)
                {
                    if (!KomAdjMatrix[j][i].isNonAvailible)
                    {
                        KomAdjMatrix[j][i].value -= minValues2[i];
                    }
                }
            }

            //Calculating new lower border
            for(int i = 0; i < minValues1.size(); i++)
            {
                if (minValues1[i] != startMinValue)
                    lowerBorder += minValues1[i];
            }
            for(int i = 0; i < minValues2.size(); i++)
            {
                if (minValues2[i] != startMinValue)
                    lowerBorder += minValues2[i];
            }

            WorkTree->insert_right(lowerBorder, KomAdjMatrix, SolvationPath);
        }

        SolvationPath = WorkTreeBase->find_min().second->get_path();

        return SolvationPath;
    }
};





#endif // GRAPH_H
