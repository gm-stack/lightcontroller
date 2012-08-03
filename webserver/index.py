#!/usr/bin/env python2
import socket
import random

def index(req):
	return """
<html>
<head>
<script src="//ajax.googleapis.com/ajax/libs/jquery/1.7.2/jquery.min.js" type="text/javascript"></script>
<link rel="stylesheet" media="screen" type="text/css" href="colorpicker.css" />
<script type="text/javascript" src="js/colorpicker.js"></script>
<script>
$(document).ready(function() {
	$('#colorPickerHolder').ColorPicker({
		flat: true,
		onSubmit: function(hsb,hex,rgb,el) {
			$.get('http://dasblinkenlights.stackunderflow.com/index.py/setColour', {'r':rgb.r, 'g':rgb.g, 'b':rgb.b}, function(data) {
				$("#status").html(data);
			});
		}
	});

});
</script>
</head>
<body>
<p id="colorPickerHolder"></p><br>
<div id="status">Click the colour picker button in the bottom right to select a colour.</div>
</body>
</html>
"""

def setColour(req,r=None,g=None,b=None):
	if (r == None) and (g == None) and (b == None):
		return "Colour not set"
	r_val = int(r) if r != None else 0
	g_val = int(g) if g != None else 0
	b_val = int(b) if b != None else 0
	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	sock.sendto(chr(r_val)+chr(g_val)+chr(b_val), ("172.22.0.59", 54127))
	return "Lights set to %i,%i,%i" % (r_val, g_val, b_val)
