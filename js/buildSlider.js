
function buildSlider(pid, isdemo, punkturl, adid) {
	
	if (!isdemo)
		evShowcaseSliderDisp(punkturl, pid, adid);
	
	var Scroller = function(container, left, right){
	    var self = this;
	    this.c = container;
	    this.l = left;
	    this.r = right;
	    // Define how to scroll
	    this.sID = 0; // Scroll ID so they don't fight eachother
	    this.prettyScroll = function(sto, sid){
	        var mid = sid || ++this.sID,
	            c = self.c,
	            w = c.scrollWidth,
	            x = c.scrollLeft,
	            t = (sto===undefined?x:sto),
	            dx = 10;
	        if(mid !== self.sID) return;
	        if(t > w) t = w;
	        else if (t < 0) t = 0;
	        if(Math.abs(t - x) <= dx) return c.scrollLeft = t;
	        if(t > x) c.scrollLeft = x + dx;
	        else c.scrollLeft = x - dx;
	        setTimeout(function(){self.prettyScroll(t, mid)},20);
	    };
	    // Add functions to left&right buttons
	    left.addEventListener('click',function(){
	        var w = self.c.offsetWidth;
	        self.prettyScroll(self.c.scrollLeft - w);
	    },false);
	    right.addEventListener('click',function(){
	        var w = self.c.offsetWidth;
	        self.prettyScroll(self.c.scrollLeft + w);
	    },false);
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
