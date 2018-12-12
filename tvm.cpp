#include <cctype>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <sstream>
#include <cassert>
#include <map>
#include <string>
#include <cstring>
#include <algorithm>
using namespace std;

/*
 * data.h
 *
 *  Created on: 2016. 12. 3.
 *      Author: woogyun
 */

#ifndef DATA_H_
#define DATA_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>

#define VECT
#define STACKSIZE               1000
#define HEAPSIZE                1000
#define PRINTLINE               10
#define PRINTSIZE               50
#define PRINTSTATISTICS         2
#define PRINTSTATICSWIDTH       15

/*
#define INF     137
#define OUTF    138
#define MOD     139
#define ADDF    140
#define SUBF    141
#define MULF    142
#define DIVF    143
#define NEGF    144
#define EQF     145
#define NEF     146
#define LEF     147
#define LTF     148
#define GEF     149
#define GTF     150
*/

struct labelTag{
    std::string label;
    long  nPos;
};

struct tokenTag{
    std::string opCode;
    std::string value;
    std::string label;
    long  opCnt;
};

// Data Structures
typedef labelTag labels;
typedef std::vector<labels> lbVec;
typedef std::vector<std::string> strVec;
typedef tokenTag token;
typedef std::vector<long> iVec;

std::string lbExtract(lbVec&, std::string, long);
std::string lTrim(std::string);

long   atoc(string);
long   atol(string);
string ltoa(long n);
string ctoa(char c);
string escape(string);

token analToken(std::string);
bool  instrParse(std::string);
void  initStack(iVec&);
long  findLabel(const std::string);

long  nIntSize(long);

// function to manipuate micro operation

void op_ldc(long);
void op_ldo(long);
void op_ind();
void op_sro(long);
void op_sto();
void op_add();
void op_sub();
void op_mul();
void op_div();
void op_neg();
void op_and();
void op_or();
void op_not();
void op_equ();
void op_geq();
void op_leq();
void op_les();
void op_grt();
void op_neq();
void op_fjp(long);
void op_lod(long);
void op_lda(long);
void op_str(long);
void op_mst();
void op_cup(long, long);
void op_ssp(long);
void op_ent(long);
void op_retf();
void op_retp();
void op_movs(long);

void op_new(long);
void op_del();

void op_dup();
void op_pop();
void op_sti(long);

// end of micro operations

long pow(long, long);
bool tokenCheck(string);		// check the op codes
void printState(int, int, int);		// print machine status
string intToStr(const long );

void printResult(void);
void printStatistics(void);
void error(const char *msg);




#endif /* DATA_H_ */

/*==========================================================
  global data
  ===========================================================*/
iVec  Mem(STACKSIZE+HEAPSIZE);
iVec  FreeList(0);
const int STACK_UP = STACKSIZE;
const int HEAP_UP  = STACK_UP + HEAPSIZE;
long  PC, SP = 0, FP, mp, value, HP = STACKSIZE;

lbVec lbVector;
lbVec::iterator lbIter;

strVec Instr;
bool stFlag;
bool STEP;	// step-by-step

const char *OpCode[] = {
    "ldo", "ldc", "ind", "lod", "lda",          // 5     5
    "sro", "sto", "str",                        // 3     8
    "ujp", "fjp",                               // 2    10
    "mst", "cup", "ssp", "retf", "retp",        // 5    15
    "in",  "out", "inc", "outc",                // 4    19
    "add", "sub", "mul", "div", "neg",          // 5    24
    "and",  "or", "not",                        // 3    27
    "equ", "neq", "geq", "grt", "leq", "les",   // 6    33
    "new", "del",                               // 2    35
    "dup", "pop", "sti",                        // 3    38
    "ent", "movs",                              // 2    40
    "stp"                                       // 1    41
};

enum OP {
    LDO, LDC, IND, LOD, LDA,
    SRO, STO, STR,
    UJP, FJP,
    MST, CUP, SSP, RETF,RETP,
    IN,  OUT, INC, OUTC,
    ADD, SUB, MUL, DIV, NEG,
    AND,  OR, NOT,
    EQU, NEQ, GEQ, GRT, LEQ, LES,
    NEW, DEL,
    DUP, POP, STI,
    ENT, MOVS,
    STP
};

