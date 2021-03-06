import mir_iface
import time

from threading import *

class Mir_proto:
	def __init__(self, addr, port, client, plugin = "monitor.so"):
		self.iface = mir_iface.Mir_iface()
		try:
			self.iface.connect(addr, port)
		except:
			throw("Failed to connect")
		else:
			self.iface.send_command(0, plugin + ':' + client + '\000')
			self.stop_executing = 0
			self.got_quit = 0
			self.serv__th = Thread(target = self.__execute)
			self.serv__th.start()
		self.recieved = []
		self.to_send = []
	
	def disconnect(self,silent = 0):
		if not self.got_quit:
			self.iface.send_command(0xffff,"")
		while not self.got_quit:
			time.sleep(0.01)
		self.stop_executing = 1
		self.serv__th.join()
		self.iface.disconnect()

	def __execute(self):
		while not self.stop_executing:
			#print "proto exec"
			while len(self.to_send):
				l = self.to_send.pop(0)
				self.iface.send_command(l[0], l[1])
			while 1:
				r = self.iface.recv_command()
				if r != [] :
					self.recieved.append(r)
					if r[0] == 0xffff:
						self.got_quit = 1
						print "got quit"
				else:
					break
			time.sleep(0.01)

	def send_command(self, c):
		self.to_send.append(c)

	def receive_command(self):
		if not len(self.recieved):
			return None
		return self.recieved.pop(0)

