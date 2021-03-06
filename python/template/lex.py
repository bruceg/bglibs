# Copyright (C) 2000,2005 Bruce Guenter <bruce@untroubled.org>
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

import re

import syntax

###############################################################################
# Lexical analysis

_rx_begin_cmd = re.compile(r'(^\s+|)<\?\s*', re.MULTILINE)
_rx_end_cmd = re.compile(r'[ \t]*\?>[ \t]*\r?\n?')

class Lexer:
    def __init__(self, content):
        self.currpos = 0
        self.after_escape = 0
        self.content = content

    def parse_node(self):
        if self.currpos >= len(self.content):
            return None
        if self.after_escape:
            return self.parse_node_after()
        else:
            return self.parse_node_before()
    def parse_node_after(self):
        end = _rx_end_cmd.search(self.content, self.currpos)
        if not end:
            raise ValueError, "Unterminated command escape sequence"
        self.after_escape = 0
        cmd = self.content[self.currpos:end.start()]
        self.currpos = end.end()
        return self.parse_cmd(cmd)
    def parse_node_before(self):
        start = _rx_begin_cmd.search(self.content, self.currpos)
        if start:
            self.after_escape = 1
            endpos = start.start()
            nextpos = start.end()
        else:
            endpos = len(self.content)
            nextpos = endpos
        if self.currpos == endpos:
            self.currpos = nextpos
            return self.parse_node_after()
        result = syntax.Section(self.content[self.currpos:endpos])
        self.currpos = nextpos
        return result
    def parse_cmd(self, cmdstr):
        for type in syntax.special_node_types:
            match = type.rx.match(cmdstr)
            if match:
				return apply(type, match.groups())
        return syntax.Expr(cmdstr)
