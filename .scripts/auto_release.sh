# Change to the directory containing the files
cd ../MINI_DEV_RELEASES

# Find the file with the highest version number
highest_version_file=$(ls Coesius_Lib_MINI_DEV-v*.hpp | sort -t- -k2 -n | tail -n1)

# Extract the version number from the file name
version=$(echo $highest_version_file | grep -oP '(?<=-).*(?=.hpp)')

# Use the GitHub CLI to create a new release with the file
gh release create "AUTO" "$highest_version_file" --title "Release dev-$version" --notes "Automated release for dev-mini v$version"

echo "Created GitHub release for version $version with file $highest_version_file"