#include "IntegrateGAnswer.h"
#include "Platform.h"
#ifdef MSP430
#include "IntegrateGAnswer.h"
#include "MARG.h"
#endif

IntegrateGAnswer::IntegrateGAnswer()
:   Command(type_integrate_g_answer),
	rotation()
{
}

void IntegrateGAnswer::execute(Context& context)
{
}

void IntegrateGAnswer::serialize(Serializer& serializer, Serialization& serialization, Context& context) const
{
	serializer.serialize((uint8_t*)rotation.values, sizeof(rotation));
}

void IntegrateGAnswer::deserialize(Deserializer& deserializer, Serialization& serialization, Context& context)
{
	deserializer.deserialize((uint8_t*)rotation.values, sizeof(rotation));
}

int IntegrateGAnswer::maxDataSize() const
{
	return max_data_size;
}
