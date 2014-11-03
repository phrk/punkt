
function makeHttpsUrl(url) {
	
	return "https" + url.substring(4);
}

function renderShowcaseSlider240x400x3 (pid, show, formatter_args, format_files_path, click_url, https) {

	if (https) {
		format_files_path = makeHttpsUrl(format_files_path);
	}

	var left_button_img = format_files_path+ 'left.png';
	var right_button_img = format_files_path+ 'right.png';
	if (formatter_args.buttons == "minimalistic_white") {
		left_button_img = format_files_path+ 'left_w.png';
		right_button_img = format_files_path+ 'right_w.png';
	}

	var bgimage = "none";
	
	if (formatter_args.bgimage != null) {
	
		if (formatter_args.bgimage == "atomic_explosion_240")
			bgimage = format_files_path + 'atomic_explosion_240.jpg';
	
		if (formatter_args.bgimage == "pale_blue_dot_240")
			bgimage = format_files_path + 'pale_blue_dot_240.jpg';
	}
	
	if (bgimage != "none")
		bgimage = 'background=' + bgimage;

	var ret = '<link rel="stylesheet" type="text/css" href="' +format_files_path+ 'slider.css"> \
		<div class="oshop_240_400_slider"> \
		<table bgcolor=' + formatter_args.bgcolor + ' ' + bgimage +  ' ><tr><td> \
		<img id="punkt_place_' + pid + '_goLeft" src=' + left_button_img + ' class="oshop_240_400_slider_goleft"/> \
		<img id="punkt_place_' + pid + '_goRight" src=' + right_button_img + ' class="oshop_240_400_slider_goright"/> \
		<div id="punkt_place_' + pid + '_oshop_240_400_slider_container" class="oshop_240_400_slider_container">';
	
	var nitem = 0;
	var nitem_in_page = 0;
	var cur_page = 0;
	
	ret += '	<div class="oshop_240_400_slider_content" id="content-place-' + pid + '-0" >';
	ret += '<table>';
	
	for (var i in show.items) {
	
		ret += '<tr>';
		ret += '<td style="white-space: normal;">';
		
		if (click_url != null)
			show.items[i].directurl = click_url + escape(show.items[i].directurl);
		
		var imgurl;
		if (formatter_args.imgq == "1")
			imgurl = show.items[i].imgname_q1;
		else if (formatter_args.imgq == "2")
			imgurl = show.items[i].imgname_q2;
		else if (formatter_args.imgq == "3")
			imgurl = show.items[i].imgname_q3;	
		
		if (https)
			ret += '<a href=' + show.items[i].directurl + ' target="_blank"><img src=' + makeHttpsUrl(imgurl) + ' class="oshop_240_400_slider_img"></a>';
		else
			ret += '<a href=' + show.items[i].directurl + ' target="_blank"><img src=' + imgurl + ' class="oshop_240_400_slider_img"></a>';
		
		ret += '</td>';
		ret += '<td><a href=' + show.items[i].directurl + ' style="font-family:' + formatter_args.textfont + ' ; font-size:11pt; color:' + formatter_args.textcolor 
				+ ';  overflow-wrap: break-word;" target="_blank" >';
				
		
		ret += show.items[i].caption + "</a><br>";
		
		
		if (formatter_args.show_shop_domain=="on") {
			ret += '<a href=' + show.items[i].directurl + ' style="font-size:10pt; color:' + formatter_args.domaincolor + ';" target="_blank">';
			ret += show.items[i].shop_domain + '</a>';
		}
		
		ret += '<br><a href=' + show.items[i].directurl + ' style="color:' + formatter_args.pricecolor + '; text-decoration:none;" target="_blank">';
		ret += show.items[i].price + 'руб';
		ret += '</a>';
		ret += '</td>';
		ret += '</tr>';

		if (document.punkt_showcase_slider_items_displayed == null) {
			document.punkt_showcase_slider_items_displayed = new Array();
		}
		
		if (document.punkt_showcase_slider_items_displayed[pid] == null) {
			document.punkt_showcase_slider_items_displayed[pid] = {};
			document.punkt_showcase_slider_items_displayed[pid].pages = {};
		}
		
		if (document.punkt_showcase_slider_items_displayed[pid].pages[cur_page] == null) {
			document.punkt_showcase_slider_items_displayed[pid].pages[cur_page] = {};
			document.punkt_showcase_slider_items_displayed[pid].pages[cur_page].items = {};
		}
		
		document.punkt_showcase_slider_items_displayed[pid].pages[cur_page].items[ show.items[i].id ] = false;
		
		nitem++;
		nitem_in_page++;
		
		if (nitem_in_page >= 3) {

			ret += '</table>';
			ret += '</div>';
			
			if (nitem == show.items.length)
				break;
			
			ret += ' <div class="oshop_240_400_slider_content" id="content-place-'+ pid + '-' + i +'">';
			ret += '<table>';
			nitem_in_page = 0;
			cur_page++;
		}
	}
		
	ret += '</table>';
	ret +=	'	</div>';
	ret +=	'</div> \
		</td></tr></table></div>';

	return ret;
}

