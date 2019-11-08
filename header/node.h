#pragma once
#include<iostream>
#include<math.h>

class node
{
private:
    float output;
    float rawOutput;
    vector<float> weights;
    float rate;
    float bias;
    float nabla;
    float cost;
    int index;
public:
    node();
    node(int index);
    node(int prevSize, int index);
    float getOutput();
    void setOutput(float out);
    vector<float>& getWeights();
    float getCost();
    float getNabla();
    float getBias();
    float getRate();
    void updateBias();
    void feedc(vector<vector<vector<node>>> &cLayer);
    void feed(vector<node> &prevLayer);
    void backOut(float expected, unsigned int size);
    void back(vector<node> &nextLayer);
    bool softMax(float sum);
    void updateWeights(vector<node> &prevLayer);
    void updateWeights(vector<vector<vector<node>>> &cLayer);
    float RELU(float x);
    float deriRELU(float x);
    void restart();
};

node::node()
{
    output = 0.0f;
    rate = 0.05f;
    bias = 0.5f;
    nabla = 0.0f;
    cost = 0.0f;
}

node::node(int index)
{
     output = 0.0f;
    rate = 0.05f;
    bias = 0.5f;
    nabla = 0.0f;
    cost = 0.0f;
    this->index = index;
}

node::node(int prevSize, int index)
{
    srand(time(NULL));
    output = 0.0f;
    rate = 0.05f;
    bias = 0.5f;
    nabla = 0.0f;
    cost = 0.0f;
    this->index = index;
    weights.resize(prevSize);
    for(int i = 0; i < prevSize; i++)
        weights[i] = 2 * ((double) rand() / (RAND_MAX)) - 1;
}

float node::getOutput()
{
    return output;
}

void node::setOutput(float out)
{
    output = RELU(out);
}

vector<float>& node::getWeights()
{
    return weights;
}

float node::getCost()
{
    return cost;
}

float node::getNabla()
{
    return nabla;
}

float node::getBias()
{
    return bias;
}

float node::getRate()
{
    return rate;
}

void node::updateBias()
{
    bias -= rate * nabla;
}

void node::feedc(vector<vector<vector<node>>> &cLayer)
{
    float sum = 0.0f;
    int w = 0;
    for(unsigned int i = 0; i < cLayer.size(); i++)
    {
        for(unsigned int j = 0; j < cLayer[i].size(); j++)
        {
            for(unsigned int k = 0; k < cLayer[i][j].size(); k++)
            {
                sum += cLayer[i][j][k].getOutput() * weights[w];
                w++;
            }
        }
    }
    /*cout << "NONSOFT OUTPUT" << endl;
    cout << sum + bias << endl;*/
    rawOutput = sum + bias;
    setOutput(sum + bias);
}

void node::feed(vector<node> &prevLayer)
{
    float sum = 0.0f;
    for(unsigned int i = 0; i < prevLayer.size(); i++)
        sum += prevLayer[i].getOutput() * weights[i];

    /*cout << "OUTPUT" << endl;
    cout << sum + bias << endl;*/
    rawOutput = sum + bias;
    setOutput(sum + bias);
}

void node::backOut(float expected, unsigned int size)
{
    float dCda = (-2 * (output - expected)) / float(size);
    nabla = dCda * deriRELU(rawOutput);
}

void node::back(vector<node> &nextLayer)
{
    float sum = 0.0f;
    for(unsigned int i = 0; i < nextLayer.size(); i++)
        sum += nextLayer[i].getWeights()[index] * nextLayer[i].getNabla();

    nabla = sum * deriRELU(rawOutput);
}

bool node::softMax(float sum)
{
    output = exp(- 1 * output) / sum;
    if(isnan(output))
        return true;
    return false;
}

void node::updateWeights(vector<node> &prevLayer)
{
    for(unsigned int i = 0; i < prevLayer.size(); i++)
    {
        float dCdw = prevLayer[i].getOutput() * nabla;
        weights[i] -= rate * dCdw;
    }

    bias -= rate * nabla;
}

void node::updateWeights(vector<vector<vector<node>>> &cLayer)
{
    int w = 0;
    for(unsigned int i = 0; i < cLayer.size(); i++)
    {
        for(unsigned int j = 0; j < cLayer[i].size(); j++)
        {
            for(unsigned int k = 0; k < cLayer[i][j].size(); k++)
            {
                float dCdw = cLayer[i][j][k].getOutput() * nabla;
                weights[w] -= rate * dCdw;
                w++;
            }
        }
    }
    bias -= rate * nabla;
}

/*float node::sigmoid(float x)
{
    return (1 / (1 + exp(-1 * x)));
}*/


float node::RELU(float x)
{
    if(x > 0)
        return x;
    else
        return 0.01 * x;
    
    return 0;
}

float node::deriRELU(float x)
{
    if(x > 0)
        return 1;
    else
        return 0.01;
    
    return 0;
}

void node::restart()
{
    output = 0.0f;
    bias = 0.5f;
    nabla = 0.0f;

    for(unsigned int i = 0; i < weights.size(); i++)
        weights[i] = 2 * ((double) rand() / (RAND_MAX)) - 1;
}