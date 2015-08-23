#include "Glove.h"
#include "MARG.h"
#include "Timer.h"
#include "MARGRotationMadgwick.h"
#include <fstream>
using namespace std;

void Glove::initialize()
{
	Timer::initialize();
}

Glove::Glove(const double ahrs_beta,
			 const double ahrs_zeta,
			 const string& packet_rate_filename)
:	forearm_(make_shared<ArmPart>(30)),
	upper_arm_(make_shared<ArmPart>(25)),
	hand_(forearm_, upper_arm_),
	battery_(),
	is_capacitive_pin_touched_(false),
	dropped_readouts_(0),
	is_calibrated_(false),
	readouts_(0),
	readout_rate_(0),
	ahrs_beta_(ahrs_beta),
	ahrs_zeta_(ahrs_zeta),
	packet_rate_filename_(packet_rate_filename)
{
	timer_readout_.start();
	timer_calibration_.start();
#ifdef LOG_ACCURACY
	timer_accuracy_.start();
#endif
}

void Glove::setCalibration(const Calibration calibration[3])
{
	hand_.marg_calibration_ = calibration[0];
	forearm_->marg_calibration_ = calibration[1];
	upper_arm_->marg_calibration_ = calibration[2];
}

void Glove::initializeHeading(const Vector<int16_t> a[3],
					          const Vector<int16_t> m[3])
{
	hand_.marg_rotation_->quaternion_ = Quaternion(
		(a[0] - hand_.margCalibration().a_offset) * (hand_.margCalibration().hasInvertedPlacement() ? Vector<float>(-1, -1, 1) : Vector<float>(1, 1, 1)) * hand_.margCalibration().a_scale,
		(m[0] * hand_.margCalibration().m_rotation - hand_.margCalibration().m_offset) * (hand_.margCalibration().hasInvertedPlacement() ? Vector<float>(-1, -1, 1) : Vector<float>(1, 1, 1))
	);
	forearm_->marg_rotation_->quaternion_ = Quaternion(
		(a[0] - forearm_->margCalibration().a_offset) * (forearm_->margCalibration().hasInvertedPlacement() ? Vector<float>(-1, -1, 1) : Vector<float>(1, 1, 1)) * forearm_->margCalibration().a_scale,
		(m[0] * forearm_->margCalibration().m_rotation - forearm_->margCalibration().m_offset) * (forearm_->margCalibration().hasInvertedPlacement() ? Vector<float>(-1, -1, 1) : Vector<float>(1, 1, 1))
	);
	upper_arm_->marg_rotation_->quaternion_ = Quaternion(
		(a[0] - upper_arm_->margCalibration().a_offset) * (upper_arm_->margCalibration().hasInvertedPlacement() ? Vector<float>(-1, -1, 1) : Vector<float>(1, 1, 1)) * upper_arm_->margCalibration().a_scale,
		(m[0] * upper_arm_->margCalibration().m_rotation - upper_arm_->margCalibration().m_offset) * (upper_arm_->margCalibration().hasInvertedPlacement() ? Vector<float>(-1, -1, 1) : Vector<float>(1, 1, 1))
	);
}

