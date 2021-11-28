#include <iostream>
#include <string>
#include <cstdio>
#include <ctype.h>
#include <vector>
#include <algorithm>
#include <stack>
using namespace std;

enum type_of_lex {
    LEX_NULL,//0
    // 1         2           3       4       5          6          7       8       9
    LEX_AND, LEX_BOOL, LEX_STRING, LEX_FOR, LEX_ELSE, LEX_END, LEX_IF, LEX_FALSE, LEX_INT,
    //  10       11       12       13          14         15       16          17         18
    LEX_NOT, LEX_OR, LEX_PROGRAM, LEX_READ, LEX_TRUE, LEX_WHILE, LEX_WRITE, LEX_BREAK, LEX_GOTO,
    LEX_FIN, // 19
    //  1           2              3      4          5          6           7           8            9         10
    LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_MARK, LEX_LBRACE, LEX_RBRACE, LEX_LPAREN, LEX_RPAREN, LEX_ASSIGN, LEX_LSS,
    // 11       12      13         14          15          16     17       18         19       20
    LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_MOD, LEX_LEQ, LEX_NEQ, LEX_EQUAL, LEX_GEQ,
    LEX_STRWRITE, // 40
    LEX_NUM, // 41
    LEX_ID,     // 42
    LEX_STR,    // 43
    POLIZ_ADDRESS, // 44
    POLIZ_FGO, // 45
    POLIZ_LABEL, // 46
    POLIZ_GO, // 47
    LEX_UNMINUS, //48
};


class Lex {
    type_of_lex t_lex;
    int v_lex;
public:
    Lex (type_of_lex t = LEX_NULL, int v = 0): t_lex (t), v_lex (v)  { }
    type_of_lex  get_type () const {
        return t_lex;
    }
    int get_value () const {
        return v_lex;
    }
    friend ostream & operator<< ( ostream &s, Lex l );
};



class Ident {
    string name;
    bool declare; // описан идентификатор в программе или нет
    type_of_lex type;
    bool assign; // присвоено значение или нет
    int value;
public:
    Ident() {
        declare = false;
        assign = false;
    }
    bool operator== (const string& s) const {
        return name == s;
    }
    Ident(const string n) {
        name = n;
        declare = false;
        assign = false;
    }
    string get_name () const {
        return name;
    }
    bool get_declare () const {
        return declare;
    }
    void put_declare () {
        declare = true;
    }

    type_of_lex get_type () const {
        return type;
    }
    void put_type (type_of_lex t) {
        type = t;
    }
    bool get_assign () const {
        return assign;
    }
    void put_assign () {
        assign = true;
    }
    int  get_value () const {
        return value;
    }
    void put_value (int v) {
        value = v;
    }
};



vector<Ident> TID; // таблица идентификаторов

int put (const string & buf) {
    vector<Ident>::iterator k;
    if ((k = find ( TID.begin (), TID.end (), buf)) != TID.end () )
        return k - TID.begin();
    TID.push_back ( Ident(buf) );
    return TID.size () - 1;
}

vector<string> TCS; // таблица строковых констант

int put_string (const string & buf) {
    vector<string>::iterator k;
    if ((k = find ( TCS.begin (), TCS.end (), buf)) != TCS.end () )
        return k - TCS.begin();
    TCS.push_back (buf);
    return TCS.size () - 1;
}

vector<int> TGOTO; // таблица меток
vector<int> TPGOTO; // таблица адресов полиза меток
vector<int> checkJ;
stack <vector<int>> STBREAK;


void check_id_jump(int number) {
    vector<int>::iterator k;
    if ((k = find(checkJ.begin(), checkJ.end(), number)) != checkJ.end()) {
        throw "more than 1 label";
    } else {
        checkJ.push_back(number);
    }
}

int put_goto(const int buf, const int p) {
    vector<int>::iterator k;
    if ((k = find(TGOTO.begin(), TGOTO.end(), buf)) != TGOTO.end())
        return TPGOTO[k - TGOTO.begin()];
    if (p != 0) {
        TGOTO.push_back(buf);
        TPGOTO.push_back(p);
    }
    return 0;
}

class Scanner {
    FILE * fp;
    char c;
    int look (const string buf, const char ** list) {
        int i = 0;
        while (list[i]) {
            if (buf == list[i])
                return i;
            ++i;
        }
        return 0;
    }
    void gc () {
        c = fgetc (fp);
    }
public:
    static const char * TW [], * TD [];
    Scanner (const char * program) {
        if ( !(fp = fopen ( program, "r" )) )
            throw  "can’t open file" ;
    }
    Lex get_lex ();
};

