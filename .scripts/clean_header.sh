# The file name is passed as an argument to the script
file=$1

# Create a temporary file
temp_file=$(mktemp)

# Add '#pragma once' at the top
echo '#pragma once' > $temp_file

# Find all STL includes, sort them, and remove duplicates
grep '#include <.*>' $file | sort -u >> $temp_file

# Find all non-STL includes, sort them, and remove duplicates
grep '#include ".*"' $file | sort -u >> $temp_file

# Add a single newline after the includes
echo "" >> $temp_file

# Get all lines that are not includes or '#pragma once', and append to the temp file
grep -v '#include <.*>' $file | grep -v '#include ".*"' | grep -v '#pragma once' | cat -s >> $temp_file

# Replace the original file with the temp file
mv $temp_file $file