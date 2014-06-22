
function evShowcaseSliderDisp(punkturl, pid, adid) {
	
	xmlHttp = new XMLHttpRequest();
	xmlHttp.open( "GET", punkturl + "?evtype=fev&fid=1&ev=showdisp&pid=" + pid + "&adid=" + adid, false);
	xmlHttp.send( null );
}
