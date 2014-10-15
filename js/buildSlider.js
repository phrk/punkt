
function buildSlider240x400x3(pid, isdemo, punkturl, adid) {
	
	if (!isdemo)
		evShowcaseSliderDisp(punkturl, pid, adid);
	
	if (document.punkt_showcase_slider_cur_page == null)
		document.punkt_showcase_slider_cur_page = 0;
	
	if (document.punkt_showcase_slider_moving == null)
		document.punkt_showcase_slider_moving = 0;
	
	if (document.punkt_showcase_slider_clicked == null)
		document.punkt_showcase_slider_clicked = false;
	
	function onItemsPageDisplayed(page) {
		
		if (isdemo)
			return;
		
		for (i in page.items) {
			if (page.items[i] == false) {
				
				page.items[i] = true;
				evShowcaseSliderItemDisp(punkturl, pid, adid, i);
			}
		}
	}
	
	onItemsPageDisplayed(document.punkt_showcase_slider_items_displayed[pid].pages[0]);
	
	var Scroller = function(container, left, right) {
	    var self = this;
	    this.c = container;
	    this.l = left;
	    this.r = right;
	    // Define how to scroll
	    this.sID = 0; // Scroll ID so they don't fight eachother
	    
		this.prettyScroll = function(sto, sid) {
			
			
	        var mid = sid || ++this.sID,
	            c = self.c,
	            w = c.scrollWidth,
	            x = c.scrollLeft,
	            t = (sto===undefined?x:sto),
	            dx = 10;
	        if(mid !== self.sID) {
				//this.ismoving = false;
				document.punkt_showcase_slider_moving--;
				return;
			}
	        if(t > w) t = w;
	        else if (t < 0) t = 0;
	        if(Math.abs(t - x) <= dx) {
				//this.ismoving = false;
				document.punkt_showcase_slider_moving--;
				return c.scrollLeft = t;
			}
	        if(t > x) c.scrollLeft = x + dx;
	        else c.scrollLeft = x - dx;
			
			document.punkt_showcase_slider_moving++;
	        setTimeout(function(){self.prettyScroll(t, mid)},1);
			document.punkt_showcase_slider_moving--;
	    };
	    // Add functions to left&right buttons
	    function onLeftClick() {
			
			if (document.punkt_showcase_slider_moving != 0)
				return;
			
			document.punkt_showcase_slider_moving++;
			
			document.punkt_showcase_slider_cur_page--;
			if (document.punkt_showcase_slider_cur_page < 0) {
				document.punkt_showcase_slider_cur_page = (Object.keys(document.punkt_showcase_slider_items_displayed[pid].pages).length - 1);
	        	var w = self.c.offsetWidth;
	        	self.prettyScroll(self.c.scrollLeft + w * document.punkt_showcase_slider_cur_page);
			} else {
	        	var w = self.c.offsetWidth;
	        	self.prettyScroll(self.c.scrollLeft - w);
			}
			
			onItemsPageDisplayed(document.punkt_showcase_slider_items_displayed[pid].pages[document.punkt_showcase_slider_cur_page]);
	    }
		
		function onRightClick() {
			
			if (document.punkt_showcase_slider_moving != 0)
				return;
	
			document.punkt_showcase_slider_moving++;
	
			document.punkt_showcase_slider_cur_page++;
			if (document.punkt_showcase_slider_cur_page < Object.keys(document.punkt_showcase_slider_items_displayed[pid].pages).length) {
		
		        var w = self.c.offsetWidth;
		        self.prettyScroll(self.c.scrollLeft + w);
				onItemsPageDisplayed(document.punkt_showcase_slider_items_displayed[pid].pages[document.punkt_showcase_slider_cur_page]);
			}
			else {
				document.punkt_showcase_slider_cur_page = 0;
		        var w = self.c.offsetWidth;
		        self.prettyScroll(self.c.scrollLeft - w * (Object.keys(document.punkt_showcase_slider_items_displayed[pid].pages).length - 1));
			}
	    }
		
		left.addEventListener('click', function() {
			document.punkt_showcase_slider_clicked = true;
			onLeftClick();
		} ,false);
		
	    right.addEventListener('click', function() {
			document.punkt_showcase_slider_clicked = true;
			onRightClick();
		}, false);
		
		function simulateRightClick() {
			if (document.punkt_showcase_slider_clicked)
				return;
			onRightClick();
			setTimeout(function() {
				simulateRightClick();
			}, 3000);
		}
		
		setTimeout(function() {
			
			simulateRightClick();
		}, 5000);
		
		
	    // Remove Text from #container so we get inserted spaces
	    (function(e){
	        var i = e.childNodes.length;
	        while(i-->0) if(e.childNodes[i].nodeType === 3) e.removeChild(e.childNodes[i]);
	    })(container);
	};
	var myScroll = new Scroller(document.getElementById('punkt_place_' + pid + '_oshop_240_400_slider_container'),
								document.getElementById('punkt_place_' + pid + '_goLeft'),
								document.getElementById('punkt_place_' + pid + '_goRight'));
}

