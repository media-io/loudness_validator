#ifndef AVCORRECTOR_HPP
#define AVCORRECTOR_HPP

#include <AvTranscoder/reader/AudioReader.hpp>

union Sample {
    int value;
    unsigned char bytes[4];
};

class AvCorrector
{
public:
    AvCorrector(const std::vector<avtranscoder::InputStreamDesc>& arrayToCorrect);

    void correct(const float gain);

private:
    int getSampleMaxValue(const std::string& sampleFormat);

    size_t correctFrame(unsigned char* srcData, unsigned char* dstData, const size_t nbChannels, const size_t nbSamples,
        const size_t bytesPerSample, const float gain);

    bool isEndOfCorrection();

private:
    // for progress
    size_t _totalNbSamplesToCorrect;
    size_t _cumulOfSamplesCorrected;
    std::vector<size_t> _nbSamplesToCorrect;

    // to check audio before correct
    std::vector<size_t> _inputNbChannels;
    std::vector<size_t> _inputSampleRate;

    int _currentReaderSamplesMaxValue;

    // for io
    std::vector<avtranscoder::AudioReader*> _audioReader;
};

#endif
