/***************************************************
Copyright (c) 2019 Luis Llamas
(www.luisllamas.es)
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#include "SyncBases.h"
#include "SyncTransformation.h"
#include "SyncOperation.h"

SyncConcatenate SyncFunction::operator+(SyncFunction& op)
{
	return SyncConcatenate(*this, op);
}

SyncTransformationSpeed& SyncFunction::Speed(float scaleFactor)
{
	const auto newOp = new SyncTransformationSpeed(*this, scaleFactor);
	return *newOp;
}

SyncTransformationScaleY& SyncFunction::ScaleY(float scaleFactor)
{
	const auto newOp = new SyncTransformationScaleY(*this, scaleFactor);
	return *newOp;
}

SyncTransformationOffsetY& SyncFunction::OffsetY(float offset)
{
	const auto newOp = new SyncTransformationOffsetY(*this, offset);
	return *newOp;
}

SyncTransformationSliceX& SyncFunction::SliceX(long offset)
{
	const auto newOp = new SyncTransformationSliceX(*this, offset);
	return *newOp;
}

SyncTransformationDelay& SyncFunction::Delay(unsigned long delay)
{
	const auto newOp = new SyncTransformationDelay(*this, delay);
	return *newOp;
}

SyncTransformationInverse& SyncFunction::Inverse()
{
	const auto newOp = new SyncTransformationInverse(*this);
	return *newOp;
}

SyncTransformationReverse& SyncFunction::Reverse()
{
	const auto newOp = new SyncTransformationReverse(*this);
	return *newOp;
}

SyncRepeatN& SyncFunction::Repeat(unsigned repetitions)
{
	const auto newOp = new SyncRepeatN(*this, repetitions);
	return *newOp;
}

SyncRepeatInfinite& SyncFunction::Repeat()
{
	const auto newOp = new SyncRepeatInfinite(*this);
	return *newOp;
}

SyncMirroring& SyncFunction::Mirroring()
{
	const auto newOp = new SyncMirroring(*this);
	return *newOp;
}
