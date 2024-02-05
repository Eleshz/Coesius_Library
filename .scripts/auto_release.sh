# Change to the directory containing the files
cd ../AUTO_RELEASES

# Find the file with the highest version number
highest_version_file=$(ls Sia_Lib_AUTO-*.hpp | sort -t- -k2 -V | tail -n1)

# Extract the version number from the file name
version=$(echo $highest_version_file | grep -oP '(?<=-).*(?=.hpp)')

# Use the GitHub CLI to create a new release with the file
gh release create "v$version" "$highest_version_file" --title "Release $version" --notes "Automated release for version $version"

echo "Created GitHub release for version $version with file $highest_version_file"