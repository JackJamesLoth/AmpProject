/*
  ==============================================================================

    Model.h
    Created: 15 Jan 2021 10:15:55am
    Author:  School

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ConvLayer.h"
#include "Conv1d.h"
#include <Eigen/Dense>

class AmpModel {
public:
    AmpModel();

    void initModel(int dilations, int channels, int ampBufferSize, int audioBufferSize);
    void forward(AudioBuffer<float>& buffer, int numSamples, int channel, int writePosition);
    void forwardOld(AudioBuffer<float>& buffer, int numSamples, int channel, int writePosition);
    void copyInputData(const float** inputData, int numSamples);
    int idx(int ch, int i, int numSamples);

    void loadJSONData(File file);
private:

    
    int num_dilations;
    int num_channels;

    Conv1d initial_layer;
    Conv1d initial_layerBias;

    ConvLayer layer1;
    ConvLayer layer2;
    ConvLayer layer3;
    ConvLayer layer4;
    ConvLayer layer5;
    ConvLayer layer6;
    ConvLayer layer7;
    ConvLayer layer8;
    ConvLayer layer9;
    ConvLayer layer10;

    ConvLayer layer1Bias;
    ConvLayer layer2Bias;
    ConvLayer layer3Bias;
    ConvLayer layer4Bias;
    ConvLayer layer5Bias;
    ConvLayer layer6Bias;
    ConvLayer layer7Bias;
    ConvLayer layer8Bias;
    ConvLayer layer9Bias;
    ConvLayer layer10Bias;

    Conv1d linear_mixer;
    Conv1d linear_mixerBias;

    AudioBuffer<float> mBuffer;
    Eigen::MatrixXf mData;


    
};