const int N_INSTR_CNT = sizeof OpCode / sizeof *OpCode;

string StdOut = "";
string StdIn = "";
int InstrCount[N_INSTR_CNT];
int NumInstrs;

/*===========================================================*/

using namespace std;

// this function extract label & label's position from input string.
std::string lbExtract(lbVec &lVec, std::string str, long  nCnt)
{
    std::string strTmp, strLab;
    labels lbTmp;

    long  nPos;

    strTmp = str;

    for (nPos = 0; nPos != (long)str.size(); nPos++) {
        if (str[nPos] == ':') {
            strLab = str.substr(0, nPos);

            lbTmp.label = strLab;
            lbTmp.nPos = nCnt;

            lVec.push_back(lbTmp);

            strTmp = str.substr(nPos + 1, str.size());
        }
    }

    return strTmp;
}

std::string lTrim(std::string str)
{
    str.erase(str.begin(),
              find_if(str.begin(), str.end(),
                      not1(ptr_fun<int, int>(isspace))));
    return str;
}

enum RdState { RD_GOOD, RD_EOF, RD_BAD, RD_FAIL };

RdState readInt(string &line, long &n)
{
    istringstream in(line);
    if (!(in >> n)) {
        if (in.eof())
            return RD_EOF;
        else if (in.fail())
            //error("bad input: an integer expected");
            return RD_FAIL;
        else // in.bad()
            return RD_BAD;
    }
    else {
        // string nstr = std::to_string(n);     // C++11
        int ccnt = ltoa(n).length();
        line = lTrim(line);
        line.erase(0, ccnt);
        return RD_GOOD;       // Succeeded
    }
}

void readChar(string &line, char &c)
{
    istringstream in(line);
    if (!in.get(c))
        error("Malformed input (an integer expected)\n");
    else {
        // string cstr = to_string(c);          // C++11
        line.erase(0, 1);
    }
}