const char * // Таблица служебных слов
        Scanner::TW[] = { "", "and", "bool", "string", "for", "else", "end", "if", "false", "int", "not", "or", "program",
                          "read", "true", "while", "write", "break", "goto", nullptr };

const char * // Таблица ограничителей
                        // 0    1    2    3     4    5    6    7   8    9    10   11   12  13   14   15   16   17     18    19    20   21
        Scanner::TD[] = { "@", ";", ",", ":", "\"", "{", "}","(", ")", "=", "<", ">", "+", "-", "*", "/", "%", "<=", "!=", "==", ">=", nullptr };

Lex Scanner::get_lex () {
    enum state { H, IDENT, NUMB, COM, ALE, NEQ, STR };
    int digit, posInTD, s=1;
    string buf; // буфер для идентификатора или служебного слова
    state currentState = H;
    do {
        gc ();
        switch (currentState) {
            case H:
                if ( c==' ' || c == '\n' || c== '\r' || c == '\t' );
                else if ( isalpha (c) ) {
                    buf.push_back (c);
                    currentState = IDENT;
                }
                else if ( isdigit (c) ) {
                    digit   = c - '0';
                    currentState = NUMB;
                }
                else if ( c == '/' ) {
                    buf.push_back(c);
                    gc();
                    if ( c == '*') {
                        buf.pop_back();
                        currentState = COM;
                    }
                    else {
                        ungetc(c, fp);
                        if ( ( posInTD = look ( buf, TD) ) ){
                            return Lex ( (type_of_lex)( posInTD + (int) LEX_FIN ), posInTD );
                        }
                    }

                }
                else if ( c == '=' || c == '<' || c == '>' ) {
                    buf.push_back (c);
                    currentState = ALE;
                }
                else if (c == EOF)
                    return Lex ( LEX_FIN );
                else if (c == '!') {
                    buf.push_back (c);
                    currentState = NEQ;
                }
                else if (c == '"') {
                    currentState = STR;
                }
                else {
                    buf.push_back (c);
                    if ( ( posInTD = look ( buf, TD) ) ){
                        return Lex ( (type_of_lex)( posInTD + (int) LEX_FIN ), posInTD );
                    }
                    else
                        throw c;
                }
                break;
            case IDENT:
                if ( isalpha (c) || isdigit (c) ) {
                    buf.push_back (c);
                }
                else {
                    ungetc (c, fp);
                    if ( (posInTD = look (buf, TW) ) ) {
                        return Lex ( (type_of_lex) posInTD, posInTD ); // если это служебное слово, то получаем лексему служебного слова
                    }
                    else {
                        posInTD = put (buf); // пытаемся положить в TD или находим уже существуещий идентификатор
                        return Lex (LEX_ID, posInTD);
                    }
                }
                break;
            case NUMB:
                if (isdigit (c)) {
                    digit = digit * 10 + ( c - '0' );
                }
                else {
                    ungetc (c, fp);
                    return Lex ( LEX_NUM, digit*s );
                }
                break;
            case COM:
                if (c == '*') {
                    gc();
                    if (c == '/') currentState = H;
                }
                else if (c == EOF)
                    throw c;
                break;
            case ALE:
                if (c == '=') {
                    buf.push_back(c);
                    posInTD = look( buf, TD );
                    return Lex ( (type_of_lex) ( posInTD + (int) LEX_FIN ), posInTD );
                }
                else {
                    ungetc(c, fp);
                    posInTD = look(buf, TD);
                    return Lex((type_of_lex) (posInTD + (int) LEX_FIN), posInTD);
                }
            case NEQ:
                if (c == '=') {
                    buf.push_back(c);
                    posInTD = look (buf, TD);
                    return Lex (LEX_NEQ, posInTD);
                }
                else
                    throw '!';
            case STR:
                if (c != '"')
                {
                    if (c == EOF) throw "unexpected EOF";
                    buf.push_back(c);
                }
                else
                {
                    posInTD = put_string(buf);
                    return Lex(LEX_STR, posInTD);
                }
                break;
        }
    } while (true);
}

ostream & operator<< (ostream &s, Lex l) {
    string t;
    if (l.t_lex <= LEX_GOTO)
        t = Scanner::TW[l.t_lex];
    else if (l.t_lex >= LEX_FIN && l.t_lex <= LEX_GEQ)
        t = Scanner::TD[l.t_lex - LEX_FIN];
    else if (l.t_lex == LEX_NUM)
        t = "NUMB";
    else if (l.t_lex == LEX_ID)
        t = TID[l.v_lex].get_name ();
    else if (l.t_lex == LEX_STR)
        t = TCS[l.v_lex];
    else if ( l.t_lex == POLIZ_LABEL )
        t = "Label";
    else if ( l.t_lex == POLIZ_ADDRESS ) {
        t = "&" + TID[l.v_lex].get_name();
    }
    else if ( l.t_lex == POLIZ_GO )
        t = "!";
    else if ( l.t_lex == POLIZ_FGO )
        t = "!F";
    else if ( l.t_lex == LEX_STRWRITE )
        t = "str write";
    else
        throw l;
    s << '(' << t << ',' << l.v_lex << ");" << endl;
    return s;
}

