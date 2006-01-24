#!/usr/bin/perl
use strict;
use Getopt::Std;

my %opts;
getopts('hc', \%opts);
if (scalar(@ARGV) != 1
    || $opts{'c'} + $opts{'h'} != 1) {
    print
	"usage: $0 -c|-h program.cli >FILE\n",
	"Generate command-line parsing structures from a description file.\n",
	"\n",
	"  -c  Generate C source code\n",
	"  -h  Generate C header code\n";
    exit(1);
}

my $filename = $ARGV[0];
open(IN, '<', $filename)
    || die "Could not open '$filename': $!\n";

my $line;
my @options;
my %decls;
my %defns;
my $prefix;
my $suffix;
my %header = (
	      'min' => -1,
	      'max' => -1,
	      'usage' => '',
	      'show-pid' => 0,
	      'includes' => "#include <cli/cli.h>\n",
	      'debug-bits' => 0,
	      );

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
		'long' => undef,
		'type' => 'SEPARATOR',
		'flag' => '0',
		'varname' => undef,
		'init' => '0',
		'help' => $1,
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
}

sub read_header {
    while ($line = <IN>) {
	chomp($line);
	last unless $line;
	unless ($line =~ /^([^\s:]+):\s*(.*)$/) {
	    die "Invalid header line:\n  $line\n";
	}
	my $field = $1;
	my $value = $2;
	$field =~ tr/A-Z/a-z/;
	if ($field eq 'min') {
	    $header{'min'} = $value;
	}
	elsif ($field eq 'max') {
	    $header{'max'} = $value;
	}
	elsif ($field eq 'usage') {
	    $header{'usage'} = $value;
	}
	elsif ($field eq 'show-pid') {
	    $header{'show-pid'} = defined($value) ? $value : 1;
	}
	elsif ($field eq 'include') {
	    $header{'includes'} .= "#include $value\n";
	}
	elsif ($field eq 'debug-bits') {
	    $header{'debug-bits'} = $value;
	}
	else {
	    die "Invalid header line:\n  $line\n";
	}
    }
}

sub read_sections {
    $line = <IN>;
    while ($line) {
	chomp($line);
	if ($line eq '[prefix]') {
	    $prefix = read_text($prefix);
	    $prefix =~ s/^\s+//;
	    $prefix =~ s/\s+$//;
	}
	elsif ($line eq '[suffix]') {
	    $suffix = read_text($suffix);
	    $suffix =~ s/^\s+//;
	    $suffix =~ s/\s+$//;
	}
	elsif ($line eq '[options]') {
	    read_options();
	}
	else {
	    die "Invalid section header:\n  $line\n";
	}
    }
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
    $text = $prefix;
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
    $text .= $suffix;
    $text .= "\n";
    $text;
}

sub output_c {
    print "/* This file was automatically generated, do not edit. */\n";
    print "#include <string.h>\n";
    print "#include <iobuf/obuf.h>\n";
    print $header{'includes'};

    my $program = $filename;
    $program =~ s/\.cli$//;
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

read_header();
read_sections();

postprocess_options();

if ($opts{'c'}) {
    output_c();
}
elsif ($opts{'h'}) {
    output_h();
}