bool instrParse(std::string str)
{
    token tmpToken;
    char cState;

    NumInstrs++;

    if (STEP) {
        printState(PC, SP, FP);
        string line;
        getline(cin, line);
        cState = line[0];
        if (cState == 'q' || cState == 'Q') exit(0);
    }

    if (str != "") {
        tmpToken = analToken(str);

        if (tmpToken.opCode == "ldc") {
            if (tmpToken.opCnt != 2)
                return false;
            InstrCount[LDC]++;
            if (tmpToken.value[0] == '\'') {
                op_ldc(atoc(tmpToken.value));
            }
            else
                op_ldc(atol(tmpToken.value));
        }
        else if (tmpToken.opCode == "ldo") {
            if (tmpToken.opCnt != 2)
                return false;
            InstrCount[LDO]++;
            op_ldo(atol(tmpToken.value));
        }
        else if (tmpToken.opCode == "lod") {
            if (tmpToken.opCnt != 2)
                return false;
            InstrCount[LOD]++;
            op_lod(atol(tmpToken.value));
        }
        else if (tmpToken.opCode == "lda") {
            if (tmpToken.opCnt != 2)
                return false;
            InstrCount[LDA]++;
            op_lda(atol(tmpToken.value));
        }
        else if (tmpToken.opCode == "str") {
            if (tmpToken.opCnt != 2)
                return false;
            InstrCount[STR]++;
            op_str(atol(tmpToken.value));
        }
        else if (tmpToken.opCode == "mst") {
            if (tmpToken.opCnt != 1)
                return false;
            InstrCount[MST]++;
            op_mst();
        }
        else if (tmpToken.opCode == "cup") {
            if (tmpToken.opCnt != 3)
                return false;
            InstrCount[CUP]++;
            long  nPos = findLabel(tmpToken.label);
            op_cup(atol(tmpToken.value), nPos);
        }
        else if (tmpToken.opCode == "ssp") {
            if (tmpToken.opCnt != 2)
                return false;
            InstrCount[SSP]++;
            op_ssp(atol(tmpToken.value));
        }
        else if (tmpToken.opCode == "ent") {
            if (tmpToken.opCnt != 2)
                return false;
            InstrCount[ENT]++;
            op_ent(atol(tmpToken.value));
        }
        else if (tmpToken.opCode == "retf") {
            if (tmpToken.opCnt != 1)
                return false;
            InstrCount[RETF]++;
            op_retf();
        }
        else if (tmpToken.opCode == "retp") {
            if (tmpToken.opCnt != 1)
                return false;
            InstrCount[RETP]++;
            op_retp();
        }
        else if (tmpToken.opCode == "movs") {
            if (tmpToken.opCnt != 2)
                return false;
            InstrCount[MOVS]++;
            op_movs(atol(tmpToken.value));
        }
        else if (tmpToken.opCode == "ind") {
            if (tmpToken.opCnt != 1)
                return false;
            InstrCount[IND]++;
            op_ind();
        }
        else if (tmpToken.opCode == "sro") {
            if (tmpToken.opCnt != 2)
                return false;
            InstrCount[SRO]++;
            op_sro(atol(tmpToken.value));
        }
        else if (tmpToken.opCode == "sto") {
            if (tmpToken.opCnt != 1)
                return false;
            InstrCount[STO]++;
            op_sto();
        }
        else if (tmpToken.opCode == "add") {
            if (tmpToken.opCnt != 1)
                return false;
            InstrCount[ADD]++;
            op_add();
        }
        else if (tmpToken.opCode == "sub") {
            if (tmpToken.opCnt != 1)
                return false;
            InstrCount[SUB]++;
            op_sub();
        }
        else if (tmpToken.opCode == "mul") {
            if (tmpToken.opCnt != 1)
                return false;
            InstrCount[MUL]++;
            op_mul();
        }
        else if (tmpToken.opCode == "div") {
            if (tmpToken.opCnt != 1)
                return false;
            InstrCount[DIV]++;
            op_div();
        }
        else if (tmpToken.opCode == "neg") {
            if (tmpToken.opCnt != 1)
                return false;
            InstrCount[NEG]++;
            op_neg();
        }
        else if (tmpToken.opCode == "and") {
            if (tmpToken.opCnt != 1)
                return false;
            InstrCount[AND]++;
            op_and();
        }
        else if (tmpToken.opCode == "or") {
            if (tmpToken.opCnt != 1)
                return false;
            InstrCount[OR]++;
            op_or();
        }
        else if (tmpToken.opCode == "equ") {
            if (tmpToken.opCnt != 1)
                return false;
            InstrCount[EQU]++;
            op_equ();
        }
        else if (tmpToken.opCode == "les") {
            if (tmpToken.opCnt != 1)
                return false;
            InstrCount[LES]++;
            op_les();
        }
        else if (tmpToken.opCode == "grt") {
            if (tmpToken.opCnt != 1)
                return false;
            InstrCount[GRT]++;
            op_grt();
        }
        else if (tmpToken.opCode == "geq") {
            if (tmpToken.opCnt != 1)
                return false;
            InstrCount[GEQ]++;
            op_geq();
        }
        else if (tmpToken.opCode == "leq") {
            if (tmpToken.opCnt != 1)
                return false;
            InstrCount[LEQ]++;
            op_leq();
        }
        else if (tmpToken.opCode == "neq") {
            if (tmpToken.opCnt != 1)
                return false;
            InstrCount[NEQ]++;
            op_neq();
        }
        else if (tmpToken.opCode == "fjp") {
            if (tmpToken.opCnt != 2)
                return false;
            InstrCount[FJP]++;
            long nPos = findLabel(tmpToken.value);
            if (nPos == -1)
                op_fjp(atol(tmpToken.value));
            else
                op_fjp(nPos);
        }
        else if (tmpToken.opCode == "ujp") {
            if (tmpToken.opCnt != 2)
                return false;
            InstrCount[UJP]++;
            long nPos = findLabel(tmpToken.value);
            if (nPos == -1)
                PC = atol(tmpToken.value) - 1;
            else
                PC = nPos - 1;
        }
        else if (tmpToken.opCode == "in") {
            long  nGet;
            InstrCount[IN]++;
            SP++;
            if (STEP) std::cout << "stdin? ";
            auto status = readInt(StdIn, nGet);
            while (status != RD_GOOD) {
                string nextLine;
                switch (status) {
                    case RD_EOF:
                        getline(cin, nextLine);
                        nextLine += '\n';
                        StdIn += nextLine;
                        status = readInt(StdIn, nGet);
                        break;
                    case RD_FAIL:
                        error("an integer expected");
                }
            }
            Mem[SP] = (long) nGet;
            if (STEP) cout << endl;
        }
        else if (tmpToken.opCode == "inc") {
            char  nGet;
            InstrCount[INC]++;
            SP++;
            if (STEP) cout << "stdin? ";
            if (StdIn.length() == 0) {
                string nextLine;
                getline(cin, nextLine);
                nextLine += '\n';
                StdIn += nextLine;
            }
            readChar(StdIn, nGet);
            Mem[SP] = nGet;
            if (STEP) cout << endl;
        }
        else if (tmpToken.opCode == "out") {
            InstrCount[OUT]++;
            StdOut.append(intToStr(Mem[SP]));
            if (STEP)
                cout << "stdout: " << StdOut << endl;
            else
                cout << Mem[SP];
            value = Mem[SP];
            Mem[SP--] = 0;
        }
        else if (tmpToken.opCode == "outc") {
            InstrCount[OUTC]++;
            StdOut.push_back(char(Mem[SP]));
            if (STEP)
                cout << "stdout: " << escape(StdOut) << endl;
            else
                cout << char(Mem[SP]);
            value = Mem[SP];
            Mem[SP--] = 0;
        }
        else if (tmpToken.opCode == "new") {
            if (tmpToken.opCnt != 2)
                return false;
            InstrCount[NEW]++;
            op_new(atol(tmpToken.value));
        }
        else if (tmpToken.opCode == "del") {
            InstrCount[DEL]++;
            op_del();
        }
        else if (tmpToken.opCode == "dup") {
            InstrCount[DUP]++;
            op_dup();
        }
        else if (tmpToken.opCode == "pop") {
            InstrCount[DUP]++;
            op_pop();
        }
        else if (tmpToken.opCode == "sti") {
            if (tmpToken.opCnt != 2)
                return false;
            InstrCount[STI]++;
            op_sti(atol(tmpToken.value));
        }
        else if (tmpToken.opCode == "stp") {
            stFlag = true;
            InstrCount[STP]++;
            cout << endl;
        }
    }
    else
        return false;

    return true;
}

