#include<iostream>
#include<vector>
#include <cstring>
#include"Image.h"
#include"net.h"

using namespace std;

void printConv(Net &net);
void printFilter(Net &net);
void printOutput(Net &net);
void printImage(Image &im);
char* createArray(string s);

int main()
{
    char* cfile = createArray("./fotos/orange1.bmp");
    char* cfile2 = createArray("./fotos/apple.bmp");
    char* cfile3 = createArray("./fotos/orange2.bmp");
    char* cfile4 = createArray("./fotos/appleii.bmp");
    //char* cfile5 = createArray("./fotos/banana.bmp");

    Image X(cfile);
    Image O(cfile2);
    Image orange2(cfile3);
    Image apple2(cfile4);
    //Image banana(cfile5);

    delete[] cfile;
    delete[] cfile2;
    delete[] cfile3;
    delete[] cfile4;
    //delete[] cfile5;

    vector<int> top = {2};
    vector<float> expected = {0, 0};
    Net net(28, 28, top, X.width, O.height);

    
    //printConv(net);

    /*net.feedfoward(orange);
    printOutput(net);

    net.feedfoward(apple);
    printOutput(net);*/

    cout << "Training" << endl;
    for(unsigned int i = 0; i < 100; i++)
    {
        expected.clear();
        expected = {0, 1};
        net.feedfoward(X);
        net.backProp(expected);
        net.updateWeights();

        expected.clear();
        expected = {1, 0};
        net.feedfoward(O);
        net.backProp(expected);
        net.updateWeights();
    }
    //printConv(net);

    net.feedfoward(X);
    printOutput(net);

    net.feedfoward(O);
    printOutput(net);

    net.feedfoward(orange2);
    printOutput(net);

    net.feedfoward(apple2);
    printOutput(net);

    //printOutput(net);

    /*net.feedfoward(banana);
    printOutput(net);*/
    cout << endl;
    return 0;
}

void printFilter(Net &net)
{
    for(unsigned int i = 0; i < net.getFilters().size(); i++)
    {
        cout << "Filter " << i << endl;
        for(unsigned int j = 0; j < net.getFilters()[i].size(); j++)
        {
            for(unsigned int k = 0; k < net.getFilters()[i][j].size(); k++)
            {
                cout << net.getFilters()[i][j][k] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
}

void printOutput(Net &net)
{
    cout << "Output " << endl;
    for(unsigned int i = 0; i < net.getFLayers().size(); i++)
    {
        for(unsigned int j = 0; j < net.getFLayers()[i].size(); j++)
        {
            cout << net.getFLayers()[i][j].getOutput() << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void printConv(Net &net)
{
    for(unsigned int i = 0; i < net.getcLayer().size(); i++)
    {
        cout << "Convo " << i << endl;
        for(unsigned int j = 0; j < net.getcLayer()[i].size(); j++)
        {
            for(unsigned int k = 0; k < net.getcLayer()[i][j].size(); k++)
            {
                cout << net.getcLayer()[i][j][k].getOutput() << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
}

void printImage(Image &im)
{
    for(unsigned int i = 0; i < im.colors.size(); i++)
    {
        for(unsigned int j = 0; j < im.colors[i].size(); j++)
        {
            cout << (int)im.colors[i][j].red << " ";
        }
        cout << endl;
    }
    //cout << im.colors[im.colors.size() - 1][im.colors.size() - 1].red << endl;
}

char* createArray(string s)
{
    int size = s.length();
    char* cfile = new char[size + 1];
    strcpy(cfile, s.c_str());
    return cfile;
}