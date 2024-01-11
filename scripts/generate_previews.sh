#!/bin/bash
script_path=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
date=$(date '+%d/%m/%Y')
version=v$(cat "$script_path/../VERSION")
upper_block="
MOENCHZMQ(C)2024 Max Born Institute
FoXSS TEAM; RELEASE $date
BUILD TAG: $version"
middle_block="
MOENCH TANGO DS IMAGE INITIALIZATION
RESOLUTION: 400px x 400px
CURRENT CHANNEL:
"
footer="
Copyright (C)2024 @lrlunin
All rights reserved.
"
middle="
$proc_type
$pump_type
"
output_file_h="$script_path/../src/tangods/data.hpp"
> "$output_file_h"
# loop from index 0 to 9
for proc_type in "ANALOG" "THRESHOLD" "COUNTING" 
do
    for pump_type in "PUMPED" "UNPUMPED"
    do
    filename="$script_path"/pngs/output_"$proc_type"_"$pump_type".png
    convert "$script_path"/preview_template_empty.png\
            -colorspace gray\
            -font "$script_path"/RobotoMono-Regular.ttf\
            -fill white\
            -density 96\
            -pointsize 12\
            -gravity northwest\
            -annotate +9+112\
            "$upper_block"\
            -annotate +9+180\
            "$middle_block"\
            -pointsize 11\
            -annotate +9+355\
            "$footer"\
            -font "$script_path"/RobotoMono-Bold.ttf\
            -pointsize 35\
            -gravity center\
            -annotate +0+97\
            "$proc_type\n$pump_type"\
             $filename
    convert "$filename"\
            -flip\
            -define h:format=gray\
             h:- | sed s\\MagickImage\\"$proc_type"_"$pump_type"\\g >> "$output_file_h"
    done
done
