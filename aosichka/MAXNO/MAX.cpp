#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <conio.h>

std::ifstream fin;

int PC = 0, TC = 0, PS = 0, PS0 = 0;

std::string path = "programa.txt";



struct Register{

    Register(){for (int i = 1; i <= 20; i++){n.push_back(0);}}

    int operator [](int k){return n[k];}

    void set(int k, int p){n[k] = p;}

    Register& operator = (Register const& a) { n = a.n; return *this; }

    void save(int a){

        int b = abs(a);
        int i;
        for (i = 0; i < 20; i++){
            n[19 - i] = b % 2;
            b /= 2;
        }
        if ( a < 0){
            for (i = 0; i < 20;i++){n[i] = !n[i];}
            n[19] += 1;
            for (i = 0; i < 19; i++){
                if (n[19 - i] == 2){n[18 - i] += 1; n[19 - i] = 0;}
            }
            if (n[0] == 2){n[0] = 0;}
        }
    }


    private:
        std::vector<int> n;

}A, R1, R2, R3, R4, AK47;

std::vector<Register> REGISTERS = {A, R1, R2, R3, R4, AK47};

std::ostream & operator << (std::ostream & out, Register a){for (size_t i = 0; i < 20; i++){std::cout << a[i] << " ";}return out;}

int Register_check(std::string s){

    if (s == "R1"){return 1;}
    if (s == "R2"){return 2;}
    if (s == "R3"){return 3;}
    if (s == "R4"){return 4;}
    return 0;
}

int integer(std::string s){
    int l = 0, sum = 0;
    int i = 0;
    if (s[0] == '-'){i = 1;}
    size_t size = s.size();
    for (; i < size; i++){
        l = int(s[i]);
        sum *= 10;
        if (l >= 48 && l <= 57){
            sum += l - 48;
        }
        else{throw std::logic_error("not a constant");}
    }
    if (s[0] == '-'){sum *= -1; PS0 = 1;}
    else {PS0 = 0;}
    return sum;
}

void status(std::string s1, std::string komand){
    std::cout << " A   =  " << REGISTERS[0] << " \t Ins = " + komand + " | "  + s1 << std::endl
         << " R1  =  " << REGISTERS[1] << " \t PC = " << PC << std::endl
         << " R2  =  " << REGISTERS[2] << " \t TC = " << TC << std::endl
         << " R3  =  " << REGISTERS[3] << " \t PS = " << PS << std::endl 
         << " R4  =  " << REGISTERS[4] << std::endl << std::endl;
}

void status(Register R, std::string komand){
    std::cout << " A   =  " << REGISTERS[0] << " \t Ins = " + komand + " | "  << R << std::endl
         << " R1  =  " << REGISTERS[1] << " \t PC = " << PC << std::endl
         << " R2  =  " << REGISTERS[2] << " \t TC = " << TC << std::endl
         << " R3  =  " << REGISTERS[3] << " \t PS = " << PS << std::endl 
         << " R4  =  " << REGISTERS[4] << std::endl << std::endl;
}

void mov(int reg = 5){TC = 2; REGISTERS[0] = REGISTERS[reg];}

void save(int reg){TC = 2; REGISTERS[reg] = REGISTERS[0];}

int quon(bool k){TC = 2;int sum = 0;for (int i = 0; i < 20; i++){sum += REGISTERS[0][i];}if (!k){sum = 20 - sum;} PS = 0;return sum;}

void line(){for(int i = 0; i < 30; i++){std::cout << "____";}std::cout << std::endl << std::endl;}

void Move(std::string s){

    std::cout << "komand = mov " + s << std::endl;
    if (Register_check(s)){
        int reg = Register_check(s);
        TC = 1; PC += 1;
        status(s, "mov");
        getch();
        mov(reg);
        status(s, "mov");
    }
    else{
        REGISTERS[5].save(integer(s));
        TC = 1; PC += 1;
        status(REGISTERS[5], "mov");
        getch();
        PS = PS0;
        mov();
        status(REGISTERS[5], "mov");
    }
    line();
    getch();

}

void Save(std::string s){
    std::cout << "komand = save " + s << std::endl;
    if (Register_check(s)){
        int reg = Register_check(s);
        TC = 1; PC += 1;
        status(s, "save");
        getch();
        save(reg);
        status(s, "save");
    }
    else {throw std::logic_error(s + " is not a register");}
    line();
    getch();
}

void Quon(std::string s){
    int k = integer(s);
    if (k == 1|| k == 0){
        std::cout << "komand = quon " + s << std::endl;
        TC = 1; PC += 1;
        status(s, "quon");
        k = quon(k);
        REGISTERS[0].save(k);
        getch();
        status(s, "quon");
    }
    else {throw std::logic_error(s +" not 0 or 1");}
    line();
    getch();
}

void start(){
    std::string s, s1;

    while(!fin.eof()){
        try{
            s = "";
            fin >> s;
            s1 = "";
            if (s == ""){continue;}
            fin >> s1;
            if (s == "mov"){
                Move(s1);
            }
            else if (s == "save"){
                Save(s1);
            }
            else if (s == "quon"){
                Quon(s1);
            }
        }
        catch (const std::exception & ex){
                std::cout << ex.what();
                return;
            }

    }

}


int main(){
    
    fin.open(path);

    if (!fin.is_open()){
        std::cout << "file is not open.";
    }
    else{
        start();
    }
    return 0;
}
