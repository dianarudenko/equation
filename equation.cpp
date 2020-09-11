#include <math.h>
#include <iostream>
#include <string.h>

#define EPS 0.00001
#define INF_ROOTS -1

#define TEST(func, res)                                                           \
    do {                                                                          \
        std::cout << "Testing " << #func << "\nExpected result: " << res << "\n"; \
        if (func == res) {                                                        \
            std::cout << "OK\n";                                                  \
        } else {                                                                  \
            std::cout << "WRONG\nGot: " << func << "\n";                          \
        }                                                                         \
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

/*!
* Класс содержит функции, предназначенные для вычисления корней квадратного уравнения.
*/

class EquationSolver {
    int numOfRoots = 0;
    double x1 = 0.0;
    double x2 = 0.0;

public:

    /*!
    * Данный метод получает на вход коэффициенты уравнения, решает данное уравнение и \
    * записывает результат в соответствующие поля класса.
    */

    void solve (const double & a, const double & b, const double & c) {
        if (isZero(a)) {
            if(isZero(b)) {
                if (isZero(c)) {
                    numOfRoots = INF_ROOTS;
                } else /* c != 0 */ {
                    numOfRoots = 0;
                }
            } else /* b != 0 */ {
                numOfRoots = 1;
                x1 = -c / b;
            }
        } else /* a != 0 */ {
            double d = discriminant (a, b, c);
            if (d < 0) {
                numOfRoots = 0;
            } else if (isZero(d)) {
                numOfRoots = 1;
                x1 = -b / (2 * a);
            } else /* d > 0 */ {
                numOfRoots = 2;
                x1 = (-b + sqrt(d)) / (2 * a);
                x2 = (-b - sqrt(d)) / (2 * a);
            }
        }
    }

    /*!
    * Данный метод предназначен для вычисления дискриминанта уравнения. Параметрами являются коэффициенты уравненияю.
    * \return Возвращает значение дискриминанта
    */

    double discriminant (const double & a, const double & b, const double & c) const {
        return b * b - 4 * a * c;
    }

    /*!
    * Данный метод предназначен для вывода корней уравнения, выводит настоящее состояние класса.
    */

    void print () {
        switch (numOfRoots) {
            case INF_ROOTS:
                std::cout << "Infinite roots\n";
                break;
            case 0:
                std::cout << "No roots\n";
                break;
            case 1:
                std::cout << "One root:\nx: " << x1 << "\n";
                break;
            case 2:
                std::cout << "Two roots:\nx1: " << x1 << "\nx2: " << x2 << "\n";
                break;
        }
    }

    /*!
    * Данный метод предназначен для вывода i-го корня уравнения (отсчет ведется с 0), выводит настоящее состояние класса.
    * При бесконечном множестве корней выдае ошибку.
    * \param number Номер корня, который нужно получить
    * \return Значение указанного корня
    */

    double getRoot (int number) const {
        if (numOfRoots == INF_ROOTS) {
            throw INF_ROOTS_ERR;
        }
        if ((number < 0) || (number > numOfRoots - 1)) {
            throw OUT_OF_ROOTS_RANGE;
        }
        if (number == 0) {
            return x1;
        } else {
            return x2;
        }
    }

    /*!
    * Данный метод предназначен для вывода корней уравнения, выводит настоящее состояние класса.
    * При бесконечном множестве корней выдае ошибку.
    * \return Указатель на массив элементов типа double, содержащий значения корней уравнения
    */

    double * getRoots () {
        if (numOfRoots == INF_ROOTS) {
            throw INF_ROOTS_ERR;
        }
        if (numOfRoots == 0) {
            return NULL;
        }
        double * res = new double [numOfRoots];
        if (numOfRoots == 1) {
            res [0] = x1;
        } else {
            res [0] = x1;
            res [1] = x2;
        }
        return res;
    }

    /*!
    * Данный метод предназначен для вывода количества корней уравнения.
    */

    int getRootsNumber () {
        return numOfRoots;
    }

    /*!
    * Данный метод предназначен для естирования функции solve данноко класса.
    * \param a Коэффициент №1
    * \param b Коэффициент №2
    * \param c Коэффициент №3
    * \param num Ожидаемое количество корней
    * \param x1 Ожидаемое значение корня №1
    * \param x2 Ожидаемое значение корня №2
    */

    void testSolve (const double & a, const double & b, const double & c, int num, double x1 = 0, double x2 = 0) {
        solve (a, b, c);
        TEST (numOfRoots, num);
        if ((numOfRoots != INF_ROOTS) && (numOfRoots == num)) {
            if (num == 1) {
                TEST (getRoot (0), x1);
            } else if (num == 2) {
                double r1 = this -> x1;
                double r2 = this -> x2;
                if (((r1 - r2) * (x1 - x2)) < 0) {
                    TEST (r1, x2);
                    TEST (r2, x1);
                } else {
                    TEST (r1, x1);
                    TEST (r2, x2);
                }
            }
        }
    }

    /*!
    * Данный метод предназначен для естирования функции discriminant данноко класса.
    * \param a Коэффициент №1
    * \param b Коэффициент №2
    * \param c Коэффициент №3
    * \param result Ожидаемый результат
    */

    void testDiscriminant (const double & a, const double & b, const double & c, const double & result) {
        TEST (discriminant (a, b, c), result);
    }
};

/*!
* Без указания режима просто считает решение уравнения
* Режимы:
* -test -- Запускает готовые тесты
* -test-now -- Тест в реальном времени
* -test-discrimiant -- Тест дискриминанта
*/

int main (int argc, char ** argv) {
    EquationSolver solver;
    if (argc > 1) {
        if (strcmp (argv [1], "-test") == 0) {
            solver.testSolve (0, 0, 0, INF_ROOTS);
            solver.testSolve (1, 2, 1, 1, -1);
            solver.testSolve (1, -2, 1, 1, 1);
            solver.testSolve (1, 4, 3, 2, -1, -3);
        } else if (strcmp (argv [1], "-test-now") == 0) {
            std::cout << "Please, enter the coefficients\n";
            double a = 0.0, b = 0.0, c = 0.0, x1 = 0.0, x2 = 0.0;
            int num = 0;
            std::cin >> a >> b >> c;
            std::cout << "Please, enter number of the roots\n";
            std::cin >> num;
            if ((num != INF_ROOTS) && (num != 0)) {
                std::cout << "Please, enter the roots\n";
                std::cin >> x1 >> x2;
            }
            solver.testSolve (a, b, c, num, x1, x2);
        } else if (strcmp (argv [1], "-test-disc") == 0) {
            std::cout << "Please, enter the coefficients\n";
            double a = 0.0, b = 0.0, c = 0.0, res = 0.0;
            std::cin >> a >> b >> c;
            std::cout << "Please, enter the result\n";
            std::cin >> res;
            solver.testDiscriminant (a, b, c, res);
        }
    } else {
        double a = 0.0, b = 0.0, c = 0.0;
        std::cout << "Please, enter the coefficients\n";
        std::cin >> a >> b >> c;
        solver.solve (a, b, c);
        solver.print ();
    }
}