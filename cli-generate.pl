#!/usr/bin/perl
use strict;
use Getopt::Std;

my %opts;
getopts('chm', \%opts);
if (scalar(@ARGV) != 1
    || $opts{'c'} + $opts{'h'} + $opts{'m'} != 1) {
    print
	"usage: $0 -c|-h program.cli >FILE\n",
	"Generate command-line parsing structures from a description file.\n",
	"\n",
	"  -c  Generate C source code\n",
	"  -h  Generate C header code\n",
	"  -m  Generate man page source\n";
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

sub output_h {
    my $guard = $filename;
    $guard =~ tr/a-z/A-Z/;
    $guard =~ s/[^0-9A-Z]/_/g;
    print "#ifndef ${guard}_H\n";
    print "#define ${guard}_H\n";
    print "/* This file was automatically generated, do not edit. */\n";
    print $header{'includes'};

    foreach my $var (sort keys %decls) {
	print $decls{$var};
    }

    print "#endif\n";
}

sub max_width {
    my $max = 0;
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
			     $$option{'prehelp'}, $$option{'help'});
	    if (defined($$option{'default'})) {
		$text .= sprintf("  %${width}s  (Defaults to %s)\n",
				 '', $$option{'default'});
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
    print "/* This file was automatically generated, do not edit. */\n";
    print "#include <string.h>\n";
    print "#include <iobuf/obuf.h>\n";
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
	    my $default = c_escape($$option{'default'});
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

sub reformat_m_tag {
    my ($tag, $text) = @_;
    # TeXinfo tags:
    # code kbd key samp verb var env file command option dfn
    # cite abbr acronym indicateurl email
    # strong emph
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
    elsif ($tag eq 'code'
	   || $tag eq 'example'
	   || $tag eq 'samp') {
	$text;
    }
    else {
	print STDERR "Warning, unknown tag \@$tag, ignoring\n";
	$text;
    }
}

sub parse_m_text {
    $_ = shift;
    s/\@program\b/\\fB$program\\fR/g;
    s/\@([a-zA-Z]+)\{(.*?)\}/reformat_m_tag($1,$2)/eg;
    s/^\n+//;
    s/\n*$/\n/;
    $_;
}

sub output_m_section {
    my ($section) = @_;
    my $text = $sections{$section};
    if ($text) {
	$section =~ tr/a-z/A-Z/;
	print ".SH $section\n";
	print parse_m_text($text);
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
	    print $$option{'help'}, "\n";
	    print parse_m_text($$option{'description'})
		if $$option{'description'};
	    print "(Defaults to $$option{'default'})\n"
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
    my $usage;
    print
	".\\\" This file was automatically generated, do not edit.\n",
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
    foreach $section ('return value',
		      'errors',
		      'examples',
		      'environment',
		      'files',
		      'see also',
		      'notes',
		      'caveats',
		      'diagnostics',
		      'bugs',
		      'restrictions',
		      'author',
		      'history') {
	output_m_section($section);
    }
}

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
