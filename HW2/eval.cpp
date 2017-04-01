#include <string>
#include <iostream>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

bool precedence(char a, char b)
{
	if (a == '!' && b == '!') return true;
	if (a == '&' && (b == '!' || b == '&')) return true;
	if (a == '|') return true;
	return false;
}


int evaluate(string infix, const bool values[], string& postfix, bool& result)
{
	if (infix == "") return 1;
	//infix -> postfix
	postfix = "";
	stack<char> ss;
	char before = '|';
	for (int i = 0; i < infix.length(); i++)
	{
		if (infix[i] == ' ') continue;
		if (infix[i] >= '0' && infix[i] <= '9')
		{
			if (before != '!' && before != '&' && before != '|' && before != '(') return 1;
			postfix += infix[i];
		}
			
		else if (infix[i] == '(')
		{
			if (before != '!' && before != '&' && before != '|' && before != '(') return 1;
			ss.push(infix[i]);
			
		}
			
		else if (infix[i] == ')')
		{
			if (before != ')' && !(before >= '0' && before <= '9')) return 1;
			while (ss.top() != '(')
			{
				postfix += ss.top();
				ss.pop();
			}
			if (ss.empty()) return 1;
			ss.pop();
		}
		else
		{
			if (infix[i] == '!')
			{
				if (before != '!' && before != '&' && before != '|' && before != '(') return 1;

			}
			else if (before != ')' && !(before >= '0' && before <= '9')) return 1;
			while (!ss.empty() && ss.top() != '(' && precedence(infix[i], ss.top()))
			{
				postfix += ss.top();
				ss.pop();
			}
			ss.push(infix[i]);
		}
		before = infix[i];
	}
	while (!ss.empty())
	{
		if (ss.top() == '(') return 1;
		postfix += ss.top();
		ss.pop();
	}
	if (!(before >= '0' && before <= '9') && before != ')') return 1;
	// evaluate the postfix.
	stack<bool> s;
	for (int i = 0; i < postfix.length(); i++)
	{
		if (postfix[i] >= '0' && postfix[i] <= '9')
			s.push(values[int(postfix[i]) - int('0')]);
		else {
			if (postfix[i] == '!') {
				bool bj = s.top();
				s.pop();
				bj = !bj;
				s.push(bj);
			}
			else
			{
				bool bj1, bj2;
				bj1 = s.top(); s.pop();
				bj2 = s.top(); s.pop();
				if (postfix[i] == '|') s.push(bj1 | bj2);
				if (postfix[i] == '&') s.push(bj1 & bj2);
			}
		}
	}
	result = s.top();
	return 0;
}


int main()
{
		bool ba[10] = {
			//  0      1      2      3      4      5      6      7      8      9
			true,  true,  true,  false, false, false, true,  false, true,  false
		};
		string pf;
		bool answer;
		assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" &&  answer);
		assert(evaluate("8|", ba, pf, answer) == 1);
		assert(evaluate("4 5", ba, pf, answer) == 1);
		assert(evaluate("01", ba, pf, answer) == 1);
		assert(evaluate("()", ba, pf, answer) == 1);
		assert(evaluate("2(9|8)", ba, pf, answer) == 1);
		assert(evaluate("2(&8)", ba, pf, answer) == 1);
		assert(evaluate("(6&(7|7))|8", ba, pf, answer) == 0 && answer);
		assert(evaluate("", ba, pf, answer) == 1);
		assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0
			&& pf == "43!03&&|" && !answer);
		assert(evaluate(" 9  ", ba, pf, answer) == 0 && pf == "9" && !answer);
		ba[2] = false;
		ba[9] = true;
		assert(evaluate("((9))", ba, pf, answer) == 0 && pf == "9"  &&  answer);
		assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" && !answer);
		cout << "Passed all tests" << endl;
}