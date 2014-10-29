
function renderStaticImage(click_url, img_url, https) {
	
	
	return '<a href=' + click_url + ' target="_blank">'
			+ '<img src=' + img_url+ '> </img>'
			+ '</a>';
}

function staticImagePost(pid, demo, system_url, adid) {
	
	if (demo)
		return;
	
	var xmlHttp = new XMLHttpRequest();
	xmlHttp.open( "GET", system_url + "?evtype=tev&fid=1&ev=disp&pid=" + pid + "&adid=" + adid, true);
	
	xmlHttp.onload = function (e) {
		if (xmlHttp.readyState === 4) {
			if (xmlHttp.status === 200) {
			} else {
			}
		}
	};
	xmlHttp.send( null );
}
