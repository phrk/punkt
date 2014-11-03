#include "VkProfile.h"

VkProfile::VkProfile(const std::string &_json_dump) {
	
	json_error_t error;
	json_t *root = json_loads(_json_dump.c_str(), 0, &error);
	
	json_t *j_uid = json_object_get(root, "uid");
	if (json_is_string(j_uid)) {
		id = json_string_value(j_uid);
	} else {
		std::cout << "VkProfile::VkProfile could not parse uid\n";
		throw "VkProfile::VkProfile could not parse uid";
	}
	
	json_t *j_first_name = json_object_get(root, "first_name");
	if (json_is_string(j_first_name)) {
		first_name = json_string_value(j_first_name);
	} else {
		std::cout << "VkProfile::VkProfile could not parse first_name\n";
		throw "VkProfile::VkProfile could not parse first_name";
	}
	
	json_t *j_last_name = json_object_get(root, "last_name");
	if (json_is_string(j_last_name)) {
		last_name = json_string_value(j_last_name);
	} else {
		std::cout << "VkProfile::VkProfile could not parse last_name\n";
		throw "VkProfile::VkProfile could not parse last_name";
	}
	
	has_sex = false;
	json_t *j_sex = json_object_get(root, "sex");
	if (json_is_integer(j_sex)) {
		has_sex = true;
		sex = json_integer_value(j_sex);
	} else {
		
	}
	
	bdate = "";
	json_t *j_bdate = json_object_get(root, "bdate");
	if (json_is_string(j_bdate)) {
		bdate = json_string_value(j_bdate);
	} else {
	}
	
	city = 0;
	json_t *j_city = json_object_get(root, "city");
	if (json_is_integer(j_city)) {
		city = json_integer_value(j_city);
	} else {	
	}
	
	country = 0;
	json_t *j_country = json_object_get(root, "country");
	if (json_is_integer(j_country)) {
		country = json_integer_value(j_country);
	} else {	
	}
	
	timezone = 0;
	json_t *j_timezone = json_object_get(root, "timezone");
	if (json_is_integer(j_timezone)) {
		timezone = json_integer_value(j_timezone);
	} else {	
	}
	
	photo_url = "";
	json_t *j_photo_url = json_object_get(root, "photo_url");
	if (json_is_string(j_photo_url)) {
		photo_url = json_string_value(j_photo_url);
	} else {
	}
	
	site = "";
	json_t *j_site = json_object_get(root, "site");
	if (json_is_string(j_site)) {
		site = json_string_value(j_site);
	} else {
	}
	
	university = "";
	json_t *j_university = json_object_get(root, "university");
	if (json_is_string(j_university)) {
		university = json_string_value(j_university);
	} else {
	}
	
	graduation = "";
	json_t *j_graduation = json_object_get(root, "graduation");
	if (json_is_string(j_graduation)) {
		graduation = json_string_value(j_graduation);
	} else {
	}
	
	json_t *j_friends = json_object_get(root, "friends");
	if (json_is_array(j_friends)) {
		for(int i = 0; i < json_array_size(j_friends); i++) {
			json_t *j_friend_i = json_array_get(j_friends, i);
			if (json_is_integer(j_friend_i)) {
				friends.push_back(inttostr(json_integer_value(j_friend_i)));
			}
		}
	}
	
	json_t *j_groups = json_object_get(root, "groups");
	if (json_is_array(j_groups)) {
		for(int i = 0; i < json_array_size(j_groups); i++) {
			json_t *j_group_i = json_array_get(j_groups, i);
			if (json_is_integer(j_group_i)) {
				groups.push_back(inttostr(json_integer_value(j_group_i)));
			}
		}
	}
}

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
