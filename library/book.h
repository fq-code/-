#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string.h>
#include<cstdio>
#include <fstream>
//#define FALSE 0
//#define TRUE 1
#define ll long long
const int m = 4; //m树，4阶B树
using namespace std;

//图书结构体
typedef struct {
	char name[20];     //书名
	char auther[30];   //作者
	ll booknum;      //书号
	int store;         //库存
	int sign;          //标记书是否被借出
}book;

//B树结构体
typedef book KeyType;   
typedef struct BTNode {
	int keynum;          //关键字个数
	struct BTNode *parent;  //结点的父亲结点
	KeyType key[m + 1];  //结点中的关键码
	struct BTNode *child[m + 1];   //孩子结点
}BTNode, *BTree;

typedef struct {      //关键字的状态结构体
	BTNode *ptr;
	int i;           //关键字的位置
	int flag;        //是否存在关键字
}Result;             //关键字结果

//设结点中的指针向量为空，初始化
void SetNull(BTree &p) {
	int i = 0;
	while (i <= p->keynum) {
		p->child[i] = NULL;
		i++;
	}
}
//在结点中查找关键字
int SearchNode(BTree p, KeyType *k) {
	int i = 1;
	while (i <= p->keynum) {
		if (k->booknum < p->key[i].booknum) return i - 1;
		i++;
	}
	return i - 1;
}
//在B树中查找关键字
Result SearchBTree(BTree t, KeyType *k) {
	BTree p = t, q = NULL;
	bool found = false;       //用来表示是否找到了关键字
	int i = 0;
	Result result;
	while (p && !found) {
		i = SearchNode(p, k);
		if (i > 0 && p->key[i].booknum == k->booknum) found = true;
		else {
			q = p;
			p = p->child[i];
		}
	}
	if (found) {
		result.ptr = p;
		result.i = i;
		result.flag = 1;    //找到，就将标志位置为1
	}
	else {
		result.ptr = q;
		result.i = i;
		result.flag = 0;   //反之，将标志位置为0
	}
	return result;
}
//赋值函数
void Assign(KeyType *p, KeyType *q) {
	p->store = q->store;
	strcpy(p->auther, q->auther);
	p->booknum = q->booknum;
	strcpy(p->name, q->name);
	p->sign = q->sign;
}
//在新结点中插入新的关键字k和指针p
void InsertInNode(BTree &q, int i, KeyType *k, BTree p) {
	int j;
	for (j = q->keynum; j > i; j--) Assign(&(q->key[j + 1]), &(q->key[j]));    //新的大于原来的关键字，就要后移
	Assign(&(q->key[j + 1]), k);          //插入成功，插入在i+1的位置
	for (j = q->keynum; j > i; j--) q->child[j + 1] = q->child[j];
	q->child[j + 1] = p;
	if (p) p->parent = q;
	q->keynum++;
}
//分裂结点p,q用来存放新结点
void Split(BTree p, BTree &q) {
	int s = m % 2 == 0 ? m / 2 - 1 : m / 2, i, j = 0, t;
	p->keynum = s;
	q = (BTree)malloc(sizeof(BTNode));
	SetNull(q);
	for (i = s + 2; i <= m; i++) {
		q->child[j] = p->child[i - 1];
		Assign(&(q->key[++j]), &(p->key[i]));
	}
	q->child[j] = p->child[i - 1];
	q->keynum = j;
	for (t = s + 1; t <= m; ++t) {
		if (p->child[t] != NULL) p->child[t]->parent = q;
	}
}
//在q结点第i个位置插入关键字
void InsertBTree(BTree &t, KeyType *k, BTree q, int i) {
	BTree split = NULL, root; //split为分裂出来的结点
	bool finish = false;//表示是否插入成功
	int s;
	KeyType *x;
	x = (KeyType*)malloc(sizeof(KeyType));
	Assign(x, k);
	while (q && !finish) {
		InsertInNode(q, i, x, split);
		if (q->keynum < m) finish = true;
		else {
			s = m % 2 == 0 ? m / 2 : m / 2 + 1;
			Split(q, split);
			Assign(x, &(q->key[s]));     //中间元素分裂出来
			q = q->parent;
			if (q) i = SearchNode(q, x);
		}
	}
	if (!finish) {  //进行了根结点的分裂
		root = (BTree)malloc(sizeof(BTNode));
		SetNull(root);
		Assign(&(root->key[1]), x);
		root->child[0] = t;
		root->child[1] = split;
		root->keynum = 1;
		root->parent = NULL;
		if (t) t->parent = root;
		if (split) split->parent = root;
		t = root;
	}
}
//在B树中插入一个关键字
void InsertNode(BTree &t, KeyType *key) {
	Result result;
	result = SearchBTree(t, key);
	if (!result.flag) { //关键字不存在，插入
		InsertBTree(t, key, result.ptr, result.i);
	}
}