function buildSlider304x224x1(pid, isdemo, punkturl, adid) {
	
	if (!isdemo)
		evShowcaseSliderDisp(punkturl, pid, adid);
	
	if (document.punkt_showcase_slider_cur_page == null)
		document.punkt_showcase_slider_cur_page = 0;
	
	if (document.punkt_showcase_slider_moving == null)
		document.punkt_showcase_slider_moving = 0;
	
	if (document.punkt_showcase_slider_clicked == null)
		document.punkt_showcase_slider_clicked = false;
	
	function onItemsPageDisplayed(page) {
		
		if (isdemo)
			return;
		
		for (i in page.items) {
			if (page.items[i] == false) {
				
				page.items[i] = true;
				evShowcaseSliderItemDisp(punkturl, pid, adid, i);
			}
		}
	}
	
	onItemsPageDisplayed(document.punkt_showcase_slider_items_displayed[pid].pages[0]);
	
	var Scroller = function(container, left, right) {
	    var self = this;
	    this.c = container;
	    this.l = left;
	    this.r = right;
	    // Define how to scroll
	    this.sID = 0; // Scroll ID so they don't fight eachother
	    
		this.prettyScroll = function(sto, sid) {
			
			
	        var mid = sid || ++this.sID,
	            c = self.c,
	            w = c.scrollWidth,
	            x = c.scrollLeft,
	            t = (sto===undefined?x:sto),
	            dx = 10;
	        if(mid !== self.sID) {
				//this.ismoving = false;
				document.punkt_showcase_slider_moving--;
				return;
			}
	        if(t > w) t = w;
	        else if (t < 0) t = 0;
	        if(Math.abs(t - x) <= dx) {
				//this.ismoving = false;
				document.punkt_showcase_slider_moving--;
				return c.scrollLeft = t;
			}
	        if(t > x) c.scrollLeft = x + dx;
	        else c.scrollLeft = x - dx;
			
			document.punkt_showcase_slider_moving++;
	        setTimeout(function(){self.prettyScroll(t, mid)},1);
			document.punkt_showcase_slider_moving--;
	    };
	    // Add functions to left&right buttons
	    function onLeftClick() {
			
			if (document.punkt_showcase_slider_moving != 0)
				return;
			
			document.punkt_showcase_slider_moving++;
			
			document.punkt_showcase_slider_cur_page--;
			if (document.punkt_showcase_slider_cur_page < 0) {
				document.punkt_showcase_slider_cur_page = (Object.keys(document.punkt_showcase_slider_items_displayed[pid].pages).length - 1);
	        	var w = self.c.offsetWidth;
	        	self.prettyScroll(self.c.scrollLeft + w * document.punkt_showcase_slider_cur_page);
			} else {
	        	var w = self.c.offsetWidth;
	        	self.prettyScroll(self.c.scrollLeft - w);
			}
			
			onItemsPageDisplayed(document.punkt_showcase_slider_items_displayed[pid].pages[document.punkt_showcase_slider_cur_page]);
	    }
		
		function onRightClick() {
			
			if (document.punkt_showcase_slider_moving != 0)
				return;
	
			document.punkt_showcase_slider_moving++;
	
			document.punkt_showcase_slider_cur_page++;
			if (document.punkt_showcase_slider_cur_page < Object.keys(document.punkt_showcase_slider_items_displayed[pid].pages).length) {
		
		        var w = self.c.offsetWidth;
		        self.prettyScroll(self.c.scrollLeft + w);
				onItemsPageDisplayed(document.punkt_showcase_slider_items_displayed[pid].pages[document.punkt_showcase_slider_cur_page]);
			}
			else {
				document.punkt_showcase_slider_cur_page = 0;
		        var w = self.c.offsetWidth;
		        self.prettyScroll(self.c.scrollLeft - w * (Object.keys(document.punkt_showcase_slider_items_displayed[pid].pages).length - 1));
			}
	    }
		
		left.addEventListener('click', function() {
			document.punkt_showcase_slider_clicked = true;
			onLeftClick();
		} ,false);
		
	    right.addEventListener('click', function() {
			document.punkt_showcase_slider_clicked = true;
			onRightClick();
		}, false);
		
		function simulateRightClick() {
			if (document.punkt_showcase_slider_clicked)
				return;
			onRightClick();
			setTimeout(function() {
				simulateRightClick();
			}, 3000);
		}
		
		setTimeout(function() {
			
			simulateRightClick();
		}, 5000);
		
		
	    // Remove Text from #container so we get inserted spaces
	    (function(e){
	        var i = e.childNodes.length;
	        while(i-->0) if(e.childNodes[i].nodeType === 3) e.removeChild(e.childNodes[i]);
	    })(container);
	};
	var myScroll = new Scroller(document.getElementById('punkt_place_' + pid + '_oshop_304_224_slider_container'),
								document.getElementById('punkt_place_' + pid + '_goLeft'),
								document.getElementById('punkt_place_' + pid + '_goRight'));
}

function buildSlider4(pid, isdemo, punkturl, adid) {
	
}

function buildSlider(pid, isdemo, punkturl, adid, type) {
	
	if (type === '240-400-3') {
		buildSlider240x400x3(pid, isdemo, punkturl, adid);
	} else if (type === '304-224-1') {
		buildSlider304x224x1(pid, isdemo, punkturl, adid);
	} else if (type === '4') {
		buildSlider4(pid, isdemo, punkturl, adid);
	}
}
