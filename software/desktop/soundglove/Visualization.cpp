#include "Visualization.h"
#include "Platform.h"
#include "Formatter.h"
#include <irrlicht.h>
#include <stdexcept>
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

/* Ignore "warning C4244: 'argument': conversion from 'double' to 'irr::f32',
 * possible loss of data"
*/
#ifdef OS_WINDOWS
#pragma warning( push )
#pragma warning( disable : 4244 )
#endif

const double Visualization::hand_arm_thickness = 10;
const double Visualization::hand_size = 10;

Visualization::Visualization(const Context& context,
							 const int width,
							 const int height)
:   glove_(context.glove),
	width_(width),
	height_(height),
	device_(nullptr),
	driver_(nullptr),
	scene_(nullptr),
	gui_(nullptr),
	camera_(nullptr),
	hand_box_(nullptr),
	forearm_box_(nullptr),
	upper_arm_box_(nullptr)
{
	device_ = createDevice(
		EDT_OPENGL,
		dimension2d<u32>(width_, height_),
		32,
		false,
		false,
		false,
		0);

	if (device_ != nullptr) {
		driver_ = device_->getVideoDriver();
		scene_ = device_->getSceneManager();
		gui_ = device_->getGUIEnvironment();
		font_ = device_->getGUIEnvironment()->getBuiltInFont();

		camera_ = scene_->addCameraSceneNode(0, vector3df(0, 100, 0), vector3df(0, 0, 0));
		upper_arm_box_ = scene_->addCubeSceneNode(1);
		forearm_box_ = scene_->addCubeSceneNode(1);
		hand_box_ = scene_->addCubeSceneNode(1);

		hand_box_->setScale(vector3df(hand_size, hand_size, hand_arm_thickness));
		forearm_box_->setScale(vector3df(glove_->forearm().length(), hand_arm_thickness, hand_arm_thickness));
		upper_arm_box_->setScale(vector3df(glove_->upperArm().length(), hand_arm_thickness, hand_arm_thickness));
		upper_arm_box_->setPosition(vector3df(0, 0, 0));

		ITexture* texture = driver_->addTexture(dimension2d<u32>(2, 2), "texture", ECF_A8R8G8B8);
		u32* texture_pixel = (u32*)texture->lock();
		texture_pixel[0] = SColor(0, 0, 0, 0).color;
		texture_pixel[1] = SColor(0, 255, 255, 255).color;
		texture_pixel[2] = SColor(0, 255, 255, 255).color;
		texture_pixel[3] = SColor(0, 0, 0, 0).color;
		texture->unlock();

		styleBox(hand_box_, texture);
		styleBox(forearm_box_, texture);
		styleBox(upper_arm_box_, texture);
	}
	else {
		throw runtime_error(Formatter() << "Failed to initiailize Visualization!");
	}
}

void Visualization::styleBox(irr::scene::IMeshSceneNode* box, ITexture* texture)
{
	box->setMaterialFlag(EMF_LIGHTING, false);
	box->setMaterialTexture(0, texture);
	box->setMaterialType(EMT_SOLID);
}

Visualization::~Visualization()
{
	if (device_ != nullptr) {
		device_->drop();
	}
}

