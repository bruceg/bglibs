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

import sys

import context
import syntax
import lex

###############################################################################
# Parsing mechanism

def parse(content):
    '''Parse the content into nodes, and then order into a tree structure'''
    list = [ ]
    lexer = lex.Lexer(content)
    node = lexer.parse_node()
    while node is not None:
        list.append(node)
        node = lexer.parse_node()
    (node,index) = parse_body(list, 0)
    if index < len(list):
        raise ValueError, "Parsing ended before end-of-file"
    return node.optimize()

def parse_body(nodes, index):
    body = syntax.Body()
    (node,newindex) = parse_statement(nodes, index)
    while node:
        body.append(node)
        (node,newindex) = parse_statement(nodes, newindex)
    return (body,newindex)

def parse_statement(nodes, index):
	tmp = parse_text_section(nodes, index)
	if tmp[0]: return tmp
	tmp = parse_if_section(nodes, index)
	if tmp[0]: return tmp
	tmp = parse_for_section(nodes, index)
	if tmp[0]: return tmp
	tmp = parse_foreach_section(nodes, index)
	if tmp[0]: return tmp
	tmp = parse_include_section(nodes, index)
	if tmp[0]: return tmp
	tmp = parse_exit_section(nodes, index)
	if tmp[0]: return tmp
	tmp = parse_with_section(nodes, index)
	if tmp[0]: return tmp
	tmp = parse_other_section(nodes, index)
	if tmp[0]: return tmp
	return (None, index)

def parse_text_section(nodes, index):
    if index < len(nodes) and isinstance(nodes[index], syntax.Section):
        return (nodes[index], index+1)
    return (None, index)

def parse_if_section(nodes, index):
    if index < len(nodes) and isinstance(nodes[index], syntax.If):
        top = nodes[index]
        (body, newindex) = parse_body(nodes, index+1)
        if body:
            top.body = body
            while newindex < len(nodes) and \
                  isinstance(nodes[newindex], syntax.ElseIf):
                (body, tmpindex) = parse_body(nodes, newindex+1)
                if body:
                    nodes[newindex].body = body
                    top.append_elseif(nodes[newindex])
                    newindex = tmpindex
            if newindex < len(nodes) and \
			   isinstance(nodes[newindex], syntax.Else):
                (body, tmpindex) = parse_body(nodes, newindex+1)
                if body:
                    nodes[newindex].body = body
                    top.else_node = nodes[newindex]
                    newindex = tmpindex
            if newindex < len(nodes) and \
			   isinstance(nodes[newindex], syntax.End):
                return (top, newindex+1)
    return (None, index)

def parse_for_section(nodes, index):
    if index < len(nodes) and isinstance(nodes[index], syntax.For):
        top = nodes[index]
        (body, endindex) = parse_body(nodes, index+1)
        if body and \
           endindex < len(nodes) and \
		   isinstance(nodes[endindex], syntax.End):
            top.body = body
            return (top, endindex+1)
    return (None, index)

def parse_foreach_section(nodes, index):
    if index < len(nodes) and isinstance(nodes[index], syntax.Foreach):
        top = nodes[index]
        (body, endindex) = parse_body(nodes, index+1)
        if body and \
           endindex < len(nodes) and \
		   isinstance(nodes[endindex], syntax.End):
            top.body = body
            return (top, endindex+1)
    return (None, index)

def parse_include_section(nodes, index):
    if index < len(nodes) and isinstance(nodes[index], syntax.Include):
        return (nodes[index], index+1)
    return (None, index)

def parse_exit_section(nodes, index):
    if index < len(nodes) and isinstance(nodes[index], syntax.Exit):
        return (nodes[index], index+1)
    return (None, index)

def parse_with_section(nodes, index):
    if index < len(nodes) and isinstance(nodes[index], syntax.With):
        top = nodes[index]
        (body, endindex) = parse_body(nodes, index+1)
        if body and \
           endindex < len(nodes) and \
		   isinstance(nodes[endindex], syntax.End):
            top.body = body
            return (top, endindex+1)
    return (None, index)

def parse_other_section(nodes, index):
    if index < len(nodes) and isinstance(nodes[index], syntax.Expr):
        return (nodes[index], index+1)
    return (None, index)

def format(content, ctxt, out=sys.stdout):
    if not isinstance(ctxt, context.Context):
        ctxt = context.Context(out, ctxt)
    tree = parse(content)
    try:
        tree.execute(ctxt)
    except syntax.Exit:
        pass
