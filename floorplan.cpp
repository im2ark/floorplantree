#include "floorplan.h"
#include <algorithm>
using namespace std;
#include<iostream>
TreeNode::TreeNode(Block* block) {
	_left = NULL;
	_right = NULL;
	_parent = NULL;
	_block = block;
}
Tree::Tree(vector<Block*> block_vec, double W, double H) {
	TreeNode* head = new TreeNode(block_vec[0]);
	_headnode = head;

	_contour.resize(W);
	_width = W;
	_height = H;
	_treeW = 0;
	_treeH = 0;
	head->getBlock()->setPos(0, 0,
		head->getBlock()->getWidth(), head->getBlock()->getHeight());
	_contour.resize(W);
	//cout << head->getBlock()->getName() << " " << head->getBlock()->getX1() << " " << head->getBlock()->getY1() << " " << head->getBlock()->getX2() << " " << head->getBlock()->getY2() << endl;
	for (int i = 0; i < head->getBlock()->getWidth(); ++i) {
		_contour[i] = head->getBlock()->getHeight();
	}
	TreeNode* tmp_head = new TreeNode();
	tmp_head = head;
	for (int i = 1; i < block_vec.size(); ++i) {
		TreeNode* cur = new TreeNode(block_vec[i]);
		int localmaxy = -1;
		if (block_vec[i - 1]->getX2() + block_vec[i]->getWidth() > W) {
			//TreeNode* cur = new TreeNode(block_vec[i]);
			cur->setParent(tmp_head);
			for (int j = tmp_head->getBlock()->getX1(); j < tmp_head->getBlock()->getX1() + block_vec[i]->getWidth(); ++j) {
				if (_contour[j] > localmaxy) localmaxy = _contour[j];
			}
			for (int j = tmp_head->getBlock()->getX1(); j < tmp_head->getBlock()->getX1() + block_vec[i]->getWidth(); ++j) {
				_contour[j] = localmaxy + block_vec[i]->getHeight();
			}
			cur->getBlock()->setPos(0, localmaxy, cur->getBlock()->getWidth(), localmaxy + cur->getBlock()->getHeight());
		//	cout << cur->getBlock()->getName() << " " << cur->getBlock()->getX1() << " " << cur->getBlock()->getY1() << " " << cur->getBlock()->getX2() << " " << cur->getBlock()->getY2() << endl;
			tmp_head->setRight(cur);
			cur->setParent(tmp_head);
			tmp_head = cur;

		}
		else {
			TreeNode* cur1 = tmp_head;
			while (cur1->getLeft() != NULL) { cur1 = cur1->getLeft(); }
			for (int j = cur1->getBlock()->getX2(); j < cur1->getBlock()->getX2() + cur->getBlock()->getWidth(); ++j) {
				if (_contour[j] > localmaxy) localmaxy = _contour[j];
			}
			for (int j = cur1->getBlock()->getX2(); j < cur1->getBlock()->getX2() + cur->getBlock()->getWidth(); ++j) {
				_contour[j] = localmaxy + block_vec[i]->getHeight();
			}
			cur->getBlock()->setPos(cur1->getBlock()->getX2(), localmaxy, cur1->getBlock()->getX2() + cur->getBlock()->getWidth(), localmaxy + cur->getBlock()->getHeight());
			//TreeNode* cur = new TreeNode(block_vec[i]);
			cur1->setLeft(cur);
			cur->setParent(cur1);
			//cout << cur->getBlock()->getName() << " " << cur->getBlock()->getX1() << " " << cur->getBlock()->getY1() << " " << cur->getBlock()->getX2() << " " << cur->getBlock()->getY2() << endl;
		}

		if (_treeW < cur->getBlock()->getX2()) _treeW = cur->getBlock()->getX2();
		if (_treeH < cur->getBlock()->getY2()) _treeH = cur->getBlock()->getY2();
	}

}
void Tree::traverse1(TreeNode* node) {
	//cout << node << endl;
	if (node->getLeft() != NULL) traverse1(node->getLeft());
	//cout << "left null" << endl;
	if (node->getRight() != NULL) traverse1(node->getRight());
	//cout << "right null" << endl;
}
string Tree::delandins(string b1,string b2) {
	TreeNode* del = _node_map[b1];
	TreeNode* ins = _node_map[b2];
	string str = del->getBlock()->getName();
	TreeNode* move = new TreeNode(new Block(str, del->getBlock()->getWidth(), del->getBlock()->getHeight(), del->getBlock()->getR()));
	TreeNode* tmp = del;
	TreeNode* inschild = NULL;
	if (del->getParent() == ins) return "no";
	if (rand() % 2 == 0) {
		if (ins->getLeft() != NULL) inschild = ins->getLeft();
		move->setParent(ins);
		ins->setLeft(move);
		if (inschild != NULL) {
			if (rand() % 2 == 0) move->setLeft(inschild);
			else move->setRight(inschild);
			inschild->setParent(move);
		}
	}
	else {
		if (ins->getRight() != NULL) inschild = ins->getRight();
		move->setParent(ins);
		ins->setRight(move);
		if (inschild != NULL) {
			if (rand() % 2 == 0) move->setLeft(inschild);
			else move->setRight(inschild);
			inschild->setParent(move);
		}
	}
	//cout << " in = " << ins << endl;
	//cout << " delete = " << del << endl;
	if (del->getLeft() == NULL && del->getRight() == NULL) {
		if (del->getParent()->getLeft() != NULL && del->getParent()->getLeft()->getBlock()->getName() == del->getBlock()->getName()) {
			del->getParent()->setLeft(NULL);
		}
		else del->getParent()->setRight(NULL);
		del = NULL;
		delete del;
	}
	else {
		while (1) {
			if (tmp->getLeft() != NULL) { 
				tmp->getLeft()->setParent(tmp);
				tmp = tmp->getLeft(); 
				//cout << "tmp = " << tmp << endl;
			}
			else if (tmp->getRight() != NULL) { 
				tmp->getRight()->setParent(tmp);
				tmp = tmp->getRight();
				//cout << "tmp = " << tmp << endl;
			}
			else {
				TreeNode var = *del;
				string str = tmp->getBlock()->getName();
				Block* xd = new Block(str, tmp->getBlock()->getWidth(), tmp->getBlock()->getHeight(), tmp->getBlock()->getR());
				del->setBlock(xd);
				tmp->setBlock(var.getBlock());

				//cout << " tmp parent = " << tmp->getParent() << endl;
				if (tmp->getParent()->getLeft() != NULL && tmp->getParent()->getLeft()->getBlock()->getName() == tmp->getBlock()->getName()) {
					tmp->getParent()->setLeft(NULL);
				}
				else tmp->getParent()->setRight(NULL);
				tmp = NULL;
				delete tmp;
				break;
			}
		}
	}
	return "yes";
}
void Tree::rotate(string x) {
	TreeNode* tmp = _node_map[x];
	tmp->getBlock()->dorotate();
}
void Tree::swapSubTree(string x) {
	TreeNode* cur = _node_map[x];
	if (cur->getParent() != NULL) {
		while ((cur->getLeft() == NULL || cur->getRight() == NULL)) {
			cur = cur->getParent();

			if (cur->getParent() == NULL) break;
		}
	}
	if (cur == _headnode && (cur->getLeft() == NULL || cur->getRight() == NULL)) return;
	TreeNode* tmp = cur->getLeft();
	cur->setLeft(cur->getRight());
	cur->setRight(tmp);
	//cout << "cur->setLeft = " << cur->getLeft() << endl;
	//cout << "cur->setRight = " << cur->getRight() << endl;
}

