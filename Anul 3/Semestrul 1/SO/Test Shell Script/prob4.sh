function max()
{
	if test $1 -ge $2
	then
		return 1
	else
		return 2
	fi
}
if test $# == 3
then	
	max $1 $2
	v=$?
	if test $v == 1
	then
		max $1 $3
		v=$?
		if test $v == 1
		then
			echo "$1"
		else
			echo "$3"
		fi
	else
		max $2 $3
		v=$?
		if test $v == 1
		then 
			echo "$2"
		else
			echo "$3"
		fi
	fi
else
	echo "Trebuie 3 numere ca si argumente"
	exit 1
fi