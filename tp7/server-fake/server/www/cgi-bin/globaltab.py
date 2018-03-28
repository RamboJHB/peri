#!/usr/bin/env python
class task:
	tab = [[0,0],[0,0],[0,0],[0,0],[0,0],[0,0],[0,0],[0,0],[0,0],[0,0],[0,0],[0,0],[0,0],[0,0],[0,0]]
	def set_value(self,time,value):
		global tab
		for i in range(1,14):
			self.tab[i-1][0] = self.tab[i][0]
			self.tab[i-1][1] = self.tab[i][1]
		self.tab[14][0] = time
		self.tab[14][1] = value

	def get_value(self):
		return self.tab
