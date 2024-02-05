src_dir="sia/dev"
dest_dir="sia/public"
cleaner="./clean_header.sh"

# Check if the file already exists in the destination directory
if [ ! -f $dest_dir/Sia_network_full.hpp ]; then
    # Copy the file
    cp $src_dir/network.hpp $dest_dir/network_full.hpp
fi

# For each .ipp include found
grep -o '#include <.*\.ipp>' $dest_dir/network_full.hpp | while read -r line ; do
    # Extract the .ipp filename
    filename=$(echo $line | cut -d'<' -f 2 | cut -d'>' -f 1)

    # Remove the first directory from the filename
    filename=${filename#*/}

    # Escape special characters in the line for use in awk
    line_escaped=$(echo $line | sed 's/[]\/$*.^[]/\\&/g')

    # Check if the .ipp file exists in the source directory
    if [ -f $src_dir/$filename ]; then
        # Replace the include line with the content of the .ipp file
        awk -v r="$src_dir/$filename" "/$line_escaped/{while((getline < r) > 0) print; next}1" $dest_dir/network_full.hpp > temp && mv temp $dest_dir/network_full.hpp
    else
        echo "File $filename not found in $src_dir!"
    fi
done