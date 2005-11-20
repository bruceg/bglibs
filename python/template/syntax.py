# Copyright (C) 2000 Bruce Guenter <bruce@untroubled.org>
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

import os
import re

import context
import parser

path = [ '' ]

###############################################################################
# Node types

class Node:
	pass

def indent(level): return ' - ' * level

def tabs(level, str): return '%s%s' % ('\t' * level, str)

def optimize(list):
	for i in range(len(list)):
		list[i] = list[i].optimize()
	return list

class Body(Node):
	def __init__(self, list=None):
		self.list = list or [ ]
	def __repr__(self):
		return "Body(%s)" % repr(self.list)
	def append(self, node):
		self.list.append(node)
	def showtree(self, level):
		print indent(level), "Body"
		for item in self.list:
			item.showtree(level+1)
	def optimize(self):
		optimize(self.list)
		if len(self.list) == 1:
			return self.list[0]
		return self
	def execute(self, context):
		for item in self.list:
			item.execute(context)

class Section(Node):
	'''Encapsulates an optionally formatted text section'''
	def __init__(self, string):
		self.section = string
	def __repr__(self):
		return "Section(%s)" % repr(self.section)
	def showtree(self, level):
		print indent(level), "Section(%d bytes)" % len(self.section)
	def optimize(self): return self
	def execute(self, context):
		return context.out.write(self.section % context)

class End(Node):
	'''Ends a block node, like Start, Foreach, and If'''
	rx = re.compile(r'^end(\s+.*|)$')
	def __init__(self, ignored):
		pass

class Exit(Node,Exception):
	'''Causes parsing to stop'''
	rx = re.compile(r'^exit$')
	def __init__(self):
		pass
	def __repr__(self):
		return "Exit()"
	def showtree(self, level):
		print indent(level), "Exit"
	def optimize(self): return self
	def execute(self, context):
		raise self

class If(Node):
	rx = re.compile(r'^if\s+(.+)$')
	def __init__(self, expr, body=None, else_node=None, elseifs=None):
		self.expr_str = expr
		self.expr = compile(expr, 'template', 'eval')
		self.body = body
		self.else_node = else_node
		self.elseifs = elseifs or [ ]
	def __repr__(self):
		return "If(%s, %s, %s, %s)" % (repr(self.expr_str),
									   repr(self.body),
									   repr(self.else_node),
									   repr(self.elseifs))
	def append_elseif(self, node):
		self.elseifs.append(node)
	def showtree(self, level):
		print indent(level), "If", self.expr_str
		self.body.showtree(level+1)
		for elseif in self.elseifs:
			elseif.showtree(level)
		if self.else_node:
			self.else_node.showtree(level)
	def optimize(self):
		self.body = self.body.optimize()
		if self.else_node:
			self.else_node = self.else_node.optimize()
		optimize(self.elseifs)
		return self
	def execute(self, context):
		if context.eval(self.expr):
			return self.body.execute(context)
		for elseif in self.elseifs:
			if elseif.execute(context):
				return
		if self.else_node:
			return self.else_node.execute(context)

class Else(Node):
	'''Negates an If clause'''
	rx = re.compile(r'^else$')
	def __init__(self, body=None):
		self.body = body
	def __repr__(self):
		return "Else(%s)" % repr(self.body)
	def showtree(self, level):
		print indent(level), "Else"
		self.body.showtree(level+1)
	def optimize(self):
		self.body = self.body.optimize()
		return self
	def execute(self, context):
		return self.body.execute(context)

class ElseIf(Node):
	'''Additional clauses to an If clause'''
	rx = re.compile(r'^else\s+if\s+(.+)$')
	def __init__(self, expr, body=None):
		self.expr_str = expr
		self.expr = compile(expr, 'template', 'eval')
		self.body = body
	def __repr__(self):
		return "ElseIf(%s, %s)" % (repr(self.expr_str), repr(self.body))
	def showtree(self, level):
		print indent(level), "ElseIf", self.expr_str
		self.body.showtree(level+1)
	def optimize(self):
		self.body = self.body.optimize()
		return self
	def execute(self, context):
		if context.eval(self.expr):
			self.body.execute(context)
			return 1
		return None

class For(Node):
	'''Iterates over each item in a list'''
	rx = re.compile(r'^for\s+(\w+)\s+in\s+(.+)$')
	def __init__(self, var, expr, body=None):
		self.var = var
		self.expr_str = expr
		self.expr = compile(expr, 'template', 'eval')
		self.body = body
	def __repr__(self):
		return "For(%s, %s, %s)" % (repr(self.var),
									repr(self.expr_str),
									repr(self.body))
	def showtree(self, level):
		print indent(level), "For", self.var, "in", self.expr_str
		self.body.showtree(level + 1)
	def optimize(self):
		self.body = self.body.optimize()
		return self
	def execute(self, context):
		list = context.eval(self.expr)
		for item in list:
			context[self.var] = item
			self.body.execute(context)

class Foreach(Node):
	'''Iterates over each item in a list of dicts'''
	rx = re.compile(r'^foreach\s+(.+)$')
	def __init__(self, expr, body=None):
		self.expr_str = expr
		self.expr = compile(expr, 'template', 'eval')
		self.body = body
	def __repr__(self):
		return "Foreach(%s, %s)" % (`self.expr_str`, `self.body`)
	def showtree(self, level):
		print indent(level), "Foreach", self.expr_str
		self.body.showtree(level + 1)
	def optimize(self):
		self.body = self.body.optimize()
		return self
	def execute(self, context):
		list = context.eval(self.expr)
		for item in list:
			context.update(item)
			self.body.execute(context)

_include_cache = { }
class Include(Node):
	'''Reads in and parses another file'''
	rx = re.compile(r'^include\s+(.+)$')
	def __init__(self, expr):
		self.expr_str = expr
		self.expr = compile(expr, 'template', 'eval')
	def __repr__(self):
		return "Include(%s)" % `self.expr_str`
	def showtree(self, level):
		print indent(level), "Include", self.expr_str
	def optimize(self): return self
	def execute(self, context):
		filename = context.eval(self.expr)
		try:
			tree = _include_cache[filename]
		except KeyError:
			tree = None
			for prefix in path:
				fullname = os.path.join(prefix, filename)
				try:
					content = open(fullname, 'r').read()
					tree = parser.parse(content)
					break
				except IOError:
					pass
			if tree is None:
				raise IOError, "Could not find file '%s'" % filename
		tree.execute(context)

class With(Node):
	'''Temporarily adds some symbols to the running context'''
	rx = re.compile(r'^with\s+(.+)$')
	def __init__(self, expr, body=None):
		self.expr_str = expr
		self.expr = compile(expr, 'template', 'eval')
		self.body = body
	def __repr__(self):
		return "With(%s, %s)" % (`self.expr_str`, `self.body`)
	def showtree(self, level):
		print indent(level), "With", self.expr_str
		self.body.showtree(level + 1)
	def optimize(self):
		self.body = self.body.optimize()
		return self
	def execute(self, context):
		context.push()
		dict = context.eval(self.expr)
		context.update(dict)
		self.body.execute(context)
		context.pop()

special_node_types = [ Else, ElseIf, End, Exit, For, Foreach, If,
					   Include, With ]

class Expr(Node):
	def __init__(self, expr):
		self.expr_str = expr
		self.expr = compile(expr, 'template', 'exec')
	def __repr__(self):
		return "Expr(%s)" % `self.expr_str`
	def showtree(self, level):
		print indent(level), "Expr", self.expr_str
	def optimize(self): return self
	def execute(self, context):
		return context.execute(self.expr)
