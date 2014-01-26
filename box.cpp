#include "box.h"
#include <assert.h>

Box::Box(const math::vec3d &position)
{
	std::shared_ptr<Library> lib = std::make_shared<Library>();
	assert(loadTrackFile("data/animations/redbox/idle.json", "idle", lib));
	setLibrary(lib);
	ensureTrack("idle");
	setScale(1.5);
	setPosition(position);
}
