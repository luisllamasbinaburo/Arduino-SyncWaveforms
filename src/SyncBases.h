/***************************************************
Copyright (c) 2019 Luis Llamas
(www.luisllamas.es)
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#ifndef _SYNCBASES_h
#define _SYNCBASES_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#pragma region Forward definitions
class SyncTransformationSpeed;
class SyncTransformationScaleY;
class SyncTransformationOffsetY;
class SyncTransformationSliceX;
class SyncTransformationDelay;
class SyncTransformationInverse;
class SyncTransformationReverse;
class SyncConcatenate;
class SyncRepeatN;
class SyncRepeatInfinite;
class SyncMirroring;
class SyncConcatenate;
#pragma endregion

class ISyncFunction
{
public:
	
	virtual float GetValue() = 0;
	virtual float GetValue(unsigned long overWriteMillis) = 0;
	virtual unsigned long GetElapsed() = 0;

private:
	virtual float Calculate(unsigned long elapsedMillis) = 0;

protected:
	virtual ~ISyncFunction() {}
};


class SyncFunction : ISyncFunction
{
public:
	SyncFunction(unsigned long interval) : Interval(interval), StarTime(millis()) {}

	unsigned long Interval;
	bool IsActive = true;

	virtual void Reset()
	{
		
	}

	virtual void Restart()
	{
		StarTime = millis();
	}

	SyncConcatenate operator +(SyncFunction& op);

	float GetValue() override final
	{
		return GetValue(GetElapsed());
	}

	float GetValue(unsigned long elapsedMillis) override final
	{
		//if (!IsActive) return 0.0;
		return Calculate(elapsedMillis);
	}

	unsigned long GetElapsed() override
	{
		return static_cast<unsigned long> (millis() - StarTime);
	}

	// "Fluent" behavior
	SyncTransformationSpeed& Speed(float scaleFactor);
	SyncTransformationScaleY& ScaleY(float scaleFactor);
	SyncTransformationOffsetY& OffsetY(float offset);
	SyncTransformationSliceX& SliceX(long offset);
	SyncTransformationDelay& Delay(unsigned long delay);
	SyncTransformationInverse& Inverse();
	SyncTransformationReverse& Reverse();

	SyncRepeatN& Repeat(unsigned int repetitions);
	SyncRepeatInfinite& Repeat();
	SyncMirroring& Mirroring();

	unsigned long StarTime;

protected:
	virtual ~SyncFunction() {}

	float Calculate(unsigned long elapsedMillis) override { return 0.0; }

};


class SyncTransformation : public SyncFunction
{
public:

	void Restart() override
	{
		StarTime = millis();
		//_op1.StarTime = StarTime;
	}


	SyncTransformation(SyncFunction& op1) : SyncFunction(op1.Interval), _op1(op1) {}

	float Calculate(unsigned long elapsedMillis) override = 0;

	SyncFunction & _op1;
};


class SyncOperation : public SyncFunction
{
public:

	void Restart() override
	{
		StarTime = millis();
		/*_op1.StarTime = StarTime;
		_op2.StarTime = StarTime;*/
	}


	SyncOperation(SyncFunction& op1, SyncFunction& op2, unsigned long t) : SyncFunction(t), _op1(op1), _op2(op2) {}
	SyncOperation(SyncFunction& op1, SyncFunction& op2) : SyncFunction(max(op1.Interval, op2.Interval)), _op1(op1), _op2(op2) {}

	float Calculate(unsigned long elapsedMillis) override = 0;

	SyncFunction & _op1;
	SyncFunction & _op2;
};
#endif