//////////////////////////  Класс Parser  /////////////////////////////////
template <class T, class T_EL>
void from_st ( T & st, T_EL & i ) {
    i = st.top();
    st.pop();
}

class Parser {
    Lex curr_lex;
    type_of_lex c_type;
    int c_val;
    Scanner scan;
    stack <int> st_int;
    stack <type_of_lex> st_lex;
    void begin();// процедуры РС-метода
    void descriptions();
    void description();
    void operators();
    void op();
    void Expr();
    void Assign();
    void Or();
    void Arifm_Expr();
    void Term();
    void Factor();
    void Cmp();
    void And();
    void dec ( type_of_lex type);
    void check_id ();
    void check_op ();
    void check_not ();
    void check_unmin();
    void eq_type ();
    void eq_bool ();
    void check_id_in_read ();
    void gl () {
        curr_lex  = scan.get_lex ();
        c_type    = curr_lex.get_type ();
        c_val     = curr_lex.get_value ();
    }
    void ungl(const Lex& lex) {
        st_int.push(lex.get_value());
        st_lex.push(lex.get_type());
    }
public:
    vector <Lex> poliz;
    Parser ( const char *program ) : scan (program) { }
    void  analyze();
};

void Parser::analyze () {
    gl ();
    begin ();
    int k = 0;
    //for_each( poliz.begin(), poliz.end(), [](Lex l){ cout << l; });
    for ( Lex l : poliz ) {
        cout << k << " - "<< l ;
        k++;
    }
    cout << endl << "Nice code!" << endl;
}

void Parser::dec(type_of_lex type) { // Считывание из стека номер TID'a и заполнение информации
    int i;
    while (!st_int.empty()) {
        from_st(st_int, i);
        if (TID[i].get_declare()) throw "twice declaration of ind";
        else {
            TID[i].put_declare();
            if (type == LEX_STRING) {
                TID[i].put_value(TCS.size());
            }
            TID[i].put_type(type);
        }
    }
}
// begin - > program { descriptions operators }
void Parser::begin() {
    if (c_type == LEX_PROGRAM) gl();
    else throw "Program error";
    if (c_type == LEX_LBRACE) gl();
    else throw "No { after Program";
    descriptions(); // описания
    operators(); //операторы
    if (c_type == LEX_RBRACE) gl();
    else throw curr_lex;
    if (c_type != LEX_FIN) throw curr_lex;
}
// descriptions -> [description]
void Parser::descriptions() {
    while (c_type == LEX_INT || c_type == LEX_STRING || c_type == LEX_BOOL) {
        description();
        if (c_type == LEX_SEMICOLON) {
            gl();
        } else {
            throw curr_lex;
        }
    }
}

void Parser::description() {
    if (c_type == LEX_INT) {
        gl();
        while (c_type == LEX_ID) {
            st_int.push(c_val);
            int val =  c_val;
            gl();
            if (c_type == LEX_ASSIGN) {
                poliz.push_back(Lex(POLIZ_ADDRESS, val));
                gl();
                if (c_type == LEX_MINUS) {
                    gl();
                    if (c_type == LEX_NUM) {
                        st_lex.push(LEX_INT);
                        poliz.push_back(Lex(LEX_NUM, 0));
                        poliz.push_back(curr_lex);
                        check_unmin();
                        gl();
                    } else throw curr_lex;

                } else if (c_type == LEX_NUM) {
                    poliz.push_back(curr_lex);
                    gl();
                } else throw curr_lex;
                poliz.push_back(Lex(LEX_ASSIGN));
            }
            if (c_type == LEX_COMMA) gl();
            else break;
        }
        dec(LEX_INT);
    } else if (c_type == LEX_STRING) {
        gl();
        while (c_type == LEX_ID) {
            st_int.push(c_val);
            int val =  c_val;
            gl();
            if (c_type == LEX_ASSIGN) {
                poliz.push_back(Lex(POLIZ_ADDRESS, val));
                gl();
                if (c_type == LEX_STR) {
                    poliz.push_back(curr_lex);
                    gl();
                } else throw curr_lex;
                poliz.push_back(Lex(LEX_ASSIGN));
            }
            if (c_type == LEX_COMMA) {
                gl();
            } else break;
        }
        dec(LEX_STRING);
    } else if (c_type == LEX_BOOL) {
        gl();
        while (c_type == LEX_ID) {
            st_int.push(c_val);
            int val = c_val;
            gl();
            if (c_type == LEX_ASSIGN) {
                poliz.push_back(Lex(POLIZ_ADDRESS, val));
                gl();
                if (c_type == LEX_TRUE) {
                    poliz.push_back(Lex(LEX_TRUE,1));
                    gl();
                } else
                    if (c_type == LEX_FALSE) {
                        poliz.push_back(Lex(LEX_TRUE,0));
                        gl();
                    }
                    else throw curr_lex;
                poliz.push_back(Lex(LEX_ASSIGN));
            }
            if (c_type == LEX_COMMA) gl();
            else break;
        }
        dec(LEX_BOOL);
    }
}
// operators ->
void Parser::operators() {
    op();
    while (c_type == LEX_IF || c_type == LEX_WHILE || c_type == LEX_BREAK || c_type == LEX_READ ||
           c_type == LEX_WRITE || c_type == LEX_GOTO || c_type == LEX_LBRACE || c_type == LEX_ID ||
           c_type == LEX_FOR) {
        op();
    }
}

