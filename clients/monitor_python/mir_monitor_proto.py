import mir_proto
import time

from threading import *

class Mir_monitor_proto:
	def __init__(self, ui):
		self.ui = ui
		self.stop = 0
		self.proto = None 
		self.proto__th = None


	def connect(self, addr, port, str):
		if self.proto:
			throw("Already connected")
		try:
			self.proto = mir_proto.Mir_proto(addr,port,str)
		except:
			self.proto = None
			print "Failed to connect"
		else:
			self.proto__th = Thread(target = self.proto.execute)
			self.proto__th.start()

	def disconnect(self,silent = 0):
		if self.proto:
			self.proto.disconnect(silent)
			self.proto__th.join()
			self.proto__th = None
			self.proto = None
		else:
			throw("Not yet connected")
	
	def check_connection(self):
		#"check_conn"
		if self.proto:
			return 1
		else:
			return 0
	
	def run(self):
		while not self.stop:
			if self.proto:
				while 1:
					cmd = self.proto.receive_command()
					if cmd:
						self.parse_command(cmd)
					else:
						break
					if self.stop:
						break
			time.sleep(0.01)
	
	def stop_running(self):
		self.stop = 1
	
	def start_running(self):
		self.stop = 0


	def parse_command(self, cmd):
		id = cmd[0]
		data = cmd[1]
		if id == 0x0001:
			print "pre lines"
			self.ui.react_show_lines_states(data)
			print "lines"
		elif id == 0x0002:
			self.ui.react_show_connections_states(data)
		elif id == 0x0003:
			pass
		elif id == 0x0004:
			pass
		elif id == 0xfffe:
			self.ui.react_show_error(data)
		elif id == 0xffff:
			print "pre react disconnect"
			self.ui.react_disconnect()
			print "react disconnect"
		else:
			# send error to station
			self.action_error("Unknown command "+ id)

	
	def action_shutdown(self):
		self.proto.send_command([0x0007, ""])
		#if self.ui.proto:
		#	self.ui.connect(1)

	def action_get_lines_states(self):
		self.proto.send_command([0x0001, ""])
			
	def action_get_connections_states(self):
		self.proto.send_command([0x0002, ""])
			
	def action_get_config_file(self):
		self.proto.send_command([0x0003, ""])
			
	def action_error(self, data = ""):
		self.proto.send_command([0xfffe, data])

  

