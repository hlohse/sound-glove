#include "MeanAnswer.h"
#include "Platform.h"

MeanAnswer::MeanAnswer()
:	Command(Command::type_mean_answer)
{
}

void MeanAnswer::execute(Context& context)
{
}

void MeanAnswer::serialize(Serializer& serializer, Serialization& serialization, Context& context) const
{
	serializer.serialize((uint8_t*)sensor_calibration, sizeof(sensor_calibration));
}

void MeanAnswer::deserialize(Deserializer& deserializer, Serialization& serialization, Context& context)
{
	deserializer.deserialize((uint8_t*) sensor_calibration, sizeof(sensor_calibration));
}

int MeanAnswer::maxDataSize() const
{
	return max_data_size;
}