void Parser::op() {
    int if_pl0, if_pl1, for_pl0, for_pl1, for_pl2, for_pl3, while_pl0, while_pl1;
    if (c_type == LEX_IF) {
        gl();
        if (c_type == LEX_LPAREN) {
            gl();
            Expr();
            eq_bool();
            if_pl0 = poliz.size();
            poliz.push_back(Lex());
            poliz.push_back(Lex(POLIZ_FGO));
            if (c_type == LEX_RPAREN) {
                gl();
            } else throw "no ) in if(...)";
            op();
            if_pl1 = poliz.size();
            poliz.push_back(Lex());
            poliz.push_back(Lex(POLIZ_GO));
            poliz[if_pl0] = Lex(POLIZ_LABEL, poliz.size());
            if (c_type == LEX_ELSE) {
                // cout<<"A";
                gl();
                op();
                poliz[if_pl1] = Lex(POLIZ_LABEL, poliz.size());
            } else throw "no else in if(...)";
        } else throw "no ( in if (...)";
    }//end if
    else if (c_type == LEX_FOR) {
        gl();
        STBREAK.push(vector<int>());
        if (c_type == LEX_LPAREN) {
            gl();
            // Если есть первый аргумент
            if (c_type == LEX_ID) {
                Expr();
            }
            if (c_type == LEX_SEMICOLON) {
                gl();
            } else {
                throw curr_lex;
            }
            // Второй аргумент
            for_pl0 = poliz.size();
            if ((c_type == LEX_ID) || (c_type == LEX_NOT) ||
                (c_type == LEX_MINUS) || (c_type == LEX_NUM) ||
                (c_type == LEX_STR) || (c_type == LEX_FALSE) ||
                (c_type == LEX_TRUE) || (c_type == LEX_LPAREN)) {
                Expr();
                eq_bool();
            }
            for_pl1 = poliz.size();
            poliz.push_back(Lex());
            poliz.push_back(Lex(POLIZ_FGO));
            for_pl2 = poliz.size();
            poliz.push_back(Lex());
            poliz.push_back(Lex(POLIZ_GO));
            for_pl3 = poliz.size();
            if (c_type == LEX_SEMICOLON) {
                gl();
            } else {
                throw curr_lex;
            }
            // третий аргумент
            if (c_type == LEX_ID) {
                Expr();
            }
            poliz.push_back(Lex(POLIZ_LABEL, for_pl0));
            poliz.push_back(Lex(POLIZ_GO));
            if (c_type == LEX_RPAREN) {
                gl();
            } else {
                throw "no \")\" in for()";
            }
            poliz[for_pl2] = Lex(POLIZ_LABEL, poliz.size());
            op();
            poliz.push_back(Lex(POLIZ_LABEL, for_pl3));
            poliz.push_back(Lex(POLIZ_GO));
            poliz[for_pl1] = Lex(POLIZ_LABEL, poliz.size());
            while (!STBREAK.top().empty()) {
                poliz[STBREAK.top().back()] = Lex(POLIZ_LABEL, poliz.size());
                STBREAK.top().pop_back();
            }
            STBREAK.pop();
        } else {
            throw "no \"(\" in for()";
        }

    }//end for
    else if (c_type == LEX_WHILE) {
        STBREAK.push(vector<int>());
        while_pl0 = poliz.size();
        gl();
        if (c_type == LEX_LPAREN) {
            gl();
            Expr();
            eq_bool();
            while_pl1 = poliz.size();
            poliz.push_back(Lex());
            poliz.push_back(Lex(POLIZ_FGO));
            if (c_type == LEX_RPAREN) {
                gl();
                op();
                poliz.push_back(Lex(POLIZ_LABEL, while_pl0));
                poliz.push_back(Lex(POLIZ_GO));
                poliz[while_pl1] = Lex(POLIZ_LABEL, poliz.size());
                while (!STBREAK.top().empty()) { // если был break, то заполняем пустую лексему, которую добавили при встрече break
                    poliz[STBREAK.top().back()] = Lex(POLIZ_LABEL, poliz.size());
                    STBREAK.top().pop_back();
                }
                STBREAK.pop();
            } else throw "no ) in while ()";
        } else throw "no ( in while()";
        // end while
    } else if (c_type == LEX_READ) {
        gl();
        if (c_type == LEX_LPAREN) {
            gl();
            if (c_type == LEX_ID) {
                check_id_in_read();
                poliz.push_back(Lex(POLIZ_ADDRESS, c_val));
                gl();
            } else throw "no ident in read()";
            if (c_type == LEX_RPAREN) {
                gl();
                poliz.push_back(Lex(LEX_READ));
            } else throw "no ) after read()";
            if (c_type == LEX_SEMICOLON) {
                gl();
            } else {
                throw curr_lex;
            }
        } else throw "no ( after read()";
    }//end read
    else if (c_type == LEX_WRITE) {
        gl();
        if (c_type == LEX_LPAREN) {
            gl();
            Expr();
            if (st_lex.top() == LEX_STRING) {
                poliz.push_back(Lex(LEX_STRWRITE));
            } else {
                poliz.push_back(Lex(LEX_WRITE));
            }
            st_lex.pop();
            while (c_type == LEX_COMMA) {
                gl();
                Expr();
                if (st_lex.top() == LEX_STRING) {
                    poliz.push_back(Lex(LEX_STRWRITE));
                } else {
                    poliz.push_back(Lex(LEX_WRITE));
                }
                st_lex.pop();
            }
            if (c_type == LEX_RPAREN) {
                gl();
                //gl();
            } else
                throw "no ) after write";
            if (c_type == LEX_SEMICOLON) {
                gl();
            } else {
                throw curr_lex;
            }
        } else
            throw "no ( after write";
    }//end write
    else if (c_type == LEX_BREAK) {
        if (STBREAK.size() == 0) {
            throw "no cycle for break";
        }
        STBREAK.top().push_back(poliz.size()); // добавляем адрес полиза, чтобы потом заполнить пустую лексему меткой на конец из цикла
        poliz.push_back(Lex());
        poliz.push_back(Lex(POLIZ_GO));
        gl();
        if (c_type == LEX_SEMICOLON) {
            gl();
        } else {
            throw "no ; after break";
        }
    }
    else if (c_type == LEX_GOTO) {
        gl();
        if (c_type == LEX_ID) {
            int label_before_goto = put_goto(c_val, 0);
            if (label_before_goto != 0) {
                poliz.push_back(Lex(POLIZ_LABEL, label_before_goto)); // возвращает нам полиз метки со значением c_val из TPGOTO
                poliz.push_back(Lex(POLIZ_GO));
            } else { // если goto стоит до метки
                put_goto(c_val, poliz.size());
                poliz.push_back(Lex()); // добавляем пустую лексему, которую позже заполним при встрече с меткой
                poliz.push_back((Lex(POLIZ_GO)));
            }
            gl();
            if (c_type == LEX_SEMICOLON) {
                gl();
            } else {
                throw "no ; after goto ";
            }
        } else {
            throw "no ident after goto";
        }
    } // end goto
    else if (c_type == LEX_ID) {
        if (!TID[c_val].get_declare()) {
            int t_val1 = c_val;
            gl();
            if (c_type == LEX_COLON) {
                check_id_jump(t_val1); // проверка на повторную метку
                int label_after_goto;
                // если метка идет до goto, то добавляем в TGOTO и TPGOTO метку и адрес полиза для перехода по этой метке
                if ((label_after_goto = put_goto(t_val1, poliz.size())) != 0) { // достает адрес полиза из TPGOTO
                    poliz[label_after_goto] = Lex(POLIZ_LABEL, poliz.size()); // и в пустой лексеме, которую создали до этого, добавляем метку
                }
                gl();
                op();
            } else {
                cout << curr_lex;
                gl();
                cout << curr_lex;
                throw "not declared ID";
            }
        } else {
            check_id();
            //poliz.push_back(Lex(POLIZ_ADDRESS, c_val));
            Expr();
            operators();
        }
    } else if (c_type == LEX_LBRACE) { // Составной оператор
        gl();
        operators();
        if (c_type == LEX_RBRACE) gl();
        else throw "123";
    } else {
        Expr();
        if (c_type == LEX_SEMICOLON) {
            gl();
        } else {
            cout << "E";
            throw curr_lex;
        }
    }
}




