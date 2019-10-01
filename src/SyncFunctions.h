/***************************************************
Copyright (c) 2019 Luis Llamas
(www.luisllamas.es)
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#ifndef _SYNCFUNCTIONS_h
#define _SYNCFUNCTIONS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "SyncBases.h"

class SyncZeros : public SyncFunction
{
public:
	SyncZeros(unsigned long delay) : SyncFunction(delay) {}

	float Calculate(unsigned long elapsedMillis) override
	{
		return 0.0;
	};
};

class SyncConstant : public SyncFunction
{
public:
	SyncConstant(unsigned long t, float value) : SyncFunction(t), Value(value > 1.0 ? 1.0 : value < 0.0 ? 0.0 : value) {}

	float Value;
	float Calculate(unsigned long elapsedMillis) override
	{
		return Value;
	};
};

class SyncDelta : public SyncFunction
{
public:
	SyncDelta(unsigned long t) : SyncFunction(t) { }


	void Reset() override
	{
		_isTriggered = false;
	}


	float Calculate(unsigned long elapsedMillis) override
	{
		if (_isTriggered) return 0.0;
		_isTriggered = true;
		return 1.0;
	};

	bool _isTriggered = false;
};

class SyncStep : public SyncFunction
{
public:
	SyncStep(unsigned long t) : SyncFunction(t), T0(t/2) {}
	SyncStep(unsigned long t0, unsigned long t) : SyncFunction(t), T0(t0) {}

	float Calculate(unsigned long elapsedMillis) override
	{
		return elapsedMillis < T0 ? 1.0 : 0.0;
	};

	unsigned long T0;
};

class SyncRamp : public SyncFunction
{
public:
	SyncRamp(unsigned long t) : SyncFunction(t) {};

	float Calculate(unsigned long elapsedMillis) override
	{
		if (elapsedMillis > Interval) return 0.0;
		return static_cast<float>(elapsedMillis) / Interval;
	};
};

class SyncInverseRamp : public SyncFunction
{
public:
	SyncInverseRamp(unsigned long t) : SyncFunction(t) {};

	float Calculate(unsigned long elapsedMillis) override
	{
		if (elapsedMillis > Interval) return 0.0;
		return 1.0f - static_cast<float>(elapsedMillis) / Interval;
	};
};


class SyncTriangular : public SyncFunction
{
public:
	SyncTriangular(unsigned long t) : SyncFunction(t)
	{
		_t0 = t / 2;
		_t1 = t - _t0;
	}
	SyncTriangular(unsigned long t0, unsigned long t1) : SyncFunction(t0 + t1)
	{
		_t0 = t0;
		_t1 = t1;
	};

	float Calculate(unsigned long elapsedMillis) override
	{
		if (elapsedMillis > Interval) return 0.0;
		if (elapsedMillis < _t0)
		{
			return static_cast<float>(elapsedMillis) / _t0;
		}
		return 1.0f - (static_cast<float>(elapsedMillis) - _t0) / _t1;
	};

	unsigned long _t0;
	unsigned long _t1;
};

class SyncTrapezium : public SyncFunction
{
public:
	SyncTrapezium(unsigned long t) : SyncFunction(t)
	{
		_t0 = t / 3;
		_t1 = _t0;
		_t2 = t - _t0 - _t1;
	};

	SyncTrapezium(unsigned long t0, unsigned long t1, unsigned long t2) : SyncFunction(t0 + t1 + t2)
	{
		_t0 = t0;
		_t1 = t1;
		_t2 = t2;
	};

	float Calculate(unsigned long elapsedMillis) override
	{
		if (elapsedMillis > Interval) return 0.0;
		if (elapsedMillis < _t0)
		{
			return static_cast<float>(elapsedMillis) / _t0;
		}
		else if (elapsedMillis < _t0 + _t1)
		{
			return 1.0f;
		}
		else
		{
			return 1.0f - (static_cast<float>(elapsedMillis) - _t1 - _t0) / _t2;
		}
	};

	unsigned long _t0;
	unsigned long _t1;
	unsigned long _t2;
};


class SyncSin : public SyncFunction
{
public:
	SyncSin(unsigned long t) : SyncFunction(t) {};

	float Calculate(unsigned long elapsedMillis) override
	{
		if (elapsedMillis > Interval) return 0.0;
		return 0.5* sin(2 * PI / Interval * elapsedMillis) + 0.5;
	};
};


class SyncCos : public SyncFunction
{
public:
	SyncCos(unsigned long t) : SyncFunction(t) {};

	float Calculate(unsigned long elapsedMillis) override
	{
		if (elapsedMillis > Interval) return 0.0;
		return 0.5 * cos(2 * PI / Interval * elapsedMillis) + 0.5;
	};
};

#endif

