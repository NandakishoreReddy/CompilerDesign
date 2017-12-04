
/*******************************NAME : Nanda Kishore Reddy Thineti**********************/
/*******************************MID  : M20227955************************/
/*Description:This project acts as compiler so that it will translate any given L756 program
whose parsing does not include using rules 21 and 23.*/
#include<iostream>//for input output operations
#include<fstream>//for file operations
#include<sstream>
#include<vector>//for vector initializ ation
#include<algorithm>//for algorithm initialization that operate on range of elements
#include<string>
#include<iomanip>//for setw()

using namespace std;

//Symboletable as vector of struct
struct symboletable{
	string word;
	int clas;
	int addr;
};
//Global variables
ifstream in, sc, sy, ip, cp;
int len = 0, s_count = 0, state = 0, c, o_state = 0, n_c = 0;
int x, y;
int res_count = 0, k = 0, r, line_no = 0, lc = 0, ADDR = 0, br = 0;
string s, o_s, s1 = "";
int opr, op1, op2, op3;
int MEM[400];
char ch;
int scan[14][13];
int rule[29][8], LL[14][23];
vector<symboletable> symbols;
vector<int>int_table;
vector<int> syntax;
vector<int> info;
int cla, val;
bool eof = false, error = false;

//Function prototypes
char nextChar(ifstream&);
string nextLine(ifstream&);
void scanToken(int cla, int val, ifstream &in);
void Parser(ifstream &);
void setup();
int check(int state, char c);
void Action(int state, string s1);
int LOOKID(string str);
int LOOKINT(int p);
void Error();
void NewQuad(int opr, int op1, int op2, int op3);
int NEWTEMP();
void Semantic(int rule);
void INTRPR();
void EXCT(int opr, int op1, int op2, int op3);

//Main Function
int main() {
	ifstream in, sc, sy, ip;
	int v;
	string key, f;
	cout << "Name : Nanda Kishore Reddy Thineti" << endl;
	cout << "MID  : M20227955" << endl << endl;
	cout << "Enter file name" << endl; //prompt for input file
	cin >> f;
	sy.open("Symbolsin.txt");
	sc.open("ScanStates.txt");
	in.open(f);

	if (!sc.is_open() || !sy.is_open() || !in.is_open()) {//unable to open file
		cout << "Error in opening files" << endl;
	}
	else {
		sy >> res_count;//Reading file "Symbolsin"
		for (int i = 0; i < res_count; i++) {
			symbols.push_back(symboletable());
			sy >> key;
			sy >> v;
			symbols[i].word = key;
			symbols[i].clas = v;
			symbols[i].addr = 0;
		}
		int temp;//Reading file "ScanStates"
		for (int i = 0; i < 14; i++) {
			for (int j = 0; j < 13; j++) {
				sc >> temp;
				scan[i][j] = temp;
			}
		}
		int_table.push_back(0);//int_table initialization 
		int_table.push_back(1);//int_table initialization
		setup();
		syntax.push_back(38);//push value 38 in syntax
		syntax.push_back(24);//push value 24 in syntax
		Parser(in);//calling parser
		for (int i = int_table.size() - 1; i >= 0; i--){
			MEM[lc] = int_table[i];
			lc++;
		}
		br = lc - 1;
		for (int i = 0; i < lc; i++)
		{
			std::cout << MEM[i] << " ";
			if ((i + 1) % 4 == 0)
			{
				std::cout << std::endl << endl;
			}

		}

		cout << "\nBEGIN EXECUTION\n" << endl;
		INTRPR();
		system("pause");
	}
}
//nextChar function that returns through the next character into input stream
char nextChar(ifstream& in) {
	if (len == 0) {
		k = 0;
		s = nextLine(in);//string length is zero, call nxtline function
		s += ' ';
		len = s.length();//store length of string in 'len'
		s_count = 0;
	}
	if (len > 0) {
		ch = s[s_count];//reading first character of the string
		s_count++;
		len--;//decrement length of string
	}
	return ch;//return chracter
}