function renderShowcaseSlider (pid, show, formatter_args, format_files_path, click_url, https, type) {
	
	if (type === "240-400-3") {
		return renderShowcaseSlider240x400x3(pid, show, formatter_args, format_files_path, click_url, https);
	} else if (type === "304-224-1") {
		return renderShowcaseSlider304x224x1(pid, show, formatter_args, format_files_path, click_url, https);
	} else if (type == "520-250-4") {
		return renderShowcaseSlider4(pid, show, formatter_args, format_files_path, click_url, https);
	} else {
		return "unknown slider type: " + type;
	}
}

function renderShowcaseSlider304x224x1 (pid, show, formatter_args, format_files_path, click_url, https) {
	
	if (https) {
		format_files_path = makeHttpsUrl(format_files_path);
	}

	var left_button_img = format_files_path+ 'left.png';
	var right_button_img = format_files_path+ 'right.png';
	if (formatter_args.buttons == "minimalistic_white") {
		left_button_img = format_files_path+ 'left_w.png';
		right_button_img = format_files_path+ 'right_w.png';
	}

	var ret = '<link rel="stylesheet" type="text/css" href="' +format_files_path+ 'slider.css"> \
		<div class="oshop_304_224_slider"> \
		<table bgcolor=' + formatter_args.bgcolor + '><tr><td> \
		<img id="punkt_place_' + pid + '_goLeft" src=' + left_button_img + ' class="oshop_304_224_slider_goleft"/> \
		<img id="punkt_place_' + pid + '_goRight" src=' + right_button_img + ' class="oshop_304_224_slider_goright"/> \
		<div id="punkt_place_' + pid + '_oshop_304_224_slider_container" class="oshop_304_224_slider_container">';
	
	var nitem = 0;
	var nitem_in_page = 0;
	var cur_page = 0;
	
	ret += '	<div class="oshop_304_224_slider_content" id="content-place-' + pid + '-0" >';
	ret += '<table>';
	
	for (var i in show.items) {
	
		ret += '<tr>';
		ret += '<td>';
		
		if (click_url != null)
			show.items[i].directurl = click_url + escape(show.items[i].directurl);
		
		var imgurl;
		if (formatter_args.imgq == "1")
			imgurl = show.items[i].imgname_q1;
		else if (formatter_args.imgq == "2")
			imgurl = show.items[i].imgname_q2;
		else if (formatter_args.imgq == "3")
			imgurl = show.items[i].imgname_q3;	
		
		if (https)
			ret += '<a href=' + show.items[i].directurl + ' target="_blank"><img src=' + makeHttpsUrl(imgurl) + ' class="oshop_304_224_slider_img"></a>';
		else
			ret += '<a href=' + show.items[i].directurl + ' target="_blank"><img src=' + imgurl + ' class="oshop_304_224_slider_img"></a>';
		
		ret += '</td>';
		ret += '<td><a href=' + show.items[i].directurl + ' style="font-family:' + formatter_args.textfont + ' ; font-size:12pt; color:' + formatter_args.textcolor 
				+ ';  overflow-wrap: break-word;" target="_blank" >';
				
		
		ret += show.items[i].caption + "<br>";
		
		if (formatter_args.show_shop_domain=="on") {
			ret += '<a href=' + show.items[i].directurl + ' style="font-size:10pt; color:' + formatter_args.domaincolor + ';" target="_blank">';
			ret += show.items[i].shop_domain + '</a>';
		}
		
		ret += '<br>';
		ret += '<a href="' + show.items[i].directurl + '" style="color:' + formatter_args.pricecolor + '; text-decoration:none;" target="_blank">' + show.items[i].price + 'руб </a>';
		ret += '</a></td>';
		ret += '</tr>';

		if (document.punkt_showcase_slider_items_displayed == null) {
			document.punkt_showcase_slider_items_displayed = new Array();
		}
		
		if (document.punkt_showcase_slider_items_displayed[pid] == null) {
			document.punkt_showcase_slider_items_displayed[pid] = {};
			document.punkt_showcase_slider_items_displayed[pid].pages = {};
		}
		
		if (document.punkt_showcase_slider_items_displayed[pid].pages[cur_page] == null) {
			document.punkt_showcase_slider_items_displayed[pid].pages[cur_page] = {};
			document.punkt_showcase_slider_items_displayed[pid].pages[cur_page].items = {};
		}
		
		document.punkt_showcase_slider_items_displayed[pid].pages[cur_page].items[ show.items[i].id ] = false;
		
		nitem++;
		nitem_in_page++;
		
		if (nitem_in_page >= 1) {

			ret += '</table>';
			ret += '</div>';
			
			if (nitem == show.items.length)
				break;
			
			ret += ' <div class="oshop_304_224_slider_content" id="content-place-'+ pid + '-' + i +'">';
			ret += '<table>';
			nitem_in_page = 0;
			cur_page++;
		}
	}
		
	ret += '</table>';
	ret +=	'	</div>';
	ret +=	'</div> \
		</td></tr></table></div>';

	return ret;
}

