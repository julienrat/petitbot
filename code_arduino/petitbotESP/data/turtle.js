/**
 * Created by zouxuan on 4/27/17.
 */

(function(){
    'use strict';

    var PIXEL_RATIO = window.devicePixelRatio || 1;
    var canvasPlaceholder = document.querySelector('.canvas-placeholder');
    var canvas = document.querySelector('.canvas');
    var script = document.querySelector('.script');
    var ctx = canvas.getContext('2d');
    var cos = Math.cos, sin = Math.sin, sqrt = Math.sqrt, PI = Math.PI;
    var DEGREE = PI / 180;
    var WIDTH, HEIGHT, position, direction, visible, pen, color;

    function onResize(evt){
        WIDTH = canvasPlaceholder.getBoundingClientRect().width * PIXEL_RATIO;
        HEIGHT = canvasPlaceholder.getBoundingClientRect().height * PIXEL_RATIO;
        canvas.setAttribute('width', WIDTH);
        canvas.setAttribute('height', HEIGHT);
        canvas.style.top = canvasPlaceholder.getBoundingClientRect().top + "px";
        canvas.style.left = canvasPlaceholder.getBoundingClientRect().left + "px";
        canvas.style.width = (WIDTH / PIXEL_RATIO) + "px"
        canvas.style.height = (HEIGHT / PIXEL_RATIO) + "px"
        if (evt){
            //Menu.runSoon();
        }
    }

    function reset(){
        recenter();
        direction = deg2rad(90); // facing "up"
        visible = true;
        pen = true; // when pen is true we draw, otherwise we move without drawing
        color = 'black';
    }

    function deg2rad(degrees){ return DEGREE * degrees; }

    function drawTurtle(){
        var userPen = pen; // save pen state
        if (visible){
            penUp(); _moveForward(5); penDown();
            _turn(-150); _moveForward(12);
            _turn(-120); _moveForward(12);
            _turn(-120); _moveForward(12);
            _turn(30);
            penUp(); _moveForward(-5);
            if (userPen){
                penDown(); // restore pen state
            }
        }
    }

    function drawCircle(radius){
        // Math for this is from http://www.mathopenref.com/polygonradius.html
        var userPen = pen; // save pen state
        if (visible){
            penUp(); _moveForward(-radius); penDown();
            _turn(-90);
            var steps = Math.min(Math.max(6, Math.floor(radius / 2)), 360);
            var theta = 360 / steps;
            var side = radius * 2 * Math.sin(Math.PI / steps);
            _moveForward(side / 2);
            for (var i = 1; i < steps; i++){
                _turn(theta); _moveForward(side);
            }
            _turn(theta); _moveForward(side / 2);
            _turn(90);
            penUp(); _moveForward(radius); penDown();
            if (userPen){
                penDown(); // restore pen state
            }
        }
    }


    function _moveForward(distance){
        var start = position;
        position = {
            x: cos(direction) * distance * PIXEL_RATIO + start.x,
            y: -sin(direction) * distance * PIXEL_RATIO + start.y
        };
        if (pen){
            ctx.lineStyle = color;
            ctx.beginPath();
            ctx.moveTo(start.x, start.y);
            ctx.lineTo(position.x, position.y);
            ctx.stroke();
            
        }
               

    }
    function stop(){
			var xmlHttp = new XMLHttpRequest();			
			xmlHttp.open( "GET", "http://192.168.4.1/stop", false ); // false for synchronous request
			xmlHttp.send( null );
			return xmlHttp.responseText;
	}
	function _avance(pas){
		sleep(450);	
		var xmlHttp = new XMLHttpRequest();
		var xmlHttp2 = new XMLHttpRequest();					
			xmlHttp.open( "GET", "http://192.168.4.1/step_avance", false ); // false for synchronous request		
			xmlHttp.send();
			
	}

	function _recule(pas){
		sleep(450);
			var xmlHttp = new XMLHttpRequest();			
			xmlHttp.open( "GET", "http://192.168.4.1/step_recule", false ); // false for synchronous request
			console.log("on avance");			
			xmlHttp.send();
			
				//return xmlHttp.responseText;
	}
	function _gauche(pas){
		sleep(450);
			var xmlHttp = new XMLHttpRequest();			
			xmlHttp.open( "GET", "http://192.168.4.1/step_gauche", false ); // false for synchronous request
			console.log("on avance");			
			xmlHttp.send();
			
				//return xmlHttp.responseText;
	}
	function _droite(pas){
		sleep(450);
			var xmlHttp = new XMLHttpRequest();			
			xmlHttp.open( "GET", "http://192.168.4.1/step_droite", false ); // false for synchronous request
			console.log("on avance");			
			xmlHttp.send();
			
				//return xmlHttp.responseText;
	}

	
    function penUp(){ pen = false; }
    function penDown(){
	


		pen = true;
		}
		
    function hideTurtle(){		
			var xmlHttp = new XMLHttpRequest();			
			xmlHttp.open( "GET", "http://192.168.4.1/led1_off", false ); // false for synchronous request
			xmlHttp.send( null );
			return xmlHttp.responseText;	
		}
		
    function showTurtle(){
			var xmlHttp = new XMLHttpRequest();
				xmlHttp.open( "GET", "http://192.168.4.1/led1_on", false ); // false for synchronous request
				xmlHttp.send( null );
			return xmlHttp.responseText;
			
					
		}

		function _delay(temps){
			sleep(temps);			
		}
		
		function sleep(delay) {
			var start = new Date().getTime();
			while (new Date().getTime() < start + delay);
		}
    function forward(block){ _moveForward(Block.value(block)); }
    function back(block){ _moveForward(-Block.value(block)); }
    function delay(block){ _delay(Block.value(block)); }
    function circle(block){ drawCircle(Block.value(block)); }
    function _turn(degrees){ direction += deg2rad(degrees); }
    function left(block){ _turn(Block.value(block)); }
    function right(block){ _turn(-Block.value(block)); }
    function avance(block){ _avance(Block.value(block)); }
    function recule(block){ _recule(Block.value(block)); }
    
    function recenter(){ position = {x: WIDTH/2, y: HEIGHT/2}; }

    function clear(){
        ctx.save();
        ctx.fillStyle = 'white';
        ctx.fillRect(0,0,WIDTH,HEIGHT);
        ctx.restore();
        reset();
        ctx.moveTo(position.x, position.y);
    }

    onResize();
    clear();
    //drawTurtle();

    Menu.item('Tourne à gauche', _gauche);
    Menu.item('Tourne à droite', _droite);
   // Menu.item('Avance', forward, 10, 'steps');
   // Menu.item('Recule', back, 10, 'steps');
   // Menu.item('Circle', circle, 20, 'radius');
    Menu.item('Stop', stop);
    Menu.item('Avance un pas', _avance);
     Menu.item('Recule un pas', _recule);
   // Menu.item('Back to center', recenter);
    Menu.item('Led1 Off', hideTurtle);
    Menu.item('Led1 On', showTurtle);
	Menu.item('Delais', delay,100,'temps');
	
    script.addEventListener('beforeRun', clear, false); // always clear canvas first
    script.addEventListener('afterRun', drawTurtle, false); // show turtle if visible
    window.addEventListener('resize', onResize, false);
    document.querySelector('.run-action').addEventListener('click', Menu.runSoon, false);
   

})(window);
