#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;
//3.1.15
fstream maFile;

int R1[12], R2[12], R3[12], PS = 0, PC = 0, TC = 0, n, oper;
string command, operand, operand1, a;

void pars();

void ctu() {
    maFile << "R1:" << setw(3) << ' ';
    for (int i = 11; i >= 0; i--) {
        maFile << setw(2) << R1[i];
    }
    maFile << endl;
    maFile << "R2:" << setw(3) << ' ';
    for (int i = 11; i >= 0; i--) {
        maFile << setw(2) << R2[i];
    }
    maFile << endl;
    maFile << "R3:" << setw(3) << ' ';
    for (int i = 11; i >= 0; i--) {
        maFile << setw(2) << R3[i];
    }
    maFile << endl;
    maFile << "RS:" << setw(4) << ' ' << PS << endl;
    maFile << "PC:" << setw(4) << ' ' << PC << endl;
    maFile << "TC:" << setw(4) << ' ' << TC << endl;
}

void mv(int (&R)[12], int nn) {
    int n = nn;
    if (n < 0) {
        R[11] = 1;
        n = -n;
    }
    else R[11] = 0;
    for (int i = 0; i < 11; i++) {
        R[i] = n % 2;
        n /= 2;
    }
}

void mov(int(&R)[12], int nn) {
    PC++, TC = 1;
    ctu();
    mv(R, nn);
    maFile << '\n' << "IR:" << setw(4) << ' ' << command << ' ' << operand1 << ',' << oper << endl;
    TC++;
    if (R[11] == 1) PS = 1;
    else PS = 0;
    ctu();
    maFile << endl;
    cout << '\n' << "done :/" << '\n' << endl;
}

void v15(int(&R)[12], int l, int r) {
    if (l != r) {
        int tmp = R[l % 12];
        R[l % 12] = R[r % 12];
        R[r % 12] = tmp;
    }
}

void v15(int(&L)[12], int(&R)[12]) {
    int l = 0, r = 0;
    for (int i = 0; i < 6; i++)
        r += (R[i] * pow(2, i));
    for (int i = 0; i < 6; i++)
        l += (R[i + 6] * pow(2, i));
    v15(L, l, r);
}

void var15(int(&L)[12],int(&R)[12]) {
    PC++, TC = 1;
    ctu();
    v15(L, R);
    maFile << '\n' << "IR:" << setw(4) << ' ' << command << ' ' << operand1 << ',' << operand << endl;
    TC++;
    if (L[11] == 1) PS = 1;
    else PS = 0;
    ctu();
    maFile << endl;
    cout << '\n' << "done :/" << '\n' << endl;
}

void var15(int(&R)[12], int a) {
    PC++, TC = 1;
    ctu();
    v15(R, a / 100, a % 100);
    maFile << '\n' << "IR:" << setw(4) << ' ' << command << ' ' << operand1 << operand << endl;
    TC++;
    if (R[11] == 1) PS = 1;
    else PS = 0;
    ctu();
    maFile << endl;
    cout << '\n' << "done :/" << '\n' << endl;
}

void inv() {
    cout << '\n' << "Invalid input, try again " << '\n' << endl;
    pars();
    return;
}

void pars() {
    cout << "To input command write \"comandname operand1,operand2\"" << endl;
    cout << "Command list: " << endl
         << "    mov Rn,n" << endl
         << "    var15 Rn,Rn" << endl
         << "    var15 Rn,AABB" << endl
         << "    ext" << endl;
    getline(cin, operand);
    maFile << "IR:" << setw(4) << ' ' << operand << endl;

    if (operand == "ext")
        return;
    else if (operand.size() < 8)
        inv();

    command.clear();
    operand1.clear();
    while (operand.size() > 1 && operand[0] != ' ') {
        command += operand[0];
        operand = operand.substr(1, operand.size() - 1);
    }
    operand = operand.substr(1, operand.size() - 1);
    while (operand.size() > 1 && operand[0] != ',') {
        operand1 += operand[0];
        operand = operand.substr(1, operand.size() - 1);
    }

    if (command == "mov") {
        operand = operand.substr(1, operand.size() - 1);
        if (operand[0] == '-') {
            operand = operand.substr(1, operand.size() - 1);
            oper = -stoi(operand);
        }
        else oper = stoi(operand);
        if (operand1 == "R1")
            mov(R1, oper);
        else if (operand1 == "R2")
            mov(R2, oper);
        else if (operand1 == "R3")
            mov(R3, oper);
        else inv();
    }
    else if (command == "var15" && operand.size() == 5) {
        oper = stoi(operand.substr(1, operand.size() - 1));
        if (operand1 == "R1")
            var15(R1, oper);
        else if (operand1 == "R2")
            var15(R2, oper);
        else if (operand1 == "R3")
            var15(R3, oper);
        else inv();
    }
    else if (command == "var15") {
        operand = operand.substr(1, operand.size() - 1);
        if (operand1 == "R1")
            if (operand == "R1")
                var15(R1, R1);
            else if (operand == "R2")
                var15(R1, R2);
            else if (operand == "R3")
                var15(R1, R3);
            else inv();
        else if (operand1 == "R2")
            if (operand == "R1")
                var15(R2, R1);
            else if (operand == "R2")
                var15(R2, R2);
            else if (operand == "R3")
                var15(R2, R3);
            else inv();
        else if (operand1 == "R3")
            if (operand == "R1")
                var15(R3, R1);
            else if (operand == "R2")
                var15(R3, R2);
            else if (operand == "R3")
                var15(R3, R3);
            else inv();
        else inv();
    }
    pars();
    return;
}

int main() {
    maFile.open("output.txt");
    try {
        pars();
    }
    catch (invalid_argument e) {
        inv();
    }
    catch (out_of_range e) {
        inv();
    }
    maFile.close();

    return 0;
}
