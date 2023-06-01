#!/bin/bash
# Call create-dmg script with source folder and output file

if [ "$#" -ne 2 ]; then
    echo "Please supply 2 arguments: source-folder output-file"
fi

if [[ $(command -v brew) == "" ]]; then
    echo "Installing Homebrew"
    /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
else
    echo "Updating Homebrew"
    brew update
fi

echo "Checking if create-dmg installed..."
brew list create-dmg || brew install create-dmg

echo "Calling create-dmg with source-folder $1 and output-file $2"

# Add the following for background image, e.g.
#  --background "Assets/Installer/app-background3d-multires.tiff" \
create-dmg \
  --volname "Audio Feature Visualiser" \
  --window-pos 200 120 \
  --window-size 852 480 \
  --hide-extension "Audio Feature Visualiser.app" \
  --icon "Audio Feature Visualiser.app" 330 111 \
  --icon-size 90 \
  --app-drop-link 525 108 \
  --no-internet-enable \
  "$2" \
  "$1"

