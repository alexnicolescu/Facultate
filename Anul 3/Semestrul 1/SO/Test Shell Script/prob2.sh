function existaDoarDirectoare(){
	continut=`ls "$1" `
	IFS=$'\n'
	for i in $continut
	do
		if test -f "$1/$i"
		then
			return 0
		fi
	done
	return 1
}
function existaDoarUnFisier(){
	continut=`ls "$1" `
	nr=0
	IFS=$'\n'
	for i in $continut
	do
		if test -f "$1/$i"
		then
			nr=`expr $nr + 1 `
		else
			return 0
		fi
		if test $nr == 2
		then
			return 0
		fi
	done

	return 1
}

function cautaInformatii(){
	continut2=`ls "$1" `
	IFS=$'\n'
	for i in $continut2
	do
		while read linie
		do
			val=`echo "$linie" | grep -E "^[A-Z][a-z]* [A-Z][a-z]* [A-Z][a-z]*\.-[0-9]{5,8}$"  `
			if test -n "$val"
			then
				nume=`echo "$val" | grep -E -o "^[A-Z][a-z]*"`
				prenume=`echo "$val" | cut -f 2 -d ' '`
				concat=`echo "$val" | grep -E -o "[A-Z][a-z]*\.-[0-9]{5,8}$" `
				firma=`echo "$concat" | grep -E -o "[A-Z][a-z]*" `
				oras=`echo "$concat" | grep -E -o "[0-9]{5,8}$" `
				touch "$1/$nume.txt"
				echo "$nume $prenume" >> "$1/$nume.txt"
				echo "$firma $oras" >> "$1/$nume.txt"
			fi
		done < "$1/$i"
		break
	done

}


existaDoarDirectoare "$1"
oki=$?
if test "$oki" -eq 1 
then
	continutul=`ls "$1" `
	IFS=$'\n'
	for i in $continutul
	do
		existaDoarUnFisier "$1/$i"
		oki=$?
		if test $oki == 0
		then
			echo "Directoarele trebuie sa contina doar un fisier"
			exit 1
		fi
	done
	IFS=$'\n'
	for j in $continutul
	do
		cautaInformatii "$1/$j"
	done
else
	echo "Directorul trebuie sa contina doar directoare"
	exit 1
fi