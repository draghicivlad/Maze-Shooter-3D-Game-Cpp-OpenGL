#pragma once
#include <vector>
#include <stack>
#include <time.h>
#include <iostream>

using namespace std;

namespace implemented
{
    class MazeGenerator
    {
    public:
        int getNBH(int type)
        {
            int count = 0;
            if ((type & 1) != 0)
                count++;
            if ((type & 2) != 0)
                count++;
            if ((type & 4) != 0)
                count++;
            if ((type & 8) != 0)
                count++;

            return count;
        }

        vector<vector<int>> GenerateMaze(int sizeX, int sizeY, int noEnemies)
        {
            vector<vector<int>> mazeMatrix(sizeY, vector<int>(sizeX));
            vector<vector<int>> visitedMatrix(sizeY, vector<int>(sizeX));

            stack <pair<int, int>> myStack;
            srand(time(NULL));

            pair<int, int> initialPosition = { 0, 0 };
            visitedMatrix[0][0] = 1;
            myStack.push(initialPosition);

            int stanga = 1;
            int sus = 2;
            int dreapta = 4;
            int jos = 8;

            while (!myStack.empty())
            {
                pair<int, int> currentPosition = myStack.top();

                //mazeMatrix[currentPosition.first][currentPosition.second] = visitedMatrix
                 //   [currentPosition.first][currentPosition.second] = 1;

                //cout << currentPosition.first << " - " << currentPosition.second << endl;

                vector<pair<pair<int, int>, pair<int, int>>> validPositions;
                int currentCellType = mazeMatrix[currentPosition.first][currentPosition.second];

                //int aux = rand() % 10;
 
                if (currentPosition.first > 0)
                    if ((!visitedMatrix[currentPosition.first - 1][currentPosition.second]))
                        validPositions.push_back({ { currentPosition.first - 1, currentPosition.second }, {sus, jos} });

                if (currentPosition.first < sizeY - 1)
                    if ((!visitedMatrix[currentPosition.first + 1][currentPosition.second]) )
                        validPositions.push_back({ { currentPosition.first + 1, currentPosition.second }, {jos, sus} });

                if (currentPosition.second > 0)
                    if ((!visitedMatrix[currentPosition.first][currentPosition.second - 1]) )
                        validPositions.push_back({ { currentPosition.first, currentPosition.second - 1}, {stanga, dreapta} });

                if (currentPosition.second < sizeX - 1)
                    if ((!visitedMatrix[currentPosition.first][currentPosition.second + 1]) )
                        validPositions.push_back({ { currentPosition.first, currentPosition.second + 1 }, {dreapta, stanga} });

                if (validPositions.size() == 0) {
                    myStack.pop();
                    continue;
                }

                int indexRandom = rand() % validPositions.size();

                pair<int, int> nextPosition = validPositions[indexRandom].first;
                auto type = validPositions[indexRandom].second;

                mazeMatrix[currentPosition.first][currentPosition.second] += type.first;
                mazeMatrix[nextPosition.first][nextPosition.second] += type.second;

                visitedMatrix[nextPosition.first][nextPosition.second] = 1;
                myStack.push(nextPosition);
            }

            return mazeMatrix;
        }

        /*
        vector<vector<int>> GenerateMaze(int sizeX, int sizeY, int noEnemies)
        {
            vector<vector<int>> mazeMatrix(sizeY, vector<int>(sizeX));
            vector<vector<int>> visitedMatrix(sizeY, vector<int>(sizeX));

            stack <pair<int, int>> myStack;
            srand(time(NULL));

            pair<int, int> initialPosition = {0, 0};
            myStack.push(initialPosition);

            int stanga = 1;
            int sus = 2;
            int dreapta = 4;
            int jos = 8;

            while (!myStack.empty())
            {
                pair<int, int> currentPosition = myStack.top();

                //mazeMatrix[currentPosition.first][currentPosition.second] = visitedMatrix
                 //   [currentPosition.first][currentPosition.second] = 1;

                cout << currentPosition.first << " - " << currentPosition.second << endl;

                vector<pair<pair<int, int>, pair<int, int>>> validPositions;
                int currentCellType = mazeMatrix[currentPosition.first][currentPosition.second];

                int noWalls = 2 + rand() % 2;

                if (currentPosition.first > 0)
                    if((!visitedMatrix[currentPosition.first - 1][currentPosition.second]) && ((currentCellType & sus) == 0) && (getNBH(currentCellType) < noWalls))
                        validPositions.push_back({ { currentPosition.first - 1, currentPosition.second }, {sus, jos} });

                if (currentPosition.first < sizeY - 1)
                    if ((!visitedMatrix[currentPosition.first + 1][currentPosition.second]) && ((currentCellType & jos) == 0) && (getNBH(currentCellType) < 2))
                        validPositions.push_back({ { currentPosition.first + 1, currentPosition.second }, {jos, sus} });

                if (currentPosition.second > 0)
                    if ((!visitedMatrix[currentPosition.first][currentPosition.second - 1]) && ((currentCellType & stanga) == 0) && (getNBH(currentCellType) < noWalls))
                        validPositions.push_back({ { currentPosition.first, currentPosition.second - 1}, {stanga, dreapta} });

                if (currentPosition.second < sizeX - 1)
                    if ((!visitedMatrix[currentPosition.first][currentPosition.second + 1]) && ((currentCellType & dreapta) == 0) && (getNBH(currentCellType) < 2))
                        validPositions.push_back({ { currentPosition.first, currentPosition.second + 1 }, {dreapta, stanga} });

                if (validPositions.size() == 0) {
                    myStack.pop();
                    visitedMatrix[currentPosition.first][currentPosition.second] = 1;
                    continue;
                }

                int indexRandom = rand() % validPositions.size();

                pair<int, int> nextPosition = validPositions[indexRandom].first;
                auto type = validPositions[indexRandom].second;

                mazeMatrix[currentPosition.first][currentPosition.second] += type.first;
                mazeMatrix[nextPosition.first][nextPosition.second] += type.second;

                myStack.push(nextPosition);
            }

            return mazeMatrix;
        }
        */
    };
}
