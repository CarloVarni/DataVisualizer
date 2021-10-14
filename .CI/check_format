
# abort on error
set -e


if [ $# -ne 1 ]; then
    echo "<DIR> path not specified"
    exit 1
fi

_binary=${CLANG_FORMAT_BINARY:-clang-format}

$_binary --version

cd $1
