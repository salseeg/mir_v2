from Tkinter import *
from threading import *
import time

import locale

class Status_ui:
	def __init__(self, master, gui):
		locale.setlocale(locale.LC_ALL,"")
		self.gui = gui
		self.root = Toplevel(master)
		self.init_ui()
		self.stop_request = 0
		self.request__th = Thread(target = self.do_request)
		self.request__th.start()

	def init_ui(self, line_n = 12, con_n = 7):
		self.root.protocol("WM_DELETE_WINDOW", self.close_ui)
		#
		self.line__lbl = Label(self.root)
		self.line__lbl.config(text = "Линия [line]")
		self.line__lbl.grid()
		i = 0
		self.lines__lbls = []
		self.lines = []
		while i < line_n:
			self.lines.append(StringVar())
			self.lines[i].set("-")
			self.lines__lbls.append(Label(self.root))
			self.lines__lbls[i].config(textvariable = self.lines[i])
			self.lines__lbls[i].grid(row = 0, column = i + 1)
			i = i + 1
		#
		self.state__lbl = Label(self.root)
		self.state__lbl.config(text = "Состояние [state]")
		self.state__lbl.grid()
		i = 0
		self.state__lbls = []
		self.state = []
		while i < line_n:
			self.state.append(StringVar())
			self.state[i].set("-")
			self.state__lbls.append(Label(self.root))
			self.state__lbls[i].config(textvariable = self.state[i], font = ("-*-fixed-medium-r-normal-*-*-100-*-*-p-*-koi8-r"))
			self.state__lbls[i].grid(row = 1, column = i + 1, padx = 0, pady = 0)
			i = i + 1

		#
		self.con__lbl = Label(self.root)
		self.con__lbl.config(text = "Соединение [current_con]")
		self.con__lbl.grid()
		i = 0
		self.con__lbls = []
		self.con = []
		while i < line_n:
			self.con.append(StringVar())
			self.con[i].set("-")
			self.con__lbls.append(Label(self.root))
			self.con__lbls[i].config(textvariable = self.con[i])
			self.con__lbls[i].grid(row = 2, column = i + 1, padx = 0, pady = 0)
			i = i + 1

	def close_ui(self):
		self.stop_request = 1
		self.request__th.join()
		self.gui.status_window.set(0)
		self.gui.status_ui = None
		self.root.destroy()

	def do_request(self):
		while not self.stop_request:
			self.gui.m_proto.action_get_lines_states()
			self.gui.m_proto.action_get_connections_states()
			print "request done"
			time.sleep(0.3)
	
	def react_show_lines_states(self, data):
		print data
		strs = data.splitlines()
		ar = []
		for i in strs:
			p = i.split(":")
			r = []
			for j in p:
				r.append(j.strip())
			ar.append(r)
		#
		i = 0
		n = len(ar)
		while i < n:
			self.lines[i].set(ar[i][0])
			self.state[i].set(ar[i][1])
			self.con[i].set(ar[i][2])
			i = i + 1

		

	def react_show_connections_states(self, data):
		#self.connections_states.set(data)
		pass
		

