echo -n "" > "$4"
while read linie
do
	verif=`echo "$linie"|grep -E "^[A-Za-z][A-Za-z0-9-]*@[A-Za-z][A-Za-z0-9]*\.(com|ro|eu);([1-9]|[1-9][1-9]|1[1-9][1-9]|2[0-4][0-9]|25[0-5])\.(([0-9]|[1-9][1-9]|1[1-9][1-9]|2[0-4][0-9]|25[0-5])\.){2}([1-9]|[1-9][1-9]|1[1-9][1-9]|2[0-4][0-9]|25[0-4])$"`
	if test -n "$verif"
	then
		echo "OK" >> "$4"
	else
		echo "ERROR" >>"$4"
	fi
done < "$2"