void Parser::Arifm_Expr() {
    Term();
    while (c_type == LEX_PLUS || c_type == LEX_MINUS) {
        st_lex.push(c_type);
        gl();
        Term();
        check_op();
    }
}

void Parser::Cmp() {
    Arifm_Expr();
    if (c_type == LEX_EQUAL || c_type == LEX_LSS || c_type == LEX_GTR || c_type == LEX_LEQ || c_type == LEX_NEQ ||
        c_type == LEX_GEQ) {
        st_lex.push(c_type);
        gl();
        Arifm_Expr();
        check_op();
    }
}


void Parser::Expr() {
    Or();
    int u = 0;
    while (c_type == LEX_ASSIGN) {
        gl();
        Or();
        eq_type();
        u += 1;
    }
    for (int t = 0; t < u; t++){
        poliz.push_back(Lex(LEX_ASSIGN));
    }
}

void Parser::Or() {
    And();
    while (c_type == LEX_OR) {
        st_lex.push(c_type);
        gl();
        And();
        check_op();
    }
}

void Parser::And() {
    Cmp();
    while (c_type == LEX_AND) {
        st_lex.push(c_type);
        gl();
        Cmp();
        check_op();
    }
}


void Parser::Term() {
    Factor();
    while (c_type == LEX_TIMES || c_type == LEX_SLASH || c_type == LEX_MOD) {
        st_lex.push(c_type);
        gl();
        Factor();
        check_op();
    }
}

