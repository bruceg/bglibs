#!/usr/bin/perl

$dir = $0;
$dir =~ s|/[^/]*$||o;
unshift @INC, $dir;

sub pod2doc {
    local($_) = @_;
    s/[FI]<([^>]+)>/\\fI$1\\fR/go;
    s/C<([^>]+)>/\\fB$1\\fR/go;
    s/<([^>]+)>/<\\fI$1\\fR>/go;
    $_;
}

sub cstr2doc {
    local($_) = @_;
    s/\\"/"/go;
    s/"([^\"]+)"/C<$1>/g;
    &pod2doc($_);
}

sub body_head {
    ".TH $program $section\n.SH NAME\n$program \- $prefix\n";
}
sub body_tail { ""; }

sub section_head { ".SH @_\n"; }
sub section_tail { ""; }

sub paragraph {
    local($_,$first) = @_;
    s/^/.P\n/ unless $first;
    $_;
}

sub synopsis {
    my $s = ".B $program\n";
    if(@options) {
	my $first = 1;
	foreach $option (@options) {
	    if($option->{short}) {
		$s .= " " unless $first;
		$first = 0;
		$s .= "[\n.B \-$option->{short} $option->{type}\n]";
	    }
	    if($option->{long}) {
		$s .= " " unless $first;
		$first = 0;
		$s .= "[\n.B \-\-$option->{long}";
		$s .= "=$option->{type}" if $option->{type};
		$s .= "\n]";
	    }
	}
	$s .= "\n";
    }
    $s;
}

sub options {
    my $s;
    foreach $option (@options) {
	$s .= ".TP\n";
	$s .= "\\fB\\-$option->{short}" if $option->{short};
	$s .= " $option->{type}" if $option->{short} && $option->{type};
	$s .= "\\fR, " if $option->{short} && $option->{long};
	$s .= "\\fB\\-\\-$option->{long}" if $option->{long};
	$s .= "=$option->{type}" if $option->{long} && $option->{type};
	$s .= "\\fR\n$option->{desc}\n";
    }
    $s;
}

require 'cli_parse.pl';
