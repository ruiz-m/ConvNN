#pragma once
#include<iostream>
#include<vector>
#include<math.h>
#include<ctime>
#include"node.h"
#include"Image.h"

using namespace std;

typedef vector<vector<vector<float>>> filterSet;

class Net
{
private:
    vector<vector<vector<node>>> cLayer;
    vector<vector<node>> fLayers;
    filterSet filters;
    int fwidth;
    int fheight;
    float cost;
public:
    Image im;
    Net(int filterRow, int filterCol, vector<int> &topology, int width, int height);
    void feedfoward(Image &im);
    void convLayer();
    void fullyCon();
    filterSet& getFilters();
    vector<vector<vector<node>>>& getcLayer();
    vector<vector<node>>& getFLayers();
    void getOutput(vector<float> &expected);
    void softMax();
    void backProp(vector<float> &expected);
    void updateWeights();
    void restart();
};

Net::Net(int filterRow, int filterCol, vector<int> &topology, int width, int height)
{
    srand(time(NULL));
    fwidth = filterCol;
    fheight = filterRow;
    filters.resize(3);
    for(int i = 0; i < 3; i++)
    {
        filters[i].resize(fwidth);
        for(int j = 0; j < filterRow; j++)
        {
            filters[i][j].resize(fheight);
            for(int k = 0; k < filterCol; k++)
                filters[i][j][k] = 2 * ((double) rand() / (RAND_MAX)) - 1;
        }
    }

    int w = 0;
    cLayer.resize(3);
    for(unsigned int i = 0; i < 3; i++)
    {
        cLayer[i].resize(height - fheight + 1);
        for(unsigned int j = 0; j < height - fheight + 1; j++)
        {
            for(unsigned int k = 0; k < width - fwidth + 1; k++)
            {
                cLayer[i][j].push_back(node(w));
                w++;
            }
        }  
    }

    

    fLayers.resize(topology.size());
    for(unsigned int i = 0; i < topology[0]; i++)
        fLayers[0].push_back(node(3 * (height - fheight + 1) * (width - fwidth + 1), i));
    
    for(unsigned int i = 1; i < topology.size(); i++)
        for(unsigned int j = 0; j < topology[i]; j++)
            fLayers[i].push_back(node(topology[i - 1], i));
    
}

void Net::feedfoward(Image &im)
{   
    this->im = im;
    ////cout << "Feed" << endl;
    convLayer();
    fullyCon();
    softMax();
}

void Net::convLayer()
{
    ////cout << "Convolutional Layer" << endl;
    float output = 0.0f;
    float output1 = 0.0f;
    float output2 = 0.0f;
    
    for(unsigned int i = 0; i < cLayer[0].size(); i++)
    {
        for(unsigned int j = 0; j < cLayer[0][i].size(); j++)
        {
            for(unsigned int k = 0; k < fheight; k++)
            {
                for(unsigned int w = 0; w < fwidth; w++)
                {
                    output += filters[0][k][w] * im.colors[i + k][j + w].red;
                    output1 += filters[1][k][w] * im.colors[i + k][j + w].green;
                    output2 += filters[2][k][w] * im.colors[i + k][j + w].blue;
                }
            }
            /*if(i == 0 && j == 0)
            {
                cout << "OUTPUT" << endl;
                cout << output << endl;
            }*/
            cLayer[0][i][j].setOutput(output + cLayer[0][i][j].getBias());
            cLayer[1][i][j].setOutput(output1 + cLayer[1][i][j].getBias());
            cLayer[2][i][j].setOutput(output2 + cLayer[2][i][j].getBias());
            output = 0.0f;
            output1 = 0.0f;
            output2 = 0.0f;
        }
    }
}

void Net::fullyCon()
{
    ////cout << "Fully Connected Layer" << endl;
    for(unsigned int j = 0; j < fLayers[0].size(); j++)
        fLayers[0][j].feedc(cLayer);

    ////cout << "Fully Connected Layer2" << endl;
    for(unsigned int i = 1; i < fLayers.size(); i++)
        for(unsigned int j = 0; j < fLayers[i].size(); j++)
            fLayers[i][j].feed(fLayers[i - 1]);
}

void Net::softMax()
{
    float sum = 0.0f;
    for(unsigned int i = 0; i < fLayers[fLayers.size() - 1].size(); i++)
    {
        sum += exp(-1 * fLayers[fLayers.size() - 1][i].getOutput());
        ////cout << sum << endl;
    }
    
    for(unsigned int i = 0; i < fLayers[fLayers.size() - 1].size(); i++)
    {
        if(fLayers[fLayers.size() - 1][i].softMax(sum))
        {
            restart();
        }
    }
}

