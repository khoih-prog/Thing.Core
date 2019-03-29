#include "GenericDigitalOutputTest.h"
#include "IDigitalOutputTest.h"
#include "../Ioc.h"
#include "../GenericDigitalOutput.h"

using ::testing::Return;
using ::testing::_;

namespace Thing {
	namespace Core {
		namespace Tests {
			void GenericDigitalOutputTest::SetUp()
			{
				Hardware = &HardwareMock;
				Logger = &LoggerMock;
			}

			void GenericDigitalOutputTest::TearDown()
			{
			}

			TEST_F(GenericDigitalOutputTest, InitializeTest)
			{
				const int gpio[] = { 10, 12 };
				const int code[] = { 15, 9 };
				const Thing::Core::DigitalValue initValue[] = { Thing::Core::DigitalValue::Low, Thing::Core::DigitalValue::High };

				for (int i = 0; i < 2; ++i)
				{
					EXPECT_CALL(HardwareMock, ConfigurePin(gpio[i], Thing::Core::PinMode::Output)).Times(1);
					Thing::Core::GenericDigitalOutput output(gpio[i], code[i], initValue[i]);
					EXPECT_EQ(code[i], output.GetCode());
				}
			}

			TEST_F(GenericDigitalOutputTest, WriteValueTest)
			{
				const Thing::Core::DigitalValue initValue[] = { Thing::Core::DigitalValue::Low, Thing::Core::DigitalValue::High };
				const int gpio = 1;
				const int code = 1;

				Thing::Core::GenericDigitalOutput output(gpio, code);

				for (int i = 0; i < sizeof(initValue) / sizeof(Thing::Core::DigitalValue); ++i)
				{
					EXPECT_CALL(HardwareMock, DigitalWrite(gpio, initValue[i])).Times(1);
					output.DigitalWrite(initValue[i]);
					EXPECT_EQ(initValue[i], output.GetState());
				}
			}

			TEST_F(GenericDigitalOutputTest, WriteValueToggleTest)
			{
				const int totalToggles = 3;
				const int gpio = 1;
				const int code = 1;


				Thing::Core::DigitalValue state = DigitalValue::High;
				Thing::Core::GenericDigitalOutput output(gpio, code, state);

				for (int i = 0; i < totalToggles; ++i)
				{
					Thing::Core::DigitalValue nextValue = state == DigitalValue::High ? DigitalValue::Low : DigitalValue::High;
					EXPECT_CALL(HardwareMock, DigitalWrite(gpio, nextValue)).Times(1);
					output.DigitalWrite(DigitalValue::Toggle);
					EXPECT_EQ(nextValue, output.GetState());
					state = nextValue;
				}
			}

			TEST_F(GenericDigitalOutputTest, SetCodeTest)
			{
				const int gpio = 1;
				Thing::Core::GenericDigitalOutput output(gpio, gpio);
				IDigitalOutputTest::TestCode(output);
			}
		}
	}
}