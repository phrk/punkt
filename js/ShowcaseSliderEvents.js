
function evShowcaseSliderDisp(punkturl, pid, adid) {
	
	xmlHttp = new XMLHttpRequest();
	xmlHttp.open( "GET", punkturl + "?evtype=fev&fid=1&ev=showdisp&pid=" + pid + "&adid=" + adid, true);
	
	xmlHttp.onload = function (e) {
		if (xmlHttp.readyState === 4) {
			if (xmlHttp.status === 200) {
			} else {
			}
		}
	};
	xmlHttp.send( null );
}

function evShowcaseSliderItemDisp(punkturl, pid, adid, itemid) {
	
	xmlHttp = new XMLHttpRequest();
	xmlHttp.open( "GET", punkturl + "?evtype=fev&fid=1&ev=itemdisp&pid=" + pid + "&adid=" + adid + "&item=" + itemid, true);
	
	xmlHttp.onload = function (e) {
		if (xmlHttp.readyState === 4) {
			if (xmlHttp.status === 200) {
			} else {
			}
		}
	};
	xmlHttp.send( null );
}