filterSet& Net::getFilters()
{
    return filters;
}

vector<vector<vector<node>>>& Net::getcLayer()
{
    return cLayer;
}

vector<vector<node>>& Net::getFLayers()
{
    return fLayers;
}

void Net::getOutput(vector<float> &expected)
{
    float sum = 0.0f;
    for(unsigned int i = 0; i < fLayers[fLayers.size() - 1].size(); i++)
        sum += (expected[i] - fLayers[fLayers.size() - 1][i].getOutput()) * (expected[i] - fLayers[fLayers.size() - 1][i].getOutput());
    cost = sum / (expected.size());
    ////cout << "Cost: " << cost << endl;
}

void Net::backProp(vector<float> &expected)
{
    //////cout << "Back Prop" << endl;
    for(unsigned int i = 0; i < fLayers[fLayers.size() - 1].size(); i++)
        fLayers[fLayers.size() - 1][i].backOut(expected[i], expected.size());
    
    ////cout << "Back Prop1" << endl;
    ////cout << fLayers.size() - 2 << endl;
    for(int i = fLayers.size() - 2; i >= 0; i--)
        for(unsigned int j = 0; j < fLayers[i].size(); j++)
            fLayers[i][j].back(fLayers[i + 1]);

    //Back Prop for the convo layer
    ////cout << "Back Prop2" << endl;
    for(unsigned int i = 0; i < cLayer.size(); i++)
        for(unsigned int j = 0; j < cLayer[i].size(); j++)
            for(unsigned int k = 0; k < cLayer[i][j].size(); k++)
                cLayer[i][j][k].back(fLayers[0]);
}

void Net::updateWeights()
{
    //cout << "Update Weights" << endl;
    for(unsigned int i = 0; i < fLayers[0].size(); i++)
        fLayers[0][i].updateWeights(cLayer);
    
    //cout << "Update Weights1" << endl;
    for(unsigned int i = 1; i < fLayers.size(); i++)
        for(unsigned int j = 0; j < fLayers[i].size(); j++)
            fLayers[i][j].updateWeights(fLayers[i - 1]);
    
    //cout << "Update Weights2" << endl;
    for(unsigned int i = 0; i < fheight; i++)
    {
        for(unsigned int j = 0; j < fwidth; j++)
        {
            for(unsigned int k = 0; k < cLayer[0].size(); k++)
            {
                ////cout << cLayer[0][i].size() << endl;
                for(unsigned int w = 0; w < cLayer[0][k].size(); w++)
                {
                    ////cout << i << " " << j << " " << k << " " << w << endl;
                    float dCdw1 = im.colors[k + i][w + j].red * cLayer[0][k][w].getNabla();
                    float dCdw2 = im.colors[k + i][w + j].green * cLayer[1][k][w].getNabla();
                    float dCdw3 = im.colors[k + i][w + j].blue * cLayer[2][k][w].getNabla();
                    filters[0][i][j] -= 0.15 * dCdw1;
                    filters[1][i][j] -= 0.15 * dCdw2;
                    filters[2][i][j] -= 0.15 * dCdw3;
                    
                    cLayer[0][k][w].updateBias();
                    cLayer[1][k][w].updateBias();
                    cLayer[2][k][w].updateBias();
                }
            }
        }
    }
}

void Net::restart()
{
    cout << "Bad" << endl;
    for(unsigned int i = 0; i < filters.size(); i++)
    {
        for(unsigned int j = 0; j < filters[i].size(); j++)
        {
            for(unsigned int k = 0; k < filters[i][j].size(); k++)
            {
                filters[i][j][k] = 2 * ((double) rand() / (RAND_MAX)) - 1;
            }
        }
    }

    for(unsigned int i = 0; i < cLayer.size(); i++)
    {
        for(unsigned int j = 0; j < cLayer[i].size(); j++)
        {
            for(unsigned int k = 0; k < cLayer[i][j].size(); k++)
            {
                cLayer[i][j][k].restart();
            }
        }
    }


    for(unsigned int i = 0; i < fLayers.size(); i++)
    {
        for(unsigned int j = 0; j < fLayers[i].size(); j++)
        {
            fLayers[i][j].restart();
        }
    }
}