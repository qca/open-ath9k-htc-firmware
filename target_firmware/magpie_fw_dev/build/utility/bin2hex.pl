#!/usr/bin/perl -w
###############################################################
#binary-to-hex perl tool: obtained & modified on 10/05/08
###############################################################
use strict;

my ($format, $binfile, $outfile, $arr_form, $ColNum, $show_hex, $filesize);
$format = $ARGV[0] || usage();
$binfile = $ARGV[1] || usage();
$outfile = $ARGV[2] || $binfile.".$format";
$arr_form = $ARGV[3];
$ColNum = $ARGV[4] || usage();
$show_hex = $ARGV[5];

my ($orig, @converted);

    $orig = readdata ($binfile);

    $orig = hexdump($orig);
    if ($show_hex) {print "Original Binary text:\n\t $orig","\n\n";}

    if ($format eq 'a' || $format eq 'asm') {
        @converted = convert_to_asm ($orig);

    } elsif ($format eq 'c') {
        @converted = convert_to_c ($orig);

    } else {
        print "Unknown format to convert!\n";
        exit (-1);
    }
    if ($show_hex) {print "Converted hex text:\n",join ('', @converted), "\n";}

    writedata ($outfile, @converted);

sub convert_to_asm {
    my @data = split(' ', join (' ', @_)); #nop here only one list passed to join

    my $i = 0;
    if ($arr_form){
      foreach (@data) {
  	  if ($i++ < 8) {
	    $_=$_."h, 0";
  	  } else {
	    $_=$_."h\nbyte 0";
	    $i = 0;
	  }
      }

      unshift (@data, "byte 0");
      $data[-1] =~s/[,|\nbyte] 0$//g;
    }else{
      foreach (@data){
        if ($i++ < $ColNum-1) {
          #$_.=",";
        }else {
          $_.="\n";
          $i = 0;
        }
      }      
    }  
    return @data;
}

sub convert_to_c {
    my @data = split(' ', join (' ', @_)); #nop here only one list passed to join

    my $i = 0;
    if ($arr_form){
      foreach (@data) {
  	if ($i++ < $ColNum-1) {
	    $_.=", 0x";
	} else {
	    $_.=",\n\t0x";
	    $i = 0;
	}
      }
      unshift (@data, "unsigned char data[$filesize] = {\n\t0x");  #add some pattern at the front of @data
      $data[-1] =~s/0x$//g;
      $data[-1] =~s/[ |\n\t]//g;
      $data[-1] =~s/\,//g;
      push (@data, "\n};");
    }else{
      foreach (@data){
        if ($i++ < $ColNum-1) {
        }else {
          $_.="\n";
          $i = 0;
        }
      }      
    }
    return @data;
}

sub readdata {
    my ($line);
    my ($file) = @_;
    #printf "dbg:file = $file\n";
    open (BF, "$file") || die "Cannot open $file: $!";  #$! contains current value of errno
    binmode (BF);
    $filesize = (stat($file))[7];
    my ($DATA) = ""; #<BF>;
    my (@Data_check) = <BF>;
    foreach $line (@Data_check){
      #printf "dbg:line = $line\n";
      $DATA.=$line;
    }  
    #printf "dbg:DATA string = $DATA\n";
    close (BF);
    return ($DATA);
}

sub writedata {
    my ($file, @FomatData) = @_;
    open (AF, ">$file") || die "Cannot open $file: $!";
    my $i = 0;
    my $b0 = 0;    
    my $b1 = 0; 
    my $b2 = 0; 
    my $b3 = 0;        
    foreach (@FomatData) {
      if($ColNum eq '1') {
        if($i == 0) {
	  $b0 = $_;
	  $i++;
	} else {
	if($i == 1) {
	  $b1 = $_;
	  $i++;
	} else {
	if($i == 2) {
	  $b2 = $_;
	  $i++;
	} else {
	  print AF "$_";
	  print AF "$b2";
	  print AF "$b1";
	  print AF "$b0";
	  $i = 0;
	}}}
       }else{
          print AF "$_";
       }
    }
    close (AF);
}

sub hexdump
{
  join ' ', map { sprintf "%02X", $_ } unpack "C*", $_[0];
}

sub usage {
    print STDERR <<EOF;
    
Usage: bin2hex format binfile outfile
     format      format to convert to, 
		 asm ==> 'assembly', 
		 c ==> 'C'
     binfile     binary file you want to convert.
     outfile     output file to store the result of output.
     arr_form    displayed in array-form
     ColNum      num of columns of the shown array

EOF
   exit(-1);
}

