from Tkinter import *
from threading import *

import time, sched

import mir_monitor_proto

import status_ui
import msg_manager


class Ui(msg_manager.P_message_manager):
	def __init__(self):
		self.m_proto = mir_monitor_proto.Mir_monitor_proto(self)
		self.init_ui()
		self.status_ui = None
		msg_manager.P_message_manager.__init__(self)
		#self.command_list = []
		#self.cmd__th = Thread(target = self.cmd_run)
		#self.cmd__th.start()
		#self.cmd_stop = 0
	
	def init_ui(self):
		self.root = Tk()
		#
		self.toolbar = Frame(self.root)
		#
		self.status_window = IntVar()
		self.status__chk = Checkbutton(self.toolbar)
		self.status__chk["text"] = "status window"
		self.status__chk["variable"] = self.status_window
		self.status__chk.config(state = DISABLED, command = self.status_clicked)
		self.status__chk.grid(sticky = E+W)
		#
		self.shutdown__btn = Button(self.toolbar)
		self.shutdown__btn.config(text = "Shutdown", command = self.act_shutdown, state = DISABLED)
		self.shutdown__btn.grid(sticky = E+W)
		#
		self.toolbar.pack({"side":"left"})
		#
		self.separator = Frame(self.root)
		self.separator["width"] = 50
		self.separator.pack({"side":"left"})
		#
		self.config = Frame(self.root)
		#
		self.adress__lbl = Label(self.config)
		self.adress__lbl.config(text = "Address")
		self.adress__lbl.grid()
		#
		self.port__lbl = Label(self.config)
		self.port__lbl.config(text = "Port")
		self.port__lbl.grid()
		#
		self.address = StringVar()
		self.address.set("127.0.0.1")
		self.address__ent = Entry(self.config)
		self.address__ent.config(highlightbackground = "#777", bg = "#ffffff", bd = 1, textvariable = self.address)
		self.address__ent.grid(row = 0, column = 1, pady = 2, padx = 2)
		#
		self.port = StringVar()
		self.port.set("3003")
		self.port__ent = Entry(self.config)
		self.port__ent.config(highlightbackground = "#777", bg = "#ffffff", bd = 1, textvariable = self.port)
		self.port__ent.grid(row = 1, column = 1, pady = 2, padx = 2)
		#
		self.connect__btn_text = StringVar()
		self.connect__btn_text.set("Connect")
		self.connect__btn = Button(self.config)
		self.connect__btn.config(textvariable = self.connect__btn_text, command = self.connect_clicked)
		self.connect__btn.grid(row = 2, column = 0, columnspan = 2, pady = 2)
		#
		self.config.config(relief = RIDGE, bd = 2)
		self.config.pack()
		#
		self.root.protocol("WM_DELETE_WINDOW", self.close_ui)
	
	def status_clicked(self):
		print self.status_window.get()
		if self.status_window.get():
			self.status_ui = status_ui.Status_ui(self.root, self)
		else:
			self.status_ui.close_ui()
			self.status_ui = None

	def close_ui(self):
		self.disconnect()
		self.root.destroy()
		msg_manager.P_message_manager.done(self)
	
	def run(self):
		try:
			config_file = open("conf", "r")
		except:
			pass
		else:
			self.address.set(config_file.readline()[:-1])
			self.port.set(config_file.readline()[:-1])
			config_file.close()
		self.root.mainloop()
		try:
			config_file = open("conf", "w")
		except:
			pass
		else:
			config_file.write(self.address.get()+'\n')
			config_file.write(self.port.get()+'\n')
			config_file.close()
		self.disconnect()

	def dispatch(self):
		item = self.queue.pop(0)
		print item
		if item == "connect":
			self.connect()
		elif item == "disconnect":
			self.disconnect()

	def connect_clicked(self):
		if self.m_proto.check_connection():
			self.disconnect()
		else:
			self.connect()
	
	def disconnect(self,silent = 0):
		if self.m_proto.check_connection():
			if self.status_ui:
				print "closing status..."
				self.status_ui.close_ui()
			print "stoppping proto..."
			self.m_proto.disconnect(silent)
			print "disabling proto..."
			self.disable_proto_ui()
	
	def connect(self):
		if self.m_proto.check_connection():
			return
		self.m_proto.connect(self.address.get(), int(self.port.get()), "Python UI monitor")
		print "m_proto is connecting"
		if not self.m_proto.check_connection():
			print "Failed to connect", self.address.get(), int(self.port.get())
		else:
			self.enable_proto_ui()
				
	def enable_proto_ui(self):
		self.status__chk.config(state = NORMAL)
		self.shutdown__btn.config(state = NORMAL)
		self.connect__btn_text.set("Disconnect")

	def disable_proto_ui(self):
		if self.status_ui:
			self.status_ui.close_ui()
			self.status_ui = None
		self.status__chk.config(state = DISABLED)
		self.shutdown__btn.config(state = DISABLED)
		self.connect__btn_text.set("Connect")

	def act_shutdown(self):
		self.m_proto.action_shutdown()

	
	# proto handlers
	def react_show_error(self, data):
		# messagebox(data)
		pass
		
	def react_disconnect(self):
		#self.connect(1)
		self.send("disconnect")
		# messagebox("Disconnected by station")
		
	def react_show_lines_states(self,data):
		if self.status_ui:
			self.status_ui.react_show_lines_states(data)
		else:
			print data
	
	def react_show_connections_states(self,data):
		if self.status_ui:
			self.status_ui.react_show_connections_states(data)
		else:
			print data
	

	
