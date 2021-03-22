/*
  ==============================================================================

    Model.cpp
    Created: 15 Jan 2021 10:15:55am
    Author:  School

  ==============================================================================
*/

#include "Model.h"
#include "Conv1d.h"


using Eigen::MatrixXd;

AmpModel::AmpModel() {
    num_dilations = 1;
    num_channels = 1;

}


void AmpModel::initModel(int dilations, int channels, int ampBufferSize, int audioBufferSize) {
    DBG("Initializing amp model");

    num_dilations = dilations;
    num_channels = channels;


    /*

    // Create dummy data to pass to convolve function
    float** samples;
    samples = new float* [2];
    samples[0] = new float[5];
    samples[1] = new float[5];
    for (int i = 0; i < 5; i++) {
        samples[0][i] = (float) i+1;
    }
    for (int i = 0; i < 5; i++) {
        samples[1][i] = (float)i + 3;
    }


    float*** kernel;
    kernel = new float** [2];
    kernel[0] = new float* [2];
    kernel[0][0] = new float[2];
    kernel[0][1] = new float[2];

    kernel[1] = new float* [2];
    kernel[1][0] = new float[2];
    kernel[1][1] = new float[2];
    for (int i = 0; i < 2; i++) {
        kernel[0][0][i] = (float)i + 1;
    }
    for (int i = 0; i < 2; i++) {
        kernel[0][1][i] = (float)i + 2;
    }

    for (int i = 0; i < 2; i++) {
        kernel[1][0][i] = (float)i + 2;
    }
    for (int i = 0; i < 2; i++) {
        kernel[1][1][i] = (float)i + 3;
    }


    // Test convolution in Conv1d...

    int in_c = 2;
    int out_c = 2;
    int dil = 1;
    int k_size = 2;
    Conv1d c = Conv1d(in_c, out_c, dil, k_size, kernel);

    
    //std::vector<float> result = c.convolution(samples, kernel, 5, 2, 1);

    std::vector<std::vector<float>> result = c.convolve(samples, 5);
    DBG(result.size());
    for (std::vector<float> x : result) {
        DBG("Row");
        for (float y : x) {
            DBG(y);
        }
    }

    */

    // Create initial layer
    initial_layer = Conv1d(1, channels, 1, 1, audioBufferSize);
    initial_layerBias = Conv1d(1, channels, 1, 1, 3000);


    // Create the convolutional layers (as many as there are dilations)
    layer1 = ConvLayer(channels, channels, 1, 3, audioBufferSize);
    layer2 = ConvLayer(channels, channels, 2, 3, audioBufferSize);
    layer3 = ConvLayer(channels, channels, 4, 3, audioBufferSize);
    layer4 = ConvLayer(channels, channels, 8, 3, audioBufferSize);
    layer5 = ConvLayer(channels, channels, 16, 3, audioBufferSize);
    layer6 = ConvLayer(channels, channels, 32, 3, audioBufferSize);
    layer7 = ConvLayer(channels, channels, 64, 3, audioBufferSize);
    layer8 = ConvLayer(channels, channels, 128, 3, audioBufferSize);
    layer9 = ConvLayer(channels, channels, 256, 3, audioBufferSize);
    layer10 = ConvLayer(channels, channels, 512, 3, audioBufferSize);

    // For testing bias
    layer1Bias = ConvLayer(channels, channels, 1, 3, 3000);
    layer2Bias = ConvLayer(channels, channels, 2, 3, 3000);
    layer3Bias = ConvLayer(channels, channels, 4, 3, 3000);
    layer4Bias = ConvLayer(channels, channels, 8, 3, 3000);
    layer5Bias = ConvLayer(channels, channels, 16, 3, 3000);
    layer6Bias = ConvLayer(channels, channels, 32, 3, 3000);
    layer7Bias = ConvLayer(channels, channels, 64, 3, 3000);
    layer8Bias = ConvLayer(channels, channels, 128, 3, 3000);
    layer9Bias = ConvLayer(channels, channels, 256, 3, 3000);
    layer10Bias = ConvLayer(channels, channels, 512, 3, 3000);
    

    // Create the linear mixer layer
    linear_mixer = Conv1d((channels * dilations), 1, 1, 1, audioBufferSize);
    linear_mixerBias = Conv1d((channels * dilations), 1, 1, 1, 3000);

    // Iitialize the buffer
    //mBuffer.setSize(channels, ampBufferSize);
    mBuffer.setSize(channels, audioBufferSize*16);

    //loadJSONData();

    /*
    float* testArr = new float[10];

    for (int i = 0; i < 10; i++)
    {
        testArr[i] = (float)i;
    }

    // For dilations, just change the stride to Stride<1, dilations>
    Eigen::Map<Eigen::MatrixXf, 0, Eigen::Stride<1,1>> testMap(testArr, 3,8);
    DBG("Matrix tests");
    DBG("Size: " << testMap.size());
    for (int i = 0; i < testMap.rows(); i++)
    {
        std::string message = std::to_string(i) + ": ";
        for (int j = 0; j < testMap.cols(); j++)
        {
            message = message.append(std::to_string(testMap.coeff(i, j)) + ", ");
        }
        DBG(message);
    }

    // Resize matrix so that we append output cahnnels to end so that they can be input cahnels for next convolution
    Eigen::MatrixXf copyMatrix = testMap;
    copyMatrix.transposeInPlace();
    copyMatrix(0, 0) = 6.2;
    copyMatrix(1, 1) = 2.3;
    copyMatrix(2, 0) = 8;
    copyMatrix(0, 2) = 4.3;
    DBG("copyMatrix");
    for (int i = 0; i < copyMatrix.rows(); i++)
    {
        std::string message = std::to_string(i) + ": ";
        for (int j = 0; j < copyMatrix.cols(); j++)
        {
            message = message.append(std::to_string(copyMatrix.coeff(i, j)) + ", ");
        }
        DBG(message);
    }


    Eigen::MatrixXf testRow(6, 9);
    testRow.setZero();

    Eigen::MatrixXf a;
    a.resize(3, 3);


    // Appends each output channel to each sliding window
    for (int i = 0; i < testRow.rows(); i++)
    {
        Eigen::Ref<Eigen::MatrixXf> block = copyMatrix.block(0, 0,2,2);
        //a = copyMatrix.block(1, 1, 2, 2);
        a = Eigen::Map<Eigen::RowVectorXf, 0, Eigen::Stride<2,2>>(block.data(), 4);
        DBG("b");
        for (int i = 0; i < block.rows(); i++)
        {
            std::string message = std::to_string(i) + ": ";
            for (int j = 0; j < block.cols(); j++)
            {
                message = message.append(std::to_string(block.coeff(i, j)) + ", ");
            }
            DBG(message);
        }


        // Blocks a 3x3 part of the original matrix, maps it to a vector, the sets the column equal to that map
        testRow.row(i) = Eigen::Map<Eigen::RowVectorXf> (copyMatrix.block<3, 3>(i, 0).data(), 9);
    }

    DBG("Matrix test 2");
    DBG("Size: " << testRow.size());
    for (int i = 0; i < testRow.rows(); i++)
    {
        std::string message = std::to_string(i) + ": ";
        for (int j = 0; j < testRow.cols(); j++)
        {
            message = message.append(std::to_string(testRow.coeff(i, j)) + ", ");
        }
        DBG(message);
    }

    DBG("Test");
    */


    // Fix bias
    float testBias[3000];
    for (size_t i = 0; i < 3000; i++)
    {
        testBias[i] = 0;
    }
    Eigen::MatrixXf finalInput2(num_channels * num_dilations, 3000);
    Eigen::MatrixXf biasMatrix = Eigen::Map<Eigen::MatrixXf>(testBias, 1, 3000);
    Eigen::VectorXf newBias(16, 1);

    initial_layerBias.convolveSlidingMemory(biasMatrix, false);
    newBias.col(0) = biasMatrix.col(2999);
    layer1.addToMemory(newBias);

    layer1Bias.forwardSlidingMemory(biasMatrix, finalInput2, 0);
    newBias.col(0) = biasMatrix.col(2999);
    layer2.addToMemory(newBias);

    layer2Bias.forwardSlidingMemory(biasMatrix, finalInput2, 0);
    newBias.col(0) = biasMatrix.col(2999);
    layer3.addToMemory(newBias);

    layer3Bias.forwardSlidingMemory(biasMatrix, finalInput2, 0);
    newBias.col(0) = biasMatrix.col(2999);
    layer4.addToMemory(newBias);

    layer4Bias.forwardSlidingMemory(biasMatrix, finalInput2, 0);
    newBias.col(0) = biasMatrix.col(2999);
    layer5.addToMemory(newBias);

    layer5Bias.forwardSlidingMemory(biasMatrix, finalInput2, 0);
    newBias.col(0) = biasMatrix.col(2999);
    layer6.addToMemory(newBias);

    layer6Bias.forwardSlidingMemory(biasMatrix, finalInput2, 0);
    newBias.col(0) = biasMatrix.col(2999);
    layer7.addToMemory(newBias);

    layer7Bias.forwardSlidingMemory(biasMatrix, finalInput2, 0);
    newBias.col(0) = biasMatrix.col(2999);
    layer8.addToMemory(newBias);

    layer8Bias.forwardSlidingMemory(biasMatrix, finalInput2, 0);
    newBias.col(0) = biasMatrix.col(2999);
    layer9.addToMemory(newBias);

    layer9Bias.forwardSlidingMemory(biasMatrix, finalInput2, 0);
    newBias.col(0) = biasMatrix.col(2999);
    layer10.addToMemory(newBias);
}


