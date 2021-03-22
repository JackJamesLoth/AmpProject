/*
  ==============================================================================

    Conv1d.h
    Created: 15 Jan 2021 4:38:57pm
    Author:  School

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <Eigen/Dense>
#include <Eigen/Sparse>

class Conv1d {
public:

    Conv1d();
    Conv1d(int in_chan, int out_chan, int dil, int k_size, int audioBufferSize);

    void convolve(Eigen::RowVectorXf& matrix);
    void convolveOriginal(float* data, int numSamples);
    Eigen::MatrixXf convolveSliding(const Eigen::MatrixBase<Eigen::MatrixXf>& data);
    void convolveSlidingMemory(Eigen::MatrixBase<Eigen::MatrixXf>& data, bool reshape);
    void addToMemory(Eigen::VectorXf b);
    void processSingleSample(float* data, int i, int numSamples);
    int idx(int ch, int i, int numSamples);
    int mod(int a, int b);
    int getFilterOrder();

    float** convolveNaive(float** arr);

    void setWeight(float* data);
    void setBias(float* data);
    Eigen::VectorXf bias;

    void resetMemory();
private:
    typedef Eigen::Triplet<float> T;
    

    int in_channels;
    int out_channels;
    int dilations;
    int num_samples;

    // Sliding window implementation
    Eigen::MatrixXf kernelSliding;
    Eigen::MatrixXf outputSliding;
    Eigen::MatrixXf memory;
    

    // Original implementation
    std::vector<Eigen::MatrixXf, Eigen::aligned_allocator<Eigen::MatrixXf>> kernelOriginal;
    Eigen::RowVectorXf biasOriginal;
    std::vector<Eigen::RowVectorXf, Eigen::aligned_allocator<Eigen::RowVectorXf> > memoryOriginal;
    Eigen::RowVectorXf outVecOriginal;
    int pos;

    Eigen::SparseMatrix<float> kernel;
    Eigen::MatrixXf signal;


    float*** weights;
    float** naiveReturnMatrix;

    int kernel_size;
    int convolutionSize;
};