echo -n "" > "$4"
while read linie
do
	verif=`echo "$linie"|grep -E "^https?://[a-z][A-Za-z0-9\-]*\.(ro|eu);[0-9A-F][0-9A-F]:[0-9A-F][0-9A-F]:[0-9A-F][0-9A-F]:[0-9A-F][0-9A-F]:[0-9A-F][0-9A-F]:[0-9A-F][0-9A-F]$"`
	if test -n "$verif"
	then
		echo "OK" >> "$4"
	else
		echo "ERROR" >>"$4"
	fi
done < "$2"