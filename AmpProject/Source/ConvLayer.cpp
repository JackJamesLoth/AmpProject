/*
  ==============================================================================

    ConvLayer.cpp
    Created: 15 Jan 2021 4:50:11pm
    Author:  School

  ==============================================================================
*/

#include "ConvLayer.h"
#include "Conv1d.h"
#include <cmath>

// Defulat constructor
ConvLayer::ConvLayer() : filter1(1, 1, 1, 1, 1), filter2(1, 1, 1, 1, 1), residual(1, 1, 1, 1, 1) {
    inChannels = 1;
    outChannels = 1;
}

ConvLayer::ConvLayer(int in_chan, int out_chan, int dil, int k_size, int bufferSize) : filter1(in_chan, out_chan, dil, k_size, bufferSize), filter2(in_chan, out_chan, dil, k_size, bufferSize), residual(in_chan, out_chan, dil, 1, bufferSize) {
    inChannels = in_chan;
    outChannels = out_chan;

}


// Should add residual to existing matrix
// Should concat z to existing matrix
void ConvLayer::forward(Eigen::RowVectorXf& data) {

    // Get output from filter1
    filter1.convolve(data);

    // Get output from filter2
    //filter2.convolve(data);

    // Softsign activation on both filter1 and filter2 outputs
    //softsign(copy1);
    //softsign(copy2);

    // Multiply outputs together, save this output for later
    //copy1 = copy1.cwiseProduct(copy2);

    // TODO: Concat this to z stack matrix


    // Get output of residual filter
    //residual.convolve(copy3);

    // Add residual output to original input
    //matrix = matrix + copy3;

}
void ConvLayer::setWeight(float* data, int index) {
    if (index == 0) {
        filter1.setWeight(data);
    }
    else if (index == 1) {
        filter2.setWeight(data);
    }
    else {
        residual.setWeight(data);
    }
}

void ConvLayer::setBias(float* data, int index) {
    if (index == 0) {
        filter1.setBias(data);
    }
    else if (index == 1) {
        filter2.setBias(data);
    }
    else {
        residual.setBias(data);
    }
}

Eigen::MatrixXf ConvLayer::forwardSliding(const Eigen::MatrixBase<Eigen::MatrixXf>& data) {
    return(filter1.convolveSliding(data));
}

void ConvLayer::addToMemory(Eigen::VectorXf b) {
    filter1.addToMemory(b);
    filter2.addToMemory(b);
}
void ConvLayer::resetMemory() {
    filter1.resetMemory();
    filter2.resetMemory();
}
void ConvLayer::forwardSlidingMemory(Eigen::MatrixBase<Eigen::MatrixXf>& data, Eigen::MatrixBase<Eigen::MatrixXf>& finalInput, int index) {
    Eigen::MatrixXf data2 = data;
    Eigen::MatrixXf data3 = data;
    

    filter1.convolveSlidingMemory(data, true);


    filter2.convolveSlidingMemory(data2, true);

    

    
    // Activation
    softsign(data);
    softsign(data2);

    data2 = data.cwiseProduct(data2);


    // Save for final layer
    finalInput.block(index * outChannels, 0, outChannels, data2.cols()) = data2;

    // Residual
    residual.convolveSlidingMemory(data2, true);

    
    
    

    data = data3 + data2;

    

}

float** ConvLayer::forwardNaive(float** data) {

    // Get output from filter1
    return(filter1.convolveNaive(data));

}

void ConvLayer::forwardOriginal(float* data, int numSamples) {
    filter1.convolveOriginal(data, numSamples);
}

void ConvLayer::softsign(Eigen::MatrixBase<Eigen::MatrixXf>& data) {
    Eigen::MatrixXf copy = data;

    //Compute 1/(1+|x|)
    copy = copy.cwiseAbs();
    copy = copy.array() + 1;
    copy = copy.cwiseInverse();

    data = data.cwiseProduct(copy);
}