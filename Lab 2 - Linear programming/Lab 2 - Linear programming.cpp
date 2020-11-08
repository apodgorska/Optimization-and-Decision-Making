// Lab 2 - Linear programming
// Made by: Aleksandra Podgórska and Szymon Siąkała
// Mk 2
// Group C
// Section 2

#include <iostream>
#include <sstream>
#include <iomanip>
#include <Windows.h>

std::string prd(float x) 
{
    std::stringstream ss;
    ss << std::fixed << std::right;
    ss.fill(' ');
    ss.width(10);
    ss.precision(3);
    ss << x;
    return ss.str();
}

std::string center_txt(std::string s) 
{
    std::stringstream ss, spaces;
    int padding = 10 - s.size();
    for (int i = 0; i < padding / 2; ++i)
    {
        spaces << " ";
    }
    ss << spaces.str() << s << spaces.str();
    if (padding > 0 && padding % 2 != 0)
    {
        ss << " ";
    }
    return ss.str();
}

void display_table(float C[4], float** A, float B[2], float Z[4], float f_value, float ratio[2], float solution[2], int solution_x_number[2])
{
    std::stringstream ss1;
    ss1 << solution_x_number[0];
    std::string solution1 = ss1.str();
    solution1 = "x" + solution1;
    std::stringstream ss2;
    ss2 << solution_x_number[1];
    std::string solution2 = ss2.str();
    solution2 = "x" + solution2;
    std::cout << std::endl;
    std::cout << center_txt(" ") << " | " << center_txt(" ") << " | " << center_txt("Cj") << " | " << prd(C[0]) << " | " << prd(C[1]) << " | " << prd(C[2]) << " | " << prd(C[3]) << " | " << std::endl;
    std::cout << "------------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << center_txt("Solution") << " | " << center_txt("Cb") << " | " << center_txt("B/X") << " | " << center_txt("x1") << " | " << center_txt("x2") << " | " << center_txt("x3") << " | " << center_txt("x4") << " | " << center_txt("Ratio") << std::endl;
    std::cout << "------------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << center_txt(solution1) << " | " << prd(solution[0]) << " | " << prd(B[0]) << " | " << prd(A[0][0]) << " | " << prd(A[0][1]) << " | " << prd(A[0][2]) << " | " << prd(A[0][3]) << " | " << prd(ratio[0]) << std::endl;
    std::cout << center_txt(solution2) << " | " << prd(solution[1]) << " | " << prd(B[1]) << " | " << prd(A[1][0]) << " | " << prd(A[1][1]) << " | " << prd(A[1][2]) << " | " << prd(A[1][3]) << " | " << prd(ratio[1]) << std::endl;
    std::cout << "------------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << center_txt(" ") << " | " << center_txt(" ") << " | " << center_txt("Zj") << " | " << prd(Z[0]) << " | " << prd(Z[1]) << " | " << prd(Z[2]) << " | " << prd(Z[3]) << " | " << center_txt("f value") << std::endl;
    std::cout << "------------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << center_txt(" ") << " | " << center_txt(" ") << " | " << center_txt("Zj-Cj") << " | " << prd(Z[0] - C[0]) << " | " << prd(Z[1] - C[1]) << " | " << prd(Z[2] - C[2]) << " | " << prd(Z[3] - C[3]) << " | " << prd(f_value) << std::endl;
    std::cout << std::endl;
}

int find_max(float C[], float Z[], int number_of_x_and_number_of_constraints)
{
    float temp = Z[0] - C[0];
    int index = 0;
    for (int i = 0; i < number_of_x_and_number_of_constraints; i++)
    {
        if (temp < (Z[i] - C[i]))
        {
            temp = Z[i] - C[i];
            index = i;
        }
    }
    return index;
}

int find_min(float ratio[], int number_of_constraints)
{
    float temp = ratio[0];
    int index = 0;
    for (int i = 0; i < number_of_constraints; i++)
    {
        if (temp > ratio[i])
        {
            temp = ratio[i];
            index = i;
        }
    }
    return index;
}

