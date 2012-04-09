use strict;
use Getopt::Std;

my %opts;
getopts('chmw', \%opts);
if (scalar(@ARGV) != 1
    || $opts{'c'} + $opts{'h'} + $opts{'m'} + $opts{'w'} != 1) {
    print
	"usage: $0 -c|-h|-m|-w program.cli >FILE\n",
	"Generate command-line parsing structures from a description file.\n",
	"\n",
	"  -c  Generate C source code\n",
	"  -h  Generate C header code\n",
	"  -m  Generate man page source\n",
	"  -w  Generate HTML (web) source\n";
    exit(1);
}

my $filename = $ARGV[0];
open(IN, '<', $filename)
    || die "Could not open '$filename': $!\n";
my $program = $filename;
$program =~ s/\.cli$//;
$program =~ s/^.*\///;

my %sections;
my @options;
my %decls;
my %defns;
my %header = ( 'include' => "<cli/cli.h>\n" );

my @sections = ('return value',
		'errors',
		'examples',
		'environment',
		'files',
		'see also',
		'notes',
		'caveats',
		'warnings',
		'diagnostics',
		'bugs',
		'restrictions',
		'author',
		'authors',
		'history');

my %type_suffix = (
    'STRING' => '=VALUE',
    'INTEGER' => '=INT',
    'UINTEGER' => '=UNS',
    'STRINGLIST' => '=ITEM',
    'FUNCTION' => '=VALUE',
    'FLAG' => undef,
    'COUNTER' => undef,
);

my %type_decl = (
    'STRING' => "extern const char* %s;\n",
    'INTEGER' => "extern int %s;\n",
    'UINTEGER' => "extern unsigned int %s;\n",
    'STRINGLIST' => "extern cli_stringlist* %s;\n",
    'FUNCTION' => "extern void %s(const char*, const struct cli_option*);\n",
    'FLAG' => "extern int %s;\n",
    'COUNTER' => "extern int %s;\n",
);

my %type_defn = (
    'STRING' => "const char* %s = %s;\n",
    'INTEGER' => "int %s = %s;\n",
    'UINTEGER' => "unsigned int %s = %s;\n",
    'STRINGLIST' => "cli_stringlist* %s = %s;\n",
    'FUNCTION' => "extern void %s(const char*, const struct cli_option*);\n",
    'FLAG' => "int %s = %s;\n",
    'COUNTER' => "int %s = %s;\n",
);

