#ifndef MODULE_H
#define MODULE_H

#include <vector>
#include <string>
using namespace std;

class Terminal
{
public:
    // constructor and destructor
    Terminal(string& name, size_t x, size_t y) :
        _name(name), _x1(x), _y1(y), _x2(x), _y2(y) { }
    ~Terminal() { }

    // basic access methods
    const string getName() { return _name; }
    const size_t getX1() { return _x1; }
    const size_t getX2() { return _x2; }
    const size_t getY1() { return _y1; }
    const size_t getY2() { return _y2; }

    // set functions
    void setName(string& name) { _name = name; }
    void setPos(size_t x1, size_t y1, size_t x2, size_t y2) {
        _x1 = x1;   _y1 = y1;
        _x2 = x2;   _y2 = y2;
    }

protected:
    string      _name;      // module name
    int         _x1;        // min x coordinate of the terminal
    int         _y1;        // min y coordinate of the terminal
    int         _x2;        // max x coordinate of the terminal
    int         _y2;        // max y coordinate of the terminal

};


class Block : public Terminal
{
public:
    // constructor and destructor
    Block(string& name, size_t w, size_t h,bool r) :
        Terminal(name, 0, 0), _w(w), _h(h), _r(r) { }
    ~Block() { }


    // basic access methods
    const size_t getWidth(bool rotate = false) { return rotate ? _h : _w; }
    const size_t getHeight(bool rotate = false) { return rotate ? _w : _h; }
    const size_t getArea() { return _h * _w; }
    static size_t getMaxX() { return _maxX; }
    static size_t getMaxY() { return _maxY; }
    void setR(bool r) { _r = r; }
    bool getR() { return _r; }
    void dorotate() { swap(_w, _h); _r = !_r; }
    // set functions
    void setWidth(size_t w) { _w = w; }
    void setHeight(size_t h) { _h = h; }

    static void setMaxX(size_t x) { _maxX = x; }
    static void setMaxY(size_t y) { _maxY = y; }

private:

    int          _w;         // width of the block
    int         _h;         // height of the block
    static size_t   _maxX;      // maximum x coordinate for all blocks
    static size_t   _maxY;      // maximum y coordinate for all blocks
    bool        _r;
};


class Net
{
public:
    // constructor and destructor
    Net() { _degree = 0; }
    ~Net() { }

    // basic access methods
    const vector<Block*> getTermList() { return _termList; }

    // modify methods
    void addTerm(Block* term) { _termList.push_back(term); }
    void setDegree(int degree) { _degree = degree; }
    // other member functions
    double calcHPWL();
    int getDegree() { return _degree; }
private:
    vector<Block*>   _termList;  // list of terminals the net is connected to
    int _degree;


};

#endif  // MODULE_H
#pragma once
