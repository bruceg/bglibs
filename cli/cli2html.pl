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

sub body_head {
    "<html>
<head>
<title>Man page for $program($section)</title>
</head>
<body>
<center><h1>Man page for $program($section)</h1></center><hr>
<h2>NAME</h2><ul>
<p>$program - $prefix</p>
</ul>
";
}
sub body_tail {
    "</body>\n</html>\n";
}

sub synopsis {
    my $s = "<p><b>$program</b>\n";
    if(@options) {
	my $first = 1;
	foreach $option (@options) {
	    if($option->{short}) {
		$s .= "[<b>-$option->{short}";
		$s .= "=$option->{type}" if $option->{type};
		$s .= "</b>]\n";
	    }
	    if($option->{long}) {
		$s .= "[<b>--$option->{long}";
		$s .= "=$option->{type}" if $option->{type};
		$s .= "</b>]\n";
	    }
	}
    }
    $s . "</p>\n";
}

sub options {
    my $s = "<dl>\n";
    foreach $option (@options) {
	$s .= '<dt><b>';
	if($option->{short}) {
	    $s .= "-$option->{short}";
	    $s .= " $option->{type}" if $option->{type};
	}
	$s .= "</b>, <b>" if $option->{short} && $option->{long};
	if($option->{long}) {
	    $s .= "--$option->{long}";
	    $s .= "=$option->{type}" if $option->{type};
	}
	$s .= "</b>\n<dd>$option->{desc}";
    }
    $s . "</dl>\n";
}

sub section_head { "<h2>@_</h2><ul>\n" }
sub section_tail { "</ul>\n" }

sub paragraph {
    local($p,$first) = @_;
    "<p>$p<p>\n";
}

require 'cli_parse.pl';