token analToken(std::string str)
{
    istringstream sstream(str);
    vector<string> tokens{istream_iterator<string>(sstream),
                          istream_iterator<string>{}};
    int nToken = tokens.size();
    token tmpToken;

    switch (nToken) {
        case 1:
            tmpToken.opCode = tokens[0];
            tmpToken.label = "";
            tmpToken.value = "";
            tmpToken.opCnt = 1;
            break;
        case 2:
            tmpToken.opCode = tokens[0];
            tmpToken.label = "";
            tmpToken.value = tokens[1];
            tmpToken.opCnt = 2;
            break;
        case 3:
            tmpToken.opCode = tokens[0];
            tmpToken.label = tokens[2];
            tmpToken.value = tokens[1];
            tmpToken.opCnt = 3;
            break;
    }

    if (tokenCheck(tmpToken.opCode))
        return tmpToken;
    else {
        std::cerr << "File Format Mismatched" << std::endl;
        std::cerr << "Press the \"Enter Key\" ! ";
        getchar();
        exit(0);
    }
}

string ctoa(char c)
{
    map<string, char> esTab = {
        {"\\a", '\a'},
        {"\\b", '\b'},
        {"\\f", '\f'},
        {"\\t", '\t'},
        {"\\v", '\v'},
        {"\\r", '\r'},
        {"\\n", '\n'}
    };
    for (auto p = esTab.begin(); p != esTab.end(); ++p) {
        if (p->second == c)
            return p->first;
    }
    return string(1u, c);
}

