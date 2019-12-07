#include<iostream>
#include<vector>
#include <cstring>
#include"../header/Image.h"
#include"../header/net.h"

using namespace std;

void printConv(Net &net);
void printFilter(Net &net);
void printOutput(Net &net);
void printImage(Image &im);
char* createArray(string s);

int main()
{
    srand(time(NULL));
    char* cfile = createArray("./pictures/orange1.bmp");
    char* cfile2 = createArray("./pictures/apple.bmp");
    char* cfile3 = createArray("./pictures/orange2.bmp");
    char* cfile4 = createArray("./pictures/appleii.bmp");
    char* cfile5 = createArray("./pictures/apple3.bmp");

    Image orange(cfile);
    Image apple(cfile2);
    Image orange2(cfile3);
    Image apple2(cfile4);
    Image apple3(cfile5);

    delete[] cfile;
    delete[] cfile2;
    delete[] cfile3;
    delete[] cfile4;
    delete[] cfile5;

    vector<int> top = {2};
    vector<float> expected = {0, 0};
    Net net(28, 28, top, orange.width, orange.height);

    cout << "Training" << endl;
    for(unsigned int i = 0; i < 200; i++)
    {
        if(i % 10 == 0)
            cout << 200 - i << endl;
        
        int num = rand() % 2;
        if(num == 0)
        {
            expected.clear();
            expected = {0, 1};
            net.feedfoward(orange);
            if(net.getCost(expected) > 0.05)
            {
                net.backProp(expected);
                net.updateWeights();
            }

            expected.clear();
            expected = {0, 1};
            net.feedfoward(orange2);
            if(net.getCost(expected) > 0.05)
            {
                net.backProp(expected);
                net.updateWeights();
            }
        }
        else
        {
            expected.clear();
            expected = {1, 0};
            net.feedfoward(apple);
            if(net.getCost(expected) > 0.05)
            {
                net.backProp(expected);
                net.updateWeights();
            }
            expected.clear();
            expected = {1, 0};
            net.feedfoward(apple2);
            if(net.getCost(expected) > 0.05)
            {
                net.backProp(expected);
                net.updateWeights();
            }
            //printOutput(net);
        }
    }
    //printConv(net);

    cout << "Orange1 Expected: 0, 1" << endl;
    net.feedfoward(orange);
    printOutput(net);

    cout << "Apple1 Expected: 1, 0" << endl;
    net.feedfoward(apple);
    printOutput(net);

    cout << "Orange2 Expected: 0, 1" << endl;
    net.feedfoward(orange2);
    printOutput(net);

    cout << "Apple2 Expected: 1, 0" << endl;
    net.feedfoward(apple2);
    printOutput(net);
    
    cout << "Apple3 Expected: 1, 0" << endl;
    net.feedfoward(apple3);
    printOutput(net);
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
    for(unsigned int j = 0; j < net.getFLayers()[net.getFLayers().size() - 1].size(); j++)
    {
        cout << net.getFLayers()[net.getFLayers().size() - 1][j].getOutput() << " ";
    }
    cout << "\n" << endl;
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