void Parser::Factor() {
    if (c_type == LEX_ID) {
        check_id();
        int temp_val = c_val;
        gl();
        if (c_type == LEX_ASSIGN) {
            poliz.push_back(Lex(POLIZ_ADDRESS, temp_val));
        } else {
            poliz.push_back(Lex(LEX_ID, temp_val));
        }
    } else if (c_type == LEX_STR) {
        st_lex.push(LEX_STR);
        poliz.push_back(curr_lex);
        gl();
    } else if (c_type == LEX_NUM) {
        st_lex.push(LEX_INT);
        poliz.push_back(curr_lex);
        gl();
    } else if (c_type == LEX_TRUE) {
        st_lex.push(LEX_BOOL);
        poliz.push_back(Lex(LEX_TRUE, 1));
        gl();
    } else if (c_type == LEX_FALSE) {
        st_lex.push(LEX_BOOL);
        poliz.push_back(Lex(LEX_FALSE, 0));
        gl();
    } else if (c_type == LEX_NOT) {
        gl();
        Factor();
        check_not();
        poliz.push_back(Lex(LEX_NOT));
    } else if (c_type == LEX_MINUS) {
        gl();
        poliz.push_back(Lex(LEX_NUM, 0));
        Factor();
        check_unmin();
    } else if (c_type == LEX_LPAREN) {
        gl();
        Expr();
        if (c_type == LEX_RPAREN) gl();
        else throw curr_lex;
    } else if (c_type == LEX_SEMICOLON) {

    } else {
        cout << "Expression error";
        throw curr_lex;
    }
}

void Parser::check_id() { // проверка на то, что лексема-переменная описана
    if (TID[c_val].get_declare()) st_lex.push(TID[c_val].get_type());
    else {
        cout << curr_lex;
        throw "not declared";
    }
}

void Parser::check_op() {
    type_of_lex t1, t2, op, i = LEX_INT, s = LEX_STRING;
    from_st(st_lex, t2);
    from_st(st_lex, op);
    from_st(st_lex, t1);
    if (t1 == LEX_STR)
        t1 = LEX_STRING;
    if (t2 == LEX_STR)
        t2 = LEX_STRING;
    if (op == LEX_PLUS && t1 == s) {
        if (t1 == t2) st_lex.push(s);
        else throw "wrong types are in operation +";
    } else {
        if (op == LEX_MOD && t1 == i) {
            if (t1 == t2) st_lex.push(i);
            else throw "wrong types are in operation %";
        } else {
            if (op == LEX_EQUAL || op == LEX_LSS || op == LEX_GTR || op == LEX_LEQ || op == LEX_NEQ ||
                op == LEX_GEQ) {
                if (t1 == t2 && (t1 == i || t1 == s ))
                    st_lex.push(LEX_BOOL);
                else throw "wrong types are in operation ==, <, >, <=, !=, >=";
            } else {
                if (op == LEX_MINUS || op == LEX_TIMES || op == LEX_SLASH || op == LEX_PLUS) {
                    if (t1 == t2 && t1 == i) st_lex.push(i);
                    else throw "wrong types are in operation +,-,*,/";
                } else if (op == LEX_OR || op == LEX_AND) {
                    if (t1 == t2 && t1 == LEX_BOOL) {
                        st_lex.push(LEX_BOOL);
                    } else throw "wrong types in operation or/and";
                }

            }
        }
    }
    //cout << "1" << endl;
    poliz.push_back(Lex(op));
}