//nextLine function that reads card from input stream and prints it out
string nextLine(ifstream& in) {
	if (!in.eof()) {
		getline(in, s);//reading line of input file
		line_no++;
		cout << s << endl;
	}
	return s;
}
//check function will check each character with its state
int check(int state, char c) {
	if (c >= 'A' && c <= 'Z') {//checks for identifiers and its state
		state = scan[state][1];
	}
	else if (isdigit(c)) {//checks for digits and its state
		state = scan[state][2];
	}
	else {
		switch (c) {
		case ' ': case '@':
			state = scan[state][0];
			break;
		case '+': case '-':
			state = scan[state][3];
			break;
		case '*':
			state = scan[state][4];
			break;
		case '/':
			state = scan[state][4];
			break;
		case '(':
			state = scan[state][5];
			break;
		case ')':
			state = scan[state][6];
			break;
		case '<':
			state = scan[state][7];
			break;
		case '>':
			state = scan[state][8];
			break;
		case '=':
			state = scan[state][9];
			break;
		case ':':
			state = scan[state][10];
			break;
		case '$':
			state = scan[state][11];
			break;
		default:
			state = scan[state][12];
			break;
		}
	}
	return state;
}
//Action function checks the states in scanstates
void Action(int st, string s1) {
	switch (st)
	{
	case 1://look for reserved words and identifiers
		if (s1[0] >= 'A' && s1[0] <= 'Z') {
			c = LOOKID(s1);//look up in symbol table
			cla = 21;
			if (c > res_count) {
				val = c - 1;
				//cout << "Class " << 21 << " Value " << c << endl;
			}
			else {
				cla = c;
				val = 0;
				//cout << "Class " << c << " Value " << 0 << endl;
			}
		}
		break;
	case 2:
		val = LOOKINT(stoi(s1));//look up in int table
		cla = 22;
		//	cout << "Class " << cla << " Value " << val << endl;
		break;
	case 3:
		cla = 15;
		if (s1 == "+")
			val = 0;
		else if (s1 == "-")
			val = 1;
		//	cout << "Class " << cla << " Value " << val << endl;
		break;
	case 4:
		cla = 16;
		if (s1 == "*")
			val = 0;
		else if (s1 == "/") {
			val = 1;
		}
		//	cout << "Class " << cla << " Value " << val << endl;
		break;
	case 5:
		cla = 17;
		if (s1 == "(") {
			val = 0;
			//cout << "Class " << cla << " Value " << val << endl;
		}
		break;
	case 6:
		cla = 18;
		if (s1 == ")") {
			val = 0;
			//	cout << "Class " << cla << " Value " << val << endl;
		}
		break;
	case 7:
		cla = 19;
		if (s1 == "<") {
			val = 1;
			//cout << "Class " << cla << " Value " << val << endl;
		}
		break;
	case 8:
		cla = 19;
		if (s1 == "<="){
			val = 3;
		}
		break;
	case 9:
		cla = 19;
		if (s1 == ">") {
			val = 2;
			//	cout << "Class " << cla << " Value " << val << endl;

		}
		break;
	case 10:
		cla = 19;
		if (s1 == ">="){
			val = 4;
		}
		break;
	case 11:
		cla = 20;
		if (s1 == "=") {
			val = 0;
			//cout << "Class " << cla << " Value " << val << endl;
		}
		break;
	case 12:
		cla = 19;
		if (s1 == "<>"){
			val = 0;
		}
		break;
	case 13:
		cla = 23;
		if (s1 == ":") {
			val = 0;
			//cout << "Class " << cla << " Value " << val << endl;
		}
		break;
	case 14:
		cla = -1;
		if (s1 == "$") {//checks for the "$" and displays the "error in line" and resumes the output again
			val = 0;
			//cout << cla << "error in line" << val << endl;
		}
		break;
	case 50:
		eof = true;
		break;
	case 100:
		error = true;
		cla = 0; val = 0;
		//if doesn't match identifier and word it displays 'error in line'
		cout << "Error in line " << line_no << endl;
		//cout << "Class " << cla << " Value " << val << endl;//prints class and value
		break;
	default:
		cout << "Invalid type" << endl;
		break;
	}
}
//LOOKID function that returns through table the identifier contained in STR is located
int LOOKID(string s1) {
	int n;
	n_c = 0;
	while (n_c == 0) {
		for (int j = 0; j < symbols.size() - 1; j++) {
			if (symbols[j].word == s1) //checking for reserved words
			{
				n = symbols[j].clas;
				n_c = 1;
				return n;
			}
		}
		if (n_c == 0) {
			symbols.push_back(symboletable());
			int u = symbols.size() - 1;
			symbols[u].word = s1;
			symbols[u].clas = symbols.size() + 1;
			symbols[u].addr = -1;
		}
	}
	return 0;
}
//LOOKINT function returs integer position
int LOOKINT(int p) {
	//if not found in integer table then store it into int_table
	if (!(find(int_table.begin(), int_table.end(), p) != int_table.end())) {
		int_table.push_back(p);
		return int_table.size() - 1;
	}
	else {
		int pos = find(int_table.begin(), int_table.end(), p) - int_table.begin();
		/*cout << "pos" << pos;*/
		return pos;
	}
}

