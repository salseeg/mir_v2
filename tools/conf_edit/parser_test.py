import xml


class my_handler(xml.sax.handler.ContentHandler):
	def startElement(name, attr):
		print name, attr

	def endElement(name):
		print "/" + name
		

xrdr = None

def parse(conf_name):
	xrdr = xml.sax.make_parser()
	hdlr = my_handler
	xml.sax.parse(conf_name, hdlr)


parse("/usr/share/mir/mir.conf.xml")
	
