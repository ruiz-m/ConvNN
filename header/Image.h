#pragma once
#include<iostream>
#include<vector>
#include"Color.h"

using namespace std;

struct Image
{
    unsigned char info[54];
    unsigned int width;
    unsigned int height;
    vector<vector<Color>> colors;
    Image(char* filename);
    Image();
};

Image::Image(char* filename)
{
    FILE* f = fopen(filename, "rb");
    fread(info, sizeof(unsigned char), 54, f);

    width = *(int*)&info[18];
    height = *(int*)&info[22];

    int size = 3 * width * height;
    unsigned char* data = new unsigned char[size];
    fread(data, sizeof(unsigned char), size, f);
    fclose(f);

    colors.resize(height);
    for(unsigned int i = 0; i < height; i++)
        colors[i].resize(width);
    
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            /*cout << (int)data[3 * (i * width + j)] << " ";
            cout << (int)data[3 * (i * width + j) + 1] << " ";
            cout << (int)data[3 * (i * width + j) + 2] << " ";*/
            colors[i][j].blue = float(data[3 * (i * width + j)]) / 255;
            colors[i][j].green = float(data[3 * (i * width + j) + 1]) / 255;
            colors[i][j].red = float(data[3 * (i * width + j) + 2]) / 255;
        }
    }
    delete[] data;
}

Image::Image() {}