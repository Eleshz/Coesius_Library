# Move to 'root'
cd ..

src_dir="sia/dev"
dest_dir="sia/public"
cleaner=".scripts/clean_header.sh"

cp $src_dir/network.hpp $dest_dir/sia_lib.hpp

# For each .ipp include found
grep -o '#include <.*\.ipp>' $dest_dir/sia_lib.hpp | while read -r line ; do
    # Extract the .ipp filename
    filename=$(echo $line | cut -d'<' -f 2 | cut -d'>' -f 1)

    # Remove the first directory from the filename
    filename=${filename#*/}

    # Escape special characters in the line for use in awk
    line_escaped=$(echo $line | sed 's/[]\/$*.^[]/\\&/g')

    # Check if the .ipp file exists in the source directory
    if [ -f $src_dir/$filename ]; then
        # Replace the include line with the content of the .ipp file
        awk -v r="$src_dir/$filename" "/$line_escaped/{while((getline < r) > 0) print; next}1" $dest_dir/sia_lib.hpp > temp && mv temp $dest_dir/sia_lib.hpp
    else
        echo "File $filename not found in $src_dir!"
    fi
done

$cleaner "$dest_dir/sia_lib.hpp"