#!/usr/bin/ruby

require 'time'

cocoaClassPrefixDir=ARGV[0]

cocoaClassPrefixDir = ARGV[0]
if cocoaClassPrefixDir == nil
	$stdout << "Cannot resolve output directory\n"
	exit(-1)
end

$stdout << "Generating new class prefix for Objective-C classes in #{cocoaClassPrefixDir}\n"
File.open("#{cocoaClassPrefixDir}/aucocoaclassprefix.h", "w+") do |stream|
    
  		t = Time.now.to_i
        t.round
        id = t.to_s
        stream << "#define SMTG_AU_NAMESPACE\t"
        stream << "SMTGAUCocoa#{id}_\n"
        
end