void Visualization::update()
{
	if (device_->run()) {
		const double hand_size_flexed = hand_size * ((100 - glove_->hand().thumb().flex()) / 100);

		hand_box_->setRotation(vector3df(
			glove_->hand().rotation().position().x(),
			glove_->hand().rotation().position().z(),
			glove_->hand().rotation().position().y()
		));
		forearm_box_->setRotation(vector3df(
			glove_->forearm().rotation().position().x(),
			glove_->forearm().rotation().position().z(),
			glove_->forearm().rotation().position().y()
		));
		upper_arm_box_->setRotation(vector3df(
			glove_->upperArm().rotation().position().x(),
			glove_->upperArm().rotation().position().z(),
			glove_->upperArm().rotation().position().y()
			));

		hand_box_->setScale(vector3df(hand_size_flexed, hand_size, hand_arm_thickness));

		const vector3df position_forearm = vector3df(
			glove_->upperArm().toVector().y() / 2 + glove_->forearm().toVector().y() / 2,
			glove_->upperArm().toVector().z() / 2 + glove_->forearm().toVector().z() / 2,
			-(glove_->upperArm().toVector().x() / 2 + glove_->forearm().toVector().x() / 2)
		);
		const vector3df position_hand = position_forearm + vector3df(
			glove_->forearm().toVector().y() / 2 + hand_size_flexed / 2,
			glove_->forearm().toVector().z() / 2,
			-(glove_->forearm().toVector().x() / 2)
		);

		forearm_box_->setPosition(position_forearm);
		hand_box_->setPosition(position_hand);

		driver_->beginScene(true, true, SColor(255, 255, 255, 255));
		scene_->drawAll();
		gui_->drawAll();
		drawText();
		driver_->endScene();
	}
	else {
		throw runtime_error(Formatter() << "Failed to update visualization!");
	}
}

void Visualization::drawText()
{
	SColor black(255, 0, 0, 0);
	const int num_texts = 26;
	ostringstream texts[num_texts];
	std::string battery_usage_state;
	std::string battery_charge_state;

	for (int i = 0; i < num_texts; ++i) {
		texts[i] << fixed << setprecision(1);
	}

	switch (glove_->battery().usageState()) {
	case Battery::UsageState::PluggedIn: {
		battery_usage_state = "plugged in";
	} break;
	case Battery::UsageState::InUse: {
		battery_usage_state = "in use";
	} break;
	default: {
		battery_usage_state = "unknown";
	} break;
	}

	switch (glove_->battery().chargeState()) {
	case Battery::ChargeState::Charging: {
		battery_charge_state = "charging";
	} break;
	case Battery::ChargeState::Discharging: {
		battery_charge_state = "discharging";
	} break;
	case Battery::ChargeState::Full: {
		battery_charge_state = "full";
	} break;
	default: {
		battery_charge_state = "unknown";
	} break;
	}


	texts[0]  << "Hand";
	texts[1]  << "    " << glove_->hand().temperature().degreesCelsius();
	texts[2]  << "    Rotation";
	texts[3]  << "        x/roll  " << glove_->hand().rotation().position().x();
	texts[4]  << "        y/pitch " << glove_->hand().rotation().position().y();
	texts[5]  << "        z/yaw   " << glove_->hand().rotation().position().z();
	texts[6]  << "    Translation";
	texts[7]  << "        x       " << glove_->hand().translation().position().x();
	texts[8]  << "        y       " << glove_->hand().translation().position().y();
	texts[9]  << "        z       " << glove_->hand().translation().position().z();
	texts[10] << "Forearm";
	texts[11] << "    " << glove_->forearm().temperature().degreesCelsius();
	texts[12] << "    Rotation";
	texts[13] << "        x/roll  " << glove_->forearm().rotation().position().x();
	texts[14] << "        y/pitch " << glove_->forearm().rotation().position().y();
	texts[15] << "        z/yaw   " << glove_->forearm().rotation().position().z();
	texts[16] << "Upper Arm";
	texts[17] << "    " << glove_->upperArm().temperature().degreesCelsius();
	texts[18] << "    Rotation";
	texts[19] << "        x/roll  " << glove_->upperArm().rotation().position().x();
	texts[20] << "        y/pitch " << glove_->upperArm().rotation().position().y();
	texts[21] << "        z/yaw   " << glove_->upperArm().rotation().position().z();
	texts[22] << "Bend ";
	texts[23] << "    " << glove_->hand().thumb().flex();
	texts[24] << "Battery " << glove_->battery().percentage() << "% ("
		<< glove_->battery().charge() << "V, " << battery_usage_state << ", " << battery_charge_state << ")";
	texts[25] << (glove_->isCapacitivePinTouched() ? "Touched" : "Not touched");

	for (int i = 0; i < num_texts; ++i) {
		font_->draw(texts[i].str().c_str(), rect<s32>(0, i * 16, 16, 16), black);
	}
}