void AmpModel::copyInputData(const float** inputData, int numSamples) {
    float* writePtr = mBuffer.getWritePointer(0);
    for (int ch = 0; ch < 2; ++ch)
    {
        int start_idx = idx(ch, 0, numSamples);
        const float* chData = inputData[ch];
        for (int i = 0; i < numSamples; ++i)
            writePtr[start_idx + i] = chData[i];
    }
}

int AmpModel::idx(int ch, int i, int numSamples) {
    return ch * numSamples + i;
}

void AmpModel::forward(AudioBuffer<float>& buffer, int numSamples, int channel, int writePosition) {
    
    /*
    // Testing 
    //numSamples = 10;
    float testArray[10];
    for (size_t i = 0; i < 10; i++)
    {
        testArray[i] = 1/(((float)i)+1);
    }

    

    for (size_t i = 0; i < 10; i++)
    {
        DBG("testArray: " << testArray[i]);
    }
    */
    Eigen::MatrixXf finalInput(num_channels * num_dilations, numSamples);
    finalInput.setZero();

    // Maybe make this a row vector in the case of a 1x1 convolution?
    mData = Eigen::Map<Eigen::MatrixXf>(buffer.getWritePointer(0), 1, numSamples);
    //mData = Eigen::Map<Eigen::MatrixXf>(testArray, 1, numSamples);
    //mData.setZero();


    initial_layer.convolveSlidingMemory(mData, false);

    layer1.forwardSlidingMemory(mData, finalInput, 0);

    

    layer2.forwardSlidingMemory(mData, finalInput, 1);  
    layer3.forwardSlidingMemory(mData, finalInput, 2);
    layer4.forwardSlidingMemory(mData, finalInput, 3);
    layer5.forwardSlidingMemory(mData, finalInput, 4);
    layer6.forwardSlidingMemory(mData, finalInput, 5);
    layer7.forwardSlidingMemory(mData, finalInput, 6);
    layer8.forwardSlidingMemory(mData, finalInput, 7);
    layer9.forwardSlidingMemory(mData, finalInput, 8);
    layer10.forwardSlidingMemory(mData, finalInput, 9);
    linear_mixer.convolveSlidingMemory(finalInput, false);

    float* temp = finalInput.data();

    // Write to audio buffer
    buffer.copyFrom(0, 0, temp, numSamples);
    buffer.copyFrom(1, 0, temp, numSamples);
}

