# fetch data from script
# \param $1 section name
# \param $2 key name
# 
# example:
#   var=`script_fetch mmc display_name`
script_fetch()
{
    filp="/boot/test_config.fex"
    section=$1
    key=$2

    n0=$(sed -n '/\['"$section"'\]/=' $filp)
    n1=$(sed -n ''"$n0"',${/\['[0-9,a-z,A-Z,_]*'\]/=}' $filp | sed -n '2p')
    [ -z "$n1" ] && n1='$'
    item=$(sed -n ''"$n0"','"$n1"'p' $filp | awk -F '=' '/\['"$section"'\]/{a=1}a==1&&$0~/'"$key"'/{gsub(/[[:blank:]]*/,"",$0); print $0; exit}')
    value=${item#*=}
    start=${value:0:7}
    if [ "$start" = "string:" ]; then
        retval=${value#*string:}
    else
        start=${value:0:1}
        if [ "$start" = "\"" ]; then
            retval=${value#*\"}
            retval=${retval%\"*}
        else
            retval=$value
        fi
    fi
    echo $retval
}
