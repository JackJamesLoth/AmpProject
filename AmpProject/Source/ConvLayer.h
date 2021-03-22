/*
  ==============================================================================

    ConvLayer.h
    Created: 15 Jan 2021 4:50:11pm
    Author:  School

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Conv1d.h"
#include <Eigen/Dense>

class ConvLayer {
public:
    ConvLayer();
    ConvLayer(int in_chan, int out_chan, int dil, int k_size, int bufferSize);

    // Sends input through the layer
    void forward(Eigen::RowVectorXf& data);
    void forwardOriginal(float* data, int numSamples);
    Eigen::MatrixXf forwardSliding(const Eigen::MatrixBase<Eigen::MatrixXf>& data);
    void forwardSlidingMemory(Eigen::MatrixBase<Eigen::MatrixXf>& data, Eigen::MatrixBase<Eigen::MatrixXf>& finalInput, int index);
    float** forwardNaive(float** data);

    void addToMemory(Eigen::VectorXf b);
    void setWeight(float* data, int index);
    void setBias(float* data, int index);
    void resetMemory();

    Conv1d filter1;
    Conv1d filter2;
    Conv1d residual;
private:
    

    int inChannels;
    int outChannels;


    // Computes the softsign function
    void softsign(Eigen::MatrixBase<Eigen::MatrixXf>& data);
};