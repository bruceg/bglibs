#!/usr/bin/perl

$dir = $0;
$dir =~ s|/[^/]*$||o;
unshift @INC, $dir;

sub pod2doc {
    local($_) = @_;
    s{(\s)<([^>]+)>} {$1&lt;<b>$2</b>&gt;}go;
    s{I<([^>]+)>}    {<i>$1</i>}go;
    s{[CF]<([^>]+)>} {<tt>$1</tt>}go;
    "<p>$_</p>\n";
}

sub cstr2doc {
    local($_) = @_;
    s/\\"/"/go;
    s/"([^\"]+)"/C<$1>/g;
    &pod2doc($_);
}

sub section_head { "<h2>@_</h2>\n" }
sub paragraph {
    local($p,$first) = @_;
    "<p>$p<p>\n";
}

require 'cli_parse.pl';

print "<html>
<head>
<title>Man page for $program($section)</title>
</head>
<body>
<center><h1>Man page for $program($section)</h1></center><hr>
<h2>NAME</h2>
$program - $prefix
<h2>SYNOPSIS</h2>
<b>$program</b>
";
if(@options) {
    $first = 1;
    foreach $option (@options) {
	if($option->{short}) {
	    print "[<b>-$option->{short}";
	    print "=$option->{type}" if $option->{type};
	    print "</b>]\n";
	}
	if($option->{long}) {
	    print "[<b>--$option->{long}";
	    print "=$option->{type}" if $option->{type};
	    print "</b>]\n";
	}
    }
}
print "<h2>DESCRIPTION</h2>\n", pod2doc($description);
if(@options) {
    print "<h2>OPTIONS</h2>\n<dl>\n";
    foreach $option (@options) {
	print '<dt><b>';
	if($option->{short}) {
	    print "-$option->{short}";
	    print " $option->{type}" if $option->{type};
	}
	print "</b>, <b>" if $option->{short} && $option->{long};
	if($option->{long}) {
	    print "--$option->{long}";
	    print "=$option->{type}" if $option->{type};
	}
	print "</b>\n";
	print "<dd>", $option->{desc};
    }
    print "</dl>\n";
}
print $notes;
print "<h2>AUTHOR</h2>\n", pod2doc($author), "\n" if $author;
