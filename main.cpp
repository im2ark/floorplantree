#include <iostream>
#include <fstream>
#include <vector>
#include<unordered_map>
#include<map>
#include<cmath>
#include "module.h"
#include "floorplan.h"
using namespace std;

//global variable
int W, H = 0;
double norm_area ;
double norm_hpwl ;
int num;
std::unordered_map  <string, Block* > block_map;
std::unordered_map  <string, Block* > term_map;
vector<Block*> block_vec;
std::unordered_map  <int, Net* > net_map;  //index 0 is null
int blockSize, terminalSize, netSize = 0;
void input_parser(fstream& input_blk, fstream& input_net) {
    string str, tmp1, tmp2;
    int i = 0;
    while (input_blk >> str) {
        if (str == "Outline:") {
            input_blk >> tmp1;
            input_blk >> tmp2;
            W = stoi(tmp1);
            H = stoi(tmp2);

        }
        else if (str == "NumBlocks:") {
            input_blk >> str;
            blockSize = stoi(str);
        }
        else if (str == "NumTerminals:") {
            input_blk >> str;
            terminalSize = stoi(str);
        }
        else {
            input_blk >> tmp1;
            if (tmp1 == "terminal") {
                input_blk >> tmp1;
                input_blk >> tmp2;
                term_map[str] = new Block(str, stoi(tmp1), stoi(tmp2),0);
            }
            else {
                input_blk >> tmp2;
                block_map[str] = new Block(str, stoi(tmp1), stoi(tmp2),0);


            }
        }

    }
    int v = 1;
    while (input_net >> str) {
        
        if (str == "NumNets:") { input_net >> str; netSize = stoi(str); }
        if (str == "NetDegree:") {
            Net* tmp = new Net();
            input_net >> str;
            tmp->setDegree(stoi(str));

            for (int j = 0; j < tmp->getDegree(); j++) {
                input_net >> str;
                if (block_map.find(str) != block_map.end()) tmp->addTerm(block_map[str]);
                else if (term_map.find(str) != term_map.end()) tmp->addTerm(term_map[str]);
            }
            net_map[v] = tmp;
            v++;
        }

    }

}
double HPWL() {
    double ans = 0;
    for (unordered_map<int, Net*>::iterator iter = net_map.begin(); iter != net_map.end(); iter++) {
        Net* eachnet = iter->second;
        double x_min = 0;
        double x_max = 0;
        double y_min = 0;
        double y_max = 0;
        for (int i = 0; i < eachnet->getDegree(); i++) {
            Block* eachblock = NULL;
            if (block_map.find(eachnet->getTermList()[i]->getName()) != block_map.end()) eachblock = block_map.find(eachnet->getTermList()[i]->getName())->second;
            else { eachblock = term_map.find(eachnet->getTermList()[i]->getName())->second; }
            if (i == 0) {
                x_min, x_max = (eachblock->getX1() + eachblock->getX2()) / 2;
                y_min, y_max = (eachblock->getY1() + eachblock->getY2()) / 2;
            }
            else {
                if (x_min > (eachblock->getX1() + eachblock->getX2()) / 2) x_min = (eachblock->getX1() + eachblock->getX2()) / 2;
                if (x_max < (eachblock->getX1() + eachblock->getX2()) / 2) x_max = (eachblock->getX1() + eachblock->getX2()) / 2;
                if (y_min > (eachblock->getY1() + eachblock->getY2()) / 2) y_min = (eachblock->getY1() + eachblock->getY2()) / 2;
                if (y_max < (eachblock->getY1() + eachblock->getY2()) / 2) y_max = (eachblock->getY1() + eachblock->getY2()) / 2;
            }
        }
        ans += (x_max - x_min) + (y_max - y_min);
    }
    return ans;
}
void traverse(TreeNode* node) {
    num++;
    cout  << node->getBlock()->getX2() << " " <<node->getBlock()->getY2() << endl;
    if (node->getLeft() != NULL) traverse(node->getLeft());

    if (node->getRight() != NULL) traverse(node->getRight());

}
bool comp(Block* b1, Block* b2) {
    return b1->getArea() > b2->getArea();
}
void clear(TreeNode* headnode) {
    if (headnode == NULL) return;
    if (headnode->getLeft() != NULL) clear(headnode->getLeft());
    if (headnode->getRight() != NULL) clear(headnode->getRight());
    delete headnode;
}
void backtobest(Tree* tree, TreeNode* besttree) {
   
    if(besttree!=tree->getHeadnode()) clear(tree->getHeadnode());
    
    tree->setHeadnode(besttree);
    tree->build();
}
void setblockvec(TreeNode* headnode) {
    if (headnode == NULL) return;
    if (headnode->getLeft() != NULL) setblockvec(headnode->getLeft());
    if (headnode->getRight() != NULL) setblockvec(headnode->getRight());
    block_map[headnode->getBlock()->getName()] = headnode->getBlock();
   
}
TreeNode* record(TreeNode* headnode) {
    string str = headnode->getBlock()->getName();
    TreeNode* xd = new TreeNode(new Block(str, headnode->getBlock()->getWidth(),
        headnode->getBlock()->getHeight(), headnode->getBlock()->getR()));
    
    if (headnode->getLeft() != NULL) {
        xd->setLeft(record(headnode->getLeft()));
        xd->getLeft()->setParent(xd);
    }
    if (headnode->getRight() != NULL)
    {
        xd->setRight(record(headnode->getRight()));
        xd->getRight()->setParent(xd);
    }
    return xd;
}
TreeNode* realback(TreeNode* node) {
    string str = node->getBlock()->getName();
    TreeNode* xd = new TreeNode(new Block(str, node->getBlock()->getWidth(),
        node->getBlock()->getHeight(), node->getBlock()->getR()));
    if (node->getLeft() != NULL) {
        TreeNode* xd1 = new TreeNode(node->getLeft()->getBlock());
        xd->setLeft(xd1);
        xd1->setParent(xd);
        realback(node->getLeft());
    }
    if (node->getRight() != NULL)
    {
        TreeNode* xd2 = new TreeNode(node->getRight()->getBlock());
        xd->setRight(xd2);
        xd2->setParent(xd);
        realback(node->getRight());
    }
    return xd;;

}