string escape(string s)
{
    string ret = "";
    for (char c: s)
        ret.append(ctoa(c));
    return ret;
}

long atoc(string str)
{
    map<string, char> esTab = {
        {"\\\'", '\''},
        {"\\\"", '\"'},
        {"\\\?", '\?'},
        {"\\a", '\a'},
        {"\\b", '\b'},
        {"\\f", '\f'},
        {"\\t", '\t'},
        {"\\v", '\v'},
        {"\\r", '\r'},
        {"\\n", '\n'}
    };
    if (str[1] != '\\')
        return (long)str[1];
    int  len = str.length();
    char fst = str[0], lst = str[len-1];
    assert(fst == '\'' && lst == '\'');
    char *substr = new char[len-1];
    strcpy(substr, str.c_str()+1);
    substr[len-2] = '\0';
    for (auto p = esTab.begin(); p != esTab.end(); ++p) {
        if (p->first == substr)
            return (long)p->second;
    }
    string msg = string("Unknown escape sequence ")+str;
    error(msg.c_str());
    return 0;
}

long atol(string str)
{
    return (long)atoi(str.c_str());
}

#define SSTR(x) static_cast<ostringstream &>( \
        (ostringstream() << std::dec << x) ).str()
string ltoa(long i)
{
    string s = SSTR(i);
    return s;
}

void initStack(iVec &iStack)
{
    for (long  cnt = 0; cnt <= STACKSIZE; cnt++)
        iStack.push_back(0);
}

long  findLabel(const std::string str)
{
    labels tmpLabel;

    for (lbIter = lbVector.begin(); lbIter != lbVector.end(); lbIter++) {
        tmpLabel = *lbIter;

        if (tmpLabel.label == str) return tmpLabel.nPos;
    }

    return -1;
}

long pow(long base, long radix)
{
    long  tmpPow = base;

    if (radix == 0) return 1;

    for (long  nCnt = 0; nCnt < radix; nCnt++)
        tmpPow *= tmpPow;

    return tmpPow;
}

void error(const char *msg) {
    fprintf(stderr, "\nError: %s\n", msg);
    exit(-1);
}

void heap_overflow() {
    error("heap overflow");
}

void stack_overflow() {
    error("stack overflow");
}

/*============================================================================
  micro-operation modules
  ============================================================================*/

void op_new(long  v)
{
    assert(v >= 0);
    if (FreeList.size() > 0) {
        for (auto p = FreeList.begin(); p != FreeList.end(); ++p) {
            int addr = *p;
            int curSize = Mem[addr];
            if (curSize >= v) {       // search for the first fit
                Mem[++SP] = addr + 1;
                if (curSize != v) {
                    Mem[addr] = v;
                    int newAddr = addr + v + 1;
                    int newSize = curSize - v - 1;   // -1 for the tag
                    Mem[newAddr] = curSize;
                    *p = newAddr;
                }
                else {  // same-sized block
                    FreeList.erase(p, p+1);
                }
                return;
            }
        }
    }
    Mem[++SP] = ++HP;
    Mem[HP-1] = v;
    HP += v;
    if (HP >= HEAP_UP)  heap_overflow();
    if (SP >= STACK_UP) stack_overflow();
}

void op_del()
{
    long addr = Mem[SP--];
    long size = Mem[addr-1];
    FreeList.push_back(addr-1);
}

void op_ldc(long  v)
{
    SP++;

    Mem[SP] = v;
}

void op_ldo(long  v)
{
    SP++;
    Mem[SP] = Mem[v];
}

void op_ind(void)
{
    long  addr;
    addr = Mem[SP];
    Mem[SP] = Mem[addr];
}

void op_sro(long  v)
{
    Mem[v] = Mem[SP];
    SP--;
}

void op_sto()
{
    long  addr;
    addr = Mem[SP-1];
    Mem[addr] = Mem[SP];
    SP -= 2;
}

