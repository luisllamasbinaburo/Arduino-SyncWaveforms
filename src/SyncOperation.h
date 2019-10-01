/***************************************************
Copyright (c) 2019 Luis Llamas
(www.luisllamas.es)
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#ifndef _SYNCOPERATION_h
#define _SYNCOPERATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "SyncBases.h"

class SyncAdd : public SyncOperation
{
public:
	SyncAdd(SyncFunction& op1, SyncFunction& op2) : SyncOperation(op1, op2) {}

	float Calculate(unsigned long elapsedMillis) override
	{
		return _op1.GetValue() + _op2.GetValue();
	}
};

class SyncSubstract : public SyncOperation
{
public:
	SyncSubstract(SyncFunction& op1, SyncFunction& op2) : SyncOperation(op1, op2) {}

	float Calculate(unsigned long elapsedMillis) override
	{
		const auto rst = _op1.GetValue() - _op2.GetValue();
		return rst < 0.0 ? 0.0 : rst;
	}
};

class SyncMax : public SyncOperation
{
public:
	SyncMax(SyncFunction& op1, SyncFunction& op2) : SyncOperation(op1, op2) {}

	float Calculate(unsigned long elapsedMillis) override
	{
		return max(_op1.GetValue(), _op2.GetValue());
	}
};

class SyncMin : public SyncOperation
{
public:
	SyncMin(SyncFunction& op1, SyncFunction& op2) : SyncOperation(op1, op2) {}

	float Calculate(unsigned long elapsedMillis) override
	{
		return min(_op1.GetValue(), _op2.GetValue());
	}
};

class SyncAnd : public SyncOperation
{
public:
	SyncAnd(SyncFunction& op1, SyncFunction& op2) : SyncOperation(op1, op2) {}

	float Calculate(unsigned long elapsedMillis) override
	{
		return ((_op1.GetValue() > 0.0)  &&  (_op2.GetValue() > 0.0)) ? 1.0 : 0.0;
	}
};

class SyncOr : public SyncOperation
{
public:
	SyncOr(SyncFunction& op1, SyncFunction& op2) : SyncOperation(op1, op2) {}

	float Calculate(unsigned long elapsedMillis) override
	{
		return ((_op1.GetValue() > 0.0) || (_op2.GetValue() > 0.0)) ? 1.0 : 0.0;
	}
};

class SyncConcatenate : public SyncOperation
{
public:
	SyncConcatenate(SyncFunction& op1, SyncFunction& op2) : SyncOperation(op1, op2) { Interval = op1.Interval + op2.Interval; }

	float Calculate(unsigned long elapsedMillis) override
	{
		if (elapsedMillis <= _op1.Interval) return _op1.GetValue(elapsedMillis);
		else return _op2.GetValue(elapsedMillis - _op1.Interval);
	}
};
#endif

