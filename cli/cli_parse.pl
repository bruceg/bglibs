$section = 1;

sub type2word {
    my($type) = shift;
    return 'INT' if $type eq 'integer';
    return 'UINT' if $type eq 'uinteger';
    return 'STR' if $type eq 'string' || $type eq 'stringlist';
    return '' if $type eq 'flag' || $type eq 'counter';
    die "Invalid cli option type '$type'";
}

sub parse_option {
    local($_) = shift;
    s/^\s*\{\s*//o;
    s/\s*\},?\s*/ /o;

    my $short = $1 if s/^'([^\'])',\s*//o;
    die "Invalid cli option" unless $short || s/^0,\s*//o;

    my $long = $1 if s/^"([^\"]+)",\s*//o;
    die "Invalid cli_option" unless $long || s/^0,\s*//o;

    my $type = $1 if s/^cli_option::(\S+),\s*//o;
    die "Invalid cli_option" unless $type;
    $type = &type2word($type);

    my $val = $1 if s/^([^,]+),\s*//o;
    my $var = $1 if s/^&([^,]+),\s*//o;

    my $desc = cstr2doc($1) if s/^"([^,]+)",\s*//o;
    die "Invalid cli_option" unless $desc;
    $desc =~ s/\.?$/./o if $desc;

    my $default = $1 if s/^"([^\"]+)"\s+//o;
    die "Invalid cli_option" unless $default || s/^0\s+//o;
    $desc .= " Defaults to $default." if $default;

    s/\s*\/\/\s*/ /go;
    s/^\s*//o;
    $desc = &pod2doc($_) if $_;

    push(@options, {
	short => $short,
	long => $long,
	type => $type,
	desc => $desc
    });
}

sub parse_options {
    my $line;
    while(<>) {
	s/^\s+//o;
	s/\s+$//o;
	if($line && /^\{/o) {
	    &parse_option($line);
	    $line = "";
	}
	next if /^\{\s*0\s*\},?/o;
	last if /^\s*\};/o;
	$line =~ s/$/ $_/;
    }
    &parse_option($line) if $line;
}

sub parse_notes {
    my $first;
    while(<>) {
	chomp;
	last unless /^$/o || s/^\/\/\s*//o;
	if(/^[\sA-Z]+$/o) {
	    $notes .= &section_tail if $notes;
	    $notes .= &section_head($_);
	    undef $pgph;
	    $first = 1;
	} elsif(/^$/o) {
	    if($pgph) {
		$notes .= &paragraph(pod2doc($pgph), $first);
		undef $pgph;
		$first = 0;
	    }
	} else {
	    $pgph .= "$_\n";
	}
    }
    $notes .= &paragraph(pod2doc($pgph), $first) if $pgph;
    $notes .= &section_tail if $notes;
}

while(<>) {
    s/^\s+//o;
    s/\s+$//o;
    if(/ cli_program\s*=\s*"([^\"]+)"/o) {
	$program = $1;
    } elsif(/ cli_help_prefix\s*=\s*"([^\"]+)\\n"/o) {
	$prefix = $1;
    } elsif(/ cli_args_usage\s*=\s*"([^\"]+)"/o) {
	$usage = $1;
    } elsif(s/^.*Copyright\s*\(C\)\s*[\d,]+\s*//o) {
	$author = $_;
    }
    last if $program && $prefix;
}

while(<>) {
    s/^\s+//o;
    s/\s+$//o;
    last if / cli_options\[\]\s*=\s*\{/o;
    next unless s/^\/\/\s*//o;
    $description .= "$_\n";
}

&parse_options();

&parse_notes();

$synopses =~ s/^\s+//o;

print &body_head;

print &section_head('SYNOPSIS');
print &synopsis;
print &section_tail;

print &section_head('DESCRIPTION');
print &pod2doc($description);
print &section_tail;

if(@options) {
    print &section_head('OPTIONS');
    print &options;
    print &section_tail;
}

print $notes;

if($author) {
    print &section_head('AUTHOR');
    print pod2doc($author);
    print &section_tail;
}

print &body_tail;

1;
