/***************************************************
Copyright (c) 2019 Luis Llamas
(www.luisllamas.es)
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#ifndef _SYNCTRANSFORMATION_h
#define _SYNCTRANSFORMATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class SyncTransformationSpeed : public SyncTransformation
{
public:
	SyncTransformationSpeed(SyncFunction& op1, float scaleFactor) : SyncTransformation(op1), ScaleFactor(scaleFactor) { Interval = static_cast<unsigned long>(op1.Interval / scaleFactor); }

	float ScaleFactor;

	float Calculate(unsigned long elapsedMillis) override
	{
		return ScaleFactor * _op1.GetValue(static_cast<unsigned long>(elapsedMillis * ScaleFactor));
	}
};

class SyncTransformationScaleY : public SyncTransformation
{
public:
	SyncTransformationScaleY(SyncFunction& op1, float scaleFactor) : SyncTransformation(op1), ScaleFactor(scaleFactor) {}

	float ScaleFactor;

	float Calculate(unsigned long elapsedMillis) override
	{
		return ScaleFactor * _op1.GetValue(elapsedMillis);
	}
};

class SyncTransformationOffsetY : public SyncTransformation
{
public:
	SyncTransformationOffsetY(SyncFunction& op1, float offset) : SyncTransformation(op1), Offset(offset) {}

	float Offset;

	float Calculate(unsigned long elapsedMillis) override
	{
		return Offset + _op1.GetValue(elapsedMillis);
	}
};

class SyncTransformationSliceX : public SyncTransformation
{
public:
	SyncTransformationSliceX(SyncFunction& op1, long offsetX) : SyncTransformation(op1), Offset(offsetX) {}

	long  Offset;

	float Calculate(unsigned long elapsedMillis) override
	{
		if (elapsedMillis > Interval) return 0.0;
		auto elapsed = elapsedMillis + Offset;
		elapsed = elapsed % Interval;
		return _op1.GetValue(elapsed);
	}
};

class SyncTransformationDelay : public SyncTransformation
{
public:
	SyncTransformationDelay(SyncFunction& op1, unsigned long delay) : SyncTransformation(op1), Delay(delay) { Interval = op1.Interval + delay; }

	unsigned long Delay;

	float Calculate(unsigned long elapsedMillis) override
	{
		if (elapsedMillis > Interval) return 0.0;

		if (elapsedMillis < Delay) return 0.0;
		return _op1.GetValue(elapsedMillis - Delay);
	}
};


class SyncTransformationInverse : public SyncTransformation
{
public:
	SyncTransformationInverse(SyncFunction& op1) : SyncTransformation(op1) {}

	float Calculate(unsigned long elapsedMillis) override
	{
		return 1.0 - _op1.GetValue(elapsedMillis);
	}
};

class SyncTransformationReverse : public SyncTransformation
{
public:
	SyncTransformationReverse(SyncFunction& op1) : SyncTransformation(op1) {}

	float Calculate(unsigned long elapsedMillis) override
	{
		if (elapsedMillis > Interval) return 0.0;

		return _op1.GetValue(Interval - elapsedMillis);
	}
};


class SyncRepeatN : public SyncTransformation
{
public:
	SyncRepeatN(SyncFunction& op1, unsigned int repetitions) : SyncTransformation(op1), _repetitions(repetitions) { Interval = op1.Interval * repetitions; }

	float Calculate(unsigned long elapsedMillis) override
	{
		if (_repetitions == 0) return 0.0;

		const auto currentRepetition = elapsedMillis / _op1.Interval;
		if(currentRepetition != _lastRepetion)
		{
			_lastRepetion = currentRepetition;
			_op1.Reset();
		}
		if (currentRepetition >= _repetitions) return 0.0;
		return _op1.GetValue(elapsedMillis % _op1.Interval);
	}


protected:
	uint8_t _repetitions;
	unsigned int _lastRepetion = 0;
};

class SyncRepeatInfinite : public SyncTransformation
{
public:
	SyncRepeatInfinite(SyncFunction& op1) : SyncTransformation(op1) { Interval = op1.Interval; }

	float Calculate(unsigned long elapsedMillis) override
	{
		const auto currentRepetition = elapsedMillis / _op1.Interval;
		if (currentRepetition != _lastRepetion)
		{
			_lastRepetion = currentRepetition;
			_op1.Reset();
		}

		return _op1.GetValue(elapsedMillis % _op1.Interval);
	}

protected:
	unsigned int _lastRepetion = 0;
};

class SyncMirroring : public SyncFunction
{
public:
	SyncMirroring(SyncFunction& op1) : SyncFunction(op1.Interval * 2), _op1(op1) { }

	float Calculate(unsigned long elapsedMillis) override
	{
		if (elapsedMillis < _op1.Interval)
		{
			return _op1.GetValue(elapsedMillis);
		}
		else
		{
			return _op1.GetValue(Interval - elapsedMillis);
		}
	}

protected:
	SyncFunction& _op1;
};

#endif