void Tree::build() {
	_treeW = 0;
	_treeH = 0;
	_contour.clear();
	_contour.resize(_width);
	DFS(_headnode, 0);

}
void Tree::exchage(string b1, string b2) {
	TreeNode* a = _node_map[b1];
	TreeNode* b = _node_map[b2];
	TreeNode tmp = *b;
	b->setBlock(a->getBlock());
	a->setBlock(tmp.getBlock());

	//cout << "b = " << b->getBlock()->getHeight() << endl;
	//build();
}
void Tree::DFS(TreeNode* node, int x) {
	if (node == NULL) return;
	Block* block = node->getBlock();
	//cout << block->getName() << endl;
	int localmaxy = -1;
	if (x + block->getWidth() > _contour.size()) { _contour.resize(_contour.size() * 2); }
	for (int i = x; i < x + block->getWidth(); ++i) {
		if (_contour[i] > localmaxy) localmaxy = _contour[i];
	}
	block->setPos(x, localmaxy, x + block->getWidth(), localmaxy + block->getHeight());
	for (int i = x; i < x + block->getWidth(); ++i) {
		_contour[i] = localmaxy + block->getHeight();
	}
	if (block->getX2() > _treeW) _treeW = block->getX2();
	if (block->getY2() > _treeH) _treeH = block->getY2();
	if (node->getLeft() != NULL) DFS(node->getLeft(), x + block->getWidth());
	if (node->getRight() != NULL) DFS(node->getRight(), x);
	_node_map[block->getName()] = node;
}