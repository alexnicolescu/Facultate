function traversareDirector(){
	for i in "$1"/*".txt"
	do
		if test -f "$i"
		then
			case "$2" in
				"x")chmod 111 "$i";;
"r")chmod 444 "$i";;
"w")chmod 222 "$i";;
esac
fi
done
for i in "$1"/*
do
	if test -d "$i"
	then
		traversareDirector "$i" $2
	fi
done
}



traversareDirector "$1" $2