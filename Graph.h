#ifndef GRAPH_H
#define GRAPH_H

#include <QVector>
#include <QMap>
#include <QString>
#include <QQueue>
#include "Tree.h"

//Структура данных для элементов применяемая в задаче коммивояжера
struct ElementOfMatrix
{
    int value;
    bool isNonAvailible = false;
};

//Структура данных для нулевых элементов элементов применяемая в задаче коммивояжера
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

//Класс графа, отвечающий за математическую составляющую графа
template <typename T>
class Graph
{
private:

    QVector<T> VertexList; //Вектор названий вершин

    QVector<QVector<int>> AdjMatrix; //Матрица смежности

    //Получение индекса вершины в векторе по названию
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

    //Получение вектора соседних вершин
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

    //Вспомогательная функция для обхода в глубину
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

    //Вспомогательная функция для обхода в ширину
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

    //Проверка существования всех путей для задачи коммивояжера
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

    //Получение вектора вершин
    QVector<T> GetVertexList() {return VertexList;}

    //Добавление новой вершины
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

    //Удаление вершины из вектора
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

    //Установление значения грани
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

    //Сброс значения грани / удаление грани
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

    //Получение значения грани
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

    //Функция чтения графа в глубину
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

    //Функция чтения графа в ширину
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

    //Отчистка вектора названий и матрицы смежности
    void Clear()
    {
        VertexList.clear();
        AdjMatrix.clear();
    }

    //Нахождение кратчайшего пути от одной вершины до другой с применением метода Флойда
    QVector<T> FindShortestPath(T vertex1, T vertex2)
    {
        //Объявление матриц длин и путей
        QVector<QVector<int>> FloAdjMatrix (AdjMatrix.size(), QVector<int>(AdjMatrix.size()));
        QVector<QVector<QVector<int>>> Paths (AdjMatrix.size(), QVector<QVector<int>>(AdjMatrix.size(), QVector<int>(0)));

        int startMinValue = INT16_MAX + 1;
        int matrixSize = AdjMatrix.size();

        //Составление матрицы длин
        for (int i = 0; i < matrixSize; i++)
        {
            for (int j = 0; j < matrixSize; j++)
            {
                if (i == j)
                {
                    FloAdjMatrix[i][i] = 0;
                }
                else
                {
                    if (AdjMatrix[i][j] == 0)
                    {
                        FloAdjMatrix[i][j] = startMinValue;
                    }
                    else
                    {
                        FloAdjMatrix[i][j] = AdjMatrix[i][j];
                    }
                }
            }
        }
        //Составление матрицы путей
        for (int i = 0; i < matrixSize; i++)
        {
            for (int j = 0; j < matrixSize; j++)
            {
                if (FloAdjMatrix[i][j] != 0 && FloAdjMatrix[i][j] != startMinValue)
                {
                    Paths[i][j].append(j);
                }
                else
                {

                }
            }
        }

        //Сравнение путей и определение кратчайших в соответствии и методом Флойда
        for (int i = 0; i < matrixSize; i++){
            for (int v = 0; v < matrixSize; v++) {
                for (int a = 0; a < matrixSize; a++) {
                    for (int b = 0; b < matrixSize; b++) {
                        if (FloAdjMatrix[a][b] > FloAdjMatrix[a][v] + FloAdjMatrix[v][b]) {
                            FloAdjMatrix[a][b] = FloAdjMatrix[a][v] + FloAdjMatrix[v][b];
                            Paths[a][b].append(v);
                        }
                    }
                }
            }
        }

        //Получение кратчайшего пути между выбранными вершинами
        T curPos = vertex1;
        QVector<T> SolvationPath;
        SolvationPath.push_back(curPos);

        for (int i = 0; i < Paths[GetVertexIndex(vertex1)][GetVertexIndex(vertex2)].size(); i++)
        {
            curPos = VertexList[Paths[GetVertexIndex(vertex1)][GetVertexIndex(vertex2)][i]];
            SolvationPath.push_back(curPos);
        }

        return SolvationPath;
    }

