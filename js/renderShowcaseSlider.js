
function renderShowcaseSlider (pid, show, format_files_path) {

	var ret = '<link rel="stylesheet" type="text/css" href="' +format_files_path+ 'slider.css"> \
		<div class="oshop_240_400_slider"> \
		<table bgcolor=E6E6FA><tr><td> \
		<img id="punkt_place_' + pid + '_goLeft" src=' +format_files_path+ 'left.png class="oshop_240_400_slider_goleft"/> \
		<img id="punkt_place_' + pid + '_goRight" src=' +format_files_path+ 'right.png class="oshop_240_400_slider_goright"/> \
		<div id="punkt_place_' + pid + '_oshop_240_400_slider_container" class="oshop_240_400_slider_container">';
	
	var nitem = 0;
	var nitem_in_page = 0;
	
	ret += '	<div class="oshop_240_400_slider_content" id="content-place-' + pid + '-0" >';
	ret += '<table>';
	
	for (var i in show.items) {
	
		ret += '<tr>';
		ret += '<td>';
		ret += '<a href=' + show.items[i].clickurl + '><img src=' + show.items[i].imgurl + ' width=100px height=100px></a>';
		ret += '</td>';
		ret += '<td><a href=' + show.items[i].clickurl + ' style="font-family:serif; font-size:12pt; color:black;  word-wrap: break-word;" >';
		ret += show.items[i].caption + "<br>";
		ret += '<p style="color:A0522D; text-decoration:none;">' + show.items[i].price + 'руб </p>';
		ret += '</a></td>';
		ret += '</tr>';

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
		}
	}
		
	ret += '</table>';
	ret +=	'	</div>';
	ret +=	'</div> \
		</td></tr></table></div>';

	return ret;
}

