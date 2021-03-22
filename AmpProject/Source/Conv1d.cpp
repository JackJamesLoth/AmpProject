/*
  ==============================================================================

    Conv1d.cpp
    Created: 15 Jan 2021 4:38:57pm
    Author:  School

  ==============================================================================
*/

#include "Conv1d.h"

Conv1d::Conv1d() {

    DBG("Created Conv1d layer");

    in_channels = 0;
    out_channels = 0;
    dilations = 0;
    kernel_size = 0;
    num_samples = 0;
    //weights = k;

}

Conv1d::Conv1d(int in_chan, int out_chan, int dil, int k_size, int audioBufferSize) {

    in_channels = in_chan;
    out_channels = out_chan;
    dilations = dil;
    kernel_size = k_size;
    num_samples = audioBufferSize;

    /*
    * 
    * THIS IS FOR THE SPARSE MATRIX VERSION
    * 
    * 
    // Initialize kerel sparse matrix (NEED TO ACCOUNT FOR DILATION!!!)
    std::vector<T> tripletList;
    tripletList.reserve((k_size * in_chan) * (audioBufferSize - k_size + 1) * out_chan);
    for (int i = 0; i < out_chan; i++) {
        for (int j = 0; j < (audioBufferSize - ((k_size - 1) * dil)); j++) {
            for (int k = 0; k < in_chan; k++) {
                for (int l = 0; l < k_size; l++) {
                    int row = (k * audioBufferSize) + j + l;
                    int col = i * (audioBufferSize - ((k_size - 1) * dil)) + j;
                    tripletList.push_back(T(row, col, l+1));
                }
            }
        }
    }
    kernel.setFromTriplets(tripletList.begin(), tripletList.end());
    */


    /*
    * 
    * THIS IS FOR NAIVE VERSION
    * 
    weights = new float**[out_chan];

    //Create weights array (NAIVE IMPLEMENTATION)
    for (int i = 0; i < out_chan; i++)
    {
        weights[i] = new float* [in_chan];
        for (int j = 0; j < in_chan; j++)
        {
            weights[i][j] = new float [kernel_size];
            for (int k = 0; k < kernel_size; k++)
            {
                weights[i][j][k] = (float)k;
            }
        }
    }

    //Create return matrix
    naiveReturnMatrix = new float* [out_chan];
    for (int i = 0; i < out_chan; i++)
    {
        naiveReturnMatrix[i] = new float[audioBufferSize];
        for (int j = 0; j < audioBufferSize; j++)
        {
            naiveReturnMatrix[i][j] = 0.0;
        }
    }



    // Initialize bias matrix
    bias.resize(out_chan);
    for (float i = 0; i < bias.rows(); i++) {
        bias(i) = i;
    }
    */


    /*
    * 
    * USED FOR ORIGINAL IMPLEMENTATION
    * 
    * 
    * 
    // Set up kernel
    kernelOriginal.clear();
    kernelOriginal.reserve(kernel_size);
    for (int i = 0; i < kernel_size; i++)
    {
        Eigen::MatrixXf x(in_channels, out_channels);

        for (int in = 0; in < in_channels; in++)
        {
            for (int out = 0; out < out_channels; out++)
            {
                x(in, out) = (float) in * out;
            }
        }
        //x.setZero();
        kernelOriginal.push_back(x);
    }

    // Set up bias
    biasOriginal = Eigen::RowVectorXf(out_channels);
    biasOriginal.setZero();

    // Set up memory vector (acts as a circular buffer)
    memoryOriginal.clear();
    memoryOriginal.reserve(getFilterOrder());
    for (int i = 0; i < getFilterOrder(); i++)
    {
        Eigen::RowVectorXf x(in_channels);
        x.setZero();
        memoryOriginal.push_back(x);
    }
    pos = 0;

    //Set up output vector
    outVecOriginal = Eigen::RowVectorXf(out_channels);
    
    */

    // Set up sliding kernel
    kernelSliding.resize(out_channels, kernel_size * in_channels);
    for (int j = 0; j < out_channels; j++)
    {
        for (int k = 0; k < kernel_size * in_channels; k++)
        {
            kernelSliding(j, k) = 0;
        }
    }

    // Set up sliding output
    outputSliding.resize(out_channels, audioBufferSize);
    outputSliding.setZero();

    // Set up sliding memory
    memory.resize(out_channels, getFilterOrder() - 1 + audioBufferSize);
    memory.setZero();

    // Set up bias
    bias.resize(out_channels);
    for (size_t i = 0; i < out_channels; i++)
    {
        bias(i) = 0;
    }
    
}

void Conv1d::setWeight(float* data) {
    // TODO use maps
    size_t i = 0;
    for (int j = 0; j < out_channels; j++)
    {
        for (int k = 0; k < kernel_size * in_channels; k++)
        {
            kernelSliding(j, k) = data[i];
            i++;
        }
    }
}

