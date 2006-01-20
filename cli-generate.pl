#!/usr/bin/perl
use strict;
use Getopt::Std;

my %opts;
getopts('hc', \%opts);
if (scalar(@ARGV) != 1
    || $opts{'c'} + $opts{'h'} != 1) {
    print "usage: $0 -c|-h program.cli\n";
    exit(1);
}

my $line;

sub is_section {
    my ($line) = @_;
    $line =~ /^\[.+\]\s*$/o;
}

sub read_text {
    my ($text) = @_;
    while ($line = <IN>) {
	chomp ($line);
	last if is_section($line);
	if ($line || $text) {
	    $text =~ s/$/\n$line/;
	}
    }
    $text;
}

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

sub read_options {
    my(@options);
    $line = <IN>;
    while ($line) {
	chomp ($line);
	last if is_section($line);
	if (!$line) {
	    $line = <IN>;
	    next;
	}
	elsif ($line =~ /^--\s+(.+)$/) {
	    push(@options, {
		'short' => undef,
		'long' => $1,
		'type' => 'SEPARATOR',
		'flag' => '0',
		'varname' => undef,
		'init' => '0',
		'help' => undef,
		'default' => undef,
		'description' => undef,
	    });
	    $line = <IN>;
	}
	elsif ($line =~ /^(-([^-])\s+)?(--([^=]+)\s+)?([A-Z]+)(\s*=\s*(\S+))?\s+(\S+)(\s*=\s*("[^\"]*"|-?\d+))?$/
	       && ($2 ne '' || $4 ne '')) {
	    my $shortopt = $2;
	    my $longopt = $4;
	    my $type = $5;
	    my $flag = $7 || '0';
	    my $var = $8;
	    my $init = $10;

	    my $helpstr = <IN>;
	    chomp($helpstr);
	    my $default = ($helpstr =~ s/\s*=\s*([^=]+)$//) ? $1 : undef;

	    my $description;
	    while ($line = <IN>) {
		chomp($line);
		last if $line =~ /^-/ || is_section($line);
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
    push(@options, {
	'short' => 'h',
	'long' => 'help',
	'type' => 'FLAG',
	'flag' => 1,
	'varname' => 'do_show_usage',
	'init' => '0',
	'help' => 'Display this help and exit',
	'default' => undef,
	'description' => undef,
    });
    \@options;
}

sub read_header {
    my $header = {
	'min' => -1,
	'max' => -1,
	'usage' => '',
	'show-pid' => 0,
    };
    while ($line = <IN>) {
	chomp($line);
	last unless $line;
	if ($line =~ /^Min:\s*(-?\d+)$/i) {
	    $$header{'min'} = $1;
	}
	elsif ($line =~ /^Max:\s*(-?\d+)$/i) {
	    $$header{'max'} = $1;
	}
	elsif ($line =~ /^Usage:\s*(.+)$/i) {
	    $$header{'usage'} = $1;
	}
	elsif ($line =~ /^Show-Pid:\s*(\d+)?$/i) {
	    $$header{'show-pid'} = defined($1) ? $1 : 1;
	}
	else {
	    die "Invalid header line:\n  $line\n";
	}
    }
    $header;
}

sub read_sections {
    my $sections = { 'options' => [ ] };
    $line = <IN>;
    while ($line) {
	chomp($line);
	if ($line eq '[prefix]') {
	    $$sections{'prefix'} = read_text($$sections{'prefix'});
	    $$sections{'prefix'} =~ s/^\s+//;
	    $$sections{'prefix'} =~ s/\s+$//;
	}
	elsif ($line eq '[suffix]') {
	    $$sections{'suffix'} = read_text($$sections{'suffix'});
	    $$sections{'suffix'} =~ s/^\s+//;
	    $$sections{'suffix'} =~ s/\s+$//;
	}
	elsif ($line eq '[options]') {
	    $$sections{'options'} = read_options();
	}
	else {
	    die "Invalid section header:\n  $line\n";
	}
    }
    $sections;
}

sub postprocess_options {
    my ($options) = @_;
    foreach my $option (@$options) {
	$$option{'decl'} = $type_decl{$$option{'type'}};
	$$option{'defn'} = $type_defn{$$option{'type'}};
	my $short = defined($$option{'short'})
	    ? "-$$option{'short'}"
	    : '  ';
	my $long = defined($$option{'long'})
	    ? "--$$option{'long'}$type_suffix{$$option{'type'}}"
	    : '';
	my $mid = (defined($$option{'short'}) && defined($$option{'long'}))
	    ? ', '
	    : '  ';
	$$option{'prehelp'} = "$short$mid$long";
    }
}

sub output_h {
    my ($filename, $header, $sections) = @_;
    my $options = $$sections{'options'};
    my $guard = $filename;
    $guard =~ tr/a-z/A-Z/;
    $guard =~ s/[^0-9A-Z]/_/g;
    print "#ifndef ${guard}_H\n";
    print "#define ${guard}_H\n";
    print "/* This file was automatically generated, do not edit. */\n";
    print "#include <cli/cli.h>\n";

    foreach my $option (@$options) {
	if (my $decl = $$option{'decl'}) {
	    printf $decl, $$option{'varname'};
	}
    }

    print "#endif\n";
}

sub max_width {
    my ($options) = @_;
    my $max = 0;
    foreach my $option (@$options) {
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
    my ($prefix, $suffix, $options) = @_;
    my $width = max_width($options);
    my $text;
    $text = $prefix;
    $text .= "\n";
    foreach my $option (@$options) {
	$text .= sprintf("  %-${width}s  %s\n",
			 $$option{'prehelp'}, $$option{'help'});
	if (defined($$option{'default'})) {
	    $text .= sprintf("  %${width}s  (Defaults to %s)\n",
			     '', $$option{'default'});
	}
    }
    $text .= $suffix;
    $text .= "\n";
    $text;
}

sub output_c {
    my ($filename, $header, $sections) = @_;
    my $options = $$sections{'options'};

    print "/* This file was automatically generated, do not edit. */\n";

    my $program = $filename;
    $program =~ s/\.cli$//;
    print "const char program[] = \"$program\";\n";

    print "const char cli_args_usage[] = \"$$header{'usage'}\";\n";
    print "const int cli_args_min = $$header{'min'};\n";
    print "const int cli_args_max = $$header{'max'};\n";

    print
	"const char cli_help_prefix[] =\n\"",
	c_escape($$sections{'prefix'}),
	"\\n\";\n";
    print
	"const char cli_help_suffix[] =\n\"",
	c_escape($$sections{'suffix'}),
	"\\n\";\n";

    #my $helpstr = c_escape(make_helpstr($prefix, $suffix, $options));
    #print "const char cli_help[] =\n\"$helpstr\";\n";

    foreach my $option (@$options) {
	if (my $defn = $$option{'defn'}) {
	    my $init = $$option{'init'};
	    if ($$option{'type'} eq 'FUNCTION') {
		$init = '';
	    }
	    printf $defn, $$option{'varname'}, $init;
	}
    }

    print "cli_option cli_options[] = {\n";
    foreach my $option (@$options) {
	if ($$option{'type'} eq 'SEPARATOR') {
	    printf
		"  { 0, \"%s\", CLI_SEPARATOR, 0, 0, 0, 0 },\n",
		c_escape($$option{'long'});
	}
	else {
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
		"  { %s, %s, CLI_%s, %s, %s,\n",
		defined($$option{'short'}) ? "'$$option{'short'}'" : 0,
		defined($$option{'long'}) ? "\"$$option{'long'}\"" : 0,
		$$option{'type'},
		$$option{'flag'},
		$varptr;
	    printf
		"    \"%s\", %s },\n",
		c_escape($$option{'help'}),
		defined($default) ? "\"$default\"" : 0;
	}
    }
    print "  {0,0,0,0,0,0,0}\n";
    print "};\n";
}

my $filename = $ARGV[0];
open(IN, '<', $filename)
    || die "Could not open '$filename': $!\n";

my $header = read_header();
my $sections = read_sections();

postprocess_options($$sections{'options'});

if ($opts{'c'}) {
    output_c($filename, $header, $sections);
}
elsif ($opts{'h'}) {
    output_h($filename, $header, $sections);
}
