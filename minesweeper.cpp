#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <bits/stdc++.h>
#include <time.h>
#include <fstream>
#include <cstring>
using namespace std;

#define max_row 50
#define max_col 50
int row, column, mine, x, y, choice, level, decision, win;
bool gameover = false;
time_t start_game, end_game, elapsed_time;

/**
Description:    Checks if the argument is a number or not.
Arguments:      
    arg1        A specific character that will be checked if it is a digit
                or not.
Returns:        True or false
    type        It is a bool type variable in which if it is true, the
                arguement is not a number false if otherwise.
**/
bool number_checker(char c)
{
    return !isdigit(c);
}

/**
Description:    A comparator function that uses the numbers in the start of
                string and sort them from small to large.
Arguments:      
    arg1        The first string that will be converted into a number and
                compared it to another string.
    arg2        The second string that will be converted into another number
                and compared it to the first converted string.
Returns:        True or false
    type        It is a bool type variable in which if it is true, the
                first argument should be before the second arguement and false
                if otherwise.
**/
bool sort_leaderboard(const string& string1, const string& string2)
{
    string::const_iterator iter1 = string1.begin(), iter2 = string2.begin();

    if (isdigit(string1[0]) && isdigit(string2[0])) 
    {
        int num1, num2;
        stringstream ss(string1);
        ss >> num1;
        ss.clear();
        ss.str(string2);
        ss >> num2;

        if (num1 != num2) 
        {
            return num1 < num2;
        }
        iter1 = find_if(string1.begin(), string1.end(), number_checker);
        iter2 = find_if(string2.begin(), string2.end(), number_checker);
    }

    return lexicographical_compare(iter1, string1.end(), iter2, string2.end());
}

/**
Description:    Prints/outputs the leaderboard from a textfile
Arguments:      N/A
Returns:        N/A
**/
void view_leaderboard()
{

    cout << "Current Leader board: \n" << "Time(s)    Level     Name\n";
    vector<string> v;
    fstream score_file;
    score_file.open("Leaderboard.txt", ios::in);
    if (score_file.is_open())
    {
        string line;
        while (getline(score_file, line))
        {
            v.push_back(line);
        }
        score_file.close();
        sort(v.begin(), v.end(), sort_leaderboard);
        for (int i=0; i < v.size(); i++)
        {
            cout << v.at(i) << endl;
        }
    }
}

/**
Description:    If the player won, this function will take the name of the player
                with their elapsed time and level of difficulty and store it in a
                text file. After, it will print the current leader board with
                their name.
Arguments:      N/A
Returns:        N/A
**/
void get_playerinfo()
{
    string name;
    cout << "What do you want your name to be? \n";
    cin >> name;
    win = 1;

    fstream score_file;
    score_file.open("Leaderboard.txt", ios::app);
    if (score_file.is_open())
    {
        score_file << elapsed_time << "          " << level << "         " << name << endl;
        score_file.close();
        view_leaderboard();
    }
}  

/**
Description:    A bool function that checks whether the position of a
                selected tile is on a mine.
Arguments:
    arg1        The row position of the selected tile.
    arg2        The column position of the selected tile.
Returns:        The return value is true or false.
    type        It is a bool type variable in which if it is true it means
                that the selected tile position is on a mine, false if
                otherwise. 
**/
bool mine_checker(int x, int y, char grid[][max_row])
{
    if (grid[x][y] == 'b')
        return (true);
    else
        return (false);
}

/**
Description:    A function that creates an empty and empty board.
Arguments:
    arg1        The board that can be seen by the player.
    arg2        The masterboard where the bombs are located.
Returns:        N/A
**/
void create_grid(char grid[][max_col], char true_grid[][max_col])
{
    srand(time(0));
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            grid[i][j] = '-';
            true_grid[i][j] = '-';
        }
    }
}

/**
Description:    Places mines on random location.
Arguments:
    arg1        The masterboard is used so that the player cannot see where the
                bombs are randomly placed.
Returns:        N/A
**/
void place_mines(char true_grid[][max_col])
{
    int mine_counter = 0;
    while (mine_counter < mine)
    {
        int coordx = rand()%row;
        int coordy =rand()%column;
        if (true_grid[coordx][coordy]== '-')
        {
            true_grid[coordx][coordy] = 'b';
            mine_counter++;
        }
    } 
}

