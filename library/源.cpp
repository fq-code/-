#include"book.h"
#include<iostream>
#include<string.h>
using namespace std;


int main() {
	BTree root = NULL;
	int t, p;
	char ch1[100] = "aaaa";  //����Ա��¼����
	char ch2[100] = "bbbb";  //���ߵ�¼����
	char ch3[100] = "a";     //������������ʼ���ģ������
	char *ch = ch3;  //��Ҫ�����������
	int num;//num��Ϊ�������
	int flag1 = -1;//��������Ա��¼�˳�������
	int flag2 = -1;//�������ߵ�¼�˳�������
	while (1) {
		cout << "               *******************************************" << endl;
		cout << "               *                                         *" << endl;
		cout << "               *               ͼ�����ϵͳ              *" << endl;
		cout << "               *                                         *" << endl;
		cout << "               *      ��ܰ��ʾ������Ա��¼����Ϊaaaa     *" << endl;
		cout << "               *                ѧ����¼����Ϊbbbb       *" << endl;
		cout << "               *      ע�⣺���������������Ļ���       *" << endl;
		cout << "               *            �������ϵͳ���Զ�����       *" << endl;
		cout << "               *                                         *" << endl;
		cout << "               *******************************************" << endl;
		cout << "               *******************************************" << endl;
		cout << "               *             1.����Ա��¼                *" << endl;
		cout << "               *             2.ѧ����¼                  *" << endl;
		cout << "               *             0.�˳�                      *" << endl;
		cout << "               *******************************************" << endl;
		cout << "��ѡ���¼��ʽ��" << endl;
		cin >> t;
		if (t == 0) break;
		switch (t) {
		case 1: {
			num = 3;
			while (num--) {
				cout << "�������¼���룺";
				cin >> ch;
				if (strcmp(ch, ch1) == 0) {
					cout << "��½�ɹ�" << endl;
					while (1) {
						cout << "               **************************************" << endl;
						cout << "               *            1.ͼ��ɱ����          *" << endl;
						cout << "               *            2.��ѯ�����          *" << endl;
						cout << "               *            3.��ʾ����ͼ��          *" << endl;
						cout << "               *            4.��������          *" << endl;
						cout << "               *            0.�˳���¼              *" << endl;
						cout << "               **************************************" << endl;
						cout << "��ѡ����Ҫ���еĲ�����";
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
							cout << "�������!" << endl;
							break;
						}
					}

				}
				else cout << "�������������" << num << "�λ���" << endl;
				if (flag1 == 0) break;
			}
			break;
		}
		case 2: {
			num = 3;
			while (num--) {
				cout << "���������룺";
				cin >> ch;
				if (strcmp(ch, ch2) == 0) {
					cout << "��¼�ɹ�" << endl;
					while (1) {
						cout << "               **************************************" << endl;
						cout << "               *            1.��ѯ�����          *" << endl;
						cout << "               *            2.�������ͼ��          *" << endl;
						cout << "               *            3.�黹����ͼ��          *" << endl;
						cout << "               *            0.�˳���¼              *" << endl;
						cout << "               **************************************" << endl;
						cout << "��ѡ����Ҫ���еĲ�����";
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
				else cout << "�������������" << num << "�λ���" << endl;
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