void updatebest(Tree* tree, TreeNode* besttree) {
    if(besttree!=tree->getHeadnode()) clear(besttree);
    besttree = realback(tree->getHeadnode());
}

double cost(double a, Tree* tree, double norm_area, double norm_hpwl) {
    double d = 0.4;
    double b = 0.2;
    double c = 0.4;
    
    return d * tree->getTreeHeight() * tree->getTreeWidth()/ norm_area + b * HPWL() / norm_hpwl + c * abs((tree->getTreeWidth() / tree->getTreeHeight()) - W/H);
}
void back(Tree* tree, int a, int b, int c) {
    if (a == 0) {
        tree->exchage(block_vec[b]->getName(), block_vec[c]->getName());
    }
    else if (a == 1) {
        tree->rotate(block_vec[b]->getName());
    }
    else if (a == 2) {
        tree->swapSubTree(block_vec[b]->getName());
    }
   

}
void find_neighbor(Tree* tree,int &d,int &b,int& c) {
    int random=rand();
    int b1 = 0;
    int b2 = 0;
    int i = 0;
    string xd = "";
    if (random%4==0) {
        b1 = rand() % block_map.size();
        b2 = rand() % block_map.size();
        while (b2 == b1) b2 = rand() % block_map.size();
       
        tree->exchage(block_vec[b1]->getName(), block_vec[b2]->getName());

        i = 0;
    }
    else if (random%4==1) {
        b1 = rand() % block_map.size();
        
        tree->rotate(block_vec[b1]->getName());

        i = 1;
    }
    else if (random%4==2) {
        b1 = rand() % block_map.size();
       
        tree->swapSubTree(block_vec[b1]->getName());

        i = 2;
    }
    else {
        b1 = rand() % block_map.size();
        b2 = rand() % block_map.size();
        while (b2 == b1) b2 = rand() % block_map.size();
        xd = tree->delandins(block_vec[b1]->getName(), block_vec[b2]->getName());
         i = 3;
        
    }
    if (xd != "no") {
      
        tree->build();

        setblockvec(tree->getHeadnode());
    }
    d = i;
    b = b1;
    c = b2;
    
}

