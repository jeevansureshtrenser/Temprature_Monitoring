# Exit immediately if a command fails 
set -e 
# Project root (adjust if needed) 
PROJECT_DIR="$(dirname "$(realpath "$0")")" 
cd "$PROJECT_DIR" 
# Check if build directory exists 
if [ -d "build" ]; then 
	echo "Removing existing build directory..." 
	rm -rf build 
fi
# Create new build directory 
echo "Creating build directory..."
mkdir build
# Run make echo "Running make..." 
make
echo "Build completed successfully!"
