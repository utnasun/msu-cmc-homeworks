#include <iostream>
#include <cstdlib>
#include <ctime>

class Matrix
{
private:
    int rows;
    int cols;
    int** matr; //
public:
    Matrix();
    ~Matrix();
    Matrix(int srows, int scols);
    int* operator [] (int x);
    const int* operator [] (int x) const;
    void print() const;
};

Matrix::Matrix()
{
    rows = 2;
    cols = 2;
    matr = new int* [rows];
    for (int i=0; i<rows; i++)
    {
        matr[i] = new int [cols];
        for (int j=0; j<cols; j++)
            matr[i][j] = 1; // инициализируем матрицу 1
    }
}

Matrix::~Matrix()
{
    for (int i=0; i<rows; i++)
    {
        delete[] matr[i]; // удаляем строки
    }
    delete[] matr; // удаляем массив строк
}

void Matrix::print() const
{
    for (int i=0; i<rows; i++)
    {
        for (int j=0; j < cols; j++)
        {
            std::cout << matr[i][j] << "\t";
        }
        std::cout << std::endl;
    }
}

Matrix::Matrix(int srows, int scols)
{
    rows = srows;
    cols = scols;
    matr = new int* [rows];
    srand(time(NULL));
    for (int i=0; i<rows; i++)
    {
        matr[i] = new int [cols];
        for (int j=0; j<cols; j++)
            matr[i][j] = rand() % 15;
    }
}

int* Matrix::operator[](int x)
{
    return matr[x]; // возвращаем указатель на строку матрицы, затем уже идет индексация по массиву
}

const int* Matrix::operator[](int x) const
{
    return matr[x]; // возвращаем константный указатель на строку матрицы, поэтому mc[1][1] = 100 будет ошибкой.
}

int main()
{
    Matrix a1;
    Matrix a2(3,5);
    std::cout << "a1 print:" << std::endl;
    a1.print();
    std::cout << std::endl;
    std::cout << "a2 print:" << std::endl;
    a2.print();
    std::cout << "a2[1][1] = " << a2[1][1] << std::endl;
    int x = a2[1][1];
    std::cout << "x = a2[1][2], x = " << x << std::endl;
    a2[1][2] = 555;
    std::cout << "a2[1][2] = 555, a2.print(): " << std::endl;
    a2.print();
    const Matrix mc;
    std::cout << "const Matrix mc, mc[1][1] = " <<mc[1][1] << std::endl;
    //mc[1][1] = 100;
    std::cout << "mc.print(): " << std::endl;
    mc.print();
}

