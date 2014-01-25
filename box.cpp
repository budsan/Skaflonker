#include "box.h"
#include <assert.h>

Box::Box()
{
	std::shared_ptr<Library> lib = std::make_shared<Library>();
	assert(loadTrackFile("data/animations/redbox/idle.json", "idle", lib));
	setLibrary(lib);
	ensureTrack("idle");
}
