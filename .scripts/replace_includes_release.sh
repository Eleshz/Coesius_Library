# Move to root
cd ..

# Read the version number and convert it to an integer
version_number=$(printf "%06d" $((10#$(cat version.txt))))

# Increment the version number
new_version_number=$(printf "%06d" $((10#$version_number + 1)))

# Write the new version number to the file
echo $new_version_number > version.txt

# Define the source and destination directories
src_dir="coesius/dev"
dest_dir="MINI_DEV_RELEASES"
cleaner=".scripts/clean_header.sh"

# Copy the file
cp $src_dir/network.hpp $dest_dir/Coesius_Lib_MINI_DEV-v$new_version_number.hpp

# For each .ipp include found
grep -o '#include <.*\.ipp>' $dest_dir/Coesius_Lib_MINI_DEV-v$new_version_number.hpp | while read -r line ; do
    # Extract the .ipp filename
    filename=$(echo $line | cut -d'<' -f 2 | cut -d'>' -f 1)

    # Remove the 'coesius/dev/' from the filename
    filename=${filename#$src_dir/}

    # Escape special characters in the line for use in awk
    line_escaped=$(echo $line | sed 's/[]\/$*.^[]/\\&/g')

    # Check if the .ipp file exists in the source directory
    if [ -f $src_dir/$filename ]; then
        # Replace the include line with the content of the .ipp file
        awk -v r="$src_dir/$filename" "/$line_escaped/{while((getline < r) > 0) print; next}1" $dest_dir/Coesius_Lib_MINI_DEV-v$new_version_number.hpp > temp && mv temp $dest_dir/Coesius_Lib_MINI_DEV-v$new_version_number.hpp
    else
        echo "File $filename not found in $src_dir!"
    fi
done

$cleaner "$dest_dir/Coesius_Lib_MINI_DEV-v$new_version_number.hpp"

# Count the number of files in dest_dir
file_count=$(ls -1q "$dest_dir" | wc -l)

# Check if there are more than 5 files
if [ $file_count -gt 5 ]; then
  # Get the file with the lowest version number in dest_dir
  oldest_file=$(ls -v "$dest_dir" | head -n 1)

  # Delete the oldest file
  rm "$dest_dir/$oldest_file"
  echo "Deleted the oldest file: $oldest_file"
else 
  echo "There are 5 or fewer files in $dest_dir. No files were deleted."
fi

# Echo the file to show it worked
echo "Created file: $dest_dir/Coesius_Lib_MINI_DEV-v$new_version_number.hpp"