//Scanner function which scan the tokens and print the values onto the screen
void scanToken(int cla, int val, ifstream &in) {
	while (state >= 0) {
		ch = nextChar(in);// calls nextLine if at the end of the string
		state = check(state, ch);
		if (state > 0) {
			s1 += ch;
		}
		else if (ch != ' ') {//handling spaces
			s_count--;
			len++;
		}
		if (state < 0) {
			//Remarks handling
			if (s1 == "REM") len = 0;
		}
	}
	Action(-state, s1);//calling Action
	o_s = s1;
	state = 0;
	s1 = "";
}

//Function read/load 'Rulesin.txt' and 'LL1.txt' in 2D array
void setup() {
	ifstream filestates, Lfile;
	filestates.open("Rulesin.txt");
	Lfile.open("LL1.txt");
	if (filestates.is_open()) {
		for (int i = 0; i < 29; i++)
		{
			for (int j = 0; j <8; j++)
			{
				filestates >> rule[i][j];//read and stores Rulesin.txt in 2D array
			}
		}
	}
	else {
		cout << "ERROR in opening Rulesin File" << endl;
	}
	if (Lfile.is_open()) {//if LL1.txt file is_open
		for (int i = 0; i < 14; i++) {
			for (int j = 0; j < 23; j++) {
				Lfile >> LL[i][j];//read and stores LL1.txt in 2D array
			}
		}
	}
	else {
		cout << "ERROR in opening LL1 File" << endl;
	}
}

//Function process the class and value for statement and calls semantic and INTRPR()
void Parser(ifstream & in) {
	int ki = 0, top = 0;
	while (!in.eof() && ki == 0) {
		scanToken(cla, val, in);
		ki = 1;
		while (!error) {
			if (cla == 14)
				scanToken(cla, val, in);
			int l = syntax.size() - 1;
			/*top = syntax.back();*///	initializing top as top of the syntax
			//condition for top of the syntax matches the terminal
			if (syntax.back() >= 1 && syntax.back() <= 23) {
				if (syntax.back() == cla) {
					syntax.pop_back();//pop top of the syntax
					info.push_back(val);
					//cout << "Info stack" << endl;
					//for (int i = 0; i < info.size(); i++)
					//{
					//	cout << info[i] << "\t";
					//}
					//cout << endl;
					scanToken(cla, val, in);//call ScanToken and return class,value and token
				}
				else {
					error = true;
					cla = -1;
				}
			}
			//condition for top of the syntax matches as non-terminal
			else if (syntax.back() >= 24 && syntax.back() <= 37) {
				r = LL[syntax.back() - 24][cla - 1];
				syntax.pop_back();//pop top of the syntax
				//syntax.push_back(r*-1);
				if (in.eof()) {

					/*if (cla == 1)
					{*/
					syntax.back() = -1;
					Semantic(-syntax.back());
					/*}*/
					/*for (int i = int_table.size() - 1; i >= 0; i--){
					MEM[lc] = int_table[i];
					lc++;
					cout << int_table[i] << "\t";
					}
					br = lc - 1;*/
					/*cout << "opr" << opr << "op1" << op1 << "op2" << op2 << "op3"<<op3;*/
					/*INTRPR(br, opr, op1, op2, op3);*///calling interpreter function

					//if (cla != 1)//for missing end statement
					//{
					//	cout << " Missing end statement " << endl;
					//}
					//break;
					//system("pause");
					//exit(0);
				}
				if (r != 0){
					syntax.push_back(r*-1);
					for (int i = 0; i < 8; i++) {
						if (rule[r - 1][i] != 0) {

							syntax.push_back(rule[r - 1][i]);
						}
					}
				}
				else if (r == 0){
					error = true;
				}

			}
			//condition for top is negative 
			else if (syntax.back() < 0){
				//syntax.pop_back();//pop top of the sytax
				Semantic(-syntax.back());//calling semantic()
				syntax.pop_back();//pop top of the sytax

			}

			else if (syntax.back() == 38){//condition for top of syntax is 38
				error = true;
			}
		}
	}
}
//Print error message
void Error() {
	cout << "Error " << endl;
}
//function to generate Quads
void NewQuad(int opr, int op1, int op2, int op3){
	MEM[lc] = opr;
	MEM[lc + 1] = op1;
	MEM[lc + 2] = op2;
	MEM[lc + 3] = op3;
	lc += 4; //increment lc by 4
	//cout << opr << "\t" << op1 << "\t" << op2 << "\t" << op3 << endl;
	cout << endl;
}