/**
Description:    Prompts the user which level of difficulty they would like.
                It also sets how many rows, columns, and mines are going
                to be used.
Arguments:      N/A
Returns:        N/A
**/
void Level_information()
{

    cout << "\nWhat difficulty would you take?\n\n";
    cout << "[1] --> Beginner Level (9 x 9 grid with 10 mines)\n";
    cout << "[2] --> Intermediete Level (16 x 16 grid with 40 mines)\n";
    cout << "[3] --> Expert Level (16 x 30 grid with 99 mines)\n";
    cout << "[4] --> Customized Level (R x C grid with N mines)\n";

    cin >> level;

    if (level == 1)
    {
        row = 9;
        column = 9;
        mine = 10;
    }
    else if (level == 2)
    {
        row = 16;
        column = 16;
        mine = 40;
    }
    else if (level == 3)
    {
        row = 16;
        column = 30;
        mine = 99;
    }
    else if (level == 4)
    {
        cout << "How many rows do you want? ";
        cin >> row;
        cout << "How many columns do you want? ";
        cin >> column;
        cout << "How many mines do you want? ";
        cin >> mine;
    }
    else
    {
        cout << "Choose from the selection!\n";
        Level_information();
    }
}

/**
Description:    A recursive function which ask the user for their input on whether
                they would like to open or flag a tile or quit the game.     
Arguments:      N/A
Returns:        N/A
**/
void player_move()
{
    cout << "\nWhat do you want to do?\n";
    cout << "[1] --> Open a tile.\n" << "[2] --> Flag a tile.\n" << "[3] --> Quit.\n";
    cin >> choice;
    if (choice == 1 || choice == 2)
    {
        cout << "What column number? ";
        cin >> y;
        cout << "What row number? ";
        cin >> x;

        if (x > row || x < 0)
        {
            cout << "Remain in the grid.";
            player_move();
        }
        if (y > column || y < 0)
        {
            cout << "Remain in the grid.\n";
            player_move();
        }
    }
    else
    {
        exit(0);
    }
    
}

/**
Description:    A function that prints the grid. Both the board and
                masterboard (uncomment to enable).
Arguments:
    arg1        To be able to print the board for the players use.
    arg2        To be able to print the masterboard for the game master to see.
Returns:        N/A
**/
void print_grid(char grid[][max_col], char true_grid[][max_col])
{

    cout << "\n  ";
    for (int i = 0; i < column; i++)
    {
        cout << i << " ";
    }
    cout << "\n";
    for (int i = 0; i < row; i++)
    {
        cout << i << " ";
        for (int j = 0; j < column; j++)
        {
            cout << grid[i][j] << " ";
        }
        cout << "\n";
    }

    //uncomment to print master board/grid 
    // cout << "\n  ";
    // for (int i = 0; i < column; i++)
    // {
    //     cout << i << " ";
    // }
    // cout << "\n";
    // for (int i = 0; i < row; i++)
    // {
    //     cout << i << " ";
    //     for (int j = 0; j < column; j++)
    //     {
    //         cout << true_grid[i][j] << " ";
    //     }
    //     cout << "\n";
    // }
    
}

/**
Description:    A bool function that checks whether the position of a
                selected tile is still in the grid created.
Arguments:
    arg1        The adjacent row position of the selected tile.
    arg2        The adjacent column position of the selected tile.
Returns:        The return value is true or false.
    type        It is a bool type variable in which if it is true it means
                that the selected tile position is in the grid, false if
                otherwise. 
**/
bool in_grid(int adj_x, int adj_y)
{
    if ((adj_x > row) || (adj_x < 0))
    {
        return (false);
    } 
    if ((adj_y > column) || (adj_y < 0))
    {
        return (false);
    }
    else
        return (true);
}

/**
Description:    A function which counts the mine that is surrounding the
                selected tile.
Arguments:
    arg1        The row position of the selected tile.
    arg2        The column position of the selected tile.
    arg3        To check the selected tile in the masterboard where the bombs are placed.
Returns:        The return value is a varying number.
    type        It is a integer type variable used to display how many mines are
                surrounding the tile.
**/
int mine_counter(int x, int y, char true_grid [][max_col])
{
    int count = 0;

    //north
    if (in_grid(x-1,y) == true)
    {
        if (mine_checker(x-1,y,true_grid) == true)
        count++;
    }
    //south
    if (in_grid(x+1,y) == true)
    {
        if (mine_checker(x+1,y,true_grid) == true)
        count++;
    }
    //east
    if (in_grid(x,y+1) == true)
    {
        if (mine_checker(x,y+1,true_grid) == true)
        count++;
    }
    //west
    if (in_grid(x,y-1) == true)
    {
        if (mine_checker(x,y-1,true_grid) == true)
        count++;
    }
    //NE
    if (in_grid(x-1,y+1) == true)
    {
        if (mine_checker(x-1,y+1,true_grid) == true)
        count++;
    }
    //NW
    if (in_grid(x-1,y-1) == true)
    {
        if (mine_checker(x-1,y-1,true_grid) == true)
        count++;
    }
    //SE
    if (in_grid(x+1,y+1) == true)
    {
        if (mine_checker(x+1,y+1,true_grid) == true)
        count++;
    }
    //SW
    if (in_grid(x+1,y-1) == true)
    {
        if (mine_checker(x+1,y-1,true_grid) == true)
        count++;
    }
    return (count);
}

