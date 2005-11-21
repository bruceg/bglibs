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

import os
import string
import sys
import time

def iif(cond, true, false):
    '''A functional equivalent to C's ?: operator.

    This function returns the value of 'true' if 'cond' evaluates to non-zero,
    or the value of 'false' otherwise.  Since it's a function, both 'true' and
    'false' are unconditionally evaluated (unlike 'cond?true:false' in C).
    '''
    if cond:
        return true
    else:
        return false

def escape(str, escapes):
    for (needle, replacement) in escapes:
        str = string.replace(str, needle, replacement)
    return str

html_escapes = ( ('&', '&amp;'),
                 ('<', '&lt;'),
                 ('>', '&gt;'),
                 ('"', '&quot;') )

def escape_html(str): return escape(str, html_escapes)

url_escapes = ( ('%', '%25'),
                (' ', '%20'),
                ('&', '%26'),
                ('?', '%3f'),
                (':', '%3a'),
                (';', '%3b'),
                ('+', '%2b') )

def escape_url(str):  return escape(str, url_escapes)

def nl2br(str): return string.replace(str, '\n', '<br>')

try:
	uri = os.environ['REQUEST_URI']
	try: base_uri = uri[string.rindex(uri, '/')+1:]
	except ValueError: base_uri = uri
	try: base_uri = base_uri[:string.index(base_uri, '?')]
	except ValueError: pass
except KeyError:
	uri = None
	base_uri = 'None'

def relink(_base_=base_uri, **args):
	args = args.items()
	args.sort()
	args = map(lambda val:"%s=%s"%(val[0],escape_url(str(val[1]))), args)
	return "%s?%s" % (_base_, string.join(args, '&'))

exports = {
    'iif': iif,
    'escape': escape,
    'html': escape_html,
	'nl2br': nl2br,
	'relink': relink,
    'url': escape_url,
	'sys': sys,
	'time': time,
    }