void AmpModel::forwardOld(AudioBuffer<float>& buffer, int numSamples, int channel, int writePosition) {


    //float** readBuffer = new float* [1];
    //readBuffer[0] = buffer.getWritePointer(0);
    //copyInputData(buffer.getArrayOfReadPointers(), numSamples);
    //float* writeBuffer = mBuffer.getWritePointer(0);
    
    /*
    * 
    * SLIDING CONV WITH TEST DATA TESTS
    * 
    */
    Conv1d test_layer(1, 4, 1, 1, 10);
    ConvLayer test_layer2(4, 4, 1, 3, 10);
    float* testArr = new float[10];
    for (int i = 0; i < 10; i++)
    {
        testArr[i] = (float)i;
    }

    mData = Eigen::Map<Eigen::MatrixXf, 0, Eigen::Stride<1, 1>>(testArr, 1, 10);
    DBG("Size: " << mData.size());
    for (int i = 0; i < mData.rows(); i++)
    {
        std::string message = std::to_string(i) + ": ";
        for (int j = 0; j < mData.cols(); j++)
        {
            message = message.append(std::to_string(mData.coeff(i, j)) + ", ");
        }
        DBG(message);
    }
    test_layer.convolveSlidingMemory(mData, false);
    DBG("Size after: " << mData.size());

    for (int i = 0; i < mData.rows(); i++)
    {
        std::string message = std::to_string(i) + ": ";
        for (int j = 0; j < mData.cols(); j++)
        {
            message = message.append(std::to_string(mData.coeff(i, j)) + ", ");
        }
        DBG(message);
    }

    // This is how to add a bias vector to the whole output matrix
    Eigen::VectorXf bias(4);
    bias(0) = 5;
    bias(1) = 3;
    bias(2) = 6;
    bias(3) = 2;
    //temp = temp.colwise() + bias;

    DBG("temp");
    for (int i = 0; i < mData.rows(); i++)
    {
        std::string message = std::to_string(i) + ": ";
        for (int j = 0; j < mData.cols(); j++)
        {
            message = message.append(std::to_string(mData.coeff(i, j)) + ", ");
        }
        DBG(message);
    }

    DBG("test");
    
    //Eigen::MatrixXf a;
    //Eigen::MatrixXf b;
    //mData.resize(12, 6);
    //for (int i = 0; i < mData.cols(); i++)
    //{
    //    auto a = temp.block<4, 5>(0, i);

    //    b = Eigen::Map<Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>, 0, Eigen::OuterStride<>>(a.data(), 3,4, Eigen::OuterStride<>(a.outerStride() * 2));
        
       
    //    mData.col(i) = Eigen::Map<Eigen::VectorXf>(b.data(), 12);
    //}
    //DBG("Size before 2: " << mData.size());
    //for (int i = 0; i < mData.rows(); i++)
    //{
    //    std::string message = std::to_string(i) + ": ";
    //   for (int j = 0; j < mData.cols(); j++)
    //    {
    //        message = message.append(std::to_string(mData.coeff(i, j)) + ", ");
    //    }
    //    DBG(message);
    //}
    




    //test_layer2.forwardSlidingMemory(mData);
    DBG("Size after 2: " << mData.size());
    for (int i = 0; i < mData.rows(); i++)
    {
        std::string message = std::to_string(i) + ": ";
        for (int j = 0; j < mData.cols(); j++)
        {
            message = message.append(std::to_string(mData.coeff(i, j)) + ", ");
        }
        DBG(message);
    }

    //test_layer2.forwardSlidingMemory(mData);
    DBG("Size after 2: " << mData.size());
    for (int i = 0; i < mData.rows(); i++)
    {
        std::string message = std::to_string(i) + ": ";
        for (int j = 0; j < mData.cols(); j++)
        {
            message = message.append(std::to_string(mData.coeff(i, j)) + ", ");
        }
        DBG(message);
    }


    DBG("test");
    
    

    // SLIDING MATRIX WITH MEMORY TEST
    /*
    // Maybe make this a row vector in the case of a 1x1 convolution?
    mData = Eigen::Map<Eigen::MatrixXf, 0, Eigen::Stride<1, 1>> (buffer.getWritePointer(0), 1, numSamples);
    DBG("Size before: " << mData.size());
    Eigen::MatrixXf temp = initial_layer.convolveSlidingMemory(mData, false);
    DBG("Size after: " << temp.size());
    
    
    temp = layer1.forwardSlidingMemory(temp);
    DBG("Size after 2: " << temp.size());
    
    
    DBG("temp: " << temp.cols());
    temp = layer2.forwardSlidingMemory(temp);
    
    temp = layer3.forwardSlidingMemory(temp);
    DBG("temp: " << temp.cols());

    temp = layer4.forwardSlidingMemory(temp);
    DBG("test");
    
    */


    


    // SLIDING MATRIX TEST
    /*
    // Maybe make this a row vector in the case of a 1x1 convolution?
    mData = Eigen::Map<Eigen::MatrixXf, 0, Eigen::Stride<1, 1>> (buffer.getWritePointer(0), 1, numSamples);
    DBG("Size before: " << mData.size());
    Eigen::MatrixXf temp = initial_layer.convolveSliding(mData);
    DBG("Size after: " << temp.size());
    
    Eigen::MatrixXf b;
    //Eigen::MatrixXf a;

    mData.resize(3 * 16, numSamples - 2);
    for (int i = 0; i < mData.cols(); i++)
    {
        auto a = temp.block<16, 3>(0, i);
        b = Eigen::Map<Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>, 0, Eigen::OuterStride<>>(a.data(), 3, 16, Eigen::OuterStride<>(a.outerStride()));
        mData.col(i) = Eigen::Map<Eigen::VectorXf>(b.data(), 48);

        //Eigen::Map<Eigen::VectorXf> b(temp.block<16, 3>(0, i).data(), 48);
        //mData.col(i) = Eigen::Map<Eigen::VectorXf>(temp.block<16, 3>(0, i).data(), 48);
    }
    DBG("Size before 2: " << mData.size());
    temp = layer1.forwardSliding(mData);
    DBG("Size after 2: " << temp.size());
    
    
    mData.resize(3 * 16, (size_t) numSamples - 6);
    for (int i = 0; i < mData.cols(); i++)
    {
        auto a = temp.block<16, 5>(0, i);
        b = Eigen::Map<Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>, 0, Eigen::OuterStride<>>(a.data(), 3, 16, Eigen::OuterStride<>(a.outerStride() * 2));
        
        mData.col(i) = Eigen::Map<Eigen::VectorXf>(b.data(), 48);

        //Eigen::Map<Eigen::VectorXf> b(temp.block<16, 3>(0, i).data(), 48);
        //mData.col(i) = Eigen::Map<Eigen::VectorXf>(temp.block<16, 3>(0, i).data(), 48);
    }
    temp = layer2.forwardSliding(mData);
    
    mData.resize(3 * 16, (size_t) numSamples - 12);

    for (int i = 0; i < mData.cols(); i++)
    {
        auto a = temp.block<16, 7>(0, i);
        b = Eigen::Map<Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>, 0, Eigen::OuterStride<>>(a.data(), 3, 16, Eigen::OuterStride<>(a.outerStride() * 3));
        
        mData.col(i) = Eigen::Map<Eigen::VectorXf>(b.data(), 48);
        //Eigen::Map<Eigen::VectorXf> b(temp.block<16, 3>(0, i).data(), 48);
        //mData.col(i) = Eigen::Map<Eigen::VectorXf>(temp.block<16, 3>(0, i).data(), 48);
    }
    temp = layer3.forwardSliding(mData);


    mData.resize(3 * 16, (size_t) numSamples - 20);

    for (int i = 0; i < mData.cols(); i++)
    {
        auto a = temp.block<16, 9>(0, i);
        b = Eigen::Map<Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>, 0, Eigen::OuterStride<>>(a.data(), 3, 16, Eigen::OuterStride<>(a.outerStride() * 4));
        
        mData.col(i) = Eigen::Map<Eigen::VectorXf>(b.data(), 48);

        //Eigen::Map<Eigen::VectorXf> b(temp.block<16, 3>(0, i).data(), 48);
        //mData.col(i) = Eigen::Map<Eigen::VectorXf>(temp.block<16, 3>(0, i).data(), 48);
    }
    temp = layer4.forwardSliding(mData);
    
    */

    /*
    // Copy data into matrix
    mData.resize(numSamples);
    for (int i = 0; i < numSamples; i++) {
        mData(i) = readBuffer[i];
    }
    
    DBG("mData: " << mData.rows() << "x" << mData.cols());
    initial_layer.convolve(mData);

    DBG("mData after initial layer: " << mData.rows() << "x" << mData.cols());

    //MatrixXf testCopy = mMatrix;
    //DBG("testCopy: " << testCopy.rows() << "x" << testCopy.cols());

    // Send input through the convolutional layers
    layer1.forward(mData);

    DBG("mData after layer1: " << mData.rows() << "x" << mData.cols());
    DBG("mData after layer1: " << mData.rows() << "x" << mData.cols());
    //DBG("testCopy after layer1: " << testCopy.rows() << "x" << testCopy.cols());
    */


    /*
    * 
    * ORIGINAL EIGEN IMPLEMENTATION
    * 
    * 
    * 
    initial_layer.convolveOriginal(writeBuffer, numSamples);
    layer1.forwardOriginal(writeBuffer, numSamples);
    layer2.forwardOriginal(writeBuffer, numSamples - 2);
    layer3.forwardOriginal(writeBuffer, numSamples - 4);
    layer4.forwardOriginal(writeBuffer, numSamples - 6);
    */


    /*
    * 
    * NAIVE C++ IMPLEMENTATION
    * 
    * 
    readBuffer = initial_layer.convolveNaive(readBuffer);
    readBuffer = layer1.forwardNaive(readBuffer);
    readBuffer = layer2.forwardNaive(readBuffer);
    readBuffer = layer3.forwardNaive(readBuffer);
    readBuffer = layer4.forwardNaive(readBuffer);
    */


    //layer1.forward(mData);
    //layer2.forward(mData);
    //layer3.forward(mData);
    //layer4.forward(mData);
    //layer5.forward(mData);
    //layer6.forward(mData);
    //layer7.forward(mData);
    //layer8.forward(mData);
    //layer9.forward(mData);
    //layer10.forward(mData);

    // Send input through the linear mixer layer

    //DBG("Matrix: " << matrix.rows() << "x" << matrix.cols());

}