void Simplex_method(float C[], float constraints[][5], const int number_of_x, const int number_of_constraints, bool display_table_check)
{
    std::cout << "Searching for minimum of function:" << std::endl;
    std::cout << "z = ";
    for (int i = 0; i < number_of_x; i++)
    {
        std::cout << "(" << C[i] << ")" << "x" << i + 1;
        if (i != (number_of_x - 1))
        {
            std::cout << " + ";
        }
    }
    std::cout << std::endl;
    std::cout << "Subject to constraints:" << std::endl;
    for (int i = 0; i < number_of_constraints; i++)
    {
        std::cout << "(" << constraints[i][0] << ")x" << constraints[i][1] << " + (" << constraints[i][2] << ")x" << constraints[i][3] << " <= " << constraints[i][4] << std::endl;
    }
    float** A = new float* [number_of_constraints];
    float* B = new float[number_of_constraints];
    float* Ce = new float[number_of_constraints];
    float* Z = new float[number_of_x + number_of_constraints];
    float* solution = new float[number_of_constraints];
    int* solution_x_number = new int[number_of_constraints];
    float f_value = 0;
    float* ratio = new float[number_of_constraints];
    int index_horizontal;
    int index_vertical;
    bool check = false;
    int temp_one = number_of_x;
    for (int i = 0; i < number_of_constraints; i++)
    {
        A[i] = new float[number_of_x + number_of_constraints];
        for (int j = 0; j < number_of_x + number_of_constraints; j++)
        {
            if (constraints[i][1] == (j + 1))
            {
                A[i][j] = constraints[i][0];
            }
            else if (constraints[i][3] == (j + 1))
            {
                A[i][j] = constraints[i][2];
            }
            else
            {
                A[i][j] = 0;
            }
        }
        A[i][temp_one] = 1;
        temp_one++;
        B[i] = constraints[i][4];
    }
    for (int i = 0; i < number_of_x + number_of_constraints; i++)
    {
        Z[i] = 0;
    }
    for (int i = 0; i < number_of_constraints; i++)
    {
        solution[i] = 0;
        solution_x_number[i] = 0;
        ratio[i] = 0;
        Ce[i] = 0;
    }
    if (display_table_check)
    {
        std::cout << std::endl;
        std::cout << "Initial Simplex table" << std::endl;
        display_table(C, A, B, Z, f_value, ratio, solution, solution_x_number);
    }
    for (int i = 0; i < number_of_x + number_of_constraints; i++)
    {
        if (Z[i] - C[i] > 0)
        {
            check = false;
            break;
        }
        check = true;
    }
    while (!check)
    {
        index_horizontal = find_max(C, Z, number_of_x + number_of_constraints);
        for (int i = 0; i < number_of_constraints; i++)
        {
            if (A[i][index_horizontal] != 0)
            {
                ratio[i] = B[i] / A[i][index_horizontal];
            }
            else
            {
                ratio[i] = 9999999999999;
            }
        }
        index_vertical = find_min(ratio, number_of_constraints);
        if (display_table_check)
        {
            std::cout << "Pivoting element: " << A[index_vertical][index_horizontal] << " in column x" << index_horizontal + 1 << std::endl;
        }
        float temp = A[index_vertical][index_horizontal];
        for (int i = 0; i < number_of_x + number_of_constraints; i++)
        {
            A[index_vertical][i] = A[index_vertical][i] / temp;
        }
        B[index_vertical] = B[index_vertical] / temp;
        Ce[index_vertical] = C[index_horizontal];
        solution[index_vertical] = B[index_vertical];
        solution_x_number[index_vertical] = index_horizontal + 1;
        for (int i = 0; i < number_of_constraints; i++)
        {
            if (i != index_vertical)
            {
                temp = A[i][index_horizontal];
                for (int j = 0; j < number_of_x + number_of_constraints; j++)
                {
                    A[i][j] = A[i][j] - temp * A[index_vertical][j];
                }
                B[i] = B[i] - temp * B[index_vertical];
            }
        }
        for (int i = 0; i < number_of_x + number_of_constraints; i++)
        {
            Z[i] = 0;
            for (int j = 0; j < number_of_constraints; j++)
            {
                Z[i] = Z[i] + Ce[j] * A[j][i];
            }
        }
        f_value = 0;
        for (int i = 0; i < number_of_constraints; i++)
        {
            f_value = f_value + Ce[i]*solution[i];
        }
        if (display_table_check)
        {
            display_table(C, A, B, Z, f_value, ratio, solution, solution_x_number);
        }
        for (int i = 0; i < number_of_x + number_of_constraints; i++)
        {
            if (Z[i] - C[i] > 0)
            {
                check = false;
                break;
            }
            check = true;
        }
    }
    if (!display_table_check)
    {
        std::cout << std::endl;
    }
    std::cout << "Since all Zj-Cj <= 0 the solution is:" << std::endl;
    bool* temp_display = new bool[number_of_x];
    for (int i = 0; i < number_of_x; i++)
    {
        temp_display[i] = false;
    }
    for (int i = 0; i < number_of_constraints; i++)
    {
        for (int j = 0; j < number_of_x; j++)
        {
            if (solution_x_number[i] == j + 1)
            {
                temp_display[j] = true;
                break;
            }
        }
    }
    std::cout << std::endl;
    for (int i = 0; i < number_of_x; i++)
    {
        if (temp_display[i])
        {
            for (int j = 0; j < number_of_constraints; j++)
            {
                if (solution_x_number[j] == i + 1)
                {
                    std::cout << "x" << solution_x_number[j] << " = " << solution[j] << std::endl;
                }
            }
        }
        else
        {
            std::cout << "x" << i + 1 << " = 0" << std::endl;
        }
    }
    std::cout << "Min z = " << f_value << std::endl;
}