void op_sti(long o)
{
    long addr = Mem[SP-1];
    Mem[addr+o] = Mem[SP];
    SP--;
}

void op_str(long v)
{
    Mem[FP + v] = Mem[SP];
    SP--;
}

void op_add()
{
    long  nOP1, nOP2;

    nOP1 = Mem[SP-1];
    nOP2 = Mem[SP];

    Mem[SP] = 0;
    Mem[SP-1] = nOP1 + nOP2;
    SP--;
}

void op_sub()
{
    long  nOP1, nOP2;

    nOP1 = Mem[SP-1];
    nOP2 = Mem[SP];

    Mem[SP] = 0;
    Mem[SP-1] = nOP1 - nOP2;
    SP--;
}

void op_mul()
{
    long  nOP1, nOP2;

    nOP1 = Mem[SP-1];
    nOP2 = Mem[SP];

    Mem[SP] = 0;
    Mem[SP-1] = nOP1 * nOP2;
    SP--;
}

void op_div()
{
    long  nOP1, nOP2;

    nOP1 = Mem[SP-1];
    nOP2 = Mem[SP];

    Mem[SP] = 0;
    Mem[SP-1] = nOP1 / nOP2;
    SP--;
}

void op_neg()
{
    Mem[SP] = Mem[SP] * (-1);
}

void op_and()
{
    bool bOP1, bOP2;

    if (Mem[SP-1] == 0) bOP1 = false;
    else bOP1 = true;

    if (Mem[SP] == 0) bOP2 = false;
    else bOP2 = true;

    Mem[SP] = 0;
    Mem[SP-1] = (long )(bOP1 && bOP2);

    SP--;
}

void op_or()
{
    bool bOP1, bOP2;

    if (Mem[SP-1] == 0) bOP1 = false;
    else bOP1 = true;

    if (Mem[SP] == 0) bOP2 = false;
    else bOP2 = true;

    Mem[SP] = 0;
    Mem[SP-1] = (long )(bOP1 || bOP2);

    SP--;
}

void op_not()
{
    if (Mem[SP] == 0) Mem[SP] = (long )true;
    else Mem[SP] = (long )false;
}

void op_equ()
{
    long  nOP1, nOP2;

    nOP1 = Mem[SP-1];
    nOP2 = Mem[SP];

    Mem[SP] = 0;
    Mem[SP-1] = (long )(nOP1 == nOP2);

    SP--;
}

void op_geq()
{
    long  nOP1, nOP2;

    nOP1 = Mem[SP-1];
    nOP2 = Mem[SP];

    Mem[SP] = 0;

    Mem[SP-1] = (long )(nOP1 >= nOP2);

    SP--;
}

void op_leq()
{
    long  nOP1, nOP2;

    nOP1 = Mem[SP-1];
    nOP2 = Mem[SP];

    Mem[SP] = 0;

    Mem[SP-1] = (long )(nOP1 <= nOP2);

    SP--;
}

void op_les()
{
    long  nOP1, nOP2;

    nOP1 = Mem[SP-1];
    nOP2 = Mem[SP];

    Mem[SP] = 0;

    Mem[SP-1] = (long )(nOP1 < nOP2);

    SP--;
}

void op_grt()
{
    long  nOP1, nOP2;

    nOP1 = Mem[SP-1];
    nOP2 = Mem[SP];

    Mem[SP] = 0;

    Mem[SP-1] = (long )(nOP1 > nOP2);

    SP--;
}

void op_neq()
{
    long  nOP1, nOP2;

    nOP1 = Mem[SP-1];
    nOP2 = Mem[SP];

    Mem[SP] = 0;

    Mem[SP-1] = (long )(nOP1 != nOP2);

    SP--;
}


void op_fjp(long  v)
{
    if (Mem[SP] == 0) PC = v - 1;

    SP--;
}


void op_lod(long  v)
{
    SP++;

    Mem[SP] = Mem[FP + v];
}

void op_lda(long  v)
{
    SP++;
    Mem[SP] = (FP + v);
}

void op_dup()
{
    long top = Mem[SP++];
    Mem[SP] = top;
}

void op_pop()
{
    SP--;
}

