# Move to root
cd ..

# Read the version number and split it into an array
IFS='.' read -ra version_parts <<< "$(cat version.txt)"

# Increment the last part of the version number
last_part=$(printf "%02d" $((10#${version_parts[2]} + 1)))

# If the last part is 100, reset it to 00 and increment the middle part
if [ $last_part -eq 100 ]; then
    last_part="00"
    middle_part=$(printf "%02d" $((10#${version_parts[1]} + 1)))
else
    middle_part=${version_parts[1]}
fi

# If the middle part is 100, reset it to 00 and increment the first part
if [ $middle_part -eq 100 ]; then
    middle_part="00"
    first_part=$(printf "%d" $((10#${version_parts[0]} + 1)))
else
    first_part=${version_parts[0]}
fi

# Construct the new version number
new_version="$first_part.$middle_part.$last_part"

# Write the new version number to the file
echo $new_version > version.txt

# Define the source and destination directories
src_dir="coesius/dev"
dest_dir="AUTO_RELEASES"
cleaner=".scripts/clean_header.sh"

# Copy the file
cp $src_dir/network.hpp $dest_dir/Coesius_Lib_AUTO-$new_version.hpp

# For each .ipp include found
grep -o '#include <.*\.ipp>' $dest_dir/Coesius_Lib_AUTO-$new_version.hpp | while read -r line ; do
    # Extract the .ipp filename
    filename=$(echo $line | cut -d'<' -f 2 | cut -d'>' -f 1)

    # Remove the 'coesius/dev/' from the filename
    filename=${filename#$src_dir/}

    # Escape special characters in the line for use in awk
    line_escaped=$(echo $line | sed 's/[]\/$*.^[]/\\&/g')

    # Check if the .ipp file exists in the source directory
    if [ -f $src_dir/$filename ]; then
        # Replace the include line with the content of the .ipp file
        awk -v r="$src_dir/$filename" "/$line_escaped/{while((getline < r) > 0) print; next}1" $dest_dir/Coesius_Lib_AUTO-$new_version.hpp > temp && mv temp $dest_dir/Coesius_Lib_AUTO-$new_version.hpp
    else
        echo "File $filename not found in $src_dir!"
    fi
done

$cleaner "$dest_dir/Coesius_Lib_AUTO-$new_version.hpp"

# Count the number of files in dest_dir
file_count=$(ls -1q "$dest_dir" | wc -l)

# Check if there are more than 5 files
if [ $file_count -gt 5 ]; then
  # Get the oldest file in dest_dir
  oldest_file=$(ls -tr "$dest_dir" | head -n 1)

  # Delete the oldest file
  rm "$dest_dir/$oldest_file"
  echo "Deleted the oldest file: $oldest_file"
else 
  echo "There are 5 or fewer files in $dest_dir. No files were deleted."
fi

# Echo the file to show it worked
echo "Created file: $dest_dir/Coesius_Lib_AUTO-$new_version.hpp"