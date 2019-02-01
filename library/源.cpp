#include"book.h"
#include<iostream>
#include<string.h>
using namespace std;


int main() {
	BTree root = NULL;
	int t, p;
	char ch1[100] = "aaaa";  //管理员登录密码
	char ch2[100] = "bbbb";  //读者登录密码
	char ch3[100] = "a";     //给输入的密码初始化的，随便输
	char *ch = ch3;  //需要你输入的密码
	int num;//num作为输入次数
	int flag1 = -1;//用来管理员登录退出的条件
	int flag2 = -1;//用来读者登录退出的条件
	while (1) {
		cout << "               *******************************************" << endl;
		cout << "               *                                         *" << endl;
		cout << "               *               图书管理系统              *" << endl;
		cout << "               *                                         *" << endl;
		cout << "               *      温馨提示：管理员登录密码为aaaa     *" << endl;
		cout << "               *                学生登录密码为bbbb       *" << endl;
		cout << "               *      注意：你有三次输入错误的机会       *" << endl;
		cout << "               *            输入错误系统将自动返回       *" << endl;
		cout << "               *                                         *" << endl;
		cout << "               *******************************************" << endl;
		cout << "               *******************************************" << endl;
		cout << "               *             1.管理员登录                *" << endl;
		cout << "               *             2.学生登录                  *" << endl;
		cout << "               *             0.退出                      *" << endl;
		cout << "               *******************************************" << endl;
		cout << "请选择登录方式：" << endl;
		cin >> t;
		if (t == 0) break;
		switch (t) {
		case 1: {
			num = 3;
			while (num--) {
				cout << "请输入登录密码：";
				cin >> ch;
				if (strcmp(ch, ch1) == 0) {
					cout << "登陆成功" << endl;
					while (1) {
						cout << "               **************************************" << endl;
						cout << "               *            1.图书采编入库          *" << endl;
						cout << "               *            2.查询书库库存          *" << endl;
						cout << "               *            3.显示所有图书          *" << endl;
						cout << "               *            4.清除书库库存          *" << endl;
						cout << "               *            0.退出登录              *" << endl;
						cout << "               **************************************" << endl;
						cout << "请选择你要进行的操作：";
						cin >> p;
						if (p == 0) {
							flag1 = 0;
							break;
						}
						switch (p) {
						case 1:Append_book(&root);
							break;
						case 2:Inquire_book(root);
							break;
						case 3:Print_book(root, 0);
							break;
						case 4:Clear_book(&root);
							break;
						default:
							cout << "输入错误!" << endl;
							break;
						}
					}

				}
				else cout << "密码错误，您还有" << num << "次机会" << endl;
				if (flag1 == 0) break;
			}
			break;
		}
		case 2: {
			num = 3;
			while (num--) {
				cout << "请输入密码：";
				cin >> ch;
				if (strcmp(ch, ch2) == 0) {
					cout << "登录成功" << endl;
					while (1) {
						cout << "               **************************************" << endl;
						cout << "               *            1.查询书库库存          *" << endl;
						cout << "               *            2.借阅书库图书          *" << endl;
						cout << "               *            3.归还所借图书          *" << endl;
						cout << "               *            0.退出登录              *" << endl;
						cout << "               **************************************" << endl;
						cout << "请选择你要进行的操作：";
						cin >> p;
						if (p == 0) {
							flag2 = 0;
							break;
						}
						switch (p) {
						case 1:Inquire_book(root);
							break;
						case 2:Lend_book(&root);
							break;
						case 3:Return_book(&root);
							break;
						default:
							cout << "error!" << endl;
							break;
						}
					}
				}
				else cout << "密码错误，您还有" << num << "次机会" << endl;
				if (flag2 == 0) break;
			}
			break;
		}
		}
	}
}


/*
1 a A 1
2 b B 2
3 c C 3
4 d D 4
5 e E 5
6 f F 6
7 g G 7
8 h H 8
9 i I 9
10 j J 10
11 k K 11
12 l L 12
13 m M 13
14 n N 14
15 o O 15
16 p P 16
17 q Q 17
18 r R 18
19 s S 19
20 t T 20
21 u U 20
22 v V 22
23 w W 23
24 x X 24
25 y Y 25
26 z Z 26

*/

/*

*/