void AmpModel::loadJSONData(File file) {
    DBG("Loading amp parameters");

    //File file = File("D:/Dev/Audio/AmpProject/json/pvh_model_gain_4.json");

    DBG("Loaded file");

    var jsonData = JSON::parse(file);
    int channels = jsonData["residual_channels"];
    DBG(channels);

    auto varArray = jsonData.getProperty("variables", var()).getArray();

    for (auto& variable : *varArray) {
        int layerIdx = variable["layer_idx"];
        //DBG("index: " << layerIdx);
        std::string name = variable["name"].toString().toStdString();
        //DBG("name: " << name);

        // Initial layer
        if (layerIdx == -1) {
            if (name == "W"){
                auto wArray = variable.getProperty("data", var()).getArray();
                float data[16];

                // TODO: Change from hardcoded values
                for (size_t i = 0; i < 16; i++)
                {
                    data[i] = wArray->operator[](i);
                }
                initial_layer.setWeight(data);
                initial_layerBias.setWeight(data);
                
            }
            else if (name == "b") {
                auto bArray = variable.getProperty("data", var()).getArray();
                float data[16];
                // TODO: Change from hardcoded values
                for (size_t i = 0; i < 16; i++)
                {
                    data[i] = bArray->operator[](i);
                }
                initial_layer.setBias(data);
                initial_layerBias.setBias(data);
            }
        }
        // Linear mixer layer
        else if (layerIdx == 10) {
            if (name == "W") {
                auto wArray = variable.getProperty("data", var()).getArray();
                float data[160];

                // TODO: Change from hardcoded values
                for (size_t i = 0; i < 160; i++)
                {
                    data[i] = wArray->operator[](i);
                }
                linear_mixer.setWeight(data);
                linear_mixerBias.setWeight(data);

            }
            else if (name == "b") {
                auto bArray = variable.getProperty("data", var()).getArray();
                float data[1];
                // TODO: Change from hardcoded values
                for (size_t i = 0; i < 1; i++)
                {
                    data[i] = bArray->operator[](i);
                }
                linear_mixer.setBias(data);
                linear_mixerBias.setBias(data);
            }
        }
        else {
            auto dataArray = variable.getProperty("data", var()).getArray();
            ConvLayer* layer;
            ConvLayer* layerBias;
            switch (layerIdx) {
                case 0:
                    layer = &layer1;
                    layerBias = &layer1Bias;
                    break;
                case 1:
                    layer = &layer2;
                    layerBias = &layer2Bias;
                    break;
                case 2:
                    layer = &layer3;
                    layerBias = &layer3Bias;
                    break;
                case 3:
                    layer = &layer4;
                    layerBias = &layer4Bias;
                    break;
                case 4:
                    layer = &layer5;
                    layerBias = &layer5Bias;
                    break;
                case 5:
                    layer = &layer6;
                    layerBias = &layer6Bias;
                    break;
                case 6:
                    layer = &layer7;
                    layerBias = &layer7Bias;
                    break;
                case 7:
                    layer = &layer8;
                    layerBias = &layer8Bias;
                    break;
                case 8:
                    layer = &layer9;
                    layerBias = &layer9Bias;
                    break;
                case 9:
                    layer = &layer10;
                    layerBias = &layer10Bias;
                    break;
            }
            DBG(name);
            if (name == "W_1_conv") {
                float data[768];

                DBG(dataArray->size());


                // TODO: Change from hardcoded values
                for (size_t i = 0; i < 768; i++)
                {
                    data[i] = dataArray->operator[](i);
                }

                layer->setWeight(data, 0);
                layerBias->setWeight(data, 0);
            }
            else if (name == "b_1_conv") {
                float data[16];
                // TODO: Change from hardcoded values
                for (size_t i = 0; i < 16; i++)
                {
                    data[i] = dataArray->operator[](i);
                }

                layer->setBias(data, 0);
                layerBias->setBias(data, 0);
            }
            else if (name == "W_2_conv") {
                float data[768];
                // TODO: Change from hardcoded values
                for (size_t i = 0; i < 768; i++)
                {
                    data[i] = dataArray->operator[](i);
                }

                layer->setWeight(data, 1);
                layerBias->setWeight(data, 1);
            }
            else if (name == "b_2_conv") {
                float data[16];
                // TODO: Change from hardcoded values
                for (size_t i = 0; i < 16; i++)
                {
                    data[i] = dataArray->operator[](i);
                }

                layer->setBias(data, 1);
                layerBias->setBias(data, 1);
            }
            else if (name == "W_out") {
                float data[256];
                // TODO: Change from hardcoded values
                for (size_t i = 0; i < 256; i++)
                {
                    data[i] = dataArray->operator[](i);
                }

                layer->setWeight(data, 2);
                layerBias->setWeight(data, 2);
            }
            else if (name == "b_out") {
                float data[16];
                // TODO: Change from hardcoded values
                for (size_t i = 0; i < 16; i++)
                {
                    data[i] = dataArray->operator[](i);
                }

                layer->setBias(data, 2);
                layerBias->setBias(data, 2);
            }
        }
        
    }

}