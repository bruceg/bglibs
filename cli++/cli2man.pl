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

sub section_head { ".SH @_\n"; }
sub paragraph {
    local($_,$first) = @_;
    s/^/.P\n/ unless $first;
    $_;
}

require 'cli_parse.pl';

print ".TH $program $section\n";
print ".SH NAME\n";
print "$program \- $prefix\n";
print ".SH SYNOPSIS\n";
print ".B $program\n";
if(@options) {
    $first = 1;
    foreach $option (@options) {
	if($option->{short}) {
	    print " " unless $first;
	    $first = 0;
	    print "[\n.B \-$option->{short} $option->{type}\n]";
	}
	if($option->{long}) {
	    print " " unless $first;
	    $first = 0;
	    print "[\n.B \-\-$option->{long}";
	    print "=$option->{type}" if $option->{type};
	    print "\n]";
	}
    }
    print "\n";
}
print ".SH DESCRIPTION\n", pod2doc($description);
if(@options) {
    print ".SH OPTIONS\n";
    foreach $option (@options) {
	print ".TP\n";
	print "\\fB\\-$option->{short}" if $option->{short};
	print " $option->{type}" if $option->{short} && $option->{type};
	print "\\fR, " if $option->{short} && $option->{long};
	print "\\fB\\-\\-$option->{long}" if $option->{long};
	print "=$option->{type}" if $option->{long} && $option->{type};
	print "\\fR\n";
	print $option->{desc}, "\n";
    }
}
print $notes;
print ".SH AUTHOR\n", pod2doc($author), "\n" if $author;