void Parser::check_not() {
    if (st_lex.top() != LEX_BOOL) throw "wrong type is in not";
    else { st_lex.push(LEX_BOOL); }
}

void Parser::check_unmin() {
    if (st_lex.top() != LEX_INT) {
        throw "wrong type is in minus";
    } else {
        poliz.push_back(Lex(LEX_MINUS));
    }
}

void Parser::eq_type () {
    type_of_lex t;
    from_st ( st_lex, t);
    if (t != st_lex.top ())
        throw "wrong types are in :=";
    //st_lex.pop();
}

void Parser::eq_bool () {
    if ( st_lex.top () != LEX_BOOL )
        throw "expression is not boolean";
    st_lex.pop ();
}

void Parser::check_id_in_read() {
    if (!TID[c_val].get_declare()) throw "not declared in read";
}


void print_poliz(vector<Lex> Pol)
{
    vector<Lex>::iterator iter = Pol.begin();
    int i = 0;
    while (iter!= Pol.end())
    {
        cout << i << " " << *iter++ ;
        i++;
    }
}

///////////////////////////////// Executer
class Executer {
public:
    void execute ( vector<Lex> & poliz );
};

void Executer::execute ( vector<Lex> & poliz ) {
    Lex pc_el;
    stack < int > args;
    stack < type_of_lex > argt;
    int i, j, index = 0, size = poliz.size();
    type_of_lex typebuf;
    while ( index < size ) {
        pc_el = poliz [ index ];
        switch ( pc_el.get_type () ) {
            case LEX_TRUE: case LEX_FALSE: case LEX_NUM: case POLIZ_ADDRESS: case POLIZ_LABEL: case LEX_STR:
                args.push ( pc_el.get_value () );
                argt.push ( pc_el.get_type() );
                break;

            case LEX_ID:
                i = pc_el.get_value ();
                if ( TID[i].get_assign () ) {
                    args.push ( TID[i].get_value () );
                    argt.push ( TID[i].get_type() );
                    break;
                }
                else
                    throw "POLIZ: indefinite identifier";

            case LEX_NOT:
                from_st ( args, i );
                args.push( !i );
                from_st ( argt, typebuf );
                argt.push(typebuf);
                break;

            case LEX_OR:
                from_st ( args, i );
                from_st ( args, j );
                from_st ( argt, typebuf );
                from_st ( argt, typebuf );
                args.push ( j || i );
                argt.push( typebuf );
                break;

            case LEX_AND:
                from_st ( args, i );
                from_st ( args, j );
                from_st ( argt, typebuf);
                from_st ( argt, typebuf );
                args.push ( j && i );
                argt.push( typebuf );
                break;

            case POLIZ_GO:
                from_st ( args, i );
                index = i - 1;
                from_st ( argt, typebuf );
                break;

            case POLIZ_FGO:
                from_st ( args, i );
                from_st ( args, j );
                if ( !j ) index = i - 1;
                from_st ( argt, typebuf );
                from_st ( argt, typebuf );
                break;
            case LEX_WRITE:
                from_st(args, j);
                from_st(argt, typebuf);
                if (typebuf == LEX_BOOL) {
                    if (j == 0) cout << "FALSE" << endl;
                    else cout << "TRUE" << endl;
                }
                else if (typebuf == LEX_STR) {
                    cout << TCS[j] << endl;
                }
                else {
                    cout << j << endl;
                }
                break;
            case LEX_STRWRITE:
                from_st(args, j);
                cout << TCS[j] << endl;
                break;
            case LEX_READ:
                int k;
                from_st ( args, i );
                from_st ( argt, typebuf );
                if ( TID[i].get_type () == LEX_INT ) {
                    cout << "Input int value for " << TID[i].get_name () << ":";
                    cin >> k;
                }
                else if ( TID[i].get_type () == LEX_BOOL ){
                    string bstr;
                    while (1) {
                        cout << "Input boolean value (true or false) for " << TID[i].get_name() << ":" ;
                        cin >> bstr;
                        if ( bstr != "true" && bstr != "false" ) {
                            cout << "Error in input:true/false" << endl;
                            continue;
                        }
                        k = ( bstr == "true" ) ? 1 : 0;
                        break;
                    }
                }
                else{
                    cout<< "Input string value for " << TID[i].get_name() << ":";
                    string tempstr;
                    cin>> tempstr;
                    k = put_string(tempstr);
                }
                TID[i].put_value (k);
                TID[i].put_assign ();
                break;

            case LEX_PLUS:
                from_st ( argt, typebuf );
                from_st ( args, i );
                from_st ( args, j );
                if( typebuf == LEX_STR || typebuf == LEX_STRING ){
                    string sumstr;
                    sumstr = TCS[j] + TCS[i];
                    args.push(put_string(sumstr));
                }
                else {
                    args.push(i + j);
                }
                break;

            case LEX_TIMES:
                from_st ( args, i );
                from_st ( args, j );
                args.push ( i * j );
                from_st ( argt, typebuf );
                break;

            case LEX_MINUS:
                from_st ( args, i );
                from_st ( args, j );
                from_st ( argt, typebuf );
                args.push ( j - i );
                break;

            case LEX_SLASH:
                from_st ( args, i );
                from_st ( args, j );
                from_st ( argt, typebuf );
                if (i) {
                    args.push ( j / i );
                    break;
                }
                else
                    throw "POLIZ:divide by zero";

            case LEX_EQUAL:
                from_st ( args, i );
                from_st ( args, j );
                from_st ( argt, typebuf );
                from_st ( argt, typebuf );
                if( typebuf == LEX_STR || typebuf == LEX_STRING ){
                    args.push(TCS[j] == TCS[i]);
                }
                else {
                    args.push(i == j);
                }
                argt.push(LEX_BOOL);
                break;

            case LEX_LSS:
                from_st ( args, i );
                from_st ( args, j );
                from_st ( argt, typebuf );
                from_st ( argt, typebuf );
                if( typebuf == LEX_STR || typebuf == LEX_STRING ){
                    args.push(TCS[j] < TCS[i]);
                }
                else {
                    args.push( j < i );
                }
                argt.push(LEX_BOOL);
                break;

            case LEX_GTR:
                from_st ( args, i );
                from_st ( args, j );
                from_st ( argt, typebuf );
                from_st ( argt, typebuf );
                if( typebuf == LEX_STR || typebuf == LEX_STRING ){
                    args.push(TCS[j] > TCS[i]);
                }
                else {
                    args.push( j > i );
                }
                argt.push(LEX_BOOL);
                break;

            case LEX_LEQ:
                from_st ( args, i );
                from_st ( args, j );
                from_st ( argt, typebuf );
                from_st ( argt, typebuf );
                argt.push(LEX_BOOL);
                args.push ( j <= i );
                break;

            case LEX_GEQ:
                from_st ( args, i );
                from_st ( args, j );
                from_st ( argt, typebuf );
                from_st ( argt, typebuf );
                argt.push(LEX_BOOL);
                args.push ( j >= i );
                break;

            case LEX_NEQ:
                from_st ( args, i );
                from_st ( args, j );
                from_st ( argt, typebuf );
                from_st ( argt, typebuf );
                if (typebuf == LEX_STR || typebuf == LEX_STRING){
                    args.push(TCS[j] != TCS[i]);
                }
                else {
                    args.push(j != i);
                }
                argt.push(LEX_BOOL);
                break;
            case LEX_ASSIGN:
                type_of_lex buf4;
                from_st ( args, i );
                from_st ( args, j );
                from_st ( argt, buf4 );
                from_st ( argt, typebuf );
                //cout << "!!!" << TID[j].get_value() << " " << TID[i].get_value() << endl;
                TID[j].put_value (i);
                TID[j].put_assign ();
                args.push(i);
                argt.push(buf4);
                break;
            default:
                throw "POLIZ: unexpected elem";
        }//end of switch
        ++index;
    };//end of while
    cout << "Finish of executing!!!" << endl;
}

class Interpretator {
    Parser   pars;
    Executer E;
public:
    Interpretator ( const char* program ): pars (program) {}
    void     interpretation ();
};

void Interpretator::interpretation () {
    pars.analyze ();
    E.execute ( pars.poliz );
}

int main()
{
    /*try {
        Scanner scan("test.txt");
        Lex test_lex = scan.get_lex();
        while (test_lex.get_type() != LEX_FIN) {
            cout << test_lex;
            test_lex = scan.get_lex();
        }
        return 0;
    } */
    /*try {
        Parser pars("test.txt");
        pars.analyze();
        print_poliz(pars.poliz);
    }*/
    try {
        Interpretator I ( "test.txt" );
        I.interpretation ();
/*        cout << TGOTO[0] << " " << TGOTO[1] << endl;
        cout << TPGOTO[0] << " " << TPGOTO[1] << endl;*/
        return 0;
    }
    catch (char c) {
        cout << "unexpected symbol " << c << endl;
        return 1;
    }
    catch (Lex l) {
        cout << "unexpected lexeme" << l << endl;
        return 1;
    }
    catch ( const char *source ) {
        cout << source << endl;
        return 1;
    }
}