void Conv1d::setBias(float* data) {
    for (size_t i = 0; i < out_channels; i++)
    {
        bias(i) = data[i];
    }
}

void Conv1d::addToMemory(Eigen::VectorXf b) {
    memory = memory.colwise() + b;
}


int Conv1d::getFilterOrder() {
    return (kernel_size - 1) * dilations + 1;
}

void Conv1d::convolve(Eigen::RowVectorXf& matrix) {

    DBG("kernel: " << kernel.rows() << "x" << kernel.cols());
    DBG("matrix: " << matrix.rows() << "x" << matrix.cols());
    matrix *= kernel;
}

Eigen::MatrixXf Conv1d::convolveSliding(const Eigen::MatrixBase<Eigen::MatrixXf>& data) {

    outputSliding = kernelSliding * data;

    return(outputSliding);
}

void Conv1d::resetMemory() {
    memory.setZero();
}

void Conv1d::convolveSlidingMemory(Eigen::MatrixBase<Eigen::MatrixXf>& data, bool reshape) {



    if (reshape) {
        // Fill memory matrix with rest of data
        memory.block(0, (size_t) getFilterOrder() - 1, in_channels, data.cols()) = data;

        Eigen::MatrixXf temp;
        Eigen::MatrixXf data2;
        data2.resize((size_t) out_channels * (size_t) kernel_size, (memory.cols() - getFilterOrder()) + 1);

        // Reshape memory matrix into convolution shape
        for (int i = 0; i < data2.cols(); i++)
        {
            Eigen::Ref<Eigen::MatrixXf> a = memory.block(0, i, out_channels, getFilterOrder());
            temp = Eigen::Map<Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>, 0, Eigen::OuterStride<>>(a.data(), kernel_size, out_channels, Eigen::OuterStride<>(a.outerStride() * dilations));

            data2.col(i) = Eigen::Map<Eigen::VectorXf>(temp.data(), (size_t)out_channels * (size_t)kernel_size);
        }

        // Save memory
        // If the filter order is larger than the number of samples that are currently being calculated, move the old samples in
        // the memory matrix the beginning of the matrix and add all of the new data
        if (getFilterOrder() - 1 > num_samples) {

            auto temp = memory.block(0, num_samples, in_channels, getFilterOrder() - 1 - num_samples);
            memory.block(0, 0, in_channels, (size_t)getFilterOrder() - 1 - num_samples) = temp;
            //auto temp2 = data.block(0, 0, in_channels, num_samples);
            memory.block(0, (size_t)getFilterOrder() - 1 - num_samples, in_channels, num_samples) = data.block(0, 0, in_channels, num_samples); // TODO: Change to not use block?
        }
        else {
            memory.block(0, 0, in_channels, (size_t)getFilterOrder() - 1) = data.block(0, data.cols() - getFilterOrder() + 1, in_channels, getFilterOrder() - 1);
        }

        // Convolve
        data = (kernelSliding * data2).colwise() + bias;

        
        

    } else {
        data = (kernelSliding * data).colwise() + bias;
    }


    

    //return(outputSliding);
}

void Conv1d::convolveOriginal(float* data, int numSamples) {
    for (int i = 0; i < numSamples; i++)
    {
        processSingleSample(data, i, numSamples);
    }
}

void Conv1d::processSingleSample(float* data, int i, int numSamples) {
    auto memoryIterator = memoryOriginal.begin();

    // Fill memory array at current position
    for (int ch = 0; ch < in_channels; ch++)
    {
        (*(memoryIterator + pos))[ch] = data[idx(ch, i, numSamples)];
    }
    outVecOriginal.setZero();
    std::vector<Eigen::MatrixXf>::iterator it;
    int j = 0;
    // Iterate through kernel matrix, multiply the memory by each matrix
    for (auto it = kernelOriginal.begin(); it != kernelOriginal.end(); it++)
    {
        int readPos = mod((pos - j * dilations), getFilterOrder());
        outVecOriginal = outVecOriginal + *(memoryIterator + readPos) * (*it);
        j += 1;
    }
    outVecOriginal = outVecOriginal + biasOriginal;

    for (int ch = 0; ch < out_channels; ch++)
    {
        data[idx(ch, i, numSamples)] = outVecOriginal[ch];
    }
    pos = mod(pos + 1, getFilterOrder());
}

int Conv1d::idx(int ch, int i, int numSamples)
{
    return ch * numSamples + i;
}

int Conv1d::mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}





float** Conv1d::convolveNaive(float** arr) {

    for (int i = 0; i < out_channels; i++)
    {
        for (int j = 0; j < in_channels; j++)
        {
            for (int k = 0; k < num_samples; k++)
            {
                for (int l = 0; l < kernel_size; l++)
                {
                    naiveReturnMatrix[i][k] += arr[j][k+l];
                }
            }
        }
    }

    return(naiveReturnMatrix);
}