sub parse_options {
    my @lines = split('\n', $sections{'options'});
    my $line;
    while ($line = shift(@lines)) {
	$line =~ s/\s+$//;
	next if !$line;
	if ($line =~ /^--\s+(.+)$/) {
	    push(@options, {
		'short' => undef,
		'long' => undef,
		'type' => 'SEPARATOR',
		'flag' => '0',
		'varname' => undef,
		'init' => '0',
		'help' => $1,
		'default' => undef,
		'description' => undef,
	    });
	}
	elsif ($line =~ /^(-([^-])\s+)?(--([^=]+)\s+)?([A-Z]+)(\s*=\s*(\S+))?\s+(\S+)(\s*=\s*("[^\"]*"|-?\d+))?$/
	       && ($2 ne '' || $4 ne '')) {
	    my $shortopt = $2;
	    my $longopt = $4;
	    my $type = $5;
	    my $flag = $7 || '0';
	    my $var = $8;
	    my $init = $10;

	    my $helpstr = shift(@lines);
	    chomp($helpstr);
	    my $default = ($helpstr =~ s/\s*=\s*([^=]+)$//) ? $1 : undef;

	    my $description;
	    while ($line = shift(@lines)) {
		chomp($line);
		last if $line =~ /^-/;
		if ($description || $line) {
		    $description =~ s/$/\n$line/;
		}
	    }

	    push(@options, {
		'short' => $shortopt,
		'long' => $longopt,
		'type' => $type,
		'flag' => $flag,
		'varname' => $var,
		'init' => $init || '0',
		'help' => $helpstr,
		'default' => $default,
		'description' => $description,
	    });
	}
	else {
	    die "Option line is misformatted:\n  $line\n";
	}
    }
}

sub read_header {
    my $line;
    while ($line = <IN>) {
	chomp($line);
	last unless $line;
	unless ($line =~ /^([^\s:]+):\s*(.*)$/) {
	    die "Invalid header line:\n  $line\n";
	}
	my $field = $1;
	my $value = $2;
	$field =~ tr/A-Z/a-z/;
	$header{$field} .= "$value\n";
    }
}

sub read_sections {
    my $line;
    my $section;
    my $text;
    $line = <IN>;
    unless ($line =~ /^\[(.+)\]\s*$/) {
	die "Invalid section marker:\n  $line\n";
    }
    $section = $1;
    while ($line = <IN>) {
	if ($line =~ /^\[(.+)\]\s*$/) {
	    my $newsection = $1;
	    $text =~ s/\s+$//;
	    $sections{$section} = $text;
	    $section = $newsection;
	    $text = '';
	}
	else {
	    $text .= $line;
	}
    }
    if ($section) {
	$text =~ s/\s+$//;
	$sections{$section} = $text;
    }
}

sub header_num {
    my ($key,$default) = @_;
    $header{$key} = defined($header{$key}) ? $header{$key} + 0 : $default;
}

sub parse_header {
    my $line;
    my $key;
    foreach $key (keys %header) {
	chomp($header{$key});
    }
    foreach $line (split('\n', $header{'include'})) {
	$header{'includes'} .= "#include $line\n";
    }
    header_num('min', 0);
    header_num('max', -1);
    header_num('show-pid', 0);
    header_num('debug-bits', 0);
}

sub postprocess_options {
    foreach my $option (@options) {
	my $var = $$option{'varname'};
	my $type = $$option{'type'};
	if (my $decl = $type_decl{$type}) {
	    $decls{$var} = sprintf($decl, $var);
	}
	if (my $defn = $type_defn{$type}) {
	    $defns{$var} = sprintf($defn, $var, $$option{'init'});
	}
	$$option{'help'} =~ s/([^\.])$/$1\./;
	my $short = defined($$option{'short'})
	    ? "-$$option{'short'}"
	    : '  ';
	my $long = defined($$option{'long'})
	    ? "--$$option{'long'}$type_suffix{$type}"
	    : '';
	my $mid = (defined($$option{'short'}) && defined($$option{'long'}))
	    ? ', '
	    : '  ';
	$$option{'prehelp'} = "$short$mid$long";
    }
}

sub parse_text_list {
    local($_);
    my $part;
    my @parts;
    foreach my $part (split(/\@item\s+/s, shift)) {
	my @part = split(/\n\s*\n/, $part);
	push @parts, \@part;
    }
    shift @parts;		# Assume first one is blank
    return @parts;
}

sub parse_text {
    local($_);
    $_ = shift;
    s/[ \t]+\n/\n/g;		# Strip trailing spaces
    s/\n*$/\n/;			# Make sure there is exactly one \n at the end
    s/^\n+//s;
    my @parts;
    while ($_) {
	my @part;
	# Block modes, match everything up to the following "@end MODE"
	if (s/^\@(example|verbatim)\n(.*?)\n\@end\s+.*?\n//s) {
	    @part = ( $1, $2 );
	}
	# Table mode, break into list items with following text
	elsif (s/^\@table\s+(\S+)\n(.*?\n)\@end\s+.*?\n//s) {
	    my $mode = $1;
	    my @items = parse_text_list($2);
	    @part = ( 'table', \@items, $mode );
	}
	# List modes, break into list items with optional paragraphs
	elsif (s/^\@(itemize|enumerate)\n(.*?\n)\@end\s+.*?\n//s) {
	    my $mode = $1;
	    my @items = parse_text_list($2);
	    @part = ( $mode, \@items );
	}
	# Otherwise, match the next paragraph
	else {
	    s/^(.*?)(\n\s*\n|$)//s;
	    @part = ( undef, $1 );
	}
	push @parts, \@part;
	s/^([ \t]*\n)+//;
    }
    return @parts;
}

###############################################################################
# Functions for outputting C header file
###############################################################################

sub output_h {
    my $guard = $filename;
    $guard =~ tr/a-z/A-Z/;
    $guard =~ s/[^0-9A-Z]/_/g;
    print "#ifndef ${guard}_H\n";
    print "#define ${guard}_H\n";
    print "/* This file was automatically generated from ${filename}, do not edit. */\n";
    print $header{'includes'};

    foreach my $var (sort keys %decls) {
	print $decls{$var};
    }

    print "#endif\n";
}

###############################################################################
# Functions for outputting C source
###############################################################################

sub max_width {
    my $max = 10;
    foreach my $option (@options) {
	my $width = length($$option{'prehelp'});
	$max = $width if $width > $max;
    }
    $max;
}

sub c_escape {
    my ($s) = @_;
    $s =~ s/\"/\\\"/g;
    $s =~ s/\n/\\n"\n"/g;
    $s;
}

sub reformat_c_tag {
    my ($tag, $text) = @_;
    if ($tag eq 'samp'
	|| $tag eq 'file') {
	"\"$text\"";
    }
    else {
	# Just strip everything else
	$text;
    }
}

sub reformat_c_tags {
    my $line = shift;
    # Just strip out tags
    $line =~ s/\@([a-zA-Z]+)\{(.*?)\}/reformat_c_tag($1,$2)/eg;
    $line;
}

sub make_helpstr {
    my $width = max_width();
    my $text;
    $text .= "$header{'description'}\n" if $header{'description'};
    $text .= "$sections{'prefix'}\n" if $sections{'prefix'};
    $text .= "\n";
    foreach my $option (@options) {
	if ($$option{'type'} eq 'SEPARATOR') {
	    $text .= sprintf("\n%s:\n", $$option{'help'});
	}
	else {
	    $text .= sprintf("  %-${width}s  %s\n",
			     reformat_c_tags($$option{'prehelp'}),
			     reformat_c_tags($$option{'help'}));
	    if (defined($$option{'default'})) {
		$text .= sprintf("  %${width}s  (Defaults to %s)\n",
				 '', reformat_c_tags($$option{'default'}));
	    }
	}
    }
    $text .= sprintf("\n  %-${width}s  %s\n",
		     '-h, --help',
		     'Display this help and exit');
    $text .= $sections{'suffix'};
    $text .= "\n";
    $text;
}

sub output_c {
    print "/* This file was automatically generated from ${filename}, do not edit. */\n";
    print "#include <string.h>\n";
    print "#include <obuf.h>\n";
    print $header{'includes'};

    print "const char program[] = \"$program\";\n";

    print "const char cli_args_usage[] = \"$header{'usage'}\";\n";
    print "const int cli_args_min = $header{'min'};\n";
    print "const int cli_args_max = $header{'max'};\n";
    print "const int msg_show_pid = $header{'show-pid'};\n";
    print "int msg_debug_bits = $header{'debug-bits'};\n";

    my $helpstr = c_escape(make_helpstr());
    print
	"void cli_show_help(void) {\n",
	"  obuf_puts(&outbuf,\n",
	"\"$helpstr\");\n",
	"}\n";

    foreach my $var (sort keys %defns) {
	print $defns{$var};
    }

    print "cli_option cli_options[] = {\n";
    foreach my $option (@options) {
	if ($$option{'type'} ne 'SEPARATOR') {
	    my $default = c_escape(reformat_c_tags($$option{'default'}));
	    my $varptr = $$option{'varname'};
	    if (defined($varptr)) {
		if ($$option{'type'} ne 'FUNCTION') {
		    $varptr = "&$varptr";
		}
	    }
	    else {
		$varptr = '0';
	    }
	    printf
		"  { %s, %s, CLI_%s, %s, %s, 0, 0 },\n",
		defined($$option{'short'}) ? "'$$option{'short'}'" : 0,
		defined($$option{'long'}) ? "\"$$option{'long'}\"" : 0,
		$$option{'type'},
		$$option{'flag'},
		$varptr;
	}
    }
    print "  {0,0,0,0,0,0,0}\n";
    print "};\n";
}

###############################################################################
# Functions for outputting man page source
###############################################################################

sub reformat_m_tag {
    my ($tag, $text) = @_;
    # TeXinfo tags:
    # kbd key verb dfn
    # cite abbr acronym indicateurl email
    if ($tag eq 'strong'
	|| $tag eq 'command'
	|| $tag eq 'option') {
	"\\fB$text\\fR";
    }
    elsif ($tag eq 'emph'
	   || $tag eq 'var'
	   || $tag eq 'file'
	   || $tag eq 'env') {
	"\\fI$text\\fR";
    }
    elsif ($tag eq 'samp') {
	"\"$text\"";
    }
    elsif ($tag eq 'code') {
	"'$text'";
    }
    elsif ($tag eq 'asis') {
	$text;
    }
    else {
	print STDERR "Warning, unknown tag \@$tag, ignoring\n";
	$text;
    }
}

sub reformat_m_tags {
    my $line = shift;
    s/^\./\\./gm;
    $line =~ s/\@program\b/\\fB$program\\fR/g;
    $line =~ s/\@([a-zA-Z]+)\{(.*?)\}/reformat_m_tag($1,$2)/eg;
    $line;
}

sub parse_m_text {
    my @parts;
    foreach my $part (parse_text(shift)) {
	my $mode = $$part[0];
	$_ = $$part[1];
	if ($mode eq 'verbatim') {
	    s/^\./\\./gm;
	    s/^/.RS\n.nf\n/;
	    s/$/\n.fi\n.RE/;
	}
	elsif ($mode eq 'example') {
	    $_ = reformat_m_tags($_);
	    s/^\./\\./gm;
	    s/^/.EX\n/;
	    s/$/\n.EE/;
	}
	elsif ($mode eq 'table') {
	    my $tmode = $$part[2] || '@asis';
	    my $text = '';
	    foreach my $item (@$_) {
		$$item[0] = "$tmode\{$$item[0]\}";
		@$item = map { reformat_m_tags($_) } @$item;
		$text .= ".TP\n" . join("\n\n", @$item) . "\n";
	    }
	    $_ = $text . ".PP\n";
	}
	elsif ($mode eq 'itemize') {
	    my $text = '';
	    foreach my $item (@$_) {
		@$item = map { reformat_m_tags($_) } @$item;
		$text .= ".TP\no\n" . join("\n\n", @$item) . "\n";
	    }
	    $_ = $text . ".PP\n";
	}
	elsif ($mode eq 'enumerate') {
	    my $n = 1;
	    my $text = '';
	    foreach my $item (@$_) {
		@$item = map { reformat_m_tags($_) } @$item;
		$text .= ".TP\n${n}.\n" . join("\n\n", @$item) . "\n";
		$n++;
	    }
	    $_ = $text . ".PP\n";
	}
	else {
	    $_ = reformat_m_tags($_);
	}
	s/$/\n/;
	push @parts, $_;
    }
    $_ = join("\n", @parts);
    # 3 or more line feeds always need to be reduced to 2.
    s/\n{3,}/\n\n/g;
    # Blank lines before .TP or .PP need to be removed.
    s/\n{2,}(\.(TP|PP))/\n$1/g;
    # Blank lines after .PP need to be removed.
    s/^\.PP\n{2,}/.PP\n/gm;
    # Remove blank lines betwee the first (unindented) and second
    # (indented) paragraphs in the .TP formatter.
    s/^(\.TP\n[^\n]+)\n+/$1\n/gm;
    # Strip leading blank lines in this section.
    s/^\n+//;
    # Strip extraneous trailing .PP lines.
    s/\.PP\n*$//;
    # Strip remaining trailing blank lines.
    s/\n+$//;
    $_;
}

sub output_m_section {
    my ($section) = @_;
    my $text = $sections{$section};
    if ($text) {
	$section =~ tr/a-z/A-Z/;
	print ".SH $section\n";
	print parse_m_text($text), "\n";
    }
}

sub output_m_options {
    print ".SH OPTIONS\n";
    foreach my $option (@options) {
	if ($$option{'type'} eq 'SEPARATOR') {
	    print ".SS $$option{'help'}\n";
	}
	else {
	    print ".TP\n";
	    print ".B $$option{'prehelp'}\n";
	    print parse_m_text($$option{'help'}), "\n";
	    print parse_m_text($$option{'description'}), "\n"
		if $$option{'description'};
	    print "Defaults to ", parse_m_text($$option{'default'}), ".\n"
		if defined($$option{'default'});
	}
    }
    print
	".TP\n",
	".B -h, --help\n",
	"Display usage information and exit.\n";
}

sub output_m {
    my $section;
    if (!$header{'description'}) {
	print STDERR "Warning: The header is missing a 'description' field.\n";
    }
    print
	".\\\" This file was automatically generated from ${filename}, do not edit.\n",
	".TH $program 1\n",
	".SH NAME\n",
	"$program \\- $header{'description'}\n",
	".SH SYNOPSIS\n",
	".B $program\n";

    $_ = $header{'usage'};
    s/([\[\]])/\\fR$1\\fI/g;
    s/(^|\s+)(-\S+)/\\fB$1\\fI/g;
    print ".I $_\n";

    output_m_section('description');
    output_m_options() if @options;
    foreach $section (@sections) {
	output_m_section($section);
    }
}

###############################################################################
# Functions for outputting HTML source
###############################################################################

sub reformat_w_tag {
    my ($tag, $text) = @_;
    if ($tag eq 'strong') {
	"<strong>$text</strong>";
    }
    elsif ($tag eq 'emph') {
	"<em>$text</em>";
    }
    elsif ($tag eq 'var') {
	"<var>$text</var>";
    }
    elsif ($tag eq 'command'
	   || $tag eq 'option'
	   || $tag eq 'file'
	   || $tag eq 'env') {
	"<tt>$text</tt>";
    }
    elsif ($tag eq 'samp') {
	"\"<samp>$text</samp>\"";
    }
    elsif ($tag eq 'code') {
	"<code>$text</code>";
    }
    elsif ($tag eq 'asis') {
	$text;
    }
    else {
	print STDERR "Warning, unknown tag \@$tag, ignoring\n";
	$text;
    }
}

sub reformat_w_tags {
    my $line = shift;
    s/^\./\\./gm;
    $line =~ s/\@program\b/<b><tt>$program<\/tt><\/b>/g;
    $line =~ s/\@([a-zA-Z]+)\{(.*?)\}/reformat_w_tag($1,$2)/eg;
    $line;
}

sub parse_w_text {
    my @parts;
    foreach my $part (parse_text(shift)) {
	my $mode = $$part[0];
	$_ = $$part[1];
	if ($mode eq 'verbatim') {
	    s/\&/\&amp;/g;
	    s/</\&lt;/g;
	    s/>/\&gt;/g;
	    s/^/<pre>/;
	    s/$/<\/pre>/;
	}
	elsif ($mode eq 'example') {
	    $_ = reformat_w_tags($_);
	    s/^/<blockquote>/;
	    s/$/<\/blockquote>/;
	}
	elsif ($mode eq 'table') {
	    my $text = "<dl>\n";
	    my $tmode = $$part[2] || '@asis';
	    foreach my $item (@$_) {
		my $tag = 'dt';
		foreach my $st (@$item) {
		    $st = "$tmode\{$st\}" if $tag eq 'dt';
		    $text .= "<$tag>" . reformat_w_tags($st) . "</$tag>\n";
		    $tag = 'dd';
		}
	    }
	    $_  = $text . "</dl>\n";
	}
	elsif ($mode eq 'itemize') {
	    my $text = "<ul>\n";
	    foreach my $item (@$_) {
		$text .= '<li>' . join('<br><br>', map { reformat_w_tags($_) } @$item) . "</li>\n";
	    }
	    $_ = $text . "</ul>\n";
	}
	elsif ($mode eq 'enumerate') {
	    my $text = "<ol>\n";
	    foreach my $item (@$_) {
		$text .= '<li>' . join('<br><br>', map { reformat_w_tags($_) } @$item) . "</li>\n";
	    }
	    $_ = $text . "</ol>\n";
	}
	else {
	    $_ = '<p>' . reformat_w_tags($_) . "</p>\n";
	}
	push @parts, $_;
    }
    join("\n", @parts);
}

sub output_w_section {
    my ($section) = @_;
    my $text = $sections{$section};
    if ($text) {
	$section =~ tr/a-z/A-Z/;
	print "<a name=\"$section\"></a><h2>$section</h2>\n";
	print parse_w_text($text), "\n";
    }
}

sub output_w_options {
}

sub output_w {
    my $section;
    my $usection;
    if (!$header{'description'}) {
	print STDERR "Warning: The header is missing a 'description' field.\n";
    }
    print
	"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n",
	"<!-- This file was automatically generated from ${filename} -->\n",
	"<html>\n",
	"<head>\n",
	"<title>Manual page for $program</title>\n",
	"</head>\n",
	"<body>\n",
	"<h1>$program</h1>\n",
	"<hr />\n",
	"<a name=\"index\"></a><h2>Index</h2>\n",
	"<dl>\n",
	"<dt><a href=\"#NAME\">NAME</a></dt>\n",
	"<dt><a href=\"#SYNOPSIS\">SYNOPSIS</a></dt>\n";
    print "<dt><a href=\"#DESCRIPTION\">DESCRIPTION</a></dt>\n"
	if $sections{'description'};
    print "<dt><a href=\"#OPTIONS\">OPTIONS</a></dt>\n"
	if @options;
    foreach $section (@sections) {
	if ($sections{$section}) {
	    my $name = $section;
	    $name =~ tr/a-z/A-Z/;
	    print "<dt><a href=\"#$name\">$name</a></dt>\n";
	}
    }
    print
	"</dl>\n",
	"<hr />\n",
	"<a name=\"NAME\"></a><h2>NAME</h2>\n",
	"<p>$program - $header{'description'}</p>\n",
	"<a name=\"SYNOPSIS\"></a><h2>SYNOPSIS</h2>\n";
    $_ = $header{'usage'};
    s{([\[\]])}{</i>$1<i>}g;
    s{(^|\s+)(-\S+)}{</i><b>$1</b><i>}g;
    print "<p><i>$_</i></p>\n";

    output_w_section('description');
    output_w_options() if @options;
    foreach $section (@sections) {
	output_w_section($section);
    }

    print
	"</body>\n",
	"</html>\n";
}

    
###############################################################################
# Main routine
###############################################################################

read_header();
read_sections();

parse_header();
parse_options();
postprocess_options();

if ($opts{'c'}) {
    output_c();
}
elsif ($opts{'h'}) {
    output_h();
}
elsif ($opts{'m'}) {
    output_m();
}
elsif ($opts{'w'}) {
    output_w();
}