//与左孩子比较
void LeftCompress(BTNode *p, int i) {
	int n = p->keynum;
	for (int j = i; j < n; j++) {
		p->key[j] = p->key[j + 1];
		p->child[j] = p->child[j + 1];
	}
	p->keynum--;
}
//与右孩子比较
void RightCompress(BTNode *p, int i) {
	for (int j = p->keynum; j >= i; j--) {
		p->key[j + 1] = p->key[j];
		p->child[j + 1] = p->child[j];
	}
	p->keynum++;
}
//合并结点
void MergeNode(BTNode *p, BTNode *q, BTNode *pR, int i) {
	int n = p->keynum + 1;
	p->key[n] = q->key[i];
	p->child[n] = pR->child[0];
	for (int j = 1; j <= pR->keynum; j++) {
		p->key[n + j] = pR->key[j];
		p->child[n + j] = pR->child[j];
	}
	if (p->child[0]) {
		for (int j = 0; j <= pR->keynum; j++)
			p->child[n + j]->parent = p;
	}
	LeftCompress(q, i);
	p->keynum = p->keynum + pR->keynum + 1;
	delete pR;
	pR = NULL;
}
//与右孩子调整
void RightAdjust(BTNode *p, BTNode *q, int i) {
	BTNode *pR = q->child[i + 1];    //p的右兄弟
	if (pR->keynum >= (m + 1) / 2) {     //右兄弟可以借
		p->keynum++;
		p->key[p->keynum] = q->key[i + 1];
		p->child[p->keynum] = pR->child[0];
		if (p->child[p->keynum] != NULL)
			p->child[p->keynum]->parent = p;
		q->key[i + 1] = pR->key[1];
		pR->child[0] = pR->child[1];
		LeftCompress(pR, 1);
	}
	else
		MergeNode(p, q, pR, i + 1);
}
//与左孩子调整
void LeftAdjust(BTNode *p, BTNode *q, int i) {
	BTNode *pL = q->child[i - 1];
	if (pL->keynum >= (m + 1) / 2) {
		RightCompress(p, 1);
		p->child[1] = p->child[0];
		p->key[1] = q->key[i];
		p->child[0] = pL->child[pL->keynum];
		if (p->child[0] != NULL)
			p->child[0]->parent = p;
		q->key[i] = pL->key[pL->keynum];
		pL->keynum--;
	}
	else {
		BTNode *pR = p;
		p = pL;
		MergeNode(p, q, pR, i);
	}
}

bool Delete(BTree t,KeyType *k){
	Result re = SearchBTree(t,k);
	if (re.flag== false)
		return false;
	BTNode *p = re.ptr, *q;
	int i = re.i;
	if (p->child[i] != NULL) {    //非叶子结点，找右邻接值替换
		q = p->child[i];
		while (q->child[0] != NULL)
			q = q->child[0];
		p->key[i] = q->key[1];
		LeftCompress(q, 1);
		p = q;
	}
	else      //叶子节点
		LeftCompress(p, i);
	int mid = (m + 1) / 2;

	while (1) {
		if (p == t || p->keynum >= mid - 1)
			break;
		else {        //小于最小限制
			q = p->parent;
			for (i = 0; i <= q->keynum && q->child[i] != p; i++);
			if (i == q->keynum)
				LeftAdjust(p, q, i); //向左兄弟调整
			else
				RightAdjust(p, q, i);

			p = q;
		}
	}
	if (t->keynum == 0) {
		p = t->child[0];
		delete t;
		t = p;
		if (t != NULL)
			t->parent = NULL;
	}
	return true;
}