//temporary variable that returns ADDR
int NEWTEMP()
{
	ADDR++;
	return ADDR;
}

//function to handle semantic actions
void Semantic(int rule){
	int save, save1, save2, p, temp, var, top;

	switch (rule){
	case 1: //Halt
		NewQuad(10, 0, 0, 0);
		if (!info.empty())
			info.pop_back();
		break;
		//DCL, put id’s new offset in Symbol 
	case 2:  
		ADDR++;
		top = info.back();
		info.pop_back();
		symbols[top].addr = ADDR;
		top = info.back();
		info.pop_back();
		break;
	case 3:
		//No code generated
		break;
	case 4:
		//No code generated
		break;
	case 5:
		//No code generated
		break;
	case 6:
		//No code generated
		break;
	case 7:
		//No code generated
		break;
	case 8:
		//handle label->id
		info.pop_back();
		top = info.back();
		info.pop_back();
		p = symbols[top].addr;
		symbols[top].addr = lc;
		while ((p < 0) && (p != 2))
		{
			int p1 = MEM[-p];
			MEM[-p] = lc;
			p = p1;
		}
		break;

		//for rule <stat1> ->  LET <id> = <expr>
	case 9:
		save1 = info.back();
		info.pop_back();
		info.pop_back();
		save2 = info.back();
		info.pop_back();
		info.pop_back();
		NewQuad(4, symbols[save2].addr, save1, 0);
		break;

		// handling GOTO statement
	case 10:
		top = info.back();
		info.pop_back();
		p = symbols[top].addr;
		NewQuad(5, p, 0, 0);
		if (p <= 0){
			symbols[top].addr = -(lc - 3);
		}
		info.pop_back();
		break;

		//handling if statement
	case 11:
		top = info.back();
		info.pop_back();
		p = symbols[top].addr;
		info.pop_back();
		info.pop_back();
		op2 = info.back();  //operand 2
		info.pop_back();
		var = info.back();  // var
		info.pop_back();
		op1 = info.back(); // operand 1
		info.pop_back();
		temp = NEWTEMP();
		switch (var)
		{
		case 0: // if x<>y
			NewQuad(1, temp, op1, op2);
			NewQuad(6, p, lc + 8, 0);
			NewQuad(5, p, temp, 0);
			break;
		case 1: // if x<y
			NewQuad(1, temp, op2, op1);
			NewQuad(7, p, temp, 0);
			break;
		case 2: // if x>y
			NewQuad(1, temp, op1, op2);
			NewQuad(7, p, temp, 0);
			break;
		case 3: // if x<=y
			NewQuad(1, temp, op2, op1);
			NewQuad(0, temp, temp, -1);
			NewQuad(7, p, temp, 0);
			break;
		case 4: // if x>=y
			NewQuad(1, temp, op1, op2);
			NewQuad(0, temp, temp, 0);
			NewQuad(7, p, temp, 0);
			break;
		case 5: // if x=y
			NewQuad(1, temp, op1, op2);
			NewQuad(6, p, temp, 0);
			break;
		}
		if (p <= 0)
			symbols[top].addr = -(lc - 3);
		info.pop_back();
		break;

		//<stat1>-><for> <body> NEXT
	case 12:
		top = info.back();
		info.pop_back();
		top = info.back();
		info.pop_back();
		NewQuad(5, top, 0, 0);
		MEM[top + 9] = lc;
		break;

		//generate code for READ id
	case 13:
		top = info.back();
		info.pop_back();
		NewQuad(8, symbols[top].addr, 0, 0);
		top = info.back();
		info.pop_back();
		break;

		//generate code for WRITE id
	case 14:
		top = info.back();
		info.pop_back();
		NewQuad(9, top, 0, 0);
		top = info.back();
		info.pop_back();
		break;

		//FOR <id>=<expr> <step> TO <expr> DO
	case 15:
		info.pop_back();
		op2 = info.back();
		info.pop_back();
		info.pop_back();
		op1 = info.back();
		info.pop_back();
		save = info.back();
		info.pop_back();
		info.pop_back();
		top = info.back();
		info.pop_back();
		info.pop_back();
		//generate assignment stmt <id>=<expr>
		NewQuad(4, symbols[top].addr, save, 0);
		//skip the increment
		NewQuad(5, lc + 8, 0, 0);
		info.push_back(lc);
		NewQuad(0, symbols[top].addr, symbols[top].addr, op1);
		temp = NEWTEMP();
		NewQuad(1, temp, symbols[top].addr, op2);
		NewQuad(7, 0, temp, 0);
		break;

		//handling <step> -> STEP <expr>
	case 16:
		top = info.back();
		info.pop_back();
		info.pop_back();
		info.push_back(top);
		break;

		//handling <stat>->E
	case 17:
		info.push_back(-1);
		break;
	case 18:
		//No code generated
		break;

		//handling <relpo>->=
	case 19:
		info.pop_back();
		info.push_back(5);
		break;
	case 20:
		//No code generated
		break;
	case 22:
		//No code generated
		break;
	case 24:
		//No code generated
		break;

		//for rule <operand> -> <number> 
	case 25:
		top = info.back();
		info.pop_back();
		info.push_back(-top);
		break;

		//for rule <operand> -> <id>
	case 26:
		top = info.back();
		info.pop_back();
		info.push_back(symbols[top].addr);
		break;

		//for rule <operand> ->  ( <expr> )
		// to get rid of (  and  )
	case 27:
		info.pop_back();
		top = info.back();
		info.pop_back();
		info.pop_back();
		info.push_back(top);
		break;
	case 28:
		break;
	}
}
//function that is calling EXCT() 
void INTRPR()
{
	lc = 0;
	////condition for assigning operation and operands values
	while (lc != -1)
	{
		//assigning opr to MEM[LC]
		opr = MEM[lc];
		//assigning op1 = MEM[LC + 1]
		op1 = MEM[lc + 1];
		//assigning op2 = MEM[LC + 2]
		op2 = MEM[lc + 2];
		//assigning op3 = MEM[LC + 3]
		op3 = MEM[lc + 3];
		//counter LC value by 4
		lc += 4;
		//calling the function EXCT
		EXCT(opr, op1, op2, op3);
	}
}
//function executes the instruction whose operation code is store in opr
void EXCT(int opr, int op1, int op2, int op3){
	//br = lc - 1;
	switch (opr) { //switch case on opr
	case 0:
		MEM[op1 + br] = MEM[op2 + br] + MEM[op3 + br];
		break;
	case 1:
		MEM[op1 + br] = MEM[op2 + br] - MEM[op3 + br];
		break;
	case 2:
		MEM[op1 + br] = MEM[op2 + br] * MEM[op3 + br];
		break;
	case 3:
		MEM[op1 + br] = MEM[op2 + br] / MEM[op3 + br];
		break;
	case 4:
		MEM[op1 + br] = MEM[op2 + br];
		break;
	case 5:
		lc = op1;
		break;
	case 6:
		if (MEM[op2 + br] == 0)
		{
			lc = op1;
		}
		break;
	case 7:
		if (MEM[op2 + br] > 0)
		{
			lc = op1;
		}
		break;
	case 8:
		cout << "input integer value: ";
		cin >> x;
		MEM[op1 + br] = x;
		x = y;
		break;
	case 9:
		cout << "Output:" << MEM[op1 + br] << endl;
		break;
	case 10:
		lc = -1;
		cout << "FINISH EXECUTION" << endl;
		break;
	default:
		cout << " Invalid operator code" << endl;
		lc = -1;
		break;
	}
}