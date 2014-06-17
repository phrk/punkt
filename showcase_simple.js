
_pid = "5";
_shid = "2";

function ShowcaseSimpleRender(shid) {
		
		var geber = new GeberCli("http://localhost:4243/showcase/");
		var res = geber.getShowcase(shid, 9);
		var view = "<table><tr>";
		for (var i in res.items) {
			
			view = view + "<td width=100 height=100 valign=top>";
			view = view + "<a href=" + res.items[i].clickurl + ">";
			view = view + "<img height=100px width=100px src=" + res.items[i].imgurl + "	</img><br>";
			view = view + res.items[i].caption + "<br>";
			view = view + "<b>"+ res.items[i].price + "руб </b><br>";
			view = view + "</a> id: " + res.items[i].id + "</td>";
		}
		view = view + "</tr></table>";
		view = view + "<br>Power:" + res.power;
		
		return view;
		//document.getElementById("showcase").innerHTML = view;
}

if (document._punkt_codes == null)
	document._punkt_codes = {};

document._punkt_codes[_pid] = function () {
	//document.getElementById("punkt_place_"+_pid).innerHTML =
	return ShowcaseSimpleRender(_shid);
}

//document.getElementById("punkt_place_5").innerHTML = "Sieg heil!";
