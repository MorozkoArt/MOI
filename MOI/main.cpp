#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>

using std::cout;
using std::cin;
using std::endl;
using std::string;

bool plane_is_valid(int num_v, bool way, double* istr) // ���������� ������� ������������� ��������� ��� ������� �� max � ��������
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

bool function_is_undefined(int num_l, double* th) // �������� �� ������� ������������� ��������� � ������� th
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
    double* function; // ������ �������� ������������� ������� ������� ������.
    double* fm; // ������ � ���� �������� ��������� ������ ������� �����������. 
    double** System; // ������ � ���� �������� ������������� ����� ������� �����������. 
    int* sign; // ������ � ���� ���� ������� ����������� �������.
    int num_v; // ������ � ���� �������� ���������� ���������� �������� ������.
    int num_l; // ������ � ���� �������� ���������� ����������� �������� ������.
    bool way; // ������ � ���� ����������� ������� ������� ������ (min/max).
    double func2; // �������� �������� ������� �������.
    double** bv; // �������� �������� �������� ���������� ������. 
    double** sv; // ������� ������������� ��� ���������� ������ �������� num_l * num_v * 2.
    double* istr; /* ��������� ������, �������� ���������� �������� �������� num_v * 2, ������ �������� �������� ����������� �������������� ������� - ���� � �������� ������,
                    � ������ ������ �� ������ ��������. */
    double* th; // ��������� ������� ����������� �������, ���������������� ���������� �������� �������� num_l.
    double alm; // ����������� �������, ����������� �� ����������� �������� ������� � ������� ������.
    int i_lrow; // ������ ������� ������ �������� �����.
    int i_lcol; // ������ �������� ������� �������� �����.
    std::stringstream table; //������ ������ std::stringstream, ������� �������� ���� ���������������� ����� � �������� ����.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // ���� ������
    string num_limits, num_vars, s_var, fr_m, sn, func, w;
    int i, j;

    cout << "������� ���������� ����������� � �������: ";
    getline(cin, num_limits);
    num_l = atoi(num_limits.c_str());

    cout << "������� ���������� ���������� � ������� �����������: ";
    getline(cin, num_vars);
    num_v = atoi(num_vars.c_str());
  
    function = new double[num_v];
    System = new double* [num_l];
    for (i = 0; i < num_l; i++)
        System[i] = new double[num_v];
    fm = new double[num_l];
    sign = new int[num_l];

    cout << "\n��������� ������������ ��� ������� �������.\n" << endl;
    for (i = 0; i < num_v; i++) {
        cout << "������� ����������� ������� ������� ��� x" << i + 1 << ": ";
        getline(cin, func);
        function[i] = atof(func.c_str());                                                   
    }

    cout << "������� ����������� ������� ������� ( min, max ) : ";
    getline(cin, w);
    if (w == "max" || w == "MAX")
        way = true;
    else
        way = false;
        
    cout << "\n��������� ������� �����������.\n" << endl;
    for (i = 0; i < num_l; i++) {
        cout << "��������� " << i + 1 << "-� �����������.\n" << endl;
        for (j = 0; j < num_v; j++) {
            cout << "������� ����������� ��� x" << j + 1 << ": ";
            getline(cin, s_var);
            System[i][j] = atof(s_var.c_str());
        }
        cout << "������� ���� ��� " << i + 1 << "-� ����������� ( <=, =, >= ) : ";
        getline(cin, sn);
        if (sn == "<=")
            sign[i] = 0;
        if (sn == "=")
            sign[i] = 1;
        if (sn == ">=")
            sign[i] = 2;
        cout << "������� ��������� ���� ��� " << i + 1 << "-� �����������: ";
        getline(cin, fr_m);                            
        fm[i] = atof(fr_m.c_str());
        cout << endl;
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //�������� 
      // ������ ������� ����
    func2 = 0;
    sv = new double* [num_l];
    // ���������� ������� ������������
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
    //  ����������� ������ �������� �������
    i_lcol = 0;
    for (i = 0; i < num_v * 2 - 1; i++) {
        if (istr[i] < 0)
            if (fabs(istr[i + 1]) > fabs(istr[i]))
                i_lcol = i + 1;
    }
    // ���������� � th ���������� ������� ��������� ������ �� ������� �������� �������
    th = new double[num_l];
    for (i = 0; i < num_l; i++)
        th[i] = bv[i][1] / sv[i][i_lcol];
    // ������� ������ ������� ������
    i_lrow = 0;
    for (i = 0; i < num_l - 1; i++)
        if (th[i] > th[i + 1])
            i_lrow = i + 1;
    alm = sv[i_lrow][i_lcol];
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // ����� � ����...
    table << "������ ������� �������:\n" << endl;
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
    table << "� ������� �����������:\n" << endl;
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
        min_or_max = "��������";
    else
        min_or_max = "�������";
    table << math_sys.str() << endl;
    table << "����� ������ ������ �� " << min_or_max << " ������� ����������� ������.\n�������� ������ ������� ����:\n" << endl;

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
            table << "\n������ ���� �������� ����������� � �� ������� ���������. ������� �������." << endl;
        std::ofstream outfile("table.txt");
        outfile << table.str();
    }
    else {
        string ln_or_gn;
        if (way)
            ln_or_gn = "���������������";
        else
            ln_or_gn = "�������������";
        std::stringstream num_of_plane;
        if (!0)
            num_of_plane << "������ ������� ����";
        else
            num_of_plane << 0 + 1 << "-� ���� �����";
        table << "\n" << num_of_plane.str() << " �� �������� �����������, ���������\n� ��������� ������ ������������ " << ln_or_gn << " ��������.\nEr� ���������� ��������.\n" << endl;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // ���������� ������������ �����
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
        // ����� � ����
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
                table << "\n������ ���� �������� ����������� � �� ������� ���������. ������� �������." << endl;
            std::ofstream outfile("table.txt");
            outfile << table.str();
        }
        else {
            string ln_or_gn;
            if (way)
                ln_or_gn = "���������������";
            else
                ln_or_gn = "�������������";
            std::stringstream num_of_plane;
            if (!it_num)
                num_of_plane << "������ ������� ����";
            else
                num_of_plane << it_num + 1 << "-� ���� �����";
            table << "\n" << num_of_plane.str() << " �� �������� �����������, ���������\n� ��������� ������ ������������ " << ln_or_gn << " ��������.\nEr� ���������� ��������.\n" << endl;
        }
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (!function_is_undefined(num_l, th))
        cout << "\n������� ������� �� ����������, ������ ������ ������� �� �����\n" << endl;
    else {
        cout << "\nf(x) = " << func2 << "\n" << endl;
        for (i = 0; i < num_l; i++) {
            cout << "x" << bv[i][0] + 1 << " = " << bv[i][1] << endl;
        }
        cout << "\n��� ���������� ���� �������� � ���� table.txt\n" << endl;
    }
}