# $Id$

Mapnik OGC Server
-----------------


Introduction
------------

	Mapnik provides a server package to allow the publishing of maps
through the open and standard WMS interface published by the Open Geospatial
Consortium (OGC).  It is in implemented in Python, around the core C++
library.


Features/Caveats
----------------

- WMS 1.1.1 and 1.3.0
- CGI/FastCGI
- Supports all 3 requests: GetCapabilities, GetMap and GetFeatureInfo
- GetFeatureInfo supports text/plain output only
- JPEG/PNG output
- XML/INIMAGE/BLANK error handling
- Multiple named styles support
- Reprojection support
- Supported layer metadata: title, abstract
- Needs to be able to write to tempfile.gettempdir() (most likely "/tmp")


Dependencies
------------

Please properly install the following before proceeding further:

- jonpy (http://jonpy.sourceforge.net/)
- lxml (http://codespeak.net/lxml/)
- PIL (http://www.pythonware.com/products/pil)
- PROJ.4 (http://proj.maptools.org/)


Installation
------------

- Make sure Mapnik was compiled and linked with PROJ.4 support.  If this isn't
  the case, recompile Mapnik and make sure it is.

- The executable "ogcserver" in utils/ogcserver will work for both CGI and
  FastCGI operations.  Where to place it will depend on your server's
  configuration and is beyond this documentation.  For information on FastCGI
  go to http://www.fastcgi.com/.


Configuring the server
----------------------

- You will need to edit the ogcserver executable for now.  It is a simple
  Python text script.
  
  1) Edit the path to the interpreter in the first line.
  2) Edit the path to the config file if you don't like the default.
  
- Copy the sample configuration "ogcserver.conf" file in utils/ogcserver to
  the location you specified in the previous step.
  
- Edit the configuration file to your liking, the comments within the file will
  help you further.  Be sure to at the very minimum edit the "module"
  parameter, the server will not work without you setting it properly first.
  

Defining layers and styles for use by the ogcserver
---------------------------------------------------

	The ogcserver obviously needs layers to publish.  For now, with Mapnik, this
can only be done by writing code. In this case, a Python script will need to be
written to describe the layers and respective styles.  For information on the Python
API, look in demo/python, or in docs/epydocs.

The server needs a python module, with code that looks like this:

from mapnik.ogcserver.WMS import BaseWMSFactory
from mapnik import Layer, Style

class WMSFactory(BaseWMSFactory):

	def __init__(self):
		BaseWMSFactory.__init__(self)
		sty = Style()
		...
		self.register_style('stylename', sty)
		
		lyr = Layer('layername', '+init=epsg:4326')
		lyr.title = 'Layer title'
		lyr.abstract = 'Layer abstract'
		...
		self.register_layer(lyr, 'stylename')
		self.finalize()
		
The rules for writing this class are:

- It MUST be called 'WMSFactory'.
- It MUST sub-class mapnik.ogcserver.WMS.BaseWMSFactory.
- The __init__ MUST call the base class'.
- Layers MUST be named with the first parameter to the constructor.
- Layers MUST define an EPSG projection in the second parameter to the
  constructor.  This implies that the underlying data must be in an EPSG
  projection already.
- style and layer names are meant for machine readability, not human.  Keep
  them short and simple, without spaces or special characters.
- For human readable info, set the title and abstract properties on the layer
  object.
- DO NOT register styles using layer.styles.append(), instead, provide style
  information to the register_layer() call:
  
  register_layer(layerobject, defaultstylename, tuple of alternative style names)

- No Map() object is used or needed here.
- Be sure to call self.finalize() once you've registered everything! This will
  validate everything and let you know if there's problems.
- For a layer to be queryable via GetFeatureInfo, simply set the 'queryable'
  property to True:
  
  lyr.queryable = True


To Do
-----

- Investigate moving to cElementTree from lxml.
- Add some internal "caching" for performance improvements.
- Switch to using C/C++ libs for image generation, instead of PIL (also
  requires core changes). PIL requirement will remain for INIMAGE/BLANK
  error handling.
- Implement other connectors than CGI/FastCGI (Such as WSGI, SCGI, etc ...)


Conclusion
----------

	This is the very first implementation of a WMS for Mapnik.  Although inital
testing seems to suggest it works well, there may be bugs, and it lacks some
useful features.  Comments, contributions, and requests for help should all be
directed to the Mapnik mailing list.

Enjoy!
J.F.