/**
Description:    A recursive bool function that checks whether the adjecent 
                position of a selected tile is on a mine. 
Arguments:
    arg1        The row position of the selected tile.
    arg2        The column position of the selected tile.
    arg3        The board for the player to see.
Returns:        The return value is true or false.
    type        It is a bool type variable in which if it is true it means
                that the selected tile position is on a mine, false if
                otherwise. 
**/
bool perimeter_checker(int x, int y, char grid[][max_col], char true_grid[][max_col], int *moves_towin)
{
    if (grid[x][y] != '-')
    {
        if ((grid[x][y] == 'F') && (choice == 2))
            {
                grid[x][y] = '-';
                return (false);
            }
        else
        {
            return (false);
        }  
    }
    else
    {
        if (choice == 1)
        {
            if (true_grid[x][y] == 'b')
            {   
                grid[x][y] = 'b';
                print_grid(grid, true_grid);
                cout << "\n\nYou lose! Better luck next time.\n\n";
                return (true);
            }

            int count = mine_counter(x,y,true_grid);
            (*moves_towin)--;
            grid[x][y] = count + '0';

            if (!count)
            {
                //north
                if (in_grid(x-1,y) == true)
                {
                    if (mine_checker(x-1,y,true_grid) == false)
                    {
                        perimeter_checker(x-1,y,grid,true_grid,moves_towin);
                    }
                }

                //south
                if (in_grid(x+1,y) == true)
                {
                    if (mine_checker(x+1,y,true_grid) == false)
                    {
                        perimeter_checker(x+1,y,grid,true_grid,moves_towin);
                    }
                }
                
                //east
                if (in_grid(x,y+1) == true)
                {
                    if (mine_checker(x,y+1,true_grid) == false)
                    {
                        perimeter_checker(x,y+1,grid,true_grid,moves_towin);
                    }
                }

                //west
                if (in_grid(x,y-1) == true)
                {
                    if (mine_checker(x,y-1,true_grid) == false)
                    {
                        perimeter_checker(x,y-1,grid,true_grid,moves_towin);
                    }
                }

                //nE
                if (in_grid(x-1,y+1) == true)
                {
                    if (mine_checker(x-1,y+1,true_grid) == false)
                    {
                        perimeter_checker(x-1,y+1,grid,true_grid,moves_towin);
                    }
                }

                //nW
                if (in_grid(x-1,y-1) == true)
                {
                    if (mine_checker(x-1,y-1,true_grid) == false)
                    {
                        perimeter_checker(x-1,y-1,grid,true_grid,moves_towin);
                    }
                }
                
                //sE
                if (in_grid(x+1,y+1) == true)
                {
                    if (mine_checker(x+1,y+1,true_grid) == false)
                    {
                        perimeter_checker(x+1,y+1,grid,true_grid,moves_towin);
                    }
                }

                //sW
                if (in_grid(x+1,y-1) == true)
                {
                    if (mine_checker(x+1,y-1,true_grid) == false)
                    {
                        perimeter_checker(x+1,y-1,grid,true_grid,moves_towin);
                    }
                }
            }
        return (false);
        }
        if (choice == 2)
        {
            grid[x][y] = 'F';
        }
        return (false);
    }
}

/**
Description:    An iterative function that makes the game playable until
                it is gameover.
Arguments:      N/A
Returns:        N/A
**/
void play_minesweeper()
{
    time(&start_game);
    Level_information();
    char grid[max_row][max_col], true_grid[max_row][max_col];
    create_grid(grid, true_grid);
    place_mines(true_grid);
    print_grid(grid, true_grid);
    int moves_towin = (row*column-mine);

    do
    {
        player_move();
        gameover = perimeter_checker(x,y,grid,true_grid,&moves_towin);
        print_grid(grid, true_grid);

        if ((gameover == false) && (moves_towin <= 0))
        {
            cout << "\n\nCongratulations, you won!\n\n";
            gameover = true;
            time(&end_game);
            elapsed_time = end_game - start_game;

            cout << "Your time: " << elapsed_time << " seconds\n\n" << endl;
            if (level < 4)
            {
                cout << "Do you want to be in the leaderboard? \n[1] --> Yes \n[2] --> No\n";
                int ldd;
                cin >> ldd;
                if (ldd == 1)
                {
                    get_playerinfo();
                }
            }
        }
    } while (gameover == false);
}

/**
Description:    A void function that prompts the user if they will play
                or not.
Arguments:      N/A
Returns:        N/A
**/
void play_ornot()
{

    cout << "\n\nGOOD DAY! What do you want to do? \n\n";
    cout << "[1] ---> Visit the Leaderboard \n" << "[2] ---> Play Minesweeper \n";
    cin >> decision;
    if (decision == 1)
    {
        view_leaderboard();
    }
    if (decision == 2)
    {
        play_minesweeper();
    }
}

/**
Description:    Main function that will start the game with respect to the users 
                path of decision.
Arguments:      N/A
Returns:        N/A
**/
int main()
{
    play_ornot();
}