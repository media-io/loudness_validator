/*
 * loudnessProcess.h
 *
 *  Created on: Aug 26, 2011
 *      Author: Marc-Antoine ARNAUD
 *              MikrosImage R&D
 */

#ifndef LOUDNESS_PROCESS_H
#define LOUDNESS_PROCESS_H

#include "loudnessLoudness.h"
#include "loudnessFilter.h"
#include "loudnessHistogram.h"
#include "loudnessTruePeakMeter.h"

#include <vector>
#include <cmath>

namespace Loudness{

#define MAX_CHANNELS 5
#define FRAGMENT_SIZE 64


class LoudnessProcess
{
public:
	LoudnessProcess( float absoluteThresholdValue, float relativeThresholdValue );
	~LoudnessProcess();


	void        init                              ( const int numberOfChannels, const float frequencySampling );
	void        reset                             ( );
	void        process                           ( const int numberOfFrames, float *inputData [] );

	void        setUpsamplingFrequencyForTruePeak ( const size_t frequency );

	float       getMaxLoudnessMomentary           ( ) const { return s_momentaryLoudness.getMaxLoudnessValue(); }

	float       getMaxLoudnessShortTerm           ( ) const { return s_shortTermLoudness.getMaxLoudnessValue(); }
	float       getMinLoudnessShortTerm           ( ) const { return s_shortTermLoudness.getMinLoudnessValue(); }

	float       getIntegrated                     ( ) { float _integratedLoudness, _integratedThreshold;
							s_momentaryLoudness.processIntegrationValues( _integratedLoudness, _integratedThreshold );
							return _integratedLoudness; }

	float       getIntegratedThreshold            ( ) { float _integratedLoudness, _integratedThreshold;
							    s_momentaryLoudness.processIntegrationValues( _integratedLoudness, _integratedThreshold );
							    return _integratedThreshold; }

	float       getRangeMin                       ( ) { s_shortTermLoudness.processRangeValues();
							    return s_shortTermLoudness.getMinRange(); }

	float       getRangeMax                       ( ) { s_shortTermLoudness.processRangeValues();
							    return s_shortTermLoudness.getMaxRange(); }

	float       getRangeThreshold                 ( ) { s_shortTermLoudness.processRangeValues();
							    return s_shortTermLoudness.getThresholdRange(); }

	float       getTruePeakValue                  ( ) const { return _truePeakValue; }
	float       getTruePeakValueInDb              ( ) const { return 20.0 * std::log10( _truePeakValue ); }

	const std::vector<float> getShortTermValues   ( ) { return s_shortTermLoudness.getTemporalValues();}

	// values in dB
	const std::vector<float> getTruePeakValues    ( ) { return _vectorOfTruePeakValue;}

	const std::vector<int> getHistogramShortTerm  ( ) { return s_shortTermLoudness.getHistogram(); }


private:
	// process on a bloc of 50ms, compute the loudness value, and the found the TruePeak on the buffer
	float       detectProcess                     ( const int numberOfFrames, float& truePeakValue );

	int                   _numberOfChannels;        // Number of channels, 2 or 5.
	float                 _frequencySampling;       // Sample rate.
	int                   _fragmentSize;            // Fragments size, 1/20 second.
	int                   _fragmentCount;           // Number of samples remaining in current fragment.
	float                 _fragmentPower;           // Power accumulated for current fragment.
	float                 _power [FRAGMENT_SIZE];   // Array of fragment powers.
	int                   _writeIndex;              // Write index into _frpwr

	float                 _tmpTruePeakValue;        // TruePeak on window size (for example on 500ms )
	int                   _countTruePeakPeriod;     // TruePeak counter
	std::vector<float>    _vectorOfTruePeakValue;   // temporal TruePeak on window size
	float                 _truePeakValue;           // TruePeak on Program


	float*                _inputPointerData   [MAX_CHANNELS];
	// pre-filters
	LoudnessFilter        _filters            [MAX_CHANNELS];

	// TruePeakMeter
	LoudnessTruePeakMeter _truePeakMeter      [MAX_CHANNELS];

	
	LoudnessLoudness      s_measureLoudness;
	
	LoudnessLoudness      s_shortTermLoudness;
	LoudnessLoudness      s_momentaryLoudness;

	// Default channel gains.
	static float         _channelGain        [MAX_CHANNELS];
};

}

#endif // EBU_R128_PROCESS_H
