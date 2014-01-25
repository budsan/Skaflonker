#include "fighter.h"

#include <memory>
#include <fstream>
#include <json/json.h>

#include <debug.h>

Fighter::Fighter()
{

}

Fighter::~Fighter()
{

}

void Fighter::update()
{
	AnimatedSprite::update();
}

bool Fighter::loadDirectory(const std::string &path)
{
	std::shared_ptr<Library> lib = std::make_shared<Library>();
	if (!loadTrackFile(path+"/idle.json","idle", lib)) return false;
}

bool Fighter::loadTrackFile(const std::string& file, const std::string& name, std::shared_ptr<Library> &lib)
{
	std::ifstream sfile(file.c_str());
	if (!sfile.is_open()) {
		Guy::printLog("Error loading %s anim file", file.c_str());
		return false;
	}

	Json::Value root;
	Json::Reader reader;
	if (!reader.parse(sfile, root) ) {
		Guy::printLog("Failed to parse %s anim file, reason: %s", file.c_str(), reader.getFormatedErrorMessages().c_str());
		return false;
	}

	Track track;

	track.frameCount = root.get("frameCount", "UTF-8" ).asUInt();

	lib->trackNames.insert(std::make_pair(name, lib->tracks.size()));
	lib->tracks.push_back(track);
}
