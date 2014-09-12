#include "VkProfile.h"

VkProfile::VkProfile(const punkt::VkProfile &_pb) {
	
	id = _pb.id();
	first_name = _pb.first_name();
	last_name = _pb.last_name();
	
	if (_pb.has_sex()) {
		has_sex = true;
		sex = _pb.sex();
	}
	
	if (_pb.has_bdate())
		bdate = _pb.bdate();

	if (_pb.has_city())
		city = _pb.city();
	
	if (_pb.has_country())
		country = _pb.country();
	
	if (_pb.has_timezone())
		timezone = _pb.timezone();
	
	if (_pb.has_photo_url())
		photo_url = _pb.photo_url();
	
	if (_pb.has_site())
		site = _pb.site();
	
	if (_pb.has_university())
		university = _pb.university();
	
	if (_pb.has_graduation())
		graduation = _pb.graduation();
		
	for (size_t i = 0; i<_pb.friends_size(); i++)
		friends.push_back(_pb.friends(i));
	
	for (size_t i = 0; i<_pb.groups_size(); i++)
		groups.push_back(_pb.groups(i));
}

void VkProfile::dump(punkt::VkProfile *_pb) {
	
	_pb->set_id(id);
	_pb->set_first_name(first_name);
	_pb->set_last_name(last_name);
	
	if (has_sex)
		_pb->set_sex(sex);
	
	if (bdate != "")
		_pb->set_bdate(bdate);
	
	if (city != 0)
		_pb->set_city(city);
	
	if (country != 0)
		_pb->set_country(country);
	
	if (timezone != 0)
		_pb->set_timezone(timezone);
	
	if (photo_url != "")
		_pb->set_photo_url(photo_url);
	
	if (site != "")
		_pb->set_site(site);
	
	if (university != "")
		_pb->set_university(university);
	
	if (graduation != "")
		_pb->set_graduation(graduation);
	
	for (size_t i = 0; i<friends.size(); i++) {
		std::string *s = _pb->add_friends();
		*s = friends[i];
	}
	
	for (size_t i = 0; i<groups.size(); i++) {
		std::string *s = _pb->add_groups();
		*s = groups[i];
	}
}
