#include <math.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#define EPS 0.000001
#define INF_ROOTS -1
#define KOEFFS_NUM 3

#define TEST(func, res)                                 \
    do {                                                \
        std::cout << "Expected value:\n" << res << "\n";\
        if (func == res) {                              \
            puts ("OK");                                \
        } else {                                        \
            std::cout << "WRONG\nGot: " << func << "\n";\
        }                                               \
    } while (0)

#define INIT_3(array, a, b, c)  \
    do {                        \
        array[0] = a;           \
        array[1] = b;           \
        array[2] = c;           \
    } while (0)

enum exceptions {INF_ROOTS_ERR, OUT_OF_ROOTS_RANGE};

/*!
* Класс, предназначенный для обработки ошибок.
*/

class Exception {
    exceptions err;
public:

    /*!
    * Создает объект исключения.
    * \param err Код ошибки
    */

    Exception (exceptions err) {
        this -> err = err;
    }

    /*!
    * Печатает сообщение об ошибке.
    */

    void print () {
        if (err == INF_ROOTS_ERR) {
            std::cout << "There's infinite roots!\n";
        } else if (err == OUT_OF_ROOTS_RANGE) {
            std::cout << "Number of root is out of range!\n";
        }
    }
};

bool isZero (double num) {
    return abs (num) < EPS;
}

const double * solverInit () {
    double * koeff = new double [KOEFFS_NUM];
    printf ("Please, enter the coefficients\n");
    bool incorrectInput = true;
    while (incorrectInput) {
        incorrectInput = false;
        for (int i = 0; i < KOEFFS_NUM; i++) {
            int num = 0;
            if ((num = scanf ("%lg", koeff + i)) == EOF) { //TODO
                incorrectInput = true;
                printf ("Incorrect input! Try again\n");
                break;
            } else if (num == 0) {
                i --;
                char a;
                scanf("%c", &a);
            }
        }
    }
    return koeff;
}

/*!
* Данная функция предназначена для вычисления дискриминанта уравнения.
* \param koeff Указатель на массив с коэффициентами уравнения. Размер массива должен быть равен 3.
* \return Возвращает значение дискриминанта.
*/

double discriminant (const double koeff [KOEFFS_NUM]) {
    return koeff[1] * koeff[1] - 4 * koeff[0] * koeff[2];
}

/*!
* Данная функция решает квадраное уравнение с указанными коэффициентами.
* \param koeff Указаель на массив с коэффициентами уравнения. Размер массива должен быть равен 3.
* \param roots Указатель на массив, в котром возвращаются корни уравнения. Размер массива равен числу корней.\
* Если корней нет или их бесконечно много, roots = NULL.
* \param print Указывает, нужно ли выводить результат на стандартный поток вывода. Значение по умолчанию true.
* \return Количество корней уравнения.
*/

int solve (const double koeff [KOEFFS_NUM], double * roots, bool print = true) {
    int numOfRoots = 0;
    roots = NULL;
    if (isZero(koeff[0])) {
        if(isZero(koeff[1])) {
            if (isZero(koeff[2])) {
                numOfRoots = INF_ROOTS;
                if (print) {
                    printf ("Infinite roots\n");
                }
            } else /* koeff[2] != 0 */ {
                numOfRoots = 0;
                if (print) {
                    printf ("Zero roots\n");
                }
            }
        } else /* koeff[1] != 0 */ {
            numOfRoots = 1;
            roots = new double [numOfRoots];
            roots[0] = -koeff[2] / koeff[1];
            if (print) {
                printf ("One root:\n%lg\n", roots[0]);
            }
        }
    } else /* koeff[0] != 0 */ {
        double d = discriminant (koeff);
        if (isZero(d)) {
            numOfRoots = 1;
            roots = new double [numOfRoots];
            roots[0] = -koeff[1] / (2 * koeff[0]);
            if (print) {
                printf ("One root:\n%lg\n", roots[0]);
            }
        } else if (d < 0) {
            numOfRoots = 0;
            if (print) {
                printf ("Zero roots\n");
            }
        } else /* d > 0 */ {
            numOfRoots = 2;
            roots = new double [numOfRoots];
            roots[0] = (-koeff[1] + sqrt(d)) / (2 * koeff[0]);
            roots[1] = (-koeff[1] - sqrt(d)) / (2 * koeff[0]);
            if (print) {
                printf ("Two roots:\n%lg\n%lg\n", roots[0], roots[1]);
            }
        }
    }
    return numOfRoots;
}

