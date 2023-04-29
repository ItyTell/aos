#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <conio.h>
#include <cmath>

std::ifstream fin;

int PC = 0, TC = 0, PS = 0, PS0 = 0, queu = 0;

std::string path = "C:/c++ code/laba3_aos/aos/x64/Debug/programa.txt";

int HARAKTERISTICA = 8;
int MANTISA = 23;

std::vector<int> haracteristic(int k) {
    int a = 0;
    if (k < 0) { a = 1; k *= -1; }
    std::vector<int> n; int i; for (i = 0; i < HARAKTERISTICA; i++) { n.push_back(0); }
    for (i = 1 ; i < HARAKTERISTICA; i++) {
        n[HARAKTERISTICA - i] = k % 2;
        k /= 2;
    }
    n[0] = a;
    return n;
}


struct Register {

    // це зараз не маЇ значенн€
    Register() :n(0) {
        s = 0;
        for (int i = 0; i < HARAKTERISTICA; i++) { h.push_back(0);}
        for (int i = 0; i < MANTISA; i++) { m.push_back(0); }
    }

    // це зараз не маЇ значенн€
    Register& operator = (Register const& a) { n = a.n; m = a.m; h = a.h; s = a.s; return *this; }

    //збер≥гаЇ число у екземпл€р≥ классу
    Register(double a) :n(a) {
        if (a == 0) { s = 0; for (int i = 0; i < HARAKTERISTICA; i++) { h.push_back(0); }for (int i = 0; i < MANTISA; i++) { m.push_back(0); } return; }
        if (a < 0) { a *= -1; s = 1; }
        int k = 0;
        if (a >= 1) {
            while (a >= 1) {
                a /= 2;
                k += 1;
            }
        }
        else {
            while (a < 0.5) {
                a *= 2;
                k -= 1;
            }
        }
        h = haracteristic(k);
        for (int i = 0; i < MANTISA; i++) {
            a *= 2;
            if (a >= 1) { a -= 1; m.push_back(1); }
            else { m.push_back(0); }
        }

    }

    double n = 0; // чисоло в дес€тковому вигл€д≥
    std::vector<int> m; //мантиса
    std::vector<int> h; // характеристика
    bool s = 0; // знак 

}A, R1, R2, R3, R4, R5, R6, R7, R8;

// додаванн€ двох чисел
Register operator + (Register A1, Register A2) {
    double result = A1.n + A2.n;
    if (result > 0) { PS = 0; }
    else { PS = 1; }
    Register A(result);
    return A;
}

Register operator / (Register A1, Register A2) {
    if (A2.n == 0) { throw std::logic_error("Divide by zero!!!"); }
    double result = A1.n / A2.n;
    if (result > 0) { PS = 0; }
    else { PS = 1; }
    Register A(result);
    return A;
}


std::vector<Register> REGISTERS = { A, R1, R2, R3, R4, R5, R6, R7, R8 };

std::ostream& operator << (std::ostream& out, Register a) {
    std::cout << a.s << "   ";
    for (size_t i = 0; i < HARAKTERISTICA; i++) { std::cout << a.h[i] << " "; } std::cout << "  ";
    for (size_t j = 0; j < MANTISA; j++) { std::cout << a.m[j] << " "; }
    return out; }

double Double(std::string s) {
    int l = 0, k = 10;
    double sum = 0, p = 0;
    int i = 0;
    bool flag = 1;
    if (s[0] == '-') { i = 1; }
    size_t size = s.size();
    for (; i < size; i++) {
        l = int(s[i]);
        if (flag) {sum *= 10;}
        if (l >= 48 && l <= 57) {
             p = l - 48;
             if (!flag) { p /= k; k *= 10; }
             sum += p;
        }
        else if (l == 46) {
            if (!flag){ throw std::logic_error("two points"); }
            sum /= 10;
            flag = 0;
        }
        else { throw std::logic_error("not a constant"); }
    }
    if (s[0] == '-') { sum *= -1; PS0 = 1; }
    else { PS0 = 0; }
    return sum;
}


void status() {
    for (int i = 1; i <= 8; i++) { std::cout << "  R" << i << " " << REGISTERS[i] << std::endl; }
    std::cout << "\n PS = " << PS << "\n PC = " << PC << "\n TC = " << TC << "\n\n";
    _getch();
}

void line() { for (int i = 0; i < 24; i++) { std::cout << "___"; } std::cout << std::endl; }

void Move(std::string s) {
    PC += 1;
    queu += 1;
    if (queu > 8) { throw std::logic_error("memory is full"); }
    TC = 1;
    std::cout<< " IR: mov|" + s + "|" << std::endl;
    status();
    double operand = Double(s);
    TC = 2;
    REGISTERS[queu] = Register(operand);
    status();
}

void Add() {
    PC += 1;
    queu -= 1;
    if (queu < 1) { queu = 1; }
    TC = 1;
    std::cout << " IR: add" << std::endl;
    status();
    TC = 2;
    Register a = REGISTERS[queu] + REGISTERS[queu + 1];
    REGISTERS[queu] = a;
    status();
}

void Div() {
    PC += 1;
    queu -= 1;
    if (queu < 1) { queu = 1; }
    TC = 1;
    std::cout << " IR: div" << std::endl;
    status();
    TC = 2;
    Register a = REGISTERS[queu] / REGISTERS[queu + 1];
    REGISTERS[queu] = a;
    status();
}

void start() {
    std::string s, s1;

    while (!fin.eof()) {
        try {
            s = "";
            fin >> s;
            if (s == "") { continue; }
            if (s == "mov") {
                s1 = "";
                fin >> s1;
                Move(s1);
            }
            else if (s == "add") {
                Add();
            }
            else if (s == "div") {
                Div();
            }
            line();
        }
        catch (const std::exception& ex) {
            std::cout << ex.what();
            return;
        }

    }
}


int main() {
    fin.open(path);
    if (!fin.is_open()) {
        std::cout << "file is not open.";
    }
    else {
        start();
    }
    return 0;
}