void Glove::update(const Readout& readout)
{
	const MARGConfiguration marg_config; // Use default values
	timer_readout_.stop();

	if (dropped_readouts_ >= num_readouts_to_drop_first) {
		const MARG marg_hand(marg_config, readout.handImu(), hand_.temperature(), hand_.margCalibration());
		const MARG marg_forearm(marg_config, readout.forearmImu(), forearm_->temperature(), forearm_->margCalibration());
		const MARG marg_upper_arm(marg_config, readout.upperArmImu(), upper_arm_->temperature(), upper_arm_->margCalibration());

		hand_.marg_rotation_->update(marg_hand, timer_readout_.duration());
		forearm_->marg_rotation_->update(marg_forearm, timer_readout_.duration());
		upper_arm_->marg_rotation_->update(marg_upper_arm, timer_readout_.duration());
		hand_.hand_translation_.update(marg_hand, timer_readout_.duration());

		hand_.thumb_.update(readout.flex[0]);
		hand_.index_finger_.update(readout.flex[1]);
		hand_.middle_finger_.update(readout.flex[2]);
		hand_.ring_finger_.update(readout.flex[3]);
		hand_.little_finger_.update(readout.flex[4]);

		is_capacitive_pin_touched_ = readout.is_touched == 1;

		readouts_++;
	}
	else {
		dropped_readouts_++;
	}

#ifdef LOG_PACKET_RATE
	if (readouts_ == LOG_PACKET_RATE_NUM_READOUTS + LOG_PACKET_RATE_NUM_DISCARDS) {
		exit(0);
	}

	ofstream packet_rate_file(packet_rate_filename_, ios_base::app);
	if (packet_rate_file && readouts_ >= LOG_PACKET_RATE_NUM_DISCARDS) {
		packet_rate_file << timer_readout_.duration() * 1000 << endl;
		packet_rate_file.close();
	}
#endif

#ifdef LOG_ACCURACY
	timer_accuracy_.stop();
	if (timer_accuracy_.duration() >= 1 && is_calibrated_) {
		ofstream file(LOG_ACCURACY_FILE, ios_base::app);
		if (file) {
			file << hand_.rotation().position().x() << " "
				<< hand_.rotation().position().y() << " "
				<< hand_.rotation().position().z() << " "
				<< hand_.translation().position().x() << " "
				<< hand_.translation().position().y() - 55 << " "
				<< hand_.translation().position().z() << " "
				<< forearm_->rotation().position().x() << " "
				<< forearm_->rotation().position().y() << " "
				<< forearm_->rotation().position().z() << " "
				<< upper_arm_->rotation().position().x() << " "
				<< upper_arm_->rotation().position().y() << " "
				<< upper_arm_->rotation().position().z() << endl;
			file.close();
		}
		timer_accuracy_.start();
	}
#endif

	timer_readout_.start();

	if (!is_calibrated_) {
		timer_calibration_.stop();
		if (timer_calibration_.duration() >= calibration_delay_s) {
			shared_ptr<MARGRotationMadgwick> rotation_hand =
				dynamic_pointer_cast<MARGRotationMadgwick>(hand_.marg_rotation_);
			shared_ptr<MARGRotationMadgwick> rotation_forearm =
				dynamic_pointer_cast<MARGRotationMadgwick>(forearm_->marg_rotation_);
			shared_ptr<MARGRotationMadgwick> rotation_upper_arm =
				dynamic_pointer_cast<MARGRotationMadgwick>(upper_arm_->marg_rotation_);
			rotation_hand->setBeta(ahrs_beta_);
			rotation_forearm->setBeta(ahrs_beta_);
			rotation_upper_arm->setBeta(ahrs_beta_);
			rotation_hand->setZeta(ahrs_zeta_);
			rotation_forearm->setZeta(ahrs_zeta_);
			rotation_upper_arm->setZeta(ahrs_zeta_);
			hand_.marg_rotation_->calibrate();
			forearm_->marg_rotation_->calibrate();
			upper_arm_->marg_rotation_->calibrate();
			is_calibrated_ = true;
		}
	}
}

void Glove::update(const int16_t temperatures[3])
{
	hand_.temperature_ = { hand_.margCalibration().temperature, temperatures[0] };
	forearm_->temperature_ = { forearm_->margCalibration().temperature, temperatures[1] };
	upper_arm_->temperature_ = { upper_arm_->margCalibration().temperature, temperatures[2] };
}

void Glove::update(const int battery_charge, const bool is_used)
{
	battery_.setCharge(battery_charge, is_used);
}

Hand    Glove::hand()        const { return hand_; }
ArmPart Glove::forearm()    const { return *forearm_; }
ArmPart Glove::upperArm()    const { return *upper_arm_; }
Battery Glove::battery()     const { return battery_; }
int     Glove::readoutRate() const { return readout_rate_; }
bool    Glove::isCapacitivePinTouched() const { return is_capacitive_pin_touched_; }