function renderShowcaseSlider4(pid, show, formatter_args, format_files_path, click_url, https) {
	
	if (https) {
		format_files_path = makeHttpsUrl(format_files_path);
	}

	var left_button_img = format_files_path+ 'left.png';
	var right_button_img = format_files_path+ 'right.png';
	if (formatter_args.buttons == "minimalistic_white") {
		left_button_img = format_files_path+ 'left_w.png';
		right_button_img = format_files_path+ 'right_w.png';
	}

	var bgimage = "none";
	
	if (formatter_args.bgimage != null) {
	
		if (formatter_args.bgimage == "atomic_explosion_240")
			bgimage = format_files_path + 'atomic_explosion_240.jpg';
	
		if (formatter_args.bgimage == "pale_blue_dot_240")
			bgimage = format_files_path + 'pale_blue_dot_240.jpg';
	}
	
	if (bgimage != "none")
		bgimage = 'background=' + bgimage;

	var ret = '<link rel="stylesheet" type="text/css" href="' +format_files_path+ 'slider.css"> \
		<div class="oshop_4_slider"> \
		<table bgcolor=' + formatter_args.bgcolor + ' ' + bgimage +  ' ><tr><td> \
		<div id="punkt_place_' + pid + '_oshop_4_slider_container" class="oshop_4_slider_container">';
	
	var nitem = 0;
	var nitem_in_page = 0;
	var cur_page = 0;
	
	ret += '	<div class="oshop_4_slider_content" id="content-place-' + pid + '-0" >';
	ret += '<table>';
	
	ret += '<tr>';
	
	for (var i in show.items) {
	
		ret += '<td>';
		
		if (click_url != null)
			show.items[i].directurl = click_url + escape(show.items[i].directurl);
		
		var imgurl;
		if (formatter_args.imgq == "1")
			imgurl = show.items[i].imgname_q1;
		else if (formatter_args.imgq == "2")
			imgurl = show.items[i].imgname_q2;
		else if (formatter_args.imgq == "3")
			imgurl = show.items[i].imgname_q3;	
		
		if (https)
			ret += '<a href=' + show.items[i].directurl + ' target="_blank"><img src=' + makeHttpsUrl(imgurl) + ' class="oshop_4_slider_img"></a>';
		else
			ret += '<a href=' + show.items[i].directurl + ' target="_blank"><img src=' + imgurl + ' class="oshop_4_slider_img"></a>';
		
		ret += '</td>';
		ret += '<td><a href=' + show.items[i].directurl + ' style="font-family:' + formatter_args.textfont + ' ; font-size:12pt; color:' + formatter_args.textcolor 
				+ ';  overflow-wrap: break-word;" target="_blank" >';
		
		ret += show.items[i].caption + "<br>";
		
		if (formatter_args.show_shop_domain=="on") {
			ret += '<a href=' + show.items[i].directurl + ' style="font-size:10pt; color:' + formatter_args.domaincolor + ';" target="_blank">';
			ret += show.items[i].shop_domain + '</a>';
		}
		
		ret += '<br><a href="' +show.items[i].directurl+ '" style="color:' + formatter_args.pricecolor + '; text-decoration:none;" target="_blank">' + show.items[i].price + 'руб </a>';
		ret += '</a></td>';
		

		if (document.punkt_showcase_slider_items_displayed == null) {
			document.punkt_showcase_slider_items_displayed = new Array();
		}
		
		if (document.punkt_showcase_slider_items_displayed[pid] == null) {
			document.punkt_showcase_slider_items_displayed[pid] = {};
			document.punkt_showcase_slider_items_displayed[pid].pages = {};
		}
		
		if (document.punkt_showcase_slider_items_displayed[pid].pages[cur_page] == null) {
			document.punkt_showcase_slider_items_displayed[pid].pages[cur_page] = {};
			document.punkt_showcase_slider_items_displayed[pid].pages[cur_page].items = {};
		}
		
		document.punkt_showcase_slider_items_displayed[pid].pages[cur_page].items[ show.items[i].id ] = false;
		
		nitem++;
		nitem_in_page++;
		
		if (nitem_in_page % 2 == 0 && nitem_in_page != 0) {
			
			ret += '</tr><tr>';
		}
		
		if (nitem_in_page >= 4) {

			ret += '</tr>';
			ret += '</table>';
			ret += '</div>';
			
			if (nitem == show.items.length)
				break;
			
			ret += ' <div class="oshop_4_slider_content" id="content-place-'+ pid + '-' + i +'">';
			ret += '<table>';
			nitem_in_page = 0;
			cur_page++;
		}
	}
		
	ret += '</table>';
	ret +=	'	</div>';
	ret +=	'</div> \
		</td></tr></table></div>';

	return ret;
}
