# Arduino library SyncWaveforms

Arduino SyncWaveforms implements timed functions that runs synchronously, so it doesn't depends of the time elapsed between calls. Usefull, for example, for lightning effect or motor/robots controlling.

For example, imagine a LED that blinks slowly using a Sin wave, but also makes a 'highlight effect' when you push a button (or somethink like that). With SyncWaveforms is very easy to achieve this kind of behaviors.

## Instructions for use
The base brick of Arduino SyncWaveforms are SyncFunctions (Delta, Zeros, Constant, Step, Ramp, InverseRamp, Triangular, Trapezium, Sin and Cosine)
All basic functions have a Interval (t), and returns a Y range between 0.0 and 1.0.

```c++
auto delta = SyncDelta(250);
auto zeros = SyncZeros(250);
auto constant = SyncConstant(250, 1.0);
auto step = SyncStep(250);
auto ramp = SyncRamp(250);
auto iramp = SyncInverseRamp(250);
auto sim_tri = SyncTriangular(250);
auto asym_tri = SyncTriangular(100, 150);
auto sim_trap = SyncTrapezium(250);
auto asim_trap = SyncTrapezium(50, 150, 100);
auto sine = SyncSin(250);
auto cosine = SyncCos(250);
```

Then, you can modify this functions chaining SyncTransformations (Speed, ScaleY, OffsetY, SliceX, Delay, Inverse, Reverse, Repeat or mirroring)
```c++
sine.ScaleY(3.0).OffsetY(1.0).Speed(0.5).Repeat(5);
```

You can concantenate several SyncFunction with operator '+'
```c++
auto compound = (ramp + constant + zeros).Repeat(3).ScaleY(3.0);
```

Finally, you also can 'Restart()' any SyncFunction.

# Examples

### Blink
```c++
#include "SyncWaveforms.h"

auto pwm = SyncStep(1000).Repeat();

void setup() 
{
	while (!Serial) { ; }

	Serial.begin(115200);
}

void loop() 
{
	Serial.println(pwm.GetValue());
}
```
![alt text](https://github.com/luisllamasbinaburo/Arduino-SyncWaveforms/blob/master/images/arduino-syncwaveforms-blink.png)


### Compound
```c++
void setup() 
{
	while (!Serial) { ; }

	Serial.begin(115200);

	auto ramp = SyncRamp(150);
	auto constant = SyncConstant(300, 1.0);
	auto zeros = SyncZeros(200);
	auto compound = (ramp + constant + zeros).Repeat(3).ScaleY(3.0);
}

void loop() 
{
  Serial.println(compound.GetValue());
}
```
![alt text](https://github.com/luisllamasbinaburo/Arduino-SyncWaveforms/blob/master/images/arduino-syncwaveforms-compound.png)

### Restart
```c++
#include "SyncWaveforms.h"

void setup() 
{
	while (!Serial) { ; }

	Serial.begin(115200);

	auto sin = SyncSin(1000).Repeat();
	auto trigger = SyncTrapezium(150, 150, 150);

	auto newMillis = millis();
	auto lastMillis = newMillis;
	const auto endMillis = newMillis + 10000;
	while(newMillis < endMillis)
	{
		newMillis = millis();
		Serial.println(max(sin.GetValue() * 0.5, trigger.GetValue()));
		
		// just some timig to simulate and event
		if(newMillis - lastMillis > 3000)
		{
			lastMillis = newMillis;
			trigger.Restart();
		}
	}
}

void loop() 
{

}
```
![alt text](https://github.com/luisllamasbinaburo/Arduino-SyncWaveforms/blob/master/images/arduino-syncwaveforms-trigger.png)