void op_mst()
{
    Mem[SP + 2] = FP;
    mp = SP;
    SP += 3;
}


void op_cup(long  pt, long  addr)
{
    FP = SP - (pt + 2);
    Mem[FP + 2] = PC;
    PC = addr - 1;
}

void op_ssp(long  v)
{
    SP = FP + v;
}

void op_ent(long  v)
{
    SP = FP + v - 1;
}

void op_retf()
{
    SP = FP;
    PC = Mem[FP + 2];
    FP = Mem[FP + 1];
    mp = FP;
}

void op_retp()
{
    SP = FP - 1;
    PC = Mem[FP + 2];
    FP = Mem[FP + 1];
    mp = FP;
}

void op_movs(long  v)
{
    for (long  cnt = v - 1; cnt >= 0; cnt--)
    {
        long  nOP1 = Mem[SP];
        long  nOP2 = Mem[nOP1 + cnt];
        Mem[SP + cnt] = nOP2;
    }

    SP = SP + v + 1;
}

bool tokenCheck(std::string str)
{
    int size = sizeof OpCode / sizeof *OpCode;
    for (int i = 0; i < size; i++)
        if (str == OpCode[i]) return true;
    return false;
}

void printState(int pc, int sp, int fp)
{
    int  nInstr, nStack;
    int  iLB, iUB, sLB, sUB;
    int  nIdxCount;
    int  nTmp;

    std::string strInstr, strSP, strTmp, strTmp2;

    nInstr = Instr.size();
    nStack = Mem.size();

    iLB = min(max(pc - 5, 0), max(nInstr -11, 0));
    iUB = min(max(pc + 5, PRINTLINE), nInstr - 1);

    sLB = max(sp - 10, 0);
    sUB = max(sp,      min(sp, PRINTLINE));

    cout << "  " " Instruction List " "\t" "        Stack " "\n";
    cout << "  " "==================" "\t" "       =======" "\n" "\n";

    nIdxCount = iLB + 11;

    for (; iLB < nIdxCount ; iLB++, sLB++) {
        strInstr.assign("");
        strTmp.assign("");

        if (iLB <= iUB) {
            if (iLB == pc) strInstr = "  *   ";
            else strInstr = "      " ;

            long  nDigit = nIntSize(iLB);

            for (long  nIdb = 0; nIdb < 5 - nDigit ; nIdb++)
                strTmp.append(" ");

            strTmp.append(": ");

            strTmp.append(Instr[iLB]) ;
            nTmp = 25 - strInstr.size() - strTmp.size() - nDigit;

            for (long  nIdc = 1; nIdc <= nTmp; nIdc++)
                strTmp.append(" ");

            std::cout << strInstr << iLB << strTmp ;

        }

        strSP.assign("");
        strTmp2.assign("");

        if (sLB <= sUB) {
            if (sLB == sp) strSP = "  #   ";
            else strSP = "      " ;
            long  nDigit2 = nIntSize(sLB);

            for (long  nIdb2 = 0; nIdb2 < 5 - nDigit2 ; nIdb2++)
                strTmp2.append(" ");

            strTmp2.append(": ");

            cout << strSP << sLB << strTmp2 << Mem[sLB] ; //<< '\n';
        }

        cout << '\n';
    }

    cout << '\n';

    cout << "   pc: " << pc << "     sp: " << sp << "     fp: " << fp << "\n\n";

    cout << "************************************************************************" << "\n\n";

}

long  nIntSize(long  nTmp)
{
    long  nCnt, nBase = 10;

    for (nCnt = 1; nBase <= nTmp; nCnt++) {
        nBase *= 10;
    }

    return nCnt;

}

std::string intToStr(const long  digit)
{
    long  nCount = 0;
    long  nDigit = digit;
    char cpTmp[sizeof(long)];

    std::string strRet;

    while (nDigit > 0) {
        cpTmp[nCount++] = (nDigit % 10) + 48;
        nDigit /= 10;
    }

    for (long  nRev = (nCount - 1); nRev >= 0; nRev--) {
        strRet += cpTmp[nRev];
    }

    return strRet;
}

