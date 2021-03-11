# check for type_t* aParam
# pointers pass as param should be const ptr
# e.g. type_t* const aParam
const_ptr=`grep --color=always -nr '\*\s*a[A-Z]' src include`
if [[ $const_ptr != '' ]]; then
    echo error
    echo "$const_ptr"
    exit 1
fi