    //Решение задачи коммивояжера с применением метода вершин и границ
    QPair<QMap<T, T>, Tree<QVector<QVector<ElementOfMatrix>>, T>*> KommivoyagerTask()
    {
        //Проверка существования всех путей, обязательное условие задачи
        if (!CheckAllPathsExist())
        {
            return QPair<QMap<T, T>, Tree<QVector<QVector<ElementOfMatrix>>, T>*>();
        }

        //Объявление матрицы смежности подготовленной для применения метода
        QVector<QVector<ElementOfMatrix>> KomAdjMatrix1
                (AdjMatrix.size(), QVector<ElementOfMatrix>(AdjMatrix.size()));

        //Составление матрицы смежности
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

        //Получение минимальных значений строк и редукция строк
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

        //Получение минимальных значений столбцов и редукция столбцов
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

        //Вычисление корневой границы
        for(int i = 0; i < minValues1.size(); i++)
        {
            lowerBorder += minValues1[i];
        }

        for(int i = 0; i < minValues2.size(); i++)
        {
            lowerBorder += minValues2[i];
        }

        //Создание дерева решения и установка значений корневой вершины
        Tree<QVector<QVector<ElementOfMatrix>>, T>* WorkTreeBase = new Tree<QVector<QVector<ElementOfMatrix>>, T>;

        QMap<T, T> SolvationPath;

        WorkTreeBase->set_data(KomAdjMatrix1);
        WorkTreeBase->set_border(lowerBorder);
        WorkTreeBase->set_path(SolvationPath);
        WorkTreeBase->set_lastPath("Root");

        //Цикл поиска кратчайшего пути
        //Условие завершения - получение полного пути в вершине с минимальной нижней границей
        while (WorkTreeBase->find_min().second->get_path().size() != matrixSize)
        {
            //Выбор рабочей вершины
            //Рабочая вершина - вершина с наименьшей нижней границей
            Tree<QVector<QVector<ElementOfMatrix>>, T>* WorkTree = WorkTreeBase->find_min().second;

            //Получение данных хранимых в вершине
            QVector<QVector<ElementOfMatrix>> KomAdjMatrix = WorkTree->get_data();

            SolvationPath = WorkTree->get_path();

            lowerBorder = WorkTree->get_border();

            int matrixSize = AdjMatrix.size();

            //Нахождение нулевого элемента с наибольшей оценкой
            //Производится для выбора дальнейшего пути
            //Позиция нулевого элемента отражает пункт отправления и пункт прибытия
            ZeroElementInfo ZeroElementToDelete;

            ZeroElementToDelete.valueZE = 0;

            for (int i = 0; i < matrixSize; i++)
            {
                for (int j = 0; j < matrixSize; j++)
                {
                    //Поиск элемента в матрице смежности со значением 0
                    if (!KomAdjMatrix[i][j].isNonAvailible && KomAdjMatrix[i][j].value == 0)
                    {
                        //Нахождение минимальных значений в соответствующих столбце и строке
                        int minRow = startMinValue;
                        int minColum = startMinValue;

                        for (int k = 0; k < matrixSize; k++)
                        {
                            if (!KomAdjMatrix[i][k].isNonAvailible &&
                                    minRow > KomAdjMatrix[i][k].value && k != j)
                            {
                                minRow = KomAdjMatrix[i][k].value;
                            }
                        }

                        for (int k = 0; k < matrixSize; k++)
                        {
                            if (!KomAdjMatrix[k][j].isNonAvailible &&
                                    minColum > KomAdjMatrix[k][j].value && k != i)
                            {
                                minColum = KomAdjMatrix[k][j].value;
                            }
                        }

                        /*
                        if (minRow == startMinValue)
                        {
                            minRow = 0;
                        }
                        if (minColum == startMinValue)
                        {
                            minColum = 0;
                        }
                        */

                        //Сравнение полученных значений с предыдущим нулевым элементом с наибольшей оценкой
                        if (ZeroElementToDelete.valueZE <= minRow + minColum)
                        {
                            ZeroElementToDelete.valueZE = minRow + minColum;
                            ZeroElementToDelete.rowZE = i;
                            ZeroElementToDelete.columZE = j;
                        }
                    }
                }
            }

            //Составление нижней границы и матрицы смежности для ветви, не включающей выбранный путь
            int lowerBorderNotSelected = lowerBorder + ZeroElementToDelete.valueZE;

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

            //Редукция матрицы, не включающей выбранный путь
            //Получение минимальных значений строк и редукция строк
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

            //Получение минимальных значений столбцов и редукция столбцов
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

            //Вычисление нижней границы матрицы, не включающей выбранный путь
            /*
            for(int i = 0; i < minValues1.size(); i++)
            {
                if (minValues1[i] != startMinValue)
                    lowerBorderNotSelected += minValues1[i];
            }

            for(int i = 0; i < minValues2.size(); i++)
            {
                if (minValues2[i] != startMinValue)
                    lowerBorderNotSelected += minValues2[i];
            }
            */

            //Создание ветви решения, где был исключен выбранный путь
            QString lastPath = "!" + VertexList[ZeroElementToDelete.rowZE] + " - " + VertexList[ZeroElementToDelete.columZE];
            WorkTree->insert_left(lowerBorderNotSelected, KomAdjMatrixOther, SolvationPath, lastPath);

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

            //Редукция матрицы, включающей выбранный путь
            //Получение минимальных значений строк и редукция строк
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

            //Получение минимальных значений столбцов и редукция столбцов
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

            //Вычисление нижней границы матрицы, включающей выбранный путь
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

            //Создание ветви решения, где был включен выбранный путь
            lastPath = VertexList[ZeroElementToDelete.rowZE] + " - " + VertexList[ZeroElementToDelete.columZE];
            WorkTree->insert_right(lowerBorder, KomAdjMatrix, SolvationPath, lastPath);
        }

        //Получение кратчайайшего пути и возврат полученного значения
        SolvationPath = WorkTreeBase->find_min().second->get_path();

        QPair<QMap<T, T>, Tree<QVector<QVector<ElementOfMatrix>>, T>*> pairToReturn (SolvationPath, WorkTreeBase);

        return pairToReturn;
    }
};

#endif // GRAPH_H
