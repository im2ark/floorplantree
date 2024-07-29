#include "module.h"
#include<unordered_map>
#include <algorithm>
class TreeNode {
public:
	TreeNode(Block*);
	TreeNode() { _block = NULL; _parent = NULL; _left = NULL; _right = NULL; }
	void setLeft(TreeNode* left) {
		_left = left;
	}
	void setRight(TreeNode* right) {
		_right = right;
	}
	void setParent(TreeNode* parent) {
		_parent = parent;
	}

	void setBlock(Block* block) {
		_block = block;
	}
	Block* getBlock() { return _block; }
	TreeNode* getParent() { return _parent; }
	TreeNode* getLeft() { return _left; }
	TreeNode* getRight() { return _right; }
private:
	TreeNode* _left;
	TreeNode* _right;
	TreeNode* _parent;
	Block* _block;
};
class Tree {
public:
	Tree();
	Tree(vector<Block*>, double, double);
	string delandins(string,string);
	void traverse1(TreeNode*);
	void rotate(string);
	void setContour(int i, int h) { _contour[i] = h; }
	void swapSubTree(string);
	void setHeadnode(TreeNode* headnode) { _headnode = headnode; }
	void setHeight(int height) { _height = height; }
	void setWidth(int width) { _width = width; }
	void setTreeWidth(int width) { _treeW = width; }
	void setTreeHeight(int height) { _treeH = height; }
	void setNodeMap(unordered_map <string, TreeNode*> node_map) { _node_map = node_map; }
	void build();
	void exchage(string, string);
	void DFS(TreeNode*, int);
	unordered_map <string, TreeNode*> getNodeMap() { return _node_map; }
	vector<int> getContour() { return _contour; }
	TreeNode* getHeadnode() { return _headnode; }
	int getHeight() { return _height; }
	int getWidth() { return _width; }
	int getTreeHeight() { return _treeH; }
	int getTreeWidth() { return _treeW; }
private:
	vector<int> _contour;
	TreeNode* _headnode;
	int _height;
	int _width;
	int _treeW;
	int _treeH;
	unordered_map <string, TreeNode*> _node_map;
};


#pragma once
