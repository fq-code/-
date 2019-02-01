#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string.h>
#include<cstdio>
#include <fstream>
//#define FALSE 0
//#define TRUE 1
#define ll long long
const int m = 4; //m����4��B��
using namespace std;

//ͼ��ṹ��
typedef struct {
	char name[20];     //����
	char auther[30];   //����
	ll booknum;      //���
	int store;         //���
	int sign;          //������Ƿ񱻽��
}book;

//B���ṹ��
typedef book KeyType;   
typedef struct BTNode {
	int keynum;          //�ؼ��ָ���
	struct BTNode *parent;  //���ĸ��׽��
	KeyType key[m + 1];  //����еĹؼ���
	struct BTNode *child[m + 1];   //���ӽ��
}BTNode, *BTree;

typedef struct {      //�ؼ��ֵ�״̬�ṹ��
	BTNode *ptr;
	int i;           //�ؼ��ֵ�λ��
	int flag;        //�Ƿ���ڹؼ���
}Result;             //�ؼ��ֽ��

//�����е�ָ������Ϊ�գ���ʼ��
void SetNull(BTree &p) {
	int i = 0;
	while (i <= p->keynum) {
		p->child[i] = NULL;
		i++;
	}
}
//�ڽ���в��ҹؼ���
int SearchNode(BTree p, KeyType *k) {
	int i = 1;
	while (i <= p->keynum) {
		if (k->booknum < p->key[i].booknum) return i - 1;
		i++;
	}
	return i - 1;
}
//��B���в��ҹؼ���
Result SearchBTree(BTree t, KeyType *k) {
	BTree p = t, q = NULL;
	bool found = false;       //������ʾ�Ƿ��ҵ��˹ؼ���
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
		result.flag = 1;    //�ҵ����ͽ���־λ��Ϊ1
	}
	else {
		result.ptr = q;
		result.i = i;
		result.flag = 0;   //��֮������־λ��Ϊ0
	}
	return result;
}
//��ֵ����
void Assign(KeyType *p, KeyType *q) {
	p->store = q->store;
	strcpy(p->auther, q->auther);
	p->booknum = q->booknum;
	strcpy(p->name, q->name);
	p->sign = q->sign;
}
//���½���в����µĹؼ���k��ָ��p
void InsertInNode(BTree &q, int i, KeyType *k, BTree p) {
	int j;
	for (j = q->keynum; j > i; j--) Assign(&(q->key[j + 1]), &(q->key[j]));    //�µĴ���ԭ���Ĺؼ��֣���Ҫ����
	Assign(&(q->key[j + 1]), k);          //����ɹ���������i+1��λ��
	for (j = q->keynum; j > i; j--) q->child[j + 1] = q->child[j];
	q->child[j + 1] = p;
	if (p) p->parent = q;
	q->keynum++;
}
//���ѽ��p,q��������½��
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
//��q����i��λ�ò���ؼ���
void InsertBTree(BTree &t, KeyType *k, BTree q, int i) {
	BTree split = NULL, root; //splitΪ���ѳ����Ľ��
	bool finish = false;//��ʾ�Ƿ����ɹ�
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
			Assign(x, &(q->key[s]));     //�м�Ԫ�ط��ѳ���
			q = q->parent;
			if (q) i = SearchNode(q, x);
		}
	}
	if (!finish) {  //�����˸����ķ���
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
//��B���в���һ���ؼ���
void InsertNode(BTree &t, KeyType *key) {
	Result result;
	result = SearchBTree(t, key);
	if (!result.flag) { //�ؼ��ֲ����ڣ�����
		InsertBTree(t, key, result.ptr, result.i);
	}
}

//�����ӱȽ�
void LeftCompress(BTNode *p, int i) {
	int n = p->keynum;
	for (int j = i; j < n; j++) {
		p->key[j] = p->key[j + 1];
		p->child[j] = p->child[j + 1];
	}
	p->keynum--;
}
//���Һ��ӱȽ�
void RightCompress(BTNode *p, int i) {
	for (int j = p->keynum; j >= i; j--) {
		p->key[j + 1] = p->key[j];
		p->child[j + 1] = p->child[j];
	}
	p->keynum++;
}
//�ϲ����
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
//���Һ��ӵ���
void RightAdjust(BTNode *p, BTNode *q, int i) {
	BTNode *pR = q->child[i + 1];    //p�����ֵ�
	if (pR->keynum >= (m + 1) / 2) {     //���ֵܿ��Խ�
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
//�����ӵ���
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
	if (p->child[i] != NULL) {    //��Ҷ�ӽ�㣬�����ڽ�ֵ�滻
		q = p->child[i];
		while (q->child[0] != NULL)
			q = q->child[0];
		p->key[i] = q->key[1];
		LeftCompress(q, 1);
		p = q;
	}
	else      //Ҷ�ӽڵ�
		LeftCompress(p, i);
	int mid = (m + 1) / 2;

	while (1) {
		if (p == t || p->keynum >= mid - 1)
			break;
		else {        //С����С����
			q = p->parent;
			for (i = 0; i <= q->keynum && q->child[i] != p; i++);
			if (i == q->keynum)
				LeftAdjust(p, q, i); //�����ֵܵ���
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








//ͼ��ɱ����
void Append_book(BTree *r) {
	char ch1[100] = "aaaa";
	book b;
	Result re;
	cout << "�밴����š����������ߡ������������˳���������������Ϣ�����������Ϊ����ʱ����:" << endl;
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

//����ͼ��
void Lend_book(BTree *r) {
	book b;
	int k;
	Result re;
	cout << "���������������ţ�";
	cin >> b.booknum;
	if (b.booknum < 0) {
		cout << "�������" << endl;
		return;
	}
	cout << "�����������������";
	cin >> k;
	re = SearchBTree(*r, &b);
	if (re.flag == 0) cout << "�Բ���û�д��飡" << endl;
	else {
		if (re.ptr->key[re.i].store >= k) {
			re.ptr->key[re.i].store -= k;
			cout << "��������飬���Ϊ��" << re.ptr->key[re.i].booknum << "������Ϊ��" << re.ptr->key[re.i].name << "������Ϊ��" << re.ptr->key[re.i].auther << endl;
			cout << "����ɹ���" << endl;
			cout << "������ִ���Ϊ��" << re.ptr->key[re.i].store << endl;
		}
		else cout << "�����治�������п��Ϊ��"<< re.ptr->key[re.i].store << endl;
		if (re.ptr->key[re.i].sign == 0) cout << "�����������棬��ɱ����" << endl;
	//	if (re.pt->key[re.i].store >= 0 && re.pt->key[re.i].sign > 0) cout << "����ɹ�" << endl;
	}
}

//�鿴���
void Inquire_book(BTree r) {
	book b;
	Result re;
	cout << "��������Ҫ��ѯ�������Ϣ(��ţ�:";
	cin >> b.booknum;
	if (b.booknum < 0) {
		cout << "�������" << endl;
		return;
	}
	re = SearchBTree(r, &b);
	if (re.flag == 0) cout << "�Բ���û�д�ͼ��" << endl;
	else cout << "��������飬���Ϊ��" << re.ptr->key[re.i].booknum << "������Ϊ��" << re.ptr->key[re.i].name << "������Ϊ��" << re.ptr->key[re.i].auther << "��������Ϊ��" << re.ptr->key[re.i].store << endl;
}
//�黹ͼ��
void Return_book(BTree *r) {
	book b;
	int k;
	Result re;
	cout << "������黹�������ţ�";
	cin >> b.booknum;
	if (b.booknum < 0) {
		cout << "�������" << endl;
		return;
	}
	cout << "������黹�ı�����";
	cin >> k;
	re = SearchBTree(*r, &b);
	if (re.flag == 0) cout << "�Բ���û�д�ͼ�飡" << endl;
	else {
		re.ptr->key[re.i].store += k;
		cout << "�黹�ɹ�!" << endl;
	}
}
//��ʾ����ͼ��
void Print_book(BTree r, int n) {
	int i = 1, j;
	if (r == NULL) return;
	while (i <= r->keynum) {
		Print_book(r->child[i - 1], n + 1);
		for (j = 0; j < n; j++) cout << "           ";
		if (n >= 0) {
			cout << "---";
			cout << "��ţ�" << r->key[i].booknum << "   " << "������" <<
				r->key[i].name << "   " << "���ߣ�" << r->key[i].auther << "   " <<
				"��棺" << r->key[i].store << endl << endl;
		}
		i++;
	}
	Print_book(r->child[i - 1], n + 1);

}
//������
void Clear_book(BTree *r) {
//	book b;
	Result re;
	KeyType b;
	cout << "��������Ҫ������������Ϣ����ţ�:";
	cin >> b.booknum;
	if (b.booknum > 0) {
		re = SearchBTree(*r, &b);
		if (re.flag == 0) cout << "û�д�ͼ��" << endl;
	/*	else {
			re.ptr->key[re.i].store = 0;
			re.ptr->key[re.i].sign = 0;
		}*/
		Delete(*r, &b);
		cout << "������ɹ�" << endl;
	}
	else {
		cout << "�������" << endl;
		return;
	}
}
