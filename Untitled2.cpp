#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <cstdlib>
#include  <ncurse.h>

using namespace std;

bool gameOver;
const int width = 20;
const int height = 10;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

char maze[10][20] = {
    "###################",
    "#                 #",
    "# ### ##### ### # #",
    "# #   #     #   # #",
    "# # # # ### # # # #",
    "#   # #       #   #",
    "### # ##### # ### #",
    "#   #       #     #",
    "# # # ### # # ### #",
    "#   #     # #     #",
};

int ghostX[3], ghostY[3];
eDirection ghostDir[3];

void Setup()
{
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;

    for (int i = 0; i < 3; i++)
    {
        ghostX[i] = rand() % width;
        ghostY[i] = rand() % height;
        ghostDir[i] = static_cast<eDirection>(1 + rand() % 4);
    }
}

void Draw()
{
    system("cls");
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (j == 0 || j == width - 1)
                cout << "#";
            if (i == y && j == x)
                cout << "O";
            else if (i == fruitY && j == fruitX)
                cout << "F";
            else if (maze[i][j] == '#')
                cout << "#";
            else
            {
                bool print = false;
                for (int k = 0; k < nTail; k++)
                {
                    if (tailX[k] == j && tailY[k] == i)
                    {
                        cout << "o";
                        print = true;
                    }
                }
                for (int k = 0; k < 3; k++)
                {
                    if (ghostX[k] == j && ghostY[k] == i)
                    {
                        cout << "X";
                        print = true;
                    }
                }
                if (!print)
                    cout << " ";
            }
        }
        cout << endl;
    }
    cout << "Score:" << score << endl;
}

void Input()
{
    if (_kbhit())
    {
        switch (_getch())
        {
        case 'a':
            dir = LEFT;
            break;
        case 'd':
            dir = RIGHT;
            break;
        case 'w':
            dir = UP;
            break;
        case 's':
            dir = DOWN;
            break;
        case 'x':
            gameOver = true;
            break;
        }
    }
}

bool IsWall(int posX, int posY)
{
    return maze[posY][posX] == '#';
}

void MoveGhost(int ghostIndex)
{
    int prevGhostX = ghostX[ghostIndex];
    int prevGhostY = ghostY[ghostIndex];

    // Simpan posisi sebelumnya
    int prevX = ghostX[ghostIndex];
    int prevY = ghostY[ghostIndex];

    // Pilih arah pergerakan secara acak
    ghostDir[ghostIndex] = static_cast<eDirection>(1 + rand() % 4);

    // Gerakkan ghost berdasarkan arah yang dipilih
    switch (ghostDir[ghostIndex])
    {
    case LEFT:
        ghostX[ghostIndex]--;
        break;
    case RIGHT:
        ghostX[ghostIndex]++;
        break;
    case UP:
        ghostY[ghostIndex]--;
        break;
    case DOWN:
        ghostY[ghostIndex]++;
        break;
    default:
        break;
    }

    // Periksa apakah ghost bertabrakan dengan tembok
    if (IsWall(ghostX[ghostIndex], ghostY[ghostIndex]))
    {
        // Jika iya, kembalikan posisi sebelumnya dan ubah arahnya
        ghostX[ghostIndex] = prevX;
        ghostY[ghostIndex] = prevY;
    }
}

void Logic()
{
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < nTail; i++)
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir)
    {
    case LEFT:
        if (x > 0 && !IsWall(x - 1, y)) {
            x--;
        }
        break;
    case RIGHT:
        if (x < width - 1 && !IsWall(x + 1, y)) {
            x++;
        }
        break;
    case UP:
        if (y > 0 && !IsWall(x, y - 1)) {
            y--;
        }
        break;
    case DOWN:
        if (y < height - 1 && !IsWall(x, y + 1)) {
            y++;
        }
        break;
    default:
        break;
    }

    if (x >= width) x = 0; else if (x < 0) x = width - 1;
    if (y >= height) y = 0; else if (y < 0) y = height - 1;

    for (int i = 0; i < nTail; i++)
    {
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;
    }

      for (int i = 0; i < 3; i++)
    {
        if (x == ghostX[i] && y == ghostY[i])
            gameOver = true;
            
             // Gerakkan ghost
        MoveGhost(i);



        if (IsWall(ghostX[i], ghostY[i]))
        {
            ghostDir[i] = static_cast<eDirection>(1 + rand() % 4);
        }

        switch (ghostDir[i])
        {
        case LEFT:
            if (ghostX[i] > 0 && !IsWall(ghostX[i] - 1, ghostY[i])) {
                ghostX[i]--;
            }
            break;
        case RIGHT:
            if (ghostX[i] < width - 1 && !IsWall(ghostX[i] + 1, ghostY[i])) {
                ghostX[i]++;
            }
            break;
        case UP:
            if (ghostY[i] > 0 && !IsWall(ghostX[i], ghostY[i] - 1)) {
                ghostY[i]--;
            }
            break;
        case DOWN:
            if (ghostY[i] < height - 1 && !IsWall(ghostX[i], ghostY[i] + 1)) {
                ghostY[i]++;
            }
            break;
        default:
            break;
        }
    }

    if (gameOver) {
        cout << "Game Over!" << endl;
        return;
    }
}

int main()
{
    srand(static_cast<unsigned>(time(0)));
    Setup();
    while (!gameOver)
    {
        Draw();
        Input();
        Logic();
        Sleep(100);
    }
    return 0;
}
