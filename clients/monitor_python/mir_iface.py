import socket
import sys
import time

def short2str(short):
	str = chr(short % 256) + chr(short / 256)
	return str

def long2str(long):
	str = short2str(long % 0x10000) + short2str(long / 0x10000)
	return str

def str2short(str,start_index):
	return ord(str[start_index]) + ord(str[start_index + 1]) * 0x100

def str2long(str, start_index):
	return str2short(str, start_index) + str2short(str, start_index + 2) * 0x10000


class Mir_iface:
	def __init__(self):
		self.buf = [""]
		self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	
	def disconnect(self):
		self.sock.shutdown(2)
		self.sock.close()

	def send_command(self, id, data):
		self.sock.send(short2str(id))
		self.sock.send(long2str(len(data)))
		if not data is None:
			self.sock.send(data)

	def recv_command(self):	
		self.sock.setblocking(0)
		try:
			s = self.sock.recv(2000)
		except:
			self.sock.setblocking(1)
			return []
		self.sock.setblocking(1)
		self.buf[0] = self.buf[0] + s
		l = len(self.buf[0])
		if l < 6 :		# sizeof(short) + sizeof(long)
			return []
		size = str2long(self.buf[0], 2)
		if l < 6 + size :
			return []
		id = str2short(self.buf[0], 0)
		data = self.buf[0][6:(5 + size)]
		rest = self.buf[0][(6 + size):]
		self.buf[0] = rest
		return [id, data]
			
	def recive_command(self):
		list = []
		while 1:
			list = self.recv_command()
			if list == []:
				time.sleep(0.01)
			else:
				break
		return list

	def connect(self, addr = "127.0.0.1", port = 3003):
		"подключается к серверу. 0 - ошибка подключения"
		try:
			self.sock.connect((addr, port))
		except socket.error:
			return 0
		return 1