int main()
{
    /*Function is given as:
      z = C[0] * x1 + C[1] * x2 + C[2] * x3 + ...
      Subject to constraints:
      1. constraints[0][1] * x_number = constrains[0][1] + constraints[0][2] * x_number = constrains[0][3] <= constrains[0][4]
      2. constraints[1][1] * x_number = constrains[1][1] + constraints[1][2] * x_number = constrains[1][3] <= constrains[1][4]
      3. ...*/

    std::cout << "Program will now display few exemplary functions" << std::endl;
    std::cout << "1.Function assigned to our section with 2 variables and 2 constraints" << std::endl;
    std::cout << "2.Function assigned to our section with N variables and M constraints" << std::endl;
    std::cout << "The rest are random exemplary functions" << std::endl;
    std::cout << "Press ENTER after each one to continue" << std::endl;
    system("pause");
    system("cls");
    const int number_of_x1 = 2;
    const int number_of_constraints1 = 2;
    float C1[number_of_x1 + number_of_constraints1] = { 12,16,0,0 };
    float constraints1[number_of_constraints1][5] = { { 10,1,20,2,120 },{ 8,1,8,2,80 } };
    Simplex_method(C1, constraints1, number_of_x1, number_of_constraints1, true);
    system("pause");
    system("cls");
    const int number_of_x2 = 100;
    const int number_of_constraints2 = 50;
    float C2[number_of_x2 + number_of_constraints2] = { -35,-3,-35,-6,-16,-29,-19,-2,-8,-30,-31,-2,-4,-18,-13,-5,-30,-17,-7,-17,-8,-15,-28,-17,-33,-33,-6,-26,-34,-9,-11,-23,-7,
    -1,-35,-14,-22,-18,-25,-2,-23,-23,-20,-6,-13,-9,-8,-35,-14,-19,-23,-15,-21,-15,-7,-13,-19,-3,-1,-35,-5,-9,-7,-22,-25,-3,-5,-8,-29,-15,-30,-23,-29,-34,-18,-3,
    -31,-18,-33,-25,-33,-27,-24,-3,-1,-5,-19,-22,-17,-3,-1,-10,-21,-5,-17,-20,-4,-17,-19,-17,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    float constraints2[number_of_constraints2][5] = { {34,1,39,51,57},{37,2,28,52,13},{40,3,29,53,31},{3,4,32,54,47},{40,5,15,55,70},{16,6,17,56,31},
    {10,7,47,57,77},{17,8,3,58,79},{2,9,8,59,73},{25,10,32,60,32},{41,11,12,61,99},{28,12,4,62,22},{28,13,26,63,26},{8,14,18,64,53},{4,15,29,65,99},
    {15,16,39,66,55},{37,17,36,67,74},{25,18,42,68,10},{31,19,26,69,81},{19,20,25,70,97},{17,21,32,71,27},{19,22,42,72,21},{36,23,26,73,36},{36,24,42,74,44},
    {50,25,34,75,34},{41,26,49,76,89},{44,27,43,77,98},{34,28,45,78,67},{10,29,44,79,4},{27,30,12,80,71},{36,31,28,81,84},{46,32,31,82,14},{16,33,31,83,74},
    {2,34,7,84,61},{25,35,6,85,93},{46,36,18,86,44},{2,37,36,87,67},{5,38,23,88,84},{27,39,40,89,49},{50,40,34,90,87},{17,41,48,91,80},{38,42,43,92,42},
    {32,43,23,93,57},{23,44,31,94,92},{20,45,19,95,84},{48,46,6,96,53},{5,47,45,97,9},{48,48,46,98,100},{49,49,12,99,52},{16,50,38,100,65} };
    Simplex_method(C2, constraints2, number_of_x2, number_of_constraints2, false);
    system("pause");
    system("cls");
    const int number_of_x3 = 2;
    const int number_of_constraints3 = 2;
    float C3[number_of_x3 + number_of_constraints3] = { -5,-10,0,0 };
    float constraints3[number_of_constraints3][5] = { { 3,1,3,2,60 },{ 4,1,4,2,72 } };
    Simplex_method(C3, constraints3, number_of_x3, number_of_constraints3, true);
    system("pause");
    system("cls");
    const int number_of_x4 = 6;
    const int number_of_constraints4 = 3;
    float C4[number_of_x4 + number_of_constraints4] = { -35,-3,-35,6,16,-29,0,0,0 };
    float constraints4[number_of_constraints4][5] = { {3,1,39,2,5},{3,3,2,4,1},{40,5,2,6,3} };
    Simplex_method(C4, constraints4, number_of_x4, number_of_constraints4, false);
    system("pause");
    system("cls");
    const int number_of_x5 = 10;
    const int number_of_constraints5 = 5;
    float C5[number_of_x5 + number_of_constraints5] = { -35,-3,-35,-6,-16,-29,-19,-2,-8,-30,0,0,0,0,0 };
    float constraints5[number_of_constraints5][5] = { {34,1,39,6,57},{37,2,28,7,13},{40,3,29,8,31},{3,4,32,9,47},{40,5,15,10,70} };
    Simplex_method(C5, constraints5, number_of_x5, number_of_constraints5, false);
    system("pause");
    system("cls");
    return 0;
}