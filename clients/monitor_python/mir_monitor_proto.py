import mir_proto
import time

from threading import *

class Mir_monitor_proto:
	def __init__(self, ui):
		self.ui = ui
		self.stop = 0
		self.proto = None 
		self.run__th = None


	def connect(self, addr, port, str):
		if self.proto:
			throw("Already connected")
		try:
			self.proto = mir_proto.Mir_proto(addr,port,str)
		except:
			self.proto = None
			print "Failed to connect"
		self.stop = 0;
		self.run__th = Thread(target = self.__run)
		self.run__th.start()
		#print "m_proto.run__th.start"

	def disconnect(self,silent = 0):
		if self.proto:
			self.stop = 1
			self.run__th.join();
			self.proto.disconnect(silent)
			self.proto = None
		else:
			throw("Not yet connected")
	
	def check_connection(self):
		"""check if connected"""
		return (self.proto != None)
	
	def __run(self):
		while not self.stop:
			while 1:
				cmd = self.proto.receive_command()
				if cmd:
					self.__parse_command(cmd)
				else:
					break
				if self.stop:
					break
			time.sleep(0.1)
			#print "m_proto.__run"
	

	def __parse_command(self, cmd):
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
		print "line_states requested"
			
	def action_get_connections_states(self):
		self.proto.send_command([0x0002, ""])
			
	def action_get_config_file(self):
		self.proto.send_command([0x0003, ""])
			
	def action_error(self, data = ""):
		self.proto.send_command([0xfffe, data])

