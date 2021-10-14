
# abort on error
set -e


if [ $# -ne 1 ]; then
    echo "<DIR> path not specified"
    exit 1
fi

_binary=${CLANG_FORMAT_BINARY:-clang-format}

$_binary --version

cd $1
find . \( -iname '*.cpp' -or -iname '*.hpp' -or -iname '*.ipp' -or -iname '*.cu' -or -iname '*.cuh' \) \
  | xargs $_binary -i -style=file
  
if ! [ -z $CI ] || ! [ -z $GITHUB_ACTIONS ]; then
  mkdir DataVisualizerFormatted
  for f in $(git diff --name-only); do
    cp --parents $f DataVisualizerFormatted
  done
fi
    
echo "clang-format done"
    
set +e
git diff --exit-code --stat
result=$?

if [ "$result" -eq "128" ]; then
    echo "Format was successfully applied"
    echo "Could not create summary of affected files"
    echo "Are you in a submodule?"

fi

exit $result