void SA(Tree* tree, double a) {
    int iter = 0;
    int d = 0;
    int b = 0;
    int c = 0;
    int iterxd = 0;
   do {
       iterxd++;
       cout << "iter = " << iterxd << endl;
        TreeNode* besttree = record(tree->getHeadnode());
        
        TreeNode* legaltreenode=new TreeNode();
        legaltreenode = NULL;
        norm_area = tree->getTreeHeight() * tree->getTreeWidth();
        setblockvec(tree->getHeadnode());
        norm_hpwl = HPWL();
        
        double cost1 = cost(a, tree, norm_area, norm_hpwl);
        double cost3 = 100000000000;
        if (tree->getTreeHeight() < H && tree->getTreeWidth() < W && cost1 < cost3) {
            legaltreenode = record(tree->getHeadnode());
            cost3 = cost1;
        }
        double bestcost = cost1;
        int badnum = 0;
        
        for (double T = 100; T > 1; T *= 0.9) {
            int iteration;
            badnum = 0;
        //int base=0;
         if (T > 50)	{iteration = 200*block_map.size();}
            else if (T > 20)	{iteration = 400*block_map.size();}
            else			{iteration = 200*block_map.size();}

            for (int i = 0; i < iteration; ++i) {
                
                TreeNode* oldhead = record(tree->getHeadnode());
                 find_neighbor(tree,d,b,c);
                // cout << "d = " << d << endl;
               
                double cost2 = cost(a, tree, norm_area, norm_hpwl);
               
                if (tree->getTreeHeight() < H && tree->getTreeWidth() < W && cost2 < cost3) {
                    //cout << "legal" << endl;
                    if(legaltreenode!=NULL) clear(legaltreenode);
                    legaltreenode = record(tree->getHeadnode());
                    
                    cost3 = cost2;
                }
                
                if (cost1 > cost2) {
                    badnum = 0;
                    cost1 = cost2;
                    if (cost1 < bestcost) {
                      //  cout << "updatebest" << endl;
                       
                        updatebest(tree, besttree);
                        besttree = record(tree->getHeadnode()); 
                    }
                }
                
                else {
                    
                    if (exp(-(cost2 - cost1)) / T < 0.97) {
                        if (d == 3) {
                            besttree = record(besttree);
                            backtobest(tree, oldhead);
                        }
                        else { back(tree, d, b, c); }
                         badnum++;
                       
                    }
                    else {
                    
                        cost1 = cost2;
                    }
                    }
                if (badnum == 30*block_map.size()) {
                    //cout << "too suck" << endl;
                    if (besttree == tree->getHeadnode()) break;
                    else {  backtobest(tree, besttree); }
                    break;
                }

            }
        }
      
        backtobest(tree, besttree);
        if ((tree->getTreeHeight() > H || tree->getTreeWidth() > W) && legaltreenode != NULL) {
            cout << "-------------1" << endl;
            traverse(legaltreenode);
            cout << "-------------1" << endl;

            backtobest(tree, legaltreenode);
           
            setblockvec(tree->getHeadnode());
            
        }
      //  cout << "---------" << endl;
        setblockvec(tree->getHeadnode());
      //  cout << "---------" << endl;
        iter++;
        
    } while ((tree->getTreeWidth() > W) || (tree->getTreeHeight() > H));
   // traverse(tree->getHeadnode());
    cout << "tree H" << tree->getTreeHeight() << endl;
    cout << "tree W" << tree->getTreeWidth() << endl;
    cout << "W = " << W << endl;
    cout << "H = " << H << endl;
    

}


void report(fstream& output, double alpha, Tree* tree,double norm_area,double norm_HPWL) {
    output << alpha * tree->getTreeWidth() * tree->getTreeHeight() / norm_area + (1 - alpha) * HPWL() / norm_HPWL << endl;
    output << HPWL() << endl;
    output << tree->getTreeWidth() * tree->getTreeHeight() << endl;
     output << tree->getTreeWidth() <<" "<<tree->getTreeHeight()<<endl;
    
    output << (double)clock() / CLOCKS_PER_SEC << endl;
    for (unordered_map<string, Block*>::iterator iter = block_map.begin(); iter != block_map.end(); iter++) {
        output << iter->first << " " << iter->second->getX1() << " " << iter->second->getY1()
            << " " << iter->second->getX2() << " " << iter->second->getY2() << endl;
    }
}
int main(int argc, char** argv)
{
    fstream input_blk, input_net, output;
    double alpha;
    std::cout << argc << endl;
    if (argc == 5) {

        alpha = stod(argv[1]);
        input_blk.open(argv[2], ios::in);
        input_net.open(argv[3], ios::in);
        output.open(argv[4], ios::out);

        if (!input_blk) {
            cerr << "Cannot open the input file \"" << argv[2]
                << "\". The program will be terminated..." << endl;
            exit(1);
        }
        if (!input_net) {
            cerr << "Cannot open the input file \"" << argv[3]
                << "\". The program will be terminated..." << endl;
            exit(1);
        }
        if (!output) {
            cerr << "Cannot open the output file \"" << argv[4]
                << "\". The program will be terminated..." << endl;
            exit(1);
        }
    }
    else {
        cerr << "Usage: ./Floorplanner <alpha> <input block file> " <<
            "<input net file> <output file>" << endl;
        exit(1);
    }
    block_map.clear();
    input_parser(input_blk, input_net);

    for (unordered_map<string, Block*>::iterator iter = block_map.begin(); iter != block_map.end(); iter++) {
        block_vec.push_back(iter->second);


    }
    std::sort(block_vec.begin(), block_vec.end(), comp);
    Tree* tree = new Tree(block_vec, W, H);
    tree->build();
    SA(tree, alpha);
    report(output, alpha, tree,norm_area,norm_hpwl);
    std::cout << (double)clock() / CLOCKS_PER_SEC << endl;
    std::cout << "norm_area = " << norm_area << endl;
    std::cout << "norm_hpwl = " << norm_hpwl << endl;
    return 0;
}
