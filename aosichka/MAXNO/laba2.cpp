#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <conio.h>


using namespace std;

ifstream fin;

//vector<int> r1, r2, r3, r4;

int PC = 0, TC = 0, PS = 0;


struct Register{
    //Register(vector<int> n):n(n){}
    Register(){
        for (int i = 0; i < 12; i++){n.push_back(0);}
    }

    Register & operator = (Register const& a){
    n = a.n;
    return *this;
}

    ~Register(){n.clear();}

    int operator [] (int k){
        return n[k];
    }

    void save(vector<int> a){n = a;}


    private:
        vector<int> n;


}R1, R2, R3, R4, AKOMULATOR;

vector<Register>REGISTERS = {AKOMULATOR, R1, R2, R3, R4};


ostream & operator << (ostream & out, Register a){
    for (size_t i = 0; i <12; i++){cout << a[i] << " ";}
    return out;
}

void line(){
    for(int i = 0; i < 21; i++){
        cout << "____";
    }
    cout << endl << endl;
}

vector<int> to2(int a){
    vector<int> n;
    for (int i = 0; i < 12; i++){n.push_back(0);}
    if (a < 0){n[0] = 1; a *=-1;}
    for (int i = 0; i < 11; i++){
        n[11 - i] = a % 2;
        a /= 2;
    }
    return n;
}

void status_mov(string s1, string s2){
    cout << " R1  =  " << REGISTERS[1] << " \t Ins = mov | "  + s1 + " | " + s2 << endl
         << " R2  =  " << REGISTERS[2] << " \t PC = " << PC << endl
         << " R3  =  " << REGISTERS[3] << " \t TC = " << TC << endl
         << " R4  =  " << REGISTERS[4] << " \t PS = " << PS << endl << endl;

}

void status_mov(string s1, Register R){
    cout << " R1  =  " << REGISTERS[1] << " \t Ins = mov | "  + s1 + " | " << R << endl
         << " R2  =  " << REGISTERS[2] << " \t PC = " << PC << endl
         << " R3  =  " << REGISTERS[3] << " \t TC = " << TC << endl
         << " R4  =  " << REGISTERS[4] << " \t PS = " << PS << endl << endl;

}

void status_inv(string s1, string s2, string s3){
    cout << " R1  =  " << REGISTERS[1] << " \t Ins = inv | "  + s1 + " | " + s2 << " | " << s3 << endl
         << " R2  =  " << REGISTERS[2] << " \t PC = " << PC << endl
         << " R3  =  " << REGISTERS[3] << " \t TC = " << TC << endl
         << " R4  =  " << REGISTERS[4] << " \t PS = " << PS << endl << endl;

}

void status_inv(string s1, Register R, string s3){
    cout << " R1  =  " << REGISTERS[1] << " \t Ins = inv | "  + s1 + " | " << R << " | " << s3 << endl
         << " R2  =  " << REGISTERS[2] << " \t PC = " << PC << endl
         << " R3  =  " << REGISTERS[3] << " \t TC = " << TC << endl
         << " R4  =  " << REGISTERS[4] << " \t PS = " << PS << endl << endl;

}


void eror(){
    cout << "ERROR. SEGMET FOULT" << endl;
}

int Register_check(string s){
    if (s == "R1"){return 1;}
    if (s == "R2"){return 2;}
    if (s == "R3"){return 3;}
    if (s == "R4"){return 4;}
    return 0;
}

int oper_int(string s){
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
        else{throw logic_error("not a constant");}
    }
    if (s[0] == '-'){sum *= -1;}
    return sum;
}

void mov(int reg_num1, int reg_num2){
    TC = 2;
    REGISTERS[reg_num1] = REGISTERS[reg_num2];
    PS = REGISTERS[reg_num1][0];
}

bool ask(string s){
    if (s.size() != 1){throw logic_error("Syntax ERROR");}
    if(s[0] - '0' == 1){return 1;}
    if(s[0] - '0' == 0){return 0;}
    throw logic_error("Syntax ERROR");
}

void inv(int reg_num1, int reg_num2, bool a){
    TC = 2;
    int i = 0;
    vector<int>p;
    if (a){i = 1;}
    for (int j = 0; j < 12; j++){p.push_back(REGISTERS[reg_num2][j]);}
    for (; i < 12; i += 2){
        p[i] = !p[i];
    }
    REGISTERS[reg_num1].save(p);
    PS = REGISTERS[reg_num1][0];
}



void glavnaya_functia(){

    string s, s1 = "", s2 = "", s3 = "";

    int operand1 = 0, operand2 = 0, operand3 = 0; 

    while(!fin.eof()){
        s = "" ;
        fin >> s;
        if (s == ""){continue;}
        else if (s == "mov"){
            try
            {
                s1 = "";
                s2 = "";
                fin >> s1 >> s2;

                operand1 = Register_check(s1);
                if (operand1 == 0){throw logic_error("foooo");}
            
                operand2 = Register_check(s2);
                if (operand2 != 0){
                    TC = 1; PC += 1;
                    cout << " Komanda  " << s << " " << s1 << " " << s2 << endl;
                    status_mov(s1,s2);
                    getch();
                    mov(operand1, operand2);
                    status_mov(s1, s2);
                }

                else{
                    TC = 1; PC += 1;
                    operand2 = oper_int(s2);
                    REGISTERS[0].save(to2(operand2));
                    cout << " Komanda  " << s << " " << s1 << " " << s2 << endl;
                    status_mov(s1,REGISTERS[0]);
                    getch();
                    mov(operand1, 0);
                    status_mov(s1,REGISTERS[0]);
                }
                line();

            }
            catch (const exception & ex){
                eror();
                cout << ex.what();
                return;
            }
        }
        else if (s == "inv"){
            try
            {
                fin >> s1 >> s2 >> s3;

                operand1 = Register_check(s1);
                if (operand1 == 0){throw logic_error("foooo");}


                operand2 = Register_check(s2);
                if (operand2 == 0){
                    operand2 = oper_int(s2);
                    REGISTERS[0].save(to2(operand2));
                    operand3 = ask(s3);
                    TC = 1; PC += 1;

                    cout << " Komanda  " << s << " " << s1 << " " << s2 << " " << s3 << endl;
                    status_inv(s1,REGISTERS[0],s3);
                    getch();
                    inv(operand1, 0, operand3);
                    status_inv(s1,REGISTERS[0],s3);
                }
                else{
                    operand3 = ask(s3);
                    TC = 1; PC += 1;

                    cout << " Komanda  " << s << " " << s1 << " " << s2 << " " << s3 << endl;
                    status_inv(s1,s2,s3);
                    getch();
                    inv(operand1, operand2, operand3);
                    status_inv(s1,s2,s3);
                }
                line();

            }
            catch (const exception & ex){
                eror();
                cout << ex.what() << endl;
                return;
            }
        }
        else{eror();break;}
        getch();
    }
}

int main(){

    string path = "Python.txt";
    fin.open(path);

    if (!fin.is_open()){
        cout << "file is not open. It is your problem";
    }
    else{
        glavnaya_functia();

    }

    return 1;
}