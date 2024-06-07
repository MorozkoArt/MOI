#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>

using std::cout;
using std::cin;
using std::endl;
using std::string;

bool plane_is_valid(int num_v, bool way, double* istr) // пролверяет наличие отричательных элементов при функции на max и наоборот
{
    int i;
    bool result = true;
    if (way)
        for (i = 0; i < num_v * 2; i++)
            if (istr[i] < 0) {
                result = false;
                break;
            }
    if (!way)
        for (i = 0; i < num_v * 2; i++)
            if (istr[i] >= 0) {
                result = false;
                break;
            }

    return result;
}

bool function_is_undefined(int num_l, double* th) // проверка на наличие отрицательных элементов в столбце th
{
    int i;
    for (i = 0; i < num_l; i++)
        if (th[i] < 0) {
            return false;
        }
    return true;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    double* function; // хранит значения коэффициентов целевой функции задачи.
    double* fm; // хранит в себе значения свободных членов системы ограничений. 
    double** System; // хранит в себе значения коэффициентов самой системы ограничений. 
    int* sign; // хранит в себе знак каждого ограничения системы.
    int num_v; // хранит в себе значение количества переменных исходной задачи.
    int num_l; // хранит в себе значение количества ограничений исходной задачи.
    bool way; // хранит в себе направление целевой функции задачи (min/max).
    double func2; // содержит значение целевой фукнции.
    double** bv; // Содержит значения базисных переменных задачи. 
    double** sv; // Матрица коэффициентов при переменных задачи размером num_l * num_v * 2.
    double* istr; /* индексная строка, является одномерным массивом размером num_v * 2, первая половина которого заполняется коэффициентами функции - цели с обратным знаком,
                    а вторая нулями на первой итерации. */
    double* th; // последний столбец симплексной таблицы, инициализируется одномерным массивом размером num_l.
    double alm; // разрешающий элемент, находящийся на пересечении ведущего столбца и ведущей строки.
    int i_lrow; // индекс ведущей строки текущего плана.
    int i_lcol; // индекс ведущего столбца текущего плана.
    std::stringstream table; //объект класса std::stringstream, который содержит весь пользовательский вывод в выходной файл.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Ввод данных
    string num_limits, num_vars, s_var, fr_m, sn, func, w;
    int i, j;

    cout << "Введите количество ограничений в системе: ";
    getline(cin, num_limits);
    num_l = atoi(num_limits.c_str());

    cout << "Введите количество переменных в системе ограничений: ";
    getline(cin, num_vars);
    num_v = atoi(num_vars.c_str());
  
    function = new double[num_v];
    System = new double* [num_l];
    for (i = 0; i < num_l; i++)
        System[i] = new double[num_v];
    fm = new double[num_l];
    sign = new int[num_l];

    cout << "\nЗаполните коэффициенты при целевой функции.\n" << endl;
    for (i = 0; i < num_v; i++) {
        cout << "Введите коэффициент целевой фукнции при x" << i + 1 << ": ";
        getline(cin, func);
        function[i] = atof(func.c_str());                                                   
    }

    cout << "Введите направление целевой функции ( min, max ) : ";
    getline(cin, w);
    if (w == "max" || w == "MAX")
        way = true;
    else
        way = false;
        
    cout << "\nЗаполните систему ограничений.\n" << endl;
    for (i = 0; i < num_l; i++) {
        cout << "Заполните " << i + 1 << "-е ограничение.\n" << endl;
        for (j = 0; j < num_v; j++) {
            cout << "Введите коэффициэнт при x" << j + 1 << ": ";
            getline(cin, s_var);
            System[i][j] = atof(s_var.c_str());
        }
        cout << "Введите знак при " << i + 1 << "-м ограничении ( <=, =, >= ) : ";
        getline(cin, sn);
        if (sn == "<=")
            sign[i] = 0;
        if (sn == "=")
            sign[i] = 1;
        if (sn == ">=")
            sign[i] = 2;
        cout << "Введите свободный член при " << i + 1 << "-м ограничении: ";
        getline(cin, fr_m);                            
        fm[i] = atof(fr_m.c_str());
        cout << endl;
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Алгоритм 
      // первый опорный план
    func2 = 0;
    sv = new double* [num_l];
    // Заполнение матрицы коэффицентов
    for ( i = 0; i < num_l; i++)
        sv[i] = new double[num_v * 2];
    for (i = 0; i < num_l; i++) {
        for (j = 0; j < num_v; j++)
            sv[i][j] = System[i][j];
        for (; j < num_v * 2; j++)
            if (i + num_v == j)
                if (way)
                    sv[i][j] = 1;
                else
                    sv[i][j] = -1;
            else
                sv[i][j] = 0;
    }
    istr = new double[num_v * 2];
    bv = new double* [num_l];
    for (i = 0; i < num_l; i++) {
        bv[i] = new double[2];
        bv[i][0] = i + num_v;
        bv[i][1] = fm[i];
    }
    for (i = 0; i < num_v * 2; i++)
        if (i < num_v)
            istr[i] = function[i] * -1;
        else
            istr[i] = 0;
    //  вычисляется индекс ведущего столбца
    i_lcol = 0;
    for (i = 0; i < num_v * 2 - 1; i++) {
        if (istr[i] < 0)
            if (fabs(istr[i + 1]) > fabs(istr[i]))
                i_lcol = i + 1;
    }
    // записывает в th результаты деление свободных членов на индексы ведущего столбца
    th = new double[num_l];
    for (i = 0; i < num_l; i++)
        th[i] = bv[i][1] / sv[i][i_lcol];
    // находим индекс ведущёй строки
    i_lrow = 0;
    for (i = 0; i < num_l - 1; i++)
        if (th[i] > th[i + 1])
            i_lrow = i + 1;
    alm = sv[i_lrow][i_lcol];
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // вывод в файл...
    table << "Задана целевая функция:\n" << endl;
    std::stringstream f_x;
    f_x << "f(x) = ";
    for (i = 0; i < num_v; i++) {
        if (!i)
            f_x << function[i] << "x" << i + 1 << " ";
        else {
            if (function[i] < 0)
                f_x << "- " << fabs(function[i]) << "x" << i + 1 << " ";
            if (function[i] > 0)
                f_x << "+ " << function[i] << "x" << i + 1 << " ";
        }
    }
    string minmax;
    if (way)
        minmax = "max";
    else
        minmax = "min";
    f_x << "=> " << minmax << "\n" << endl;
    table << f_x.str();
    table << "И система ограничений:\n" << endl;
    std::stringstream math_sys;
    string math_sign;
    for (i = 0; i < num_l; i++) {
        for (j = 0; j < num_v; j++) {
            if (!j)
                math_sys << System[i][j] << "x" << j + 1 << " ";
            else
                if (System[i][j] == 1)
                    if (!j)
                        math_sys << "x" << j + 1 << " ";
                    else
                        math_sys << "+ x" << j + 1 << " ";
                else
                    if (System[i][j] == -1)
                        if (!j)
                            math_sys << "-x" << j + 1 << " ";
                        else
                            math_sys << "- x" << j + 1 << " ";
                    else {
                        if (System[i][j] < 0)
                            math_sys << "- " << fabs(System[i][j]) << "x" << j + 1 << " ";
                        else
                            math_sys << "+ " << System[i][j] << "x" << j + 1 << " ";
                        if (!sign[i])
                            math_sign = "<=";
                        if (sign[i] == 1)
                            math_sign = "=";
                        if (sign[i] == 2)
                            math_sign = ">=";
                    }
        }

        math_sys << math_sign << " " << fm[i];
        math_sys << endl;
    }
    string min_or_max;
    if (way)
        min_or_max = "максимум";
    else
        min_or_max = "минимум";
    table << math_sys.str() << endl;
    table << "Решим данную задачу на " << min_or_max << " методом симплексных таблиц.\nПостроим первый опорный план:\n" << endl;

    for (i = 0; i < num_l; i++) {

        table << "x" << bv[i][0] + 1 << "\t" << bv[i][1] << "\t";
        for (j = 0; j < num_v * 2; j++)
            table << sv[i][j] << "\t";
        if (!plane_is_valid(num_v, way, istr))
            table << th[i];
        table << "\n" << endl;
    }
    table << "f(x)\t" << func2 << "\t";
    for (i = 0; i < num_v * 2; i++)
        table << istr[i] << "\t";
    table << "\n";
    if (plane_is_valid(num_v, way, istr)) {
        if (plane_is_valid(num_v, way, istr) && function_is_undefined(num_l, th))
            table << "\nДанный план является оптимальным и не требует улучшения. Решение найдено." << endl;
        std::ofstream outfile("table.txt");
        outfile << table.str();
    }
    else {
        string ln_or_gn;
        if (way)
            ln_or_gn = "неположительные";
        else
            ln_or_gn = "положительные";
        std::stringstream num_of_plane;
        if (!0)
            num_of_plane << "Первый опорный план";
        else
            num_of_plane << 0 + 1 << "-й план также";
        table << "\n" << num_of_plane.str() << " не является оптимальным, поскольку\nв индексной строке присутствуют " << ln_or_gn << " элементы.\nErо необходимо улучшить.\n" << endl;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // вычисление оптимального плана
    int it_num = 0;
    double A, B;
    while (!plane_is_valid(num_v, way, istr) && function_is_undefined(num_l, th)) {
        A = bv[i_lrow][1];
        B = istr[i_lcol];
        func2 -= A * B / alm;
        double* tmp_bv = new double[num_l];
        bv[i_lrow][0] = i_lcol;
        A = bv[i_lrow][1];
        for (i = 0; i < num_l; i++) {
            B = sv[i][i_lcol];
            tmp_bv[i] = bv[i_lrow][1];
            if (i != i_lrow)
                tmp_bv[i] = bv[i][1] - A * B / alm;
            else
                tmp_bv[i] /= alm;
        }
        for (i = 0; i < num_l; i++)
            bv[i][1] = tmp_bv[i];
        double* tmp_istr = istr;
        B = istr[i_lcol];
        for (i = 0; i < num_v * 2; i++) {
            A = sv[i_lrow][i];
            tmp_istr[i] = istr[i] - A * B / alm;
        }
        istr = tmp_istr;
        double** tmp_sv = new double* [num_l];
        for (i = 0; i < num_l; i++)
            tmp_sv[i] = new double[num_v * 2];
        for (i = 0; i < num_l; i++)
            for (j = 0; j < num_v * 2; j++) {
                tmp_sv[i][j] = sv[i][j];
                A = sv[i_lrow][j];
                B = sv[i][i_lcol];
                if (i == i_lrow)
                    tmp_sv[i][j] /= alm;
                else
                    tmp_sv[i][j] = sv[i][j] - A * B / alm;
            }
        sv = tmp_sv;
        i_lcol = 0;
        for (i = 0; i < num_l; i++)
            th[i] = bv[i][1] / sv[i][i_lcol];
        i_lrow = 0;
        for (i = 0; i < num_l - 1; i++)
            if (th[i] > th[i + 1])
                i_lrow = i + 1;
        alm = sv[i_lrow][i_lcol];
        it_num++;
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Вывод в файл
        for (i = 0; i < num_l; i++) {

            table << "x" << bv[i][0] + 1 << "\t" << bv[i][1] << "\t";
            for (j = 0; j < num_v * 2; j++)
                table << sv[i][j] << "\t";
            if (!plane_is_valid(num_v, way, istr))
                table << th[i];
            table << "\n" << endl;
        }
        table << "f(x)\t" << func2 << "\t";
        for (i = 0; i < num_v * 2; i++)
            table << istr[i] << "\t";
        table << "\n";
        if (plane_is_valid(num_v, way, istr)) {
            if (plane_is_valid(num_v, way, istr) && function_is_undefined(num_l, th))
                table << "\nДанный план является оптимальным и не требует улучшения. Решение найдено." << endl;
            std::ofstream outfile("table.txt");
            outfile << table.str();
        }
        else {
            string ln_or_gn;
            if (way)
                ln_or_gn = "неположительные";
            else
                ln_or_gn = "положительные";
            std::stringstream num_of_plane;
            if (!it_num)
                num_of_plane << "Первый опорный план";
            else
                num_of_plane << it_num + 1 << "-й план также";
            table << "\n" << num_of_plane.str() << " не является оптимальным, поскольку\nв индексной строке присутствуют " << ln_or_gn << " элементы.\nErо необходимо улучшить.\n" << endl;
        }
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (!function_is_undefined(num_l, th))
        cout << "\nЦелевая фукнция не ограничена, данная задача решений не имеет\n" << endl;
    else {
        cout << "\nf(x) = " << func2 << "\n" << endl;
        for (i = 0; i < num_l; i++) {
            cout << "x" << bv[i][0] + 1 << " = " << bv[i][1] << endl;
        }
        cout << "\nВсе вычисления были записаны в файл table.txt\n" << endl;
    }
}