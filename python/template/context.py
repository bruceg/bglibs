# Copyright (C) 2000 Bruce Guenter <bruceg@em.ca>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

import string
import sys

import functions

###############################################################################
# Context management

global_context = {
    'math': __import__('math'),
    'random': __import__('random'),
    're': __import__('re'),
    'regsub': __import__('regsub'),
    'string': __import__('string'),
    'time': __import__('time'),
    }
global_context.update(functions.exports)

class Context:
	def __init__(self, out = sys.stdout, initdict = { }):
		self.out = out
		self.stack = [ initdict ]
		self.pop()
	def pop(self):
		self.dict = self.stack.pop()
		self.get = self.dict.get
		self.has_key = self.dict.has_key
		self.items = self.dict.items
		self.keys = self.dict.keys
		self.values = self.dict.values
		self.update = self.dict.update
	def copy(self):
		return Context(self.out, self.dict.copy())
	def push(self):
		self.stack.append(self.dict.copy())
	def eval(self, body):
		global global_context
		return eval(body, global_context, self.dict)
	def execute(self, body):
		global global_context
		exec(body, global_context, self.dict)
	def __getitem__(self, key):
		try:
			return self.dict[key]
		except KeyError:
			return self.eval(key)
	#def __getattr__(self, key):
	#	return self.dict[key]
	def __setitem__(self, key, val):
		self.dict[key] = val
