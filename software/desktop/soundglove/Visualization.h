#pragma once

#include "Export.h"
#include "Context.h"

namespace irr {
	class IrrlichtDevice;
	namespace video {
		class IVideoDriver;
		class ITexture;
	}
	namespace scene {
		class ISceneManager;
		class ICameraSceneNode;
		class IMeshSceneNode;
	}
	namespace gui {
		class IGUIEnvironment;
		class IGUIFont;
	}
}

class Visualization {
public:
	SOUNDGLOVE_API Visualization(const Context& context,
								 const int width,
								 const int height);
	SOUNDGLOVE_API ~Visualization();

	SOUNDGLOVE_API void update();

private:
	static const double hand_arm_thickness;
	static const double hand_size;

	const std::shared_ptr<Glove> glove_;
	const int width_;
	const int height_;

	irr::IrrlichtDevice* device_;
	irr::video::IVideoDriver* driver_;
	irr::scene::ISceneManager* scene_;
	irr::gui::IGUIEnvironment* gui_;
	irr::gui::IGUIFont* font_;

	irr::scene::ICameraSceneNode* camera_;
	irr::scene::IMeshSceneNode* hand_box_;
	irr::scene::IMeshSceneNode* forearm_box_;
	irr::scene::IMeshSceneNode* upper_arm_box_;

	void styleBox(irr::scene::IMeshSceneNode* box, irr::video::ITexture* texture);
	void drawText();
};
