function scaneazaDirector(){
	continut=`ls "$1"`
	IFS=$'\n'
	for i in $continut
	do
		fis=`echo "$1/$i" | grep -E "\.txt$" `
		if test -n "$fis" && test -f "$1/$i"
		then
			val=`wc -m "$1/$i" | cut -f 1 -d ' ' `
			echo "$i:$val" >> "$2"
			suma=`expr $suma + $val`
		fi
	done
}

echo -n "" > "$2"
suma=0
scaneazaDirector "$1" "$2"
echo "Total:$suma" >>"$2"