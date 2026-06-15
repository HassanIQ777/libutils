#!/usr/bin/env bash   

echo " ----- Started Updater ----- "
echo "Updating the program..."

git fetch origin && git reset --hard origin/main

if [ "$?" -eq 0 ]; then
    echo "Successfully finished updating!"
else
    echo "Failed to update."
fi