void printStatistics(void)
{
    int nIdx = 0;
    std::string strPrnStatistic;
    int nPrint = 0;
    int nSize, nInit;
    const char *title = "    Execution Statistics        ";
    const char *line  = "--------------------------------";
    const char *msg   = "    Instructions Executed: ";

    cout << line  << endl;
    cout << title << endl;
    cout << line  << endl;

    for ( ; nIdx < N_INSTR_CNT ; nIdx++) {

        switch (nIdx) {

        default:
            if (InstrCount[nIdx] != 0) {
                char buf[100];
                sprintf(buf, "    %-4s:%3d", OpCode[nIdx], InstrCount[nIdx]);
                strPrnStatistic.append(buf);
                nSize = PRINTSTATICSWIDTH - strPrnStatistic.size();
                nPrint++;
            } else {
                break;
            }
            if (nPrint == PRINTSTATISTICS) {
                std::cout << strPrnStatistic << std::endl;
                nPrint = 0;
                strPrnStatistic = "";
            } else {
                for (nInit = 0; nInit < nSize; nInit++)
                    strPrnStatistic.append(" ");
            }
            break;
        }
    }

    cout << line << endl;
    cout << msg << NumInstrs << endl;
    cout << line << std::endl;
}

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

/*==========================================================
  global data
  ===========================================================*/
/*
extern long  PC, SP, FP;

extern lbVec lbVector;

extern strVec Instr;
extern bool stFlag;

extern bool STEP;

extern string StdOut;
*/

/*-----------------------------------------------------------------
  option specification
  -------------------------------------------------------------------

  -s: step by step
  -h: usage

  ------------------------------------------------------------------*/
void usage(char *pname);

int main(int argc, char *argv[])
{
    string line;
    string filename;

    labels tmpLabel;

    long  nPos = 0;  // label position

    if (argc == 2) {
        if (argv[1][1] == 'h') {
            usage(argv[0]);
            return 0;
        }
        else if (argv[1][1] == 's') {
            STEP = true;
            cout << "Input the file name with the full path" << endl;
            cout << ": ";

            std::cin >> filename;
            std::cout << std::endl << std::endl;
        }
        else {

            if (argv[1][0] == '-') {
                std::cerr << "Unkown option " << std::endl;
                return 1;
            }
            else {
                STEP = false;
                filename = argv[1];
            }

        }
    }
    else if (argc == 3) {
        if (argv[1][1] == 'h') {
            usage(argv[0]);
            return 0;
        }
        else if (argv[1][1] == 's') {
            STEP = true;
            filename = argv[2];
        }
        else {
            std::cerr << "Error: Too many parameters" << std::endl;
            return 1;
        }
    }
    else if (argc == 1) {
        usage(argv[0]);
        return -1;
    }

    ifstream infile(filename.c_str());
    if (!infile.is_open()) {
        std::cerr << "Error: cannot open file '" << filename << "'"
                  << std::endl;
        return 1;
    }

    while (getline(infile, line)) {
        Instr.push_back(lTrim(lbExtract(lbVector, line, nPos++)));
    }

    if (STEP) {
        std::cout << "pc : " << PC << '\t'
                  << "sp : " << SP << '\t'
                  << "fp : " << FP << std::endl << std::endl;
    }

    StdOut = "";
    StdIn  = "";
    for (PC = 0 ; PC < (long)Instr.size() ; PC++) {
        if (stFlag) {
            if (STEP)
                std::cout << "stdout: " << StdOut << std::endl;
            break;
        }
        if (!instrParse(Instr[PC])) {
            cerr << "Error: file format error"
                 << " near \"" << Instr[PC] << "\"" << endl;
            return 0;
        }
    }

    printStatistics();

    return 0;
}


void usage(char *pname)
{
    std::cout << "Usage: "<< pname << " [-option] [filenames]" << std::endl;
    std::cout << std::endl;
    std::cout << "Options:" << endl;
    std::cout << "    -s   step-by-step execution" << std::endl;
    std::cout << "    -h   help" << endl;
}