//-------------------------------------------------------------------------------------------------------------

/*!
* Данная функция предназначена для тестирования функции discriminant().
* \param koeff Указатель на массив с коэффициентами уравнения. Размер массива должен быть равен 3.
* \param expected Ожидаемый результат
*/

void testDiscriminant (const double koeff [KOEFFS_NUM], const double expected) {
    printf ("testDiscriminant: Testing calculated discriminant.\n");
    TEST (discriminant (koeff), expected);
}

/*!
* Данная функция предназначена для тестирования функции solve().
* \param koeff Указатель на массив с коэффициентами уравнения. Размер массива должен быть равен 3.
* \param num Ожидаемое количество корней, если корней бесконечно много,\
* этот параметр должен быть равен -1
* \param expected Указатель на массив, в котром возвращаются корни уравнения. Размер массива равен числу корней.\
* Если корней нет или их бесконечно много, expected = NULL.
*/

void testSolve (const double koeff [KOEFFS_NUM], int num, const double * expected) {
    double actual [2];
    assert ((INF_ROOTS <= num) && (num <= 2));
    printf ("testSolve: Testing if size of the 'expected' param equals to specified number of roots.\n");
    TEST (sizeof (expected), num);
    printf ("testSolve: Testing if calculated number of roots equals to specified one.\n");
    TEST (solve (koeff, actual, false), num);
    if ((num != 0) && (num != INF_ROOTS) && (sizeof (actual) == num)) {
        if (num == 1) {
            printf ("testSolve: Testing x.\n");
            TEST (actual[0], expected[0]);
        } else /* num = 2 */ {
            if (((actual[0] - actual[1]) * (expected[0] - expected[1])) < 0) {
                printf ("testSolve: Testing x1.\n");
                TEST (actual[0], expected[1]);
                printf ("testSolve: Testing x2.\n");
                TEST (actual[1], expected[0]);
            } else {
                printf ("testSolve: Testing x1.\n");
                TEST (actual[0], expected[0]);
                printf ("testSolve: Testing x2.\n");
                TEST (actual[1], expected[1]);
            }
        }
    }
}

/*!
* Данная функция предназначена для общего тестирования программы.
*/

void test () { //TODO file
    double koeff [KOEFFS_NUM];
    double roots [2];
    double root;
    INIT_3 (koeff, 0, 0, 0);
    testSolve (koeff, INF_ROOTS, NULL);
    INIT_3 (koeff, 0, 0, 1);
    testSolve (koeff, 0, NULL);
    INIT_3 (koeff, 1, 2, 1);
    root = -1;
    testDiscriminant (koeff, 0);
    testSolve (koeff, 1, &root);
    INIT_3 (koeff, 1, -2, 1);
    root = 1;
    testDiscriminant (koeff, 0);
    testSolve (koeff, 1, &root);
    INIT_3 (koeff, 1, 4, 3);
    roots[0] = -1;
    roots[1] = -3;
    testDiscriminant (koeff, 1);
    testSolve (koeff, 2, roots);
}

/*!
* Без указания режима просто считает решение уравнения
* Режимы:
* -test -- Запускает тест программы
*/

int main (int argc, char ** argv) {
    if (argc > 1) {
        if (strcmp (argv[1], "-test")) {
            test ();
        }
    } else {
        solve (solverInit(), NULL);
    }
}