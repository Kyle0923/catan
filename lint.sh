# check for type_t* aParam
# pointers pass as param should be const ptr
# e.g. type_t* const aParam
report=lint_report
rm -fr $report
const_ptr=`grep -nr '\w\s*\*\s*a[A-Z]' src include`
if [[ $const_ptr != '' ]]; then
    echo error
    echo "$const_ptr"
    echo "$const_ptr" >> $report
    rc=1
fi

exit $rc
