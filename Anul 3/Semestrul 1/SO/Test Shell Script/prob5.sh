if test $# == 3
then
	case $2 in 
		"+")val=`expr $1 + $3`
	echo "$val";;
"-")val=`expr $1 - $3`
	echo "$val";;
"*")val=`expr $1 \* $3`
	echo "$val";;
"/")val=`expr $1 / $3`
	echo "$val";;
esac
else
	echo "Exemplu de apelare:prog op1 operatie op2"
	exit 1
fi