//图书采编入库
void Append_book(BTree *r) {
	char ch1[100] = "aaaa";
	book b;
	Result re;
	cout << "请按照书号、书名、作者、库存量的输入顺序，依次输入书的信息，当输入书号为负数时结束:" << endl;
	while (1) {
		cin >> b.booknum;
	    if (b.booknum < 0)break;
		cin >> b.name >> b.auther >> b.store;
		b.sign = b.store;
		re = SearchBTree(*r, &b);
		if (re.flag == 0) InsertNode(*r, &b);
		else {
			re.ptr->key[re.i].store += b.store;
			re.ptr->key[re.i].sign += b.sign;
		}
	}
}

//借阅图书
void Lend_book(BTree *r) {
	book b;
	int k;
	Result re;
	cout << "请输入所借书的书号：";
	cin >> b.booknum;
	if (b.booknum < 0) {
		cout << "输入错误" << endl;
		return;
	}
	cout << "输入所借书的数量：";
	cin >> k;
	re = SearchBTree(*r, &b);
	if (re.flag == 0) cout << "对不起，没有此书！" << endl;
	else {
		if (re.ptr->key[re.i].store >= k) {
			re.ptr->key[re.i].store -= k;
			cout << "您所借的书，书号为：" << re.ptr->key[re.i].booknum << "、书名为：" << re.ptr->key[re.i].name << "、作者为：" << re.ptr->key[re.i].auther << endl;
			cout << "借书成功！" << endl;
			cout << "该书的现存量为：" << re.ptr->key[re.i].store << endl;
		}
		else cout << "该书库存不够，现有库存为："<< re.ptr->key[re.i].store << endl;
		if (re.ptr->key[re.i].sign == 0) cout << "此书已清除库存，请采编入库" << endl;
	//	if (re.pt->key[re.i].store >= 0 && re.pt->key[re.i].sign > 0) cout << "借书成功" << endl;
	}
}

//查看库存
void Inquire_book(BTree r) {
	book b;
	Result re;
	cout << "请输入你要查询的书的信息(书号）:";
	cin >> b.booknum;
	if (b.booknum < 0) {
		cout << "输入错误" << endl;
		return;
	}
	re = SearchBTree(r, &b);
	if (re.flag == 0) cout << "对不起，没有此图书" << endl;
	else cout << "您所借的书，书号为：" << re.ptr->key[re.i].booknum << "、书名为：" << re.ptr->key[re.i].name << "、作者为：" << re.ptr->key[re.i].auther << "、此书库存为：" << re.ptr->key[re.i].store << endl;
}
//归还图书
void Return_book(BTree *r) {
	book b;
	int k;
	Result re;
	cout << "请输入归还的书的书号：";
	cin >> b.booknum;
	if (b.booknum < 0) {
		cout << "输入错误" << endl;
		return;
	}
	cout << "请输入归还的本数：";
	cin >> k;
	re = SearchBTree(*r, &b);
	if (re.flag == 0) cout << "对不起，没有此图书！" << endl;
	else {
		re.ptr->key[re.i].store += k;
		cout << "归还成功!" << endl;
	}
}
//显示所有图书
void Print_book(BTree r, int n) {
	int i = 1, j;
	if (r == NULL) return;
	while (i <= r->keynum) {
		Print_book(r->child[i - 1], n + 1);
		for (j = 0; j < n; j++) cout << "           ";
		if (n >= 0) {
			cout << "---";
			cout << "书号：" << r->key[i].booknum << "   " << "书名：" <<
				r->key[i].name << "   " << "作者：" << r->key[i].auther << "   " <<
				"库存：" << r->key[i].store << endl << endl;
		}
		i++;
	}
	Print_book(r->child[i - 1], n + 1);

}
//清除库存
void Clear_book(BTree *r) {
//	book b;
	Result re;
	KeyType b;
	cout << "请输入你要清除库存的书的信息（书号）:";
	cin >> b.booknum;
	if (b.booknum > 0) {
		re = SearchBTree(*r, &b);
		if (re.flag == 0) cout << "没有此图书" << endl;
	/*	else {
			re.ptr->key[re.i].store = 0;
			re.ptr->key[re.i].sign = 0;
		}*/
		Delete(*r, &b);
		cout << "清除库存成功" << endl;
	}
	else {
		cout << "输入错误" << endl;
		return;
	}
}
