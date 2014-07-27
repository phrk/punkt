
function evShowcaseSliderDisp(punkturl, pid, adid) {
	
	xmlHttp = new XMLHttpRequest();
	xmlHttp.open( "GET", punkturl + "?evtype=fev&fid=1&ev=showdisp&pid=" + pid + "&adid=" + adid, true);
	
	xhr.onload = function (e) {
		if (xhr.readyState === 4) {
			if (xhr.status === 200) {
			} else {
			}
		}
	};
	xmlHttp.send( null );
}

function evShowcaseSliderItemDisp(punkturl, pid, adid, itemid) {
	
	xmlHttp.open( "GET", punkturl + "?evtype=fev&fid=1&ev=itemdisp&pid=" + pid + "&adid=" + adid + "&item=" + itemid, true);
	
	xhr.onload = function (e) {
		if (xhr.readyState === 4) {
			if (xhr.status === 200) {
			} else {
			}
		}
	};
	xmlHttp.send( null );
}