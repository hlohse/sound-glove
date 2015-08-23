#ifndef SetCalibrationCommand_H_
#define SetCalibrationCommand_H_

#include "Platform.h"
#include "Command.h"
#include "Calibration.h"

class SetCalibrationCommand : public Command {
public:
	Calibration calibration[3];

	static const int max_data_size = 3 * Calibration::serialized_size;

	SOUNDGLOVE_API SetCalibrationCommand();

	// Command
	SOUNDGLOVE_API virtual void execute(Context& context);

protected:
	// Command
	virtual void serialize(Serializer& serializer, Serialization& serialization, Context& context) const;
	virtual void deserialize(Deserializer& deserializer, Serialization& serialization, Context& context);
	virtual int maxDataSize() const;
};

#endif /* SetCalibrationCommand_H_ */
