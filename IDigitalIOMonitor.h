#pragma once

#include "IDigitalOutput.h"
#include "DigitalOutputMonitor.h"
#include "IDigitalOutputMonitor.h"

namespace Thing
{
	namespace Core
	{
		class IDigitalIOMonitor
		{
		public:
			virtual void Toggle(IDigitalOutput& output) = 0;
			virtual void Toggle(IDigitalOutput* output) = 0;

			virtual IDigitalOutputMonitor& SetHigh(IDigitalOutput& output) = 0;
			virtual IDigitalOutputMonitor& SetHigh(IDigitalOutput* output) = 0;

			virtual IDigitalOutputMonitor& SetLow(IDigitalOutput& output) = 0;
			virtual IDigitalOutputMonitor& SetLow(IDigitalOutput* output) = 0;
		};
	}
}