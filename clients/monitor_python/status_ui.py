from Tkinter import *
from threading import *
import time


def k2u(str):
	return unicode(str,"koi8-r")

class Status_ui:
	def __init__(self, master, gui):
		self.gui = gui
		self.root = Toplevel(master)
		self.init_ui()
		self.stop_request = 0
		self.request__th = Thread(target = self.do_request)
		self.request__th.start()

	def init_ui(self, line_n = 12, con_n = 7):
		self.menubar = Menu(self.root)
		#
		self.filemenubar = Menu(self.menubar, tearoff = 0)
		self.filemenubar.add_command(label = k2u("Перезапуск станции"))
		self.filemenubar.add_separator()
		self.filemenubar.add_command(label = k2u("Выход"))
		self.menubar.add_cascade(label = k2u("Файл"), menu = self.filemenubar)
		#
		self.root.config(menu = self.menubar)
		#
		self.root.protocol("WM_DELETE_WINDOW", self.close_ui)
		#
		self.line__lbl = Label(self.root)
		self.line__lbl.config(text = k2u("Линия"))
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
		self.state__lbl.config(text = k2u("Состояние"))
		self.state__lbl.grid()
		i = 0
		self.state__lbls = []
		self.state = []
		while i < line_n:
			self.state.append(StringVar())
			self.state[i].set("-")
			self.state__lbls.append(Label(self.root))
			self.state__lbls[i].config(textvariable = self.state[i], font = ("fixed"))
			self.state__lbls[i].grid(row = 1, column = i + 1, padx = 0, pady = 0)
			i = i + 1
		#
		self.con__lbl = Label(self.root)
		self.con__lbl.config(text = k2u("Соединение"))
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
		#
		self.incom__lbl = Label(self.root)
		self.incom__lbl.config(text = k2u("Вход. соединение"))
		self.incom__lbl.grid()
		i = 0
		self.incom__lbls = []
		self.incom = []
		while i < line_n:
			self.incom.append(StringVar())
			self.incom[i].set("-")
			self.incom__lbls.append(Label(self.root))
			self.incom__lbls[i].config(textvariable = self.incom[i])
			self.incom__lbls[i].grid(row = 3, column = i + 1, padx = 0, pady = 0)
			i = i + 1
		#
		self.num__lbl = Label(self.root)
		self.num__lbl.config(text = k2u("Номер"))
		self.num__lbl.grid()
		i = 0
		self.num__lbls = []
		self.num = []
		while i < line_n:
			self.num.append(StringVar())
			self.num[i].set("-")
			self.num__lbls.append(Label(self.root))
			self.num__lbls[i].config(textvariable = self.num[i])
			self.num__lbls[i].grid(row = 4, column = i + 1, padx = 0, pady = 0)
			i = i + 1
		#
		self.owner__lbl = Label(self.root)
		self.owner__lbl.config(text = k2u("Владелец"))
		self.owner__lbl.grid()
		i = 0
		self.owner__lbls = []
		self.owner = []
		while i < line_n:
			self.owner.append(StringVar())
			self.owner[i].set("-")
			self.owner__lbls.append(Label(self.root))
			self.owner__lbls[i].config(textvariable = self.owner[i])
			self.owner__lbls[i].grid(row = 5, column = i + 1, padx = 0, pady = 0)
			i = i + 1
		###########################
		i = 0
		self.cell = []
		self.cell__lbls = []
		while i < con_n:
			self.cell.append([])
			self.cell__lbls.append([])
			j = 0
			while j < line_n:
				self.cell[i].append(StringVar())
				self.cell[i][j].set("-")
				self.cell__lbls[i].append(Label(self.root))
				self.cell__lbls[i][j].config(textvariable = self.cell[i][j])
				self.cell__lbls[i][j].grid(row = 6 + i, column = j + 1, padx = 0, pady = 0)
				j = j + 1				
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
			#print "request done"
			time.sleep(1.5)
	
	def react_show_lines_states(self, data):
		#print data
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
			self.state[i].set(k2u(ar[i][1]))
			if (ar[i][2] < 0):
				self.con[i].set(k2u("нет"))
			else:
				self.con[i].set(ar[i][2])
			if (ar[i][3] < 0):
				self.incom[i].set(k2u("нет"))
			else:
				self.incom[i].set(ar[i][2])
			if (ar[i][4] == "(null)"):
				self.num[i].set("-")
			else:
				self.num[i].set(ar[i][2])
			if (ar[i][5] < 0):
				self.owner[i].set(k2u("нет"))
			else:
				self.owner[i].set(ar[i][2])
			i = i + 1

	def react_show_connections_states(self, data):
		#self.connections_states.set(data)
		strs = data.splitlines()
		for s in strs:
			#print s
			(head, tail) = s.split(':')
			#print '-'+tail+'-'
			#print head.split(' ')
			(p, p1, istr, q) = head.split(' ')
			try: 
				i = int(istr)
			except:
				print istr;
				i = 0

			if (tail.find('free') < 0):
				con_lines = tail.split(' ')
				for js in con_lines:
					try:
						j = int(js)
					except:
						pass
					else:
						#print i
						#print j
						self.cell[i-1][j].set('=#=')
			else:	
				for c in self.cell[i-1]:
					c